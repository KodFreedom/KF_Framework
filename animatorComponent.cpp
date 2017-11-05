//--------------------------------------------------------------------------------
//
//�@animatorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "animatorComponent.h"
#include "gameObject.h"
#include "gameObjectSpawner.h"
#include "transform.h"
#include "motionInfo.h"
#include "collisionSystem.h"
#include "sphereCollider.h"
#include "motionStatus.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CAnimatorComponent::CAnimatorComponent(GameObject* const pGameObj, const string& strPath)
	: Component(pGameObj)
	, m_motionNow(MP_NEUTAL)
	, m_motionNext(MP_NEUTAL)
	, m_pMotionStatus(nullptr)
	, m_bIsGrounded(false)
{
	for (auto& pMotionDate : m_apMotionData) { pMotionDate = nullptr; }
	m_vecBorns.clear();

	//�t�@�C������f�[�^��ǂݍ���
	analyzeFile(strPath);
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool CAnimatorComponent::Init(void)
{
	auto& strTag = m_pGameObj->GetTag();
	for (auto pObj : m_vecBorns) { pObj->SetTag(strTag); }
	m_pMotionStatus = new CNormalMotionStatus;
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CAnimatorComponent::Uninit(void)
{
	for (auto pInfo : m_apMotionData)
	{
		if (!pInfo) { continue; }
		delete pInfo;
		pInfo = nullptr;
	}

	m_vecBorns.clear();

	if (m_pMotionStatus)
	{
		delete m_pMotionStatus;
		m_pMotionStatus = nullptr;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CAnimatorComponent::Update(void)
{
	m_pMotionStatus->Update(*this);

	//Attack�p
	//if (m_motionNow == MP_ATTACK)
	//{
	//	updateAttack();
	//}
}

//--------------------------------------------------------------------------------
//	�֐����FchangeMotion
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CAnimatorComponent::SetAttack(const bool& bAttack)
{
	if (!bAttack) { return; }
	m_pMotionStatus->ChangeMotion(*this, MP_ATTACK);
}

//--------------------------------------------------------------------------------
//	�֐����FchangeMotion
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CAnimatorComponent::SetGrounded(const bool& bGrounded)
{
	if (m_bIsGrounded == bGrounded) { return; }
	if (!m_bIsGrounded && bGrounded)
	{
		m_pMotionStatus->ChangeMotion(*this, MP_LAND);
	}
	m_bIsGrounded = bGrounded;
}

//--------------------------------------------------------------------------------
//	�֐����FchangeMotion
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CAnimatorComponent::SetJump(const bool& bJump)
{
	if (!bJump || !m_bIsGrounded) { return; }
	m_pMotionStatus->ChangeMotion(*this, MP_JUMP);
}

//--------------------------------------------------------------------------------
//	�֐����FchangeMotion
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CAnimatorComponent::SetMove(const float& fMovement)
{
	if (fMovement == 0.0f) { m_pMotionStatus->ChangeMotion(*this, MP_NEUTAL); }
	else { m_pMotionStatus->ChangeMotion(*this, MP_RUN); }
}

//--------------------------------------------------------------------------------
//	�֐����FchangeMotion
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
bool CAnimatorComponent::CanAct(void)
{
	if (m_motionNow == MP_ATTACK) { return false; }
	return true;
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FchangeMotion
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
//bool  CAnimatorComponent::checkCanChange(const MOTION_PATTERN& motion)
//{
//	if (m_motionNow == motion) { return false; }
//
//	switch (m_motionNow)
//	{
//	case MP_NEUTAL:
//		return true;
//	case MP_RUN:
//		return true;
//	case MP_JUMP:
//		if (motion == MP_LAND && m_bIsGrounded) { return true; }
//		return false;
//	case MP_LAND:
//		if (m_status != MS_WAIT && motion == MP_NEUTAL) { return false; }
//		return true;
//	case MP_ATTACK:
//		if (m_status == MS_WAIT) { return true; }
//		return false;
//	default:
//		break;
//	}
//
//	return true;
//}

//--------------------------------------------------------------------------------
//	�֐����FchangeMotionStatus
//  �֐������F���[�V�����X�e�[�^�X�̐؂�ւ�
//	�����F	pMotionStatus�F�V�������[�V�����X�e�[�^�X
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CAnimatorComponent::changeMotionStatus(CMotionStatus* pMotionStatus)
{
	if (m_pMotionStatus) { delete m_pMotionStatus; }
	m_pMotionStatus = pMotionStatus;
}

//--------------------------------------------------------------------------------
//	�֐����FgetMotionNext
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
MOTION_PATTERN CAnimatorComponent::getMotionNext(const MOTION_PATTERN& motion)
{
	switch (motion)
	{
	case MP_JUMP:
		return MP_LAND;
	}

	return MP_NEUTAL;
}

//--------------------------------------------------------------------------------
//	�֐����FchangeMotion
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CAnimatorComponent::analyzeFile(const string& strPath)
{
	FILE* filePointer;
	char strRead[256];

	//file open
	fopen_s(&filePointer, strPath.c_str(), "r");

	if (!filePointer) { return; }

	//�p�[�c���̓ǂݍ���
	char compare1[] = "NUmodeL = ";
	bool bFind = false;

	while (bFind != true)
	{
		fgets(strRead, sizeof(strRead), filePointer);

		for (unsigned int count = 0; count < strlen(compare1);)
		{
			if (strRead[count] == compare1[count]) { ++count; }
			else { break; }

			if (count == strlen(compare1)) { bFind = true; }
		}
	}

	char strNum[16];
	for (unsigned int count = strlen(compare1), countNum = 0; strRead[count] != '\n'; ++count, countNum++)
	{
		strNum[countNum] = strRead[count];
	}

	//NumParts
	int nNumParts = atoi(strNum);
	m_vecBorns.resize(nNumParts);

	//XFile�̓ǂݍ���
	for (int countPart = 0; countPart < nNumParts; countPart++)
	{
		//XFILE XFile;
		char compare2[] = "MODEL_FILENAME = ";
		bFind = false;

		while (bFind != true)
		{
			fgets(strRead, sizeof(strRead), filePointer);

			for (unsigned int count = 0; count < strlen(compare2);)
			{
				if (strRead[count] == compare2[count]) { ++count; }
				else { break; }

				if (count == strlen(compare2)) { bFind = true; }
			}
		}

		string strPath;
		for (unsigned int count = strlen(compare2), countPath = 0; strRead[count] != '\t'; ++count, countPath++)
		{
			strPath.push_back(strRead[count]);
		}
		string strFileName, strFileType;
		Utility::AnalyzeFilePath(strPath, strFileName, strFileType);
		GameObject* pBone = nullptr;
		if (strFileType._Equal("x"))
		{
			pBone = GameObjectSpawner::CreateXModel(strPath, Vector3(0.0f), Vector3(0.0f), Vector3(1.0f));
		}
		else if (strFileType._Equal("model"))
		{
			pBone = GameObjectSpawner::CreateModel(strPath, Vector3(0.0f), Quaternion(0.0f), Vector3(1.0f));
		}
		
		m_vecBorns[countPart] = pBone;
	}

	//character���̓ǂݍ���
	char compare0[] = "CHARACTERSET";
	bFind = false;

	while (bFind != true)
	{
		fgets(strRead, sizeof(strRead), filePointer);

		for (unsigned int count = 0; count < strlen(compare0);)
		{
			if (strRead[count] == compare0[count]) { ++count; }
			else { break; }

			if (count == strlen(compare0)) { bFind = true; }
		}
	}

	//Move
	fgets(strRead, sizeof(strRead), filePointer);
	char strMove[8] = {};
	for (unsigned int count = strlen("\tMOVE = "), countMove = 0; strRead[count] != '\t'; ++count, countMove++)
	{
		strMove[countMove] = strRead[count];
	}
	//m_fMoveSpeed = (float)atof(strMove);

	//Jump
	fgets(strRead, sizeof(strRead), filePointer);
	char strJump[8] = {};
	for (unsigned int count = strlen("\tJUMP = "), countJump = 0; strRead[count] != '\t'; ++count, countJump++)
	{
		strJump[countJump] = strRead[count];
	}
	//m_fJumpSpeed = (float)atof(strJump);

	//Radius
	fgets(strRead, sizeof(strRead), filePointer);
	char strRadius[8] = {};
	for (unsigned int count = strlen("\tRADIUS = "), countRadius = 0; strRead[count] != '\t'; ++count, countRadius++)
	{
		strRadius[countRadius] = strRead[count];
	}
	//Radius = (float)atof(strRadius);

	//parts info�̓ǂݍ���
	for (int countPart = 0; countPart < nNumParts; ++countPart)
	{
		char compare2[] = "\tPARTSSET";
		bFind = false;

		while (bFind != true)
		{
			fgets(strRead, sizeof(strRead), filePointer);

			for (unsigned int count = 0; count < strlen(compare2);)
			{
				if (strRead[count] == compare2[count]) { ++count; }
				else { break; }

				if (count == strlen(compare2)) { bFind = true; }
			}
		}

		//Index
		fgets(strRead, sizeof(strRead), filePointer);

		//Parent
		fgets(strRead, sizeof(strRead), filePointer);
		char strParent[4] = {};
		for (unsigned int count = strlen("\t\tPARENT = "), countParent = 0; strRead[count] != '\t'; ++count, countParent++)
		{
			strParent[countParent] = strRead[count];
		}

		int nParent = atoi(strParent);

		//Pos
		fgets(strRead, sizeof(strRead), filePointer);
		char strPos[3][16] = {};
		int countAxis = 0;
		int countPos = 0;
		for (unsigned int count = strlen("\t\tPOS = "); strRead[count] != '\n'; ++count)
		{
			if (strRead[count] == ' ')
			{
				countAxis++;
				countPos = 0;
				continue;
			}

			strPos[countAxis][countPos] = strRead[count];
			countPos++;
		}

		auto vOffsetPos = Vector3((float)atof(strPos[0]), (float)atof(strPos[1]), (float)atof(strPos[2]));

		//Rot
		fgets(strRead, sizeof(strRead), filePointer);
		char strRot[3][16] = {};
		countAxis = 0;
		int countRot = 0;
		for (unsigned int count = strlen("\t\tROT = "); strRead[count] != '\n'; ++count)
		{
			if (strRead[count] == ' ')
			{
				countAxis++;
				countRot = 0;
				continue;
			}

			strRot[countAxis][countRot] = strRead[count];
			countRot++;
		}

		auto vOffsetRot = Vector3((float)atof(strRot[0]), (float)atof(strRot[1]), (float)atof(strRot[2]));
		
		GameObject* pParent = nullptr;
		if (nParent < 0) { pParent = m_pGameObj; }
		else { pParent = m_vecBorns[nParent]; }
		m_vecBorns[countPart]->GetTransform()->RegisterParent(pParent->GetTransform(), vOffsetPos, vOffsetRot);
	}

	//motion�̓ǂݍ���
	int countMotion = 0;
	fgets(strRead, sizeof(strRead), filePointer);
	while (1)
	{
		char compare2[] = "MOTIONSET";
		bFind = false;

		while (bFind != true)
		{
			if (fgets(strRead, sizeof(strRead), filePointer) == NULL) { break; }

			for (unsigned int count = 0; count < strlen(compare2);)
			{
				if (strRead[count] == compare2[count]) { ++count; }
				else { break; }

				if (count == strlen(compare2)) { bFind = true; }
			}
		}

		if (bFind == false) { break; }

		m_apMotionData[(MOTION_PATTERN)countMotion] = new CMotionInfo;

		//LOOP
		fgets(strRead, sizeof(strRead), filePointer);
		char strLoop[2] = {};
		for (unsigned int count = strlen("\tLOOP = "), countLoop = 0; strRead[count] != '\t'; ++count, countLoop++)
		{
			strLoop[countLoop] = strRead[count];
		}

		m_apMotionData[(MOTION_PATTERN)countMotion]->m_bLoop
			= atoi(strLoop) != 0 ? true : false;

		//�L�[�t���[�����擾
		fgets(strRead, sizeof(strRead), filePointer);
		char strKey[2] = {};
		for (unsigned int count = strlen("\tNUM_KEY = "), countKey = 0; strRead[count] != '\t'; ++count, countKey++)
		{
			strKey[countKey] = strRead[count];
		}

		int nNumKey = atoi(strKey);
		m_apMotionData[(MOTION_PATTERN)countMotion]->m_vecMotionKey.resize(nNumKey);

		//�L�[�t���[���̓ǂݍ���
		for (int countKey = 0; countKey < nNumKey; countKey++)
		{
			char compare3[] = "\tKEYSET";
			bFind = false;

			while (bFind != true)
			{
				fgets(strRead, sizeof(strRead), filePointer);

				for (unsigned int count = 0; count < strlen(compare3);)
				{
					if (strRead[count] == compare3[count]) { ++count; }
					else { break; }

					if (count == strlen(compare3)) { bFind = true; }
				}
			}

			CMotionKey motionKey;

			//Frame
			fgets(strRead, sizeof(strRead), filePointer);
			char strFrame[4] = {};
			for (unsigned int count = strlen("\t\tFRAME = "), countFrame = 0; strRead[count] != '\n'; ++count, countFrame++)
			{
				strFrame[countFrame] = strRead[count];
			}

			motionKey.m_nFrame = atoi(strFrame);

			//Set Key per Parts
			for (int countPart = 0; countPart < nNumParts; countPart++)
			{
				//pos
				char compare4[] = "\t\t\tPOS = ";
				bFind = false;

				while (bFind != true)
				{
					fgets(strRead, sizeof(strRead), filePointer);

					for (unsigned int count = 0; count < strlen(compare4);)
					{
						if (strRead[count] == compare4[count]) { ++count; }
						else { break; }

						if (count == strlen(compare4)) { bFind = true; }
					}
				}

				char strPos[3][16] = {};
				int countAxis = 0;
				int countPos = 0;
				for (unsigned int count = strlen("\t\t\tPOS = "); strRead[count] != '\n'; ++count)
				{
					if (strRead[count] == ' ')
					{
						countAxis++;
						countPos = 0;
						continue;
					}

					strPos[countAxis][countPos] = strRead[count];
					countPos++;
				}

				auto Position = Vector3((float)atof(strPos[0]), (float)atof(strPos[1]), (float)atof(strPos[2]));

				//Rot
				fgets(strRead, sizeof(strRead), filePointer);
				char strRot[3][16] = {};
				countAxis = 0;
				int countRot = 0;
				for (unsigned int count = strlen("\t\t\tROT = "); strRead[count] != '\n'; ++count)
				{
					if (strRead[count] == ' ')
					{
						countAxis++;
						countRot = 0;
						continue;
					}

					strRot[countAxis][countRot] = strRead[count];
					countRot++;
				}

				auto vRot = Vector3((float)atof(strRot[0]), (float)atof(strRot[1]), (float)atof(strRot[2]));
				auto qRot = CKFMath::EulerToQuaternion(vRot);
				auto nodeKey = CNodeKey(Position, qRot);
				motionKey.m_listNodesKey.push_back(nodeKey);
			}

			//MotionKey�̕ۑ�
			m_apMotionData[(MOTION_PATTERN)countMotion]->m_vecMotionKey[countKey] = motionKey;
		}

		countMotion++;
	}

	fclose(filePointer);
}
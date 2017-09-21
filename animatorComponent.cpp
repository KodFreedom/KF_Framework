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
#include "transformComponent.h"
#include "motionInfo.h"
#include "KF_CollisionSystem.h"
#include "sphereColliderComponent.h"
#include "motionStatus.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CAnimatorComponent::CAnimatorComponent(CGameObject* const pGameObj, const string& strPath)
	: CComponent(pGameObj)
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
	FILE* pFile;
	char strRead[256];

	//file open
	fopen_s(&pFile, strPath.c_str(), "r");

	if (!pFile) { return; }

	//�p�[�c���̓ǂݍ���
	char strComp1[] = "NUM_MODEL = ";
	bool bFind = false;

	while (bFind != true)
	{
		fgets(strRead, sizeof(strRead), pFile);

		for (unsigned int nCnt = 0; nCnt < strlen(strComp1);)
		{
			if (strRead[nCnt] == strComp1[nCnt]) { nCnt++; }
			else { break; }

			if (nCnt == strlen(strComp1)) { bFind = true; }
		}
	}

	char strNum[16];
	for (unsigned int nCnt = strlen(strComp1), nCntNum = 0; strRead[nCnt] != '\n'; nCnt++, nCntNum++)
	{
		strNum[nCntNum] = strRead[nCnt];
	}

	//NumParts
	int nNumParts = atoi(strNum);
	m_vecBorns.resize(nNumParts);

	//XFile�̓ǂݍ���
	for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
	{
		//XFILE XFile;
		char strComp2[] = "MODEL_FILENAME = ";
		bFind = false;

		while (bFind != true)
		{
			fgets(strRead, sizeof(strRead), pFile);

			for (unsigned int nCnt = 0; nCnt < strlen(strComp2);)
			{
				if (strRead[nCnt] == strComp2[nCnt]) { nCnt++; }
				else { break; }

				if (nCnt == strlen(strComp2)) { bFind = true; }
			}
		}

		string strPath;
		for (unsigned int nCnt = strlen(strComp2), nCntPath = 0; strRead[nCnt] != '\t'; nCnt++, nCntPath++)
		{
			strPath.push_back(strRead[nCnt]);
		}
		string strFileName, strFileType;
		CKFUtility::AnalyzeFilePath(strPath, strFileName, strFileType);
		CGameObject* pBone = nullptr;
		if (strFileType._Equal("x"))
		{
			pBone = CGameObjectSpawner::CreateXModel(strPath, CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
		}
		else if (strFileType._Equal("model"))
		{
			pBone = CGameObjectSpawner::CreateModel(strPath, CKFVec3(0.0f), CKFQuaternion(0.0f), CKFVec3(1.0f));
		}
		
		m_vecBorns[nCntPart] = pBone;
	}

	//character���̓ǂݍ���
	char strComp0[] = "CHARACTERSET";
	bFind = false;

	while (bFind != true)
	{
		fgets(strRead, sizeof(strRead), pFile);

		for (unsigned int nCnt = 0; nCnt < strlen(strComp0);)
		{
			if (strRead[nCnt] == strComp0[nCnt]) { nCnt++; }
			else { break; }

			if (nCnt == strlen(strComp0)) { bFind = true; }
		}
	}

	//Move
	fgets(strRead, sizeof(strRead), pFile);
	char strMove[8] = {};
	for (unsigned int nCnt = strlen("\tMOVE = "), nCntMove = 0; strRead[nCnt] != '\t'; nCnt++, nCntMove++)
	{
		strMove[nCntMove] = strRead[nCnt];
	}
	//m_fMoveSpeed = (float)atof(strMove);

	//Jump
	fgets(strRead, sizeof(strRead), pFile);
	char strJump[8] = {};
	for (unsigned int nCnt = strlen("\tJUMP = "), nCntJump = 0; strRead[nCnt] != '\t'; nCnt++, nCntJump++)
	{
		strJump[nCntJump] = strRead[nCnt];
	}
	//m_fJumpSpeed = (float)atof(strJump);

	//Radius
	fgets(strRead, sizeof(strRead), pFile);
	char strRadius[8] = {};
	for (unsigned int nCnt = strlen("\tRADIUS = "), nCntRadius = 0; strRead[nCnt] != '\t'; nCnt++, nCntRadius++)
	{
		strRadius[nCntRadius] = strRead[nCnt];
	}
	//m_fRadius = (float)atof(strRadius);

	//parts info�̓ǂݍ���
	for (int nCntPart = 0; nCntPart < nNumParts; ++nCntPart)
	{
		char strComp2[] = "\tPARTSSET";
		bFind = false;

		while (bFind != true)
		{
			fgets(strRead, sizeof(strRead), pFile);

			for (unsigned int nCnt = 0; nCnt < strlen(strComp2);)
			{
				if (strRead[nCnt] == strComp2[nCnt]) { nCnt++; }
				else { break; }

				if (nCnt == strlen(strComp2)) { bFind = true; }
			}
		}

		//Index
		fgets(strRead, sizeof(strRead), pFile);

		//Parent
		fgets(strRead, sizeof(strRead), pFile);
		char strParent[4] = {};
		for (unsigned int nCnt = strlen("\t\tPARENT = "), nCntParent = 0; strRead[nCnt] != '\t'; nCnt++, nCntParent++)
		{
			strParent[nCntParent] = strRead[nCnt];
		}

		int nParent = atoi(strParent);

		//Pos
		fgets(strRead, sizeof(strRead), pFile);
		char strPos[3][16] = {};
		int nCntAxis = 0;
		int nCntPos = 0;
		for (unsigned int nCnt = strlen("\t\tPOS = "); strRead[nCnt] != '\n'; nCnt++)
		{
			if (strRead[nCnt] == ' ')
			{
				nCntAxis++;
				nCntPos = 0;
				continue;
			}

			strPos[nCntAxis][nCntPos] = strRead[nCnt];
			nCntPos++;
		}

		auto vOffsetPos = CKFVec3((float)atof(strPos[0]), (float)atof(strPos[1]), (float)atof(strPos[2]));

		//Rot
		fgets(strRead, sizeof(strRead), pFile);
		char strRot[3][16] = {};
		nCntAxis = 0;
		int nCntRot = 0;
		for (unsigned int nCnt = strlen("\t\tROT = "); strRead[nCnt] != '\n'; nCnt++)
		{
			if (strRead[nCnt] == ' ')
			{
				nCntAxis++;
				nCntRot = 0;
				continue;
			}

			strRot[nCntAxis][nCntRot] = strRead[nCnt];
			nCntRot++;
		}

		auto vOffsetRot = CKFVec3((float)atof(strRot[0]), (float)atof(strRot[1]), (float)atof(strRot[2]));
		
		CGameObject* pParent = nullptr;
		if (nParent < 0) { pParent = m_pGameObj; }
		else { pParent = m_vecBorns[nParent]; }
		m_vecBorns[nCntPart]->GetTransformComponent()->RegisterParent(pParent->GetTransformComponent(), vOffsetPos, vOffsetRot);
	}

	//motion�̓ǂݍ���
	int nCntMotion = 0;
	fgets(strRead, sizeof(strRead), pFile);
	while (1)
	{
		char strComp2[] = "MOTIONSET";
		bFind = false;

		while (bFind != true)
		{
			if (fgets(strRead, sizeof(strRead), pFile) == NULL) { break; }

			for (unsigned int nCnt = 0; nCnt < strlen(strComp2);)
			{
				if (strRead[nCnt] == strComp2[nCnt]) { nCnt++; }
				else { break; }

				if (nCnt == strlen(strComp2)) { bFind = true; }
			}
		}

		if (bFind == false) { break; }

		m_apMotionData[(MOTION_PATTERN)nCntMotion] = new CMotionInfo;

		//LOOP
		fgets(strRead, sizeof(strRead), pFile);
		char strLoop[2] = {};
		for (unsigned int nCnt = strlen("\tLOOP = "), nCntLoop = 0; strRead[nCnt] != '\t'; nCnt++, nCntLoop++)
		{
			strLoop[nCntLoop] = strRead[nCnt];
		}

		m_apMotionData[(MOTION_PATTERN)nCntMotion]->m_bLoop
			= atoi(strLoop) != 0 ? true : false;

		//�L�[�t���[�����擾
		fgets(strRead, sizeof(strRead), pFile);
		char strKey[2] = {};
		for (unsigned int nCnt = strlen("\tNUM_KEY = "), nCntKey = 0; strRead[nCnt] != '\t'; nCnt++, nCntKey++)
		{
			strKey[nCntKey] = strRead[nCnt];
		}

		int nNumKey = atoi(strKey);
		m_apMotionData[(MOTION_PATTERN)nCntMotion]->m_vecMotionKey.resize(nNumKey);

		//�L�[�t���[���̓ǂݍ���
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{
			char strComp3[] = "\tKEYSET";
			bFind = false;

			while (bFind != true)
			{
				fgets(strRead, sizeof(strRead), pFile);

				for (unsigned int nCnt = 0; nCnt < strlen(strComp3);)
				{
					if (strRead[nCnt] == strComp3[nCnt]) { nCnt++; }
					else { break; }

					if (nCnt == strlen(strComp3)) { bFind = true; }
				}
			}

			CMotionKey motionKey;

			//Frame
			fgets(strRead, sizeof(strRead), pFile);
			char strFrame[4] = {};
			for (unsigned int nCnt = strlen("\t\tFRAME = "), nCntFrame = 0; strRead[nCnt] != '\n'; nCnt++, nCntFrame++)
			{
				strFrame[nCntFrame] = strRead[nCnt];
			}

			motionKey.m_nFrame = atoi(strFrame);

			//Set Key per Parts
			for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
			{
				//pos
				char strComp4[] = "\t\t\tPOS = ";
				bFind = false;

				while (bFind != true)
				{
					fgets(strRead, sizeof(strRead), pFile);

					for (unsigned int nCnt = 0; nCnt < strlen(strComp4);)
					{
						if (strRead[nCnt] == strComp4[nCnt]) { nCnt++; }
						else { break; }

						if (nCnt == strlen(strComp4)) { bFind = true; }
					}
				}

				char strPos[3][16] = {};
				int nCntAxis = 0;
				int nCntPos = 0;
				for (unsigned int nCnt = strlen("\t\t\tPOS = "); strRead[nCnt] != '\n'; nCnt++)
				{
					if (strRead[nCnt] == ' ')
					{
						nCntAxis++;
						nCntPos = 0;
						continue;
					}

					strPos[nCntAxis][nCntPos] = strRead[nCnt];
					nCntPos++;
				}

				auto vPos = CKFVec3((float)atof(strPos[0]), (float)atof(strPos[1]), (float)atof(strPos[2]));

				//Rot
				fgets(strRead, sizeof(strRead), pFile);
				char strRot[3][16] = {};
				nCntAxis = 0;
				int nCntRot = 0;
				for (unsigned int nCnt = strlen("\t\t\tROT = "); strRead[nCnt] != '\n'; nCnt++)
				{
					if (strRead[nCnt] == ' ')
					{
						nCntAxis++;
						nCntRot = 0;
						continue;
					}

					strRot[nCntAxis][nCntRot] = strRead[nCnt];
					nCntRot++;
				}

				auto vRot = CKFVec3((float)atof(strRot[0]), (float)atof(strRot[1]), (float)atof(strRot[2]));
				auto qRot = CKFMath::EulerToQuaternion(vRot);
				auto nodeKey = CNodeKey(vPos, qRot);
				motionKey.m_listNodesKey.push_back(nodeKey);
			}

			//MotionKey�̕ۑ�
			m_apMotionData[(MOTION_PATTERN)nCntMotion]->m_vecMotionKey[nCntKey] = motionKey;
		}

		nCntMotion++;
	}

	fclose(pFile);
}
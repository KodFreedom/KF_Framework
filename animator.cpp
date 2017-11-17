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
#include "animator.h"
#include "gameObject.h"
#include "gameObjectSpawner.h"
#include "transform.h"
#include "motionInfo.h"
#include "collisionSystem.h"
#include "sphereCollider.h"
#include "motionState.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
Animator::Animator(GameObject* const owner, const string& filePath)
	: Component(owner)
	, currentMotionState(nullptr)
	, isGrounded(false)
	, isAttack(false)
	, isJump(false)
	, movement(0.0f)
{
	bones.clear();
	analyzeFile(filePath);
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool Animator::Init(void)
{
	auto& tag = owner->GetTag();
	for (auto born : bones) { born->SetTag(tag); }
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void Animator::Uninit(void)
{
	bones.clear();
	SAFE_DELETE(currentMotionState);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void Animator::Update(void)
{
	if (!currentMotionState) return;
	currentMotionState->Update(*this);
}

//--------------------------------------------------------------------------------
//	�֐����FGetCurrentAnimationName
//  �֐������F���̃A�j���[�V��������Ԃ�
//	�����F	�Ȃ�
//	�߂�l�Fconst string& 
//--------------------------------------------------------------------------------
const string Animator::GetCurrentAnimationName(void)
{
	if (!currentMotionState) return "null";
	return currentMotionState->GetMotionName();
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FchangeMotionStatus
//  �֐������F���[�V�����X�e�[�^�X�̐؂�ւ�
//	�����F	motionStatus�F�V�������[�V�����X�e�[�^�X
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Animator::changeMotionStatus(MotionState* motionStatus)
{
	SAFE_DELETE(currentMotionState);
	currentMotionState = motionStatus;
}

//--------------------------------------------------------------------------------
//	�֐����FchangeMotion
//  �֐������F�A�N�V�����i�ړ��A���ԁA�U���j
//	�����F	vDirection�F�ړ�����
//			bJump�F���ԃt���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Animator::analyzeFile(const string& filePath)
{
	FILE* filePointer;
	char buffer[256];

	//file open
	fopen_s(&filePointer, filePath.c_str(), "r");

	if (!filePointer) return;

	//�p�[�c���̓ǂݍ���
	char compare1[] = "NUM_MODEL = ";
	bool found = false;

	while (found != true)
	{
		fgets(buffer, sizeof(buffer), filePointer);

		for (unsigned int count = 0; count < strlen(compare1);)
		{
			if (buffer[count] == compare1[count]) { ++count; }
			else { break; }

			if (count == strlen(compare1)) { found = true; }
		}
	}

	char strNum[16];
	for (unsigned int count = strlen(compare1), countNum = 0; buffer[count] != '\n'; ++count, countNum++)
	{
		strNum[countNum] = buffer[count];
	}

	//NumParts
	int partsNumber = atoi(strNum);
	bones.resize(partsNumber);

	//XFile�̓ǂݍ���
	for (int countPart = 0; countPart < partsNumber; countPart++)
	{
		//XFILE XFile;
		char compare2[] = "MODEL_FILENAME = ";
		found = false;

		while (found != true)
		{
			fgets(buffer, sizeof(buffer), filePointer);

			for (unsigned int count = 0; count < strlen(compare2);)
			{
				if (buffer[count] == compare2[count]) { ++count; }
				else { break; }

				if (count == strlen(compare2)) { found = true; }
			}
		}

		string modelPath;
		for (unsigned int count = strlen(compare2), countPath = 0; buffer[count] != '\t'; ++count, countPath++)
		{
			modelPath.push_back(buffer[count]);
		}
		
		auto fileInfo = Utility::AnalyzeFilePath(modelPath);
		GameObject* bone = nullptr;
		if (fileInfo.Type._Equal("x"))
		{
			bone = GameObjectSpawner::CreateXModel(modelPath, Vector3(0.0f), Vector3(0.0f), Vector3(1.0f));
		}
		else if (fileInfo.Type._Equal("model"))
		{
			bone = GameObjectSpawner::CreateModel(modelPath, Vector3(0.0f), Quaternion(0.0f), Vector3(1.0f));
		}
		
		bones[countPart] = bone;
	}

	//character���̓ǂݍ���
	char compare0[] = "CHARACTERSET";
	found = false;

	while (found != true)
	{
		fgets(buffer, sizeof(buffer), filePointer);

		for (unsigned int count = 0; count < strlen(compare0);)
		{
			if (buffer[count] == compare0[count]) { ++count; }
			else { break; }

			if (count == strlen(compare0)) { found = true; }
		}
	}

	//Move
	fgets(buffer, sizeof(buffer), filePointer);
	char strMove[8] = {};
	for (unsigned int count = strlen("\tMOVE = "), countMove = 0; buffer[count] != '\t'; ++count, countMove++)
	{
		strMove[countMove] = buffer[count];
	}
	//m_fMoveSpeed = (float)atof(strMove);

	//Jump
	fgets(buffer, sizeof(buffer), filePointer);
	char strJump[8] = {};
	for (unsigned int count = strlen("\tJUMP = "), countJump = 0; buffer[count] != '\t'; ++count, countJump++)
	{
		strJump[countJump] = buffer[count];
	}
	//m_fJumpSpeed = (float)atof(strJump);

	//Radius
	fgets(buffer, sizeof(buffer), filePointer);
	char strRadius[8] = {};
	for (unsigned int count = strlen("\tRADIUS = "), countRadius = 0; buffer[count] != '\t'; ++count, countRadius++)
	{
		strRadius[countRadius] = buffer[count];
	}
	//Radius = (float)atof(strRadius);

	//parts info�̓ǂݍ���
	for (int countPart = 0; countPart < partsNumber; ++countPart)
	{
		char compare2[] = "\tPARTSSET";
		found = false;

		while (found != true)
		{
			fgets(buffer, sizeof(buffer), filePointer);

			for (unsigned int count = 0; count < strlen(compare2);)
			{
				if (buffer[count] == compare2[count]) { ++count; }
				else { break; }

				if (count == strlen(compare2)) { found = true; }
			}
		}

		//Index
		fgets(buffer, sizeof(buffer), filePointer);

		//Parent
		fgets(buffer, sizeof(buffer), filePointer);
		char strParent[4] = {};
		for (unsigned int count = strlen("\t\tPARENT = "), countParent = 0; buffer[count] != '\t'; ++count, countParent++)
		{
			strParent[countParent] = buffer[count];
		}

		int nParent = atoi(strParent);

		//Pos
		fgets(buffer, sizeof(buffer), filePointer);
		char strPos[3][16] = {};
		int countAxis = 0;
		int countPos = 0;
		for (unsigned int count = strlen("\t\tPOS = "); buffer[count] != '\n'; ++count)
		{
			if (buffer[count] == ' ')
			{
				countAxis++;
				countPos = 0;
				continue;
			}

			strPos[countAxis][countPos] = buffer[count];
			countPos++;
		}

		auto vOffsetPos = Vector3((float)atof(strPos[0]), (float)atof(strPos[1]), (float)atof(strPos[2]));

		//Rot
		fgets(buffer, sizeof(buffer), filePointer);
		char strRot[3][16] = {};
		countAxis = 0;
		int countRot = 0;
		for (unsigned int count = strlen("\t\tROT = "); buffer[count] != '\n'; ++count)
		{
			if (buffer[count] == ' ')
			{
				countAxis++;
				countRot = 0;
				continue;
			}

			strRot[countAxis][countRot] = buffer[count];
			countRot++;
		}

		auto vOffsetRot = Vector3((float)atof(strRot[0]), (float)atof(strRot[1]), (float)atof(strRot[2]));
		
		GameObject* parent = nullptr;
		if (nParent < 0) { parent = owner; }
		else { parent = bones[nParent]; }
		bones[countPart]->GetTransform()->RegisterParent(parent->GetTransform(), vOffsetPos, vOffsetRot);
	}

	////motion�̓ǂݍ���
	//int countMotion = 0;
	//fgets(buffer, sizeof(buffer), filePointer);
	//while (1)
	//{
	//	char compare2[] = "MOTIONSET";
	//	found = false;

	//	while (found != true)
	//	{
	//		if (fgets(buffer, sizeof(buffer), filePointer) == NULL) { break; }

	//		for (unsigned int count = 0; count < strlen(compare2);)
	//		{
	//			if (buffer[count] == compare2[count]) { ++count; }
	//			else { break; }

	//			if (count == strlen(compare2)) { found = true; }
	//		}
	//	}

	//	if (found == false) { break; }

	//	m_apMotionData[(MotionPattern)countMotion] = new MotionInfo;

	//	//LOOP
	//	fgets(buffer, sizeof(buffer), filePointer);
	//	char strLoop[2] = {};
	//	for (unsigned int count = strlen("\tLOOP = "), countLoop = 0; buffer[count] != '\t'; ++count, countLoop++)
	//	{
	//		strLoop[countLoop] = buffer[count];
	//	}

	//	m_apMotionData[(MotionPattern)countMotion]->IsLoop
	//		= atoi(strLoop) != 0 ? true : false;

	//	//�L�[�t���[�����擾
	//	fgets(buffer, sizeof(buffer), filePointer);
	//	char strKey[2] = {};
	//	for (unsigned int count = strlen("\tNUM_KEY = "), countKey = 0; buffer[count] != '\t'; ++count, countKey++)
	//	{
	//		strKey[countKey] = buffer[count];
	//	}

	//	int nNumKey = atoi(strKey);
	//	m_apMotionData[(MotionPattern)countMotion]->MotionKeys.resize(nNumKey);

	//	//�L�[�t���[���̓ǂݍ���
	//	for (int countKey = 0; countKey < nNumKey; countKey++)
	//	{
	//		char compare3[] = "\tKEYSET";
	//		found = false;

	//		while (found != true)
	//		{
	//			fgets(buffer, sizeof(buffer), filePointer);

	//			for (unsigned int count = 0; count < strlen(compare3);)
	//			{
	//				if (buffer[count] == compare3[count]) { ++count; }
	//				else { break; }

	//				if (count == strlen(compare3)) { found = true; }
	//			}
	//		}

	//		MotionKey motionKey;

	//		//Frame
	//		fgets(buffer, sizeof(buffer), filePointer);
	//		char strFrame[4] = {};
	//		for (unsigned int count = strlen("\t\tFRAME = "), countFrame = 0; buffer[count] != '\n'; ++count, countFrame++)
	//		{
	//			strFrame[countFrame] = buffer[count];
	//		}

	//		motionKey.FrameNumber = atoi(strFrame);

	//		//Set Key per Parts
	//		for (int countPart = 0; countPart < partsNumber; countPart++)
	//		{
	//			//pos
	//			char compare4[] = "\t\t\tPOS = ";
	//			found = false;

	//			while (found != true)
	//			{
	//				fgets(buffer, sizeof(buffer), filePointer);

	//				for (unsigned int count = 0; count < strlen(compare4);)
	//				{
	//					if (buffer[count] == compare4[count]) { ++count; }
	//					else { break; }

	//					if (count == strlen(compare4)) { found = true; }
	//				}
	//			}

	//			char strPos[3][16] = {};
	//			int countAxis = 0;
	//			int countPos = 0;
	//			for (unsigned int count = strlen("\t\t\tPOS = "); buffer[count] != '\n'; ++count)
	//			{
	//				if (buffer[count] == ' ')
	//				{
	//					countAxis++;
	//					countPos = 0;
	//					continue;
	//				}

	//				strPos[countAxis][countPos] = buffer[count];
	//				countPos++;
	//			}

	//			auto Position = Vector3((float)atof(strPos[0]), (float)atof(strPos[1]), (float)atof(strPos[2]));

	//			//Rot
	//			fgets(buffer, sizeof(buffer), filePointer);
	//			char strRot[3][16] = {};
	//			countAxis = 0;
	//			int countRot = 0;
	//			for (unsigned int count = strlen("\t\t\tROT = "); buffer[count] != '\n'; ++count)
	//			{
	//				if (buffer[count] == ' ')
	//				{
	//					countAxis++;
	//					countRot = 0;
	//					continue;
	//				}

	//				strRot[countAxis][countRot] = buffer[count];
	//				countRot++;
	//			}

	//			auto vRot = Vector3((float)atof(strRot[0]), (float)atof(strRot[1]), (float)atof(strRot[2]));
	//			auto qRot = CKFMath::EulerToQuaternion(vRot);
	//			auto nodeKey = NodeKey(Position, qRot);
	//			motionKey.NodeKeys.push_back(nodeKey);
	//		}

	//		//MotionKey�̕ۑ�
	//		m_apMotionData[(MotionPattern)countMotion]->MotionKeys[countKey] = motionKey;
	//	}

	//	countMotion++;
	//}

	fclose(filePointer);
}
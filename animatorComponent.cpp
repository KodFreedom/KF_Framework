//--------------------------------------------------------------------------------
//
//　animatorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
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
//  クラス
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  コンストラクタ
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

	//ファイルからデータを読み込む
	analyzeFile(strPath);
}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool CAnimatorComponent::Init(void)
{
	auto& strTag = m_pGameObj->GetTag();
	for (auto pObj : m_vecBorns) { pObj->SetTag(strTag); }
	m_pMotionStatus = new CNormalMotionStatus;
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
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
//  更新処理
//--------------------------------------------------------------------------------
void CAnimatorComponent::Update(void)
{
	m_pMotionStatus->Update(*this);

	//Attack用
	//if (m_motionNow == MP_ATTACK)
	//{
	//	updateAttack();
	//}
}

//--------------------------------------------------------------------------------
//	関数名：changeMotion
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
void CAnimatorComponent::SetAttack(const bool& bAttack)
{
	if (!bAttack) { return; }
	m_pMotionStatus->ChangeMotion(*this, MP_ATTACK);
}

//--------------------------------------------------------------------------------
//	関数名：changeMotion
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
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
//	関数名：changeMotion
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
void CAnimatorComponent::SetJump(const bool& bJump)
{
	if (!bJump || !m_bIsGrounded) { return; }
	m_pMotionStatus->ChangeMotion(*this, MP_JUMP);
}

//--------------------------------------------------------------------------------
//	関数名：changeMotion
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
void CAnimatorComponent::SetMove(const float& fMovement)
{
	if (fMovement == 0.0f) { m_pMotionStatus->ChangeMotion(*this, MP_NEUTAL); }
	else { m_pMotionStatus->ChangeMotion(*this, MP_RUN); }
}

//--------------------------------------------------------------------------------
//	関数名：changeMotion
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
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
//	関数名：changeMotion
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
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
//	関数名：changeMotionStatus
//  関数説明：モーションステータスの切り替え
//	引数：	pMotionStatus：新しいモーションステータス
//	戻り値：なし
//--------------------------------------------------------------------------------
void CAnimatorComponent::changeMotionStatus(CMotionStatus* pMotionStatus)
{
	if (m_pMotionStatus) { delete m_pMotionStatus; }
	m_pMotionStatus = pMotionStatus;
}

//--------------------------------------------------------------------------------
//	関数名：getMotionNext
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//	戻り値：なし
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
//	関数名：changeMotion
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
void CAnimatorComponent::analyzeFile(const string& strPath)
{
	FILE* filePointer;
	char strRead[256];

	//file open
	fopen_s(&filePointer, strPath.c_str(), "r");

	if (!filePointer) { return; }

	//パーツ数の読み込み
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

	//XFileの読み込み
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

	//character情報の読み込み
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

	//parts infoの読み込み
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

	//motionの読み込み
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

		//キーフレーム数取得
		fgets(strRead, sizeof(strRead), filePointer);
		char strKey[2] = {};
		for (unsigned int count = strlen("\tNUM_KEY = "), countKey = 0; strRead[count] != '\t'; ++count, countKey++)
		{
			strKey[countKey] = strRead[count];
		}

		int nNumKey = atoi(strKey);
		m_apMotionData[(MOTION_PATTERN)countMotion]->m_vecMotionKey.resize(nNumKey);

		//キーフレームの読み込み
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

			//MotionKeyの保存
			m_apMotionData[(MOTION_PATTERN)countMotion]->m_vecMotionKey[countKey] = motionKey;
		}

		countMotion++;
	}

	fclose(filePointer);
}
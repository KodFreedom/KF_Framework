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
#include "gameObject3D.h"
#include "transformComponent.h"
#include "motionInfo.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CAnimatorComponent::CAnimatorComponent(CGameObject* const pGameObj, const string& strPath)
	: CComponent(pGameObj)
	, m_motionNow(MP_NEUTAL)
	, m_motionNext(MP_NEUTAL)
	, m_status(MS_CHANGE)
	, m_nKeyNow(0)
	, m_nCntFrame(0)
	, m_nCntChangeFrame(0)
{
	for (int nCnt = 0; nCnt < (int)MP_MAX; ++nCnt)
	{
		m_apMotionData[(MOTION_PATTERN)nCnt] = nullptr;
	}

	m_listNodes.clear();

	//ファイルからデータを読み込む
	analyzeFile(strPath);
}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool CAnimatorComponent::Init(void)
{
	changeMotion(m_motionNow);
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CAnimatorComponent::Uninit(void)
{
	for (auto pInfo : m_apMotionData)
	{
		if (pInfo)
		{
			delete pInfo;
			pInfo = nullptr;
		}
	}

	m_listNodes.clear();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CAnimatorComponent::Update(void)
{
	auto pMotionInfo = m_apMotionData[m_motionNow];
	auto& motionKey = pMotionInfo->m_vecMotionKey[m_nKeyNow];
	auto nFrame = (int)m_status * sc_nChangeFrame + (1 - (int)m_status) * pMotionInfo->m_vecMotionKey[m_nKeyNow].m_nFrame;
	auto itrNodeKey = motionKey.m_listNodesKey.begin();
	float fRate = (float)m_nCntFrame / nFrame;

	//Nodeごと位置回転更新
	for (auto pObj : m_listNodes)
	{
		auto pTrans = pObj->GetTransformComponent();
		auto vPosNew = CKFMath::LerpVec3(pTrans->GetPosNext(), itrNodeKey->c_vPos, fRate);
		auto qRotNew = CKFMath::SlerpQuaternion(pTrans->GetRotNext(), itrNodeKey->c_qRot, fRate);
		pTrans->SetPosNext(vPosNew);
		pTrans->SetRotNext(qRotNew);
		++itrNodeKey;
	}

	//フレームカウント
	m_nCntFrame++;

	//キーフレーム切り替え
	if (m_nCntFrame == nFrame)
	{
		m_status = MS_NORMAL;
		m_nCntFrame = 0;
		m_nKeyNow++;

		//モーション切り替え
		if (m_nKeyNow == pMotionInfo->m_vecMotionKey.size())
		{
			m_nKeyNow = 0;

			if (!pMotionInfo->m_bLoop)
			{
				changeMotion(m_motionNext);
			}
		}
	}
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
	if (fMovement == 0.0f)
	{
		changeMotion(MP_NEUTAL);
	}
	else
	{
		changeMotion(MP_RUN);
	}
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
void  CAnimatorComponent::changeMotion(const MOTION_PATTERN& motion)
{
	if (!checkCanChange(motion)) { return; }

	m_status = MS_CHANGE;
	m_motionNow = motion;
	m_nCntFrame = 0;
	m_nKeyNow = 0;

	//Motion Next
	switch (motion)
	{
	case MP_JUMP:
		m_motionNext = MP_LAND;
		break;
	default:
		m_motionNext = MP_NEUTAL;
		break;
	}
}

//--------------------------------------------------------------------------------
//	関数名：changeMotion
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
//void  CAnimatorComponent::changeMotionImmediately(const MOTION_PATTERN& motion)
//{
//
//}

//--------------------------------------------------------------------------------
//	関数名：changeMotion
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
bool  CAnimatorComponent::checkCanChange(const MOTION_PATTERN& motion)
{
	if (m_motionNow == motion) { return false; }

	switch (m_motionNow)
	{
	case MP_JUMP:
		if (motion == MP_LAND) { return false; }
		return false;
	case MP_LAND:
		if (motion == MP_NEUTAL) { return false; }
		return true;
	case MP_ATTACK:
		return false;
	default:
		break;
	}

	return true;
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
	FILE* pFile;
	char strRead[256];

	//file open
	fopen_s(&pFile, strPath.c_str(), "r");

	if (!pFile) { return; }

	//パーツ数の読み込み
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

	int nNumParts = atoi(strNum);

	//親設定用
	vector<CGameObject*> vecObj;
	vecObj.resize(nNumParts);

	//XFileの読み込み
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

		auto pNode = CGameObject3D::CreateXModel(strPath, CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
		m_listNodes.push_back(pNode);
		vecObj[nCntPart] = pNode;
	}

	//character情報の読み込み
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

	//parts infoの読み込み
	auto itr = m_listNodes.begin();
	for (int nCntPart = 0; nCntPart < nNumParts; ++nCntPart, ++itr)
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
		else { pParent = vecObj[nParent]; }
		(*itr)->GetTransformComponent()->RegisterParent(pParent->GetTransformComponent(), vOffsetPos, vOffsetRot);
	}

	//motionの読み込み
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

		//キーフレーム数取得
		fgets(strRead, sizeof(strRead), pFile);
		char strKey[2] = {};
		for (unsigned int nCnt = strlen("\tNUM_KEY = "), nCntKey = 0; strRead[nCnt] != '\t'; nCnt++, nCntKey++)
		{
			strKey[nCntKey] = strRead[nCnt];
		}

		int nNumKey = atoi(strKey);
		m_apMotionData[(MOTION_PATTERN)nCntMotion]->m_vecMotionKey.resize(nNumKey);

		//キーフレームの読み込み
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

			//MotionKeyの保存
			m_apMotionData[(MOTION_PATTERN)nCntMotion]->m_vecMotionKey[nCntKey] = motionKey;
		}

		nCntMotion++;
	}

	fclose(pFile);
}
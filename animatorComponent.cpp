//--------------------------------------------------------------------------------
//
//　animatorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "animatorComponent.h"
#include "gameObject.h"
#include "transformComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
////  コンストラクタ
////--------------------------------------------------------------------------------
//CAnimatorComponent::CAnimatorComponent(CGameObject* const pGameObj, const string& strPath)
//	: CComponent(pGameObj)
//	, m_status(MS_NORMAL)
//	, m_nCntChangeFrame(0)
//{
//	m_vecPartsMotionInfo.clear();
//	m_vecParts.clear();
//
//	//ファイルからデータを読み込む
//	AnalyzeFile(strPath);
//
//	m_motionInfo.nCntFrame = 0;
//	m_motionInfo.nKeyNow = 0;
//	m_motionInfo.motionNow = MT_NEUTAL;
//}
//
////--------------------------------------------------------------------------------
////  初期化
////--------------------------------------------------------------------------------
//bool CAnimatorComponent::Init(void)
//{
//	SetMotionImmediately(MT_NEUTAL);
//	return true;
//}
//
////--------------------------------------------------------------------------------
////  コンストラクタ
////--------------------------------------------------------------------------------
//void CAnimatorComponent::Uninit(void)
//{
//
//}
//
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void CAnimatorComponent::Update(void)
{
	for (auto pObj : m_listNodes)
	{
		auto pTrans = pObj->GetTransformComponent();
		auto vPos = pTrans->GetPosNext();
		auto qRot = pTrans->GetRotNext();
	}
	////パーツ更新
	//int nFrame = 0;
	//int nNumKey = 0;
	//bool bLoop = false;
	//int nNumParts = (int)m_vecParts.size();
	//for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
	//{
	//	CKFVec3 vPartPos = m_vecParts[nCntPart]->GetTransformComponent()->GetPosNext();
	//	CKFVec3 vPartRot = m_vecParts[nCntPart]->GetTransformComponent()->GetRot();
	//	CKFVec3 vKeyPos = m_vecPartsMotionInfo[nCntPart][m_motionInfo.motionNow].vecKeyFrame[m_motionInfo.nKeyNow].vPos;
	//	CKFVec3 vKeyRot = m_vecPartsMotionInfo[nCntPart][m_motionInfo.motionNow].vecKeyFrame[m_motionInfo.nKeyNow].vRot;
	//	CKFVec3 vRotDifference = vKeyRot - vPartRot;
	//	CKFMath::NormalizeRotInPi(vRotDifference);
	//	CKFVec3 vMovement;
	//	CKFVec3 vRot;

	//	nFrame = (int)m_status * sc_nChangeFrame + (1 - (int)m_status) * m_vecPartsMotionInfo[nCntPart][m_motionInfo.motionNow].vecKeyFrame[m_motionInfo.nKeyNow].nFrame;
	//	nNumKey = m_vecPartsMotionInfo[nCntPart][m_motionInfo.motionNow].vecKeyFrame.size();
	//	bLoop = m_vecPartsMotionInfo[nCntPart][m_motionInfo.motionNow].bLoop;

	//	if ((nFrame - m_motionInfo.nCntFrame) != 0)
	//	{
	//		vMovement = (vKeyPos - vPartPos) / (float)(nFrame - m_motionInfo.nCntFrame);
	//		vRot = vRotDifference / (float)(nFrame - m_motionInfo.nCntFrame);
	//	}

	//	CKFMath::NormalizeRotInTwoPi(vRot);
	//	m_vecParts[nCntPart]->GetTransformComponent()->MovePosNext(vMovement);
	//	m_vecParts[nCntPart]->GetTransformComponent()->RotByEuler(vRot);
	//}

	////フレームカウント
	//m_motionInfo.nCntFrame++;

	////キーフレーム切り替え
	//if (m_motionInfo.nCntFrame == nFrame)
	//{
	//	m_status = MS_NORMAL;
	//	m_motionInfo.nCntFrame = 0;
	//	m_motionInfo.nKeyNow++;

	//	//モーション切り替え
	//	if (m_motionInfo.nKeyNow == nNumKey)
	//	{
	//		m_motionInfo.nKeyNow = 0;

	//		if (bLoop != true)
	//		{
	//			ChangeMotion(m_motionInfo.motionNext);
	//		}
	//	}
	//}
}

////--------------------------------------------------------------------------------
////  ファイルからデータを読み込む
////--------------------------------------------------------------------------------
//void CAnimatorComponent::AnalyzeFile(const string& strPath)
//{
//	FILE* pFile;
//	char strRead[256];
//
//	//file open
//	fopen_s(&pFile, strPath.c_str(), "r");
//
//	if (!pFile) { return; }
//
//	//パーツ数の読み込み
//	char strComp1[] = "NUM_MODEL = ";
//	bool bFind = false;
//
//	while (bFind != true)
//	{
//		fgets(strRead, sizeof(strRead), pFile);
//
//		for (unsigned int nCnt = 0; nCnt < strlen(strComp1);)
//		{
//			if (strRead[nCnt] == strComp1[nCnt]) { nCnt++; }
//			else { break; }
//
//			if (nCnt == strlen(strComp1)) { bFind = true; }
//		}
//	}
//
//	char strNum[16];
//	for (unsigned int nCnt = strlen(strComp1), nCntNum = 0; strRead[nCnt] != '\n'; nCnt++, nCntNum++)
//	{
//		strNum[nCntNum] = strRead[nCnt];
//	}
//
//	int nNumParts = atoi(strNum);
//
//	//パーツオブジェクト作成
//	for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
//	{
//		m_vecParts.push_back(new CGameObject(GOM::PRI_3D));
//	}
//
//	//XFileの読み込み
//	for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
//	{
//		//XFILE XFile;
//		char strComp2[] = "MODEL_FILENAME = ";
//		bFind = false;
//
//		while (bFind != true)
//		{
//			fgets(strRead, sizeof(strRead), pFile);
//
//			for (unsigned int nCnt = 0; nCnt < strlen(strComp2);)
//			{
//				if (strRead[nCnt] == strComp2[nCnt]) { nCnt++; }
//				else { break; }
//
//				if (nCnt == strlen(strComp2)) { bFind = true; }
//			}
//		}
//
//		string strPath;
//		for (unsigned int nCnt = strlen(strComp2), nCntPath = 0; strRead[nCnt] != '\t'; nCnt++, nCntPath++)
//		{
//			strPath[nCntPath] = strRead[nCnt];
//		}
//
//		GetManager()->GetModelManager()->BindModel(strPath);
//		CModelMeshComponent* pMesh = new CModelMeshComponent(m_vecParts[nCntPart]);
//		pMesh->SetModelPath(strPath);
//		CModelMeshDrawComponent* pDraw = new CModelMeshDrawComponent(m_vecParts[nCntPart]);
//		m_vecParts[nCntPart]->SetMeshComponent(pMesh);
//		m_vecParts[nCntPart]->SetDrawComponent(pDraw);
//	}
//
//	//character情報の読み込み
//	char strComp0[] = "CHARACTERSET";
//	bFind = false;
//
//	while (bFind != true)
//	{
//		fgets(strRead, sizeof(strRead), pFile);
//
//		for (unsigned int nCnt = 0; nCnt < strlen(strComp0);)
//		{
//			if (strRead[nCnt] == strComp0[nCnt]) { nCnt++; }
//			else { break; }
//
//			if (nCnt == strlen(strComp0)) { bFind = true; }
//		}
//	}
//
//	//Move
//	fgets(strRead, sizeof(strRead), pFile);
//	char strMove[8] = {};
//	for (unsigned int nCnt = strlen("\tMOVE = "), nCntMove = 0; strRead[nCnt] != '\t'; nCnt++, nCntMove++)
//	{
//		strMove[nCntMove] = strRead[nCnt];
//	}
//	//m_fMoveSpeed = (float)atof(strMove);
//
//	//Jump
//	fgets(strRead, sizeof(strRead), pFile);
//	char strJump[8] = {};
//	for (unsigned int nCnt = strlen("\tJUMP = "), nCntJump = 0; strRead[nCnt] != '\t'; nCnt++, nCntJump++)
//	{
//		strJump[nCntJump] = strRead[nCnt];
//	}
//	//m_fJumpSpeed = (float)atof(strJump);
//
//	//Radius
//	fgets(strRead, sizeof(strRead), pFile);
//	char strRadius[8] = {};
//	for (unsigned int nCnt = strlen("\tRADIUS = "), nCntRadius = 0; strRead[nCnt] != '\t'; nCnt++, nCntRadius++)
//	{
//		strRadius[nCntRadius] = strRead[nCnt];
//	}
//	//m_fRadius = (float)atof(strRadius);
//
//	//parts infoの読み込み
//	for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
//	{
//		//PARTS_INFO partsInfo;
//		
//
//		char strComp2[] = "\tPARTSSET";
//		bFind = false;
//
//		while (bFind != true)
//		{
//			fgets(strRead, sizeof(strRead), pFile);
//
//			for (unsigned int nCnt = 0; nCnt < strlen(strComp2);)
//			{
//				if (strRead[nCnt] == strComp2[nCnt]) { nCnt++; }
//				else { break; }
//
//				if (nCnt == strlen(strComp2)) { bFind = true; }
//			}
//		}
//
//		//Index
//		fgets(strRead, sizeof(strRead), pFile);
//
//		//Parent
//		fgets(strRead, sizeof(strRead), pFile);
//		char strParent[4] = {};
//		for (unsigned int nCnt = strlen("\t\tPARENT = "), nCntParent = 0; strRead[nCnt] != '\t'; nCnt++, nCntParent++)
//		{
//			strParent[nCntParent] = strRead[nCnt];
//		}
//
//		int nParent = atoi(strParent);
//
//		//Pos
//		fgets(strRead, sizeof(strRead), pFile);
//		char strPos[3][16] = {};
//		int nCntAxis = 0;
//		int nCntPos = 0;
//		for (unsigned int nCnt = strlen("\t\tPOS = "); strRead[nCnt] != '\n'; nCnt++)
//		{
//			if (strRead[nCnt] == ' ')
//			{
//				nCntAxis++;
//				nCntPos = 0;
//				continue;
//			}
//
//			strPos[nCntAxis][nCntPos] = strRead[nCnt];
//			nCntPos++;
//		}
//
//		CKFVec3 vPos = CKFVec3((float)atof(strPos[0]), (float)atof(strPos[1]), (float)atof(strPos[2]));
//
//		//Rot
//		fgets(strRead, sizeof(strRead), pFile);
//		char strRot[3][16] = {};
//		nCntAxis = 0;
//		int nCntRot = 0;
//		for (unsigned int nCnt = strlen("\t\tROT = "); strRead[nCnt] != '\n'; nCnt++)
//		{
//			if (strRead[nCnt] == ' ')
//			{
//				nCntAxis++;
//				nCntRot = 0;
//				continue;
//			}
//
//			strRot[nCntAxis][nCntRot] = strRead[nCnt];
//			nCntRot++;
//		}
//
//		CKFVec3 vRot = CKFVec3((float)atof(strRot[0]), (float)atof(strRot[1]), (float)atof(strRot[2]));
//		
//		CGameObject* pParent = nullptr;
//		if (nParent == -1) { pParent = m_pGameObj; }
//		else { pParent = m_vecParts.at(nParent); }
//		m_vecParts[nCntPart]->GetTransformComponent()->RegisterParent(pParent->GetTransformComponent(), vPos, vRot);
//	}
//
//	//motionの読み込み
//	VEC_MOTION vectorMotion;
//	vectorMotion.clear();
//	for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
//	{
//		m_vecPartsMotionInfo.push_back(vectorMotion);
//	}
//
//	int nCntMotion = 0;
//	fgets(strRead, sizeof(strRead), pFile);
//	while (1)
//	{
//		char strComp2[] = "MOTIONSET";
//		bFind = false;
//
//		while (bFind != true)
//		{
//			if (fgets(strRead, sizeof(strRead), pFile) == NULL) { break; }
//
//			for (unsigned int nCnt = 0; nCnt < strlen(strComp2);)
//			{
//				if (strRead[nCnt] == strComp2[nCnt]) { nCnt++; }
//				else { break; }
//
//				if (nCnt == strlen(strComp2)) { bFind = true; }
//			}
//		}
//
//		if (bFind == false) { break; }
//
//		//LOOP
//		fgets(strRead, sizeof(strRead), pFile);
//		char strLoop[2] = {};
//		for (unsigned int nCnt = strlen("\tLOOP = "), nCntLoop = 0; strRead[nCnt] != '\t'; nCnt++, nCntLoop++)
//		{
//			strLoop[nCntLoop] = strRead[nCnt];
//		}
//
//		bool bLoop = atoi(strLoop) != 0 ? true : false;
//
//		//Motion Set
//		MOTION motion;
//		motion.bLoop = bLoop;
//		motion.vecKeyFrame.clear();
//
//		for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
//		{
//			m_vecPartsMotionInfo[nCntPart].push_back(motion);
//		}
//
//		//キーフレーム数取得
//		fgets(strRead, sizeof(strRead), pFile);
//		char strKey[2] = {};
//		for (unsigned int nCnt = strlen("\tNUM_KEY = "), nCntKey = 0; strRead[nCnt] != '\t'; nCnt++, nCntKey++)
//		{
//			strKey[nCntKey] = strRead[nCnt];
//		}
//
//		int nNumKey = atoi(strKey);
//
//		//キーフレームの読み込み
//		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
//		{
//			char strComp3[] = "\tKEYSET";
//			bFind = false;
//
//			while (bFind != true)
//			{
//				fgets(strRead, sizeof(strRead), pFile);
//
//				for (unsigned int nCnt = 0; nCnt < strlen(strComp3);)
//				{
//					if (strRead[nCnt] == strComp3[nCnt]) { nCnt++; }
//					else { break; }
//
//					if (nCnt == strlen(strComp3)) { bFind = true; }
//				}
//			}
//
//			//Frame
//			fgets(strRead, sizeof(strRead), pFile);
//			char strFrame[4] = {};
//			for (unsigned int nCnt = strlen("\t\tFRAME = "), nCntFrame = 0; strRead[nCnt] != '\n'; nCnt++, nCntFrame++)
//			{
//				strFrame[nCntFrame] = strRead[nCnt];
//			}
//
//			int nFrame = atoi(strFrame);
//
//			//Set Key per Parts
//			for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
//			{
//				//pos
//				char strComp4[] = "\t\t\tPOS = ";
//				bFind = false;
//
//				while (bFind != true)
//				{
//					fgets(strRead, sizeof(strRead), pFile);
//
//					for (unsigned int nCnt = 0; nCnt < strlen(strComp4);)
//					{
//						if (strRead[nCnt] == strComp4[nCnt]) { nCnt++; }
//						else { break; }
//
//						if (nCnt == strlen(strComp4)) { bFind = true; }
//					}
//				}
//
//				char strPos[3][16] = {};
//				int nCntAxis = 0;
//				int nCntPos = 0;
//				for (unsigned int nCnt = strlen("\t\t\tPOS = "); strRead[nCnt] != '\n'; nCnt++)
//				{
//					if (strRead[nCnt] == ' ')
//					{
//						nCntAxis++;
//						nCntPos = 0;
//						continue;
//					}
//
//					strPos[nCntAxis][nCntPos] = strRead[nCnt];
//					nCntPos++;
//				}
//
//				CKFVec3 vPos = CKFVec3((float)atof(strPos[0]), (float)atof(strPos[1]), (float)atof(strPos[2]));
//
//				//Rot
//				fgets(strRead, sizeof(strRead), pFile);
//				char strRot[3][16] = {};
//				nCntAxis = 0;
//				int nCntRot = 0;
//				for (unsigned int nCnt = strlen("\t\t\tROT = "); strRead[nCnt] != '\n'; nCnt++)
//				{
//					if (strRead[nCnt] == ' ')
//					{
//						nCntAxis++;
//						nCntRot = 0;
//						continue;
//					}
//
//					strRot[nCntAxis][nCntRot] = strRead[nCnt];
//					nCntRot++;
//				}
//
//				CKFVec3 vRot = CKFVec3((float)atof(strRot[0]), (float)atof(strRot[1]), (float)atof(strRot[2]));
//
//				KEY_FRAME keyFrame;
//				keyFrame.nFrame = nFrame;
//				keyFrame.vPos = vPos;
//				keyFrame.vRot = vRot;
//				m_vecPartsMotionInfo[nCntPart][nCntMotion].vecKeyFrame.push_back(keyFrame);
//			}
//		}
//		nCntMotion++;
//	}
//
//	fclose(pFile);
//}
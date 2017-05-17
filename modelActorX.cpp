//--------------------------------------------------------------------------------
//
//　modelCharacterX.cpp
//	Author : Xu Wenjie
//	Date   : 2017-01-23
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "rendererDX.h"
#include "modelActorX.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CModelActorX::CModelActorX()
{
	m_actorInfo.vectorPartsInfoDefault.clear();
	m_actorInfo.vectorPartsMotionInfo.clear();
	m_actorInfo.vectorPartsXFileInfo.clear();
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
KFRESULT CModelActorX::Init(const LPCSTR &pTxtPath)
{
	FILE *pFile;
	char strRead[256];

	//file open
	fopen_s(&pFile, pTxtPath, "r");

	if (pFile != NULL)
	{
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

		//XFileの読み込み
		for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
		{
			XFILE XFile;
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

			char strPath[256] = {};
			for (unsigned int nCnt = strlen(strComp2), nCntPath = 0; strRead[nCnt] != '\t'; nCnt++, nCntPath++)
			{
				strPath[nCntPath] = strRead[nCnt];
			}

			LoadXFile(&XFile, strPath);

			//Save XFile
			m_actorInfo.vectorPartsXFileInfo.push_back(XFile);
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
		m_fMoveSpeed = (float)atof(strMove);

		//Jump
		fgets(strRead, sizeof(strRead), pFile);
		char strJump[8] = {};
		for (unsigned int nCnt = strlen("\tJUMP = "), nCntJump = 0; strRead[nCnt] != '\t'; nCnt++, nCntJump++)
		{
			strJump[nCntJump] = strRead[nCnt];
		}
		m_fJumpSpeed = (float)atof(strJump);

		//Radius
		fgets(strRead, sizeof(strRead), pFile);
		char strRadius[8] = {};
		for (unsigned int nCnt = strlen("\tRADIUS = "), nCntRadius = 0; strRead[nCnt] != '\t'; nCnt++, nCntRadius++)
		{
			strRadius[nCntRadius] = strRead[nCnt];
		}
		m_fRadius = (float)atof(strRadius);

		//parts infoの読み込み
		for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
		{
			PARTS_INFO partsInfo;
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

			partsInfo.nParentID = atoi(strParent);

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

			partsInfo.vPos = CKFVec3((float)atof(strPos[0]), (float)atof(strPos[1]), (float)atof(strPos[2]));

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

			partsInfo.vRot = CKFVec3((float)atof(strRot[0]), (float)atof(strRot[1]), (float)atof(strRot[2]));

			//Save PartsInfo
			m_actorInfo.vectorPartsInfoDefault.push_back(partsInfo);
		}

		//motionの読み込み
		VEC_MOTION vectorMotion;
		vectorMotion.clear();
		for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
		{
			m_actorInfo.vectorPartsMotionInfo.push_back(vectorMotion);
		}

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

			//LOOP
			fgets(strRead, sizeof(strRead), pFile);
			char strLoop[2] = {};
			for (unsigned int nCnt = strlen("\tLOOP = "), nCntLoop = 0; strRead[nCnt] != '\t'; nCnt++, nCntLoop++)
			{
				strLoop[nCntLoop] = strRead[nCnt];
			}

			bool bLoop = atoi(strLoop) != 0 ? true : false;

			//Motion Set
			MOTION motion;
			motion.bLoop = bLoop;
			motion.vectorKeyFrame.clear();

			for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
			{
				m_actorInfo.vectorPartsMotionInfo[nCntPart].push_back(motion);
			}

			//キーフレーム数取得
			fgets(strRead, sizeof(strRead), pFile);
			char strKey[2] = {};
			for (unsigned int nCnt = strlen("\tNUM_KEY = "), nCntKey = 0; strRead[nCnt] != '\t'; nCnt++, nCntKey++)
			{
				strKey[nCntKey] = strRead[nCnt];
			}

			int nNumKey = atoi(strKey);

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

				//Frame
				fgets(strRead, sizeof(strRead), pFile);
				char strFrame[4] = {};
				for (unsigned int nCnt = strlen("\t\tFRAME = "), nCntFrame = 0; strRead[nCnt] != '\n'; nCnt++, nCntFrame++)
				{
					strFrame[nCntFrame] = strRead[nCnt];
				}

				int nFrame = atoi(strFrame);

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

					CKFVec3 vPos = CKFVec3((float)atof(strPos[0]), (float)atof(strPos[1]), (float)atof(strPos[2]));

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

					CKFVec3 vRot = CKFVec3((float)atof(strRot[0]), (float)atof(strRot[1]), (float)atof(strRot[2]));

					KEY_FRAME keyFrame;
					keyFrame.nFrame = nFrame;
					keyFrame.vPos = vPos;
					keyFrame.vRot = vRot;
					m_actorInfo.vectorPartsMotionInfo[nCntPart][nCntMotion].vectorKeyFrame.push_back(keyFrame);
				}
			}
			nCntMotion++;
		}

		fclose(pFile);
		return KF_SUCCEEDED;
	}

	return KF_FAILED;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CModelActorX::Uninit(void)
{
	int nNumParts = (int)m_actorInfo.vectorPartsInfoDefault.size();
	for(int nCntParts = 0;nCntParts < nNumParts;nCntParts++)
	{
		int nNumMotion = (int)m_actorInfo.vectorPartsMotionInfo[nCntParts].size();
		for (int nCntMotion = 0; nCntMotion < nNumMotion; nCntMotion++)
		{
			m_actorInfo.vectorPartsMotionInfo[nCntParts][nCntMotion].vectorKeyFrame.clear();
		}

		m_actorInfo.vectorPartsMotionInfo[nCntParts].clear();
		ReleaseXFile(&m_actorInfo.vectorPartsXFileInfo[nCntParts]);
	}

	m_actorInfo.vectorPartsInfoDefault.clear();
	m_actorInfo.vectorPartsMotionInfo.clear();
	m_actorInfo.vectorPartsXFileInfo.clear();
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CModelActorX::Draw(const CKFMtx44 &mtxWorldParents, std::vector<PARTS_INFO> &vectorParts)
{
	Draw(mtxWorldParents, vectorParts, CMM::MAT_MAX);
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CModelActorX::Draw(const CKFMtx44 &mtxWorldParents, std::vector<PARTS_INFO> &vectorParts, const CMM::MATERIAL &matType)
{
	//パーツ数を比較する
	if (m_actorInfo.vectorPartsInfoDefault.size() != vectorParts.size()) { return; }

	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	for (int nCntParts = 0; nCntParts < (int)vectorParts.size(); nCntParts++)
	{
		CKFMtx44 mtxRot;
		CKFMtx44 mtxPos;

		//単位行列に初期化
		CKFMath::MtxIdentity(&vectorParts[nCntParts].mtxWorld);

		//モーション行列
		//回転(Y->X->Z)
		CKFMath::MtxRotationYawPitchRoll(&mtxRot, vectorParts[nCntParts].vRot);
		vectorParts[nCntParts].mtxWorld *= mtxRot;

		//平行移動
		CKFMath::MtxTranslation(&mtxPos, vectorParts[nCntParts].vPos);
		vectorParts[nCntParts].mtxWorld *= mtxPos;

		//親に対する相対位置行列
		//回転(Y->X->Z)
		CKFMath::MtxRotationYawPitchRoll(&mtxRot, m_actorInfo.vectorPartsInfoDefault[nCntParts].vRot);
		vectorParts[nCntParts].mtxWorld *= mtxRot;

		//平行移動
		CKFMath::MtxTranslation(&mtxPos, m_actorInfo.vectorPartsInfoDefault[nCntParts].vPos);
		vectorParts[nCntParts].mtxWorld *= mtxPos;

		//親パーツチェック
		if (vectorParts[nCntParts].nParentID == -1)
		{//親がないの場合引数のワールド行列をかける
			vectorParts[nCntParts].mtxWorld *= mtxWorldParents;
		}
		else
		{//親のワールド行列をかける
			vectorParts[nCntParts].mtxWorld *= vectorParts[vectorParts[nCntParts].nParentID].mtxWorld;
		}

		//デバイスに設定
		D3DXMATRIX mtx = vectorParts[nCntParts].mtxWorld;
		pDevice->SetTransform(D3DTS_WORLD, &mtx);

		if (matType == CMM::MAT_MAX)
		{
			DrawXFile(m_actorInfo.vectorPartsXFileInfo[nCntParts]);
		}
		else
		{
			DrawXFile(m_actorInfo.vectorPartsXFileInfo[nCntParts], matType);
		}	
	}
}

//--------------------------------------------------------------------------------
//  初期状態のパーツ情報を取得する
//--------------------------------------------------------------------------------
std::vector<CModelActorX::PARTS_INFO> CModelActorX::GetDefaultPartsInfo(void) const
{
	return m_actorInfo.vectorPartsInfoDefault;
}

//--------------------------------------------------------------------------------
//  モーション情報を取得する
//--------------------------------------------------------------------------------
const std::vector<CModelActorX::VEC_MOTION>	&CModelActorX::GetPartsMotionInfo(void) const
{
	return m_actorInfo.vectorPartsMotionInfo;
}

//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
CModelActorX *CModelActorX::Create(const LPCSTR &pTxtPath)
{
	CModelActorX *pActor = NULL;
	pActor = new CModelActorX;
	pActor->Init(pTxtPath);
	pActor->m_modelType = CMOM::XFILE_MOTION;
	return pActor;
}
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
#include "model.h"
#include "modelCharacterX.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CModelCharacterX::CModelCharacterX()
{
	m_vectorParts.clear();
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
KFRESULT CModelCharacterX::Init(const LPCSTR &pTxtPath)
{
	FILE *pFile;
	char strRead[256];

	//file open
	fopen_s(&pFile, pTxtPath, "r");

	if (pFile != NULL)
	{
		//Num Model
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

		//model path
		for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
		{
			PARTS parts;
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

			LoadXFile(&parts.XFileInfo, strPath);
			m_vectorParts.push_back(parts);
		}

		//character set
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

		//parts set
		for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
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

			m_vectorParts[nCntPart].nParentID = atoi(strParent);

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

			m_vectorParts[nCntPart].vPos = CKFVec3((float)atof(strPos[0]), (float)atof(strPos[1]), (float)atof(strPos[2]));

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

			m_vectorParts[nCntPart].vRot = CKFVec3((float)atof(strRot[0]), (float)atof(strRot[1]), (float)atof(strRot[2]));
		}

		//motion
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
				m_vectorParts[nCntPart].vectorMotion.push_back(motion);
			}

			//Num Key
			fgets(strRead, sizeof(strRead), pFile);
			char strKey[2] = {};
			for (unsigned int nCnt = strlen("\tNUM_KEY = "), nCntKey = 0; strRead[nCnt] != '\t'; nCnt++, nCntKey++)
			{
				strKey[nCntKey] = strRead[nCnt];
			}

			int nNumKey = atoi(strKey);

			//Key set
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
					m_vectorParts[nCntPart].vectorMotion[nCntMotion].vectorKeyFrame.push_back(keyFrame);
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
void CModelCharacterX::Uninit(void)
{
	for(int nCntParts = 0;nCntParts < (int)m_vectorParts.size();nCntParts++)
	{
		for (int nCntMotion = 0; nCntMotion < (int)m_vectorParts[nCntParts].vectorMotion.size(); nCntMotion++)
		{
			m_vectorParts[nCntParts].vectorMotion[nCntMotion].vectorKeyFrame.clear();
		}

		m_vectorParts[nCntParts].vectorMotion.clear();
		ReleaseXFile(&m_vectorParts[nCntParts].XFileInfo);
	}

	m_vectorParts.clear();
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CModelCharacterX::Draw(const CKFVec3 &vPosParents, const CKFVec3 &vRotParents, const MOTION_INFO &info)
{
	//int nFrame;
	//int nNumKey;
	//bool bLoop;

	////パーツ更新
	//for (int nCntPart = 0; nCntPart < m_nNumParts; nCntPart++)
	//{
	//	D3DXVECTOR3 vPartPos = m_apParts[nCntPart]->GetPos();
	//	D3DXVECTOR3 vPartRot = m_apParts[nCntPart]->GetRot();

	//	D3DXVECTOR3 vKeyPos = m_apMotion[nCntPart][m_nMotionNow]->GetKey(m_nKeyNow)->GetPos();
	//	D3DXVECTOR3 vKeyRot = m_apMotion[nCntPart][m_nMotionNow]->GetKey(m_nKeyNow)->GetRot();
	//	D3DXVECTOR3 vChangeRot = CalculateRotChange(vKeyRot, vPartRot);

	//	nFrame = m_apMotion[nCntPart][m_nMotionNow]->GetKey(m_nKeyNow)->GetFrame();
	//	nNumKey = m_apMotion[nCntPart][m_nMotionNow]->GetNumKey();
	//	bLoop = m_apMotion[nCntPart][m_nMotionNow]->GetLoop();

	//	if ((nFrame - m_nCntFrame) != 0)
	//	{
	//		vPartPos += (vKeyPos - vPartPos) / (float)(nFrame - m_nCntFrame);
	//		vPartRot += vChangeRot / (float)(nFrame - m_nCntFrame);
	//	}

	//	CKFMath::NormalizeRot(&vPartRot);
	//	m_apParts[nCntPart]->SetPos(vPartPos);
	//	m_apParts[nCntPart]->SetRot(vPartRot);
	//}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CModelCharacterX::Draw(const CKFVec3 &vPosParents, const CKFVec3 &vRotParents, const MOTION_INFO &info, const CMM::MATERIAL &matType)
{

}
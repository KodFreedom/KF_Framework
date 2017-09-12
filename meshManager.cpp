//--------------------------------------------------------------------------------
//	メッシュマネージャ
//　meshManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-15
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "meshManager.h"
#include "mesh.h"
#include "camera.h"
#include "KF_Utility.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  中身を削除する
//--------------------------------------------------------------------------------
void CMeshManager::UnloadAll(void)
{
	for (auto itr = m_umMesh.begin(); itr != m_umMesh.end();)
	{
		SAFE_RELEASE(itr->second.pMesh->m_pVtxBuffer);
		SAFE_RELEASE(itr->second.pMesh->m_pIdxBuffer);
		itr = m_umMesh.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//  メッシュの追加
//--------------------------------------------------------------------------------
void CMeshManager::UseMesh(const string& strName)
{
	auto itr = m_umMesh.find(strName);
	if (itr != m_umMesh.end()) 
	{ //すでに存在してる
		itr->second.usNumUsers++;
		return;
	}

	//メッシュの作成
	MESH mesh;
	mesh.usNumUsers = 1;
	mesh.pMesh = createMesh(strName);

	//保存
	m_umMesh.emplace(strName, mesh);
}

//--------------------------------------------------------------------------------
//	関数名：UseMesh
//  関数説明：メッシュの使用
//	引数：	strFileName：メッシュファイルの名前
//			strTexName：テクスチャの名前（Out）
//	戻り値：なし
//--------------------------------------------------------------------------------
void CMeshManager::UseMesh(const string& strFileName, string& texName)
{
	auto itr = m_umMesh.find(strFileName);
	if (itr != m_umMesh.end())
	{ //すでに存在してる
		itr->second.usNumUsers++;
		texName = itr->second.strTex;
		return;
	}

	if (strFileName.find(".mesh") != string::npos)
	{
		//メッシュの作成
		MESH mesh;
		mesh.usNumUsers = 1;
		mesh.pMesh = loadFromMesh(strFileName, texName);
		mesh.strTex = texName;

		//保存
		m_umMesh.emplace(strFileName, mesh);
		return;
	}

	if (strFileName.find(".x") != string::npos)
	{
		//メッシュの作成
		MESH mesh;
		mesh.usNumUsers = 1;
		mesh.pMesh = loadFromXFile(strFileName, texName);
		mesh.strTex = texName;

		//保存
		m_umMesh.emplace(strFileName, mesh);
		return;
	}
}

//--------------------------------------------------------------------------------
//	関数名：DisuseMesh
//  関数説明：メッシュの破棄
//	引数：	strName：メッシュの名前
//	戻り値：なし
//--------------------------------------------------------------------------------
void CMeshManager::DisuseMesh(const string& strName)
{
	auto itr = m_umMesh.find(strName);
	itr->second.usNumUsers--;
	if (itr->second.usNumUsers == 0)
	{//誰も使ってないので破棄する
		delete itr->second.pMesh;
		m_umMesh.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//	関数名：CreateEditorField
//  関数説明：エディタ用フィールドの作成
//	引数：	nNumBlockX：X方向のブロック数
//			nNumBlockZ：Z方向のブロック数
//			vBlockSize：ブロックのサイズ
//	戻り値：なし
//--------------------------------------------------------------------------------
void CMeshManager::CreateEditorField(const int nNumBlockX, const int nNumBlockZ, const CKFVec2& vBlockSize)
{
	auto itr = m_umMesh.find("field");
	if (itr != m_umMesh.end())
	{ //すでに存在してる
		//itr->second.usNumUsers++;
		return;
	}

	auto pMesh = new CMesh;
	pMesh->m_nNumVtx = (nNumBlockX + 1) * (nNumBlockZ + 1);
	pMesh->m_nNumIdx = ((nNumBlockX + 1) * 2 + 2) * nNumBlockZ - 1;
	pMesh->m_nNumPolygon = (nNumBlockX + 2) * 2 * nNumBlockZ - 4;
	if (!createBuffer(pMesh)) { return; }

#ifdef USING_DIRECTX
	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	CKFVec3 vStartPos = CKFVec3(-nNumBlockX * 0.5f * vBlockSize.m_fX, 0.0f, nNumBlockZ * 0.5f * vBlockSize.m_fY);
	for (int nCntZ = 0; nCntZ < nNumBlockZ + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < nNumBlockX + 1; nCntX++)
		{
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].vPos = vStartPos
				+ CKFVec3(nCntX * vBlockSize.m_fX, 0.0f, -nCntZ * vBlockSize.m_fY);
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].vUV = CKFVec2(nCntX * 1.0f, nCntZ * 1.0f);
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].ulColor = CKFColor(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].vNormal = CKFVec3(0.0f, 1.0f, 0.0f);
		}
	}

	//仮想アドレス解放
	pMesh->m_pVtxBuffer->Unlock();

	//インデックス
	WORD *pIdx;
	pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntZ = 0; nCntZ < nNumBlockZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX < (nNumBlockX + 1) * 2 + 2; nCntX++)
		{
			if (nCntX < (nNumBlockX + 1) * 2)
			{
				pIdx[nCntZ * ((nNumBlockX + 1) * 2 + 2) + nCntX] = nCntX / 2 + (nCntX + 1) % 2 * (nNumBlockX + 1) + nCntZ * (nNumBlockX + 1);
			}
			else if (nCntZ * ((nNumBlockX + 1) * 2 + 2) + nCntX < (((nNumBlockX + 1) * 2 + 2) * nNumBlockZ - 1))//縮退ポリゴン
			{
				pIdx[nCntZ * ((nNumBlockX + 1) * 2 + 2) + nCntX] = (nCntX - 1) / 2 % (nNumBlockX + 1) + nCntX % 2 * 2 * (nNumBlockX + 1) + nCntZ * (nNumBlockX + 1);
			}
		}
	}

	pMesh->m_pIdxBuffer->Unlock();
#endif

	//メッシュの作成
	MESH mesh;
	mesh.usNumUsers = 1;
	mesh.pMesh = pMesh;

	//保存
	m_umMesh.emplace("field", mesh);
}

//--------------------------------------------------------------------------------
//	関数名：UpdateEditorField
//  関数説明：エディタ用フィールドデータの更新
//	引数：	vecVtx：頂点データ
//			listChoosenIdx：選択された頂点のインデックス
//	戻り値：なし
//--------------------------------------------------------------------------------
void CMeshManager::UpdateEditorField(const vector<CKFVec3>& vecVtx, const list<int>& listChoosenIdx)
{
#ifdef USING_DIRECTX

	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	auto pMesh = GetMesh("field");
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	auto cColor = CKFColor(1.0f);
	for (int nCnt = 0; nCnt < (int)vecVtx.size(); ++nCnt)
	{
		pVtx[nCnt].vPos = vecVtx[nCnt];
		pVtx[nCnt].ulColor = cColor;
	}

	//Choosen Color
	int nNumBlockX = 100;
	int nNumBlockZ = 100;
	auto cRed = CKFColor(1.0f, 0.0f, 0.0f, 1.0f);
	for (auto nIdx : listChoosenIdx)
	{
		pVtx[nIdx].ulColor = cRed;

		//法線計算
		int nCntZ = nIdx / (nNumBlockZ + 1);
		int nCntX = nIdx - nCntZ * (nNumBlockZ + 1);
		CKFVec3 vNormal;
		CKFVec3 vPosThis = pVtx[nIdx].vPos;
		CKFVec3 vPosLeft;
		CKFVec3 vPosRight;
		CKFVec3 vPosTop;
		CKFVec3 vPosBottom;

		if (nCntX == 0)
		{
			vPosLeft = vPosThis;
			vPosRight = pVtx[nCntZ * (nNumBlockX + 1) + nCntX + 1].vPos;
		}
		else if (nCntX < nNumBlockX)
		{
			vPosLeft = pVtx[nCntZ * (nNumBlockX + 1) + nCntX - 1].vPos;
			vPosRight = pVtx[nCntZ * (nNumBlockX + 1) + nCntX + 1].vPos;
		}
		else
		{
			vPosLeft = pVtx[nCntZ * (nNumBlockX + 1) + nCntX - 1].vPos;
			vPosRight = vPosThis;
		}

		if (nCntZ == 0)
		{
			vPosTop = vPosThis;
			vPosBottom = pVtx[(nCntZ + 1) * (nNumBlockX + 1) + nCntX].vPos;
		}
		else if (nCntZ < nNumBlockZ)
		{
			vPosTop = pVtx[(nCntZ - 1) * (nNumBlockX + 1) + nCntX].vPos;
			vPosBottom = pVtx[(nCntZ + 1) * (nNumBlockX + 1) + nCntX].vPos;
		}
		else
		{
			vPosTop = pVtx[(nCntZ - 1) * (nNumBlockX + 1) + nCntX].vPos;
			vPosBottom = vPosThis;
		}
		vNormal = (vPosRight - vPosLeft) * (vPosBottom - vPosTop);
		CKFMath::VecNormalize(vNormal);
		pVtx[nIdx].vNormal = vNormal;
	}

	//仮想アドレス解放
	pMesh->m_pVtxBuffer->Unlock();
#endif
}

//--------------------------------------------------------------------------------
//	関数名：SaveEditorFieldAs
//  関数説明：エディタ用フィールドデータの保存
//	引数：	strFileName：ファイル名
//	戻り値：なし
//--------------------------------------------------------------------------------
void CMeshManager::SaveEditorFieldAs(const string& strFileName)
{
#ifdef USING_DIRECTX
	auto pMesh = GetMesh("field");

	//file open
	string strName = "data/MESH/" + strFileName + ".mesh";
	FILE *pFile;
	fopen_s(&pFile, strName.c_str(), "wb");

	//DrawType
	int nDrawType = (int)DT_TRIANGLESTRIP;
	fwrite(&nDrawType, sizeof(int), 1, pFile);

	//NumVtx
	fwrite(&pMesh->m_nNumVtx, sizeof(int), 1, pFile);

	//NumIdx
	fwrite(&pMesh->m_nNumIdx, sizeof(int), 1, pFile);

	//NumPolygon
	fwrite(&pMesh->m_nNumPolygon, sizeof(int), 1, pFile);

	//Vtx&Idx
	//頂点データ
	VERTEX_3D* pVtx;
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	//色を白に戻す
	unsigned long ulColor = CKFColor(1.0f);
	for (int nCnt = 0; nCnt < pMesh->m_nNumVtx; ++nCnt)
	{
		pVtx[nCnt].ulColor = ulColor;
	}

	//保存
	fwrite(pVtx, sizeof(VERTEX_3D), pMesh->m_nNumVtx, pFile);
	pMesh->m_pVtxBuffer->Unlock();
	
	//インデックス
	WORD* pIdx;
	pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);
	fwrite(pIdx, sizeof(WORD), pMesh->m_nNumIdx, pFile);
	pMesh->m_pIdxBuffer->Unlock();
	
	//Texture
	int nTexSize = 0;
	fwrite(&nTexSize, sizeof(int), 1, pFile);

	fclose(pFile);
#endif
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  メッシュの作成
//--------------------------------------------------------------------------------
CMesh* CMeshManager::createMesh(const string& strName)
{
	CMesh* pMesh = new CMesh;

	//デフォルトのメッシュを作成
	if (strName._Equal("cube")) { createCube(pMesh); }
	else if (strName._Equal("sphere")) { createSphere(pMesh); }
	else if (strName._Equal("skyBox")) { createSkyBox(pMesh); }
	else if (strName.find("Field") != string::npos) { loadField(strName, pMesh); }
	return pMesh;
}

//--------------------------------------------------------------------------------
//	関数名：loadFromMesh
//  関数説明：メッシュファイルからデータの読込
//	引数：	strFileName：ファイルの名前 
//			strTexName：テクスチャの名前（Out）
//	戻り値：CMesh*
//--------------------------------------------------------------------------------
CMesh* CMeshManager::loadFromMesh(const string& strFileName, string& strTexName)
{
	string strName = "data/MESH/" + strFileName;
	FILE *pFile;

	//file open
	fopen_s(&pFile, strName.c_str(), "rb");

	if (!pFile) { return nullptr; }
	
	auto pMesh = new CMesh;

	//DrawType
	fread(&pMesh->m_drawType, sizeof(int), 1, pFile);
	//pMesh->m_drawType = DRAW_TYPE::DT_TRIANGLELIST;

	//NumVtx
	fread(&pMesh->m_nNumVtx, sizeof(int), 1, pFile);

	//NumIdx
	fread(&pMesh->m_nNumIdx, sizeof(int), 1, pFile);

	//NumPolygon
	fread(&pMesh->m_nNumPolygon, sizeof(int), 1, pFile);

#ifdef USING_DIRECTX
	if (!createBuffer(pMesh)) { return nullptr; }

	//頂点データ
	VERTEX_3D* pVtx;
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	fread(pVtx, sizeof(VERTEX_3D), pMesh->m_nNumVtx, pFile);
	pMesh->m_pVtxBuffer->Unlock();

	//インデックス
	WORD *pIdx;
	pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);
	fread(pIdx, sizeof(WORD), pMesh->m_nNumIdx, pFile);
	pMesh->m_pIdxBuffer->Unlock();
#endif

	//Texture
	int nSize;
	fread(&nSize, sizeof(int), 1, pFile);
	strTexName.resize(nSize);
	fread(&strTexName[0], sizeof(char), nSize, pFile);

	fclose(pFile);

	return pMesh;
}

//--------------------------------------------------------------------------------
//	関数名：loadFromXFile
//  関数説明：Xファイルからデータの読込
//	引数：	strPath：ファイルのパス
//			strTexName：テクスチャの名前（Out）
//	戻り値：CMesh*
//--------------------------------------------------------------------------------
CMesh* CMeshManager::loadFromXFile(const string& strPath, string& strTexName)
{
	FILE* pFile;

	//file open
	fopen_s(&pFile, strPath.c_str(), "r");

	if (!pFile) { return nullptr; }

	auto pMesh = new CMesh;
	pMesh->m_drawType = DRAW_TYPE::DT_TRIANGLELIST;
	vector<CKFVec3>		vecVtx;
	vector<CKFVec3>		vecNormal;
	vector<CKFVec2>		vecUV;
	vector<CKFColor>	vecColor;
	vector<int>			vecIdx;
	vector<int>			vecNormalIdx;
	vector<int>			vecColorIdx;

	string strBuf;
	while (CKFUtility::GetStrToken(pFile, "\n", strBuf) >= 0)
	{
		if (strBuf.compare("Mesh {") == 0)
		{
			//頂点数の読込
			CKFUtility::GetStrToken(pFile, ";", strBuf);
			stringstream ss;
			int nNumVtx;
			ss << strBuf;
			ss >> nNumVtx;

			//頂点データの読込
			vecVtx.resize(nNumVtx);
			for (int nCnt = 0; nCnt < nNumVtx; ++nCnt)
			{
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecVtx[nCnt].m_fX;
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecVtx[nCnt].m_fY;
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecVtx[nCnt].m_fZ;
				CKFUtility::GetStrToken(pFile, "\n", strBuf);
			}

			//ポリゴン数の読込
			CKFUtility::GetStrToken(pFile, ";", strBuf);
			ss.clear();
			ss << strBuf;
			ss >> pMesh->m_nNumPolygon;

			//インデックスの読込
			pMesh->m_nNumVtx = 
				pMesh->m_nNumIdx = pMesh->m_nNumPolygon * 3;
			vecIdx.resize(pMesh->m_nNumIdx);
			for (int nCnt = 0; nCnt < pMesh->m_nNumPolygon; ++nCnt)
			{
				//3;を飛ばす
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				CKFUtility::GetStrToken(pFile, ",", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecIdx[nCnt * 3];
				CKFUtility::GetStrToken(pFile, ",", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecIdx[nCnt * 3 + 1];
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecIdx[nCnt * 3 + 2];
				CKFUtility::GetStrToken(pFile, "\n", strBuf);
			}
		}

		if (strBuf.compare(" MeshMaterialList {") == 0)
		{
			//Material数
			CKFUtility::GetStrToken(pFile, ";", strBuf);
			stringstream ss;
			int nNumColor;
			ss << strBuf;
			ss >> nNumColor;
			vecColor.resize(nNumColor);

			//インデックス数
			CKFUtility::GetStrToken(pFile, ";", strBuf);
			ss.clear();
			int nNumPolygon;
			ss << strBuf;
			ss >> nNumPolygon;
			vecColorIdx.resize(nNumPolygon * 3);

			//インデックスの読み込み
			for (int nCnt = 0; nCnt < nNumPolygon; ++nCnt)
			{
				CKFUtility::GetStrToken(pFile, ",;", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecColorIdx[nCnt * 3];
				vecColorIdx[nCnt * 3 + 1] = 
					vecColorIdx[nCnt * 3 + 2] =
					vecColorIdx[nCnt * 3];
			}

			//マテリアルの読み込み
			for (int nCnt = 0; nCnt < nNumColor; ++nCnt)
			{
				while (CKFUtility::GetStrToken(pFile, "\n", strBuf) >= 0)
				{
					if (strBuf.compare("  Material {") == 0)
					{
						//R
						CKFUtility::GetStrToken(pFile, ";", strBuf);
						ss.clear();
						ss << strBuf;
						ss >> vecColor[nCnt].m_fR;

						//G
						CKFUtility::GetStrToken(pFile, ";", strBuf);
						ss.clear();
						ss << strBuf;
						ss >> vecColor[nCnt].m_fG;

						//B
						CKFUtility::GetStrToken(pFile, ";", strBuf);
						ss.clear();
						ss << strBuf;
						ss >> vecColor[nCnt].m_fB;

						//A
						CKFUtility::GetStrToken(pFile, ";", strBuf);
						ss.clear();
						ss << strBuf;
						ss >> vecColor[nCnt].m_fA;

						break;
					}
				}
			}
		}

		

		if (strBuf.compare(" MeshNormals {") == 0)
		{
			//法線数の読込
			CKFUtility::GetStrToken(pFile, ";", strBuf);
			stringstream ss;
			int nNumNormal;
			ss << strBuf;
			ss >> nNumNormal;

			//法線データの読込
			vecNormal.resize(nNumNormal);
			for (int nCnt = 0; nCnt < nNumNormal; ++nCnt)
			{
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecNormal[nCnt].m_fX;
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecNormal[nCnt].m_fY;
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecNormal[nCnt].m_fZ;
				CKFUtility::GetStrToken(pFile, "\n", strBuf);
			}

			//法線インデックス数を読込(頂点数と同じのため)
			CKFUtility::GetStrToken(pFile, ";", strBuf);
			ss.clear();
			int nNumPolygonNormalIdx;
			ss << strBuf;
			ss >> nNumPolygonNormalIdx;
			if (nNumPolygonNormalIdx != pMesh->m_nNumPolygon)
			{//エラー
				MessageBox(NULL,"CMeshManager:法線数と頂点数が違う！", "エラー", MB_OK | MB_ICONWARNING);
			}

			//法線インデックスの読込
			vecNormalIdx.resize(nNumPolygonNormalIdx * 3);
			for (int nCnt = 0; nCnt < pMesh->m_nNumPolygon; ++nCnt)
			{
				//3;を飛ばす
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				CKFUtility::GetStrToken(pFile, ",", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecNormalIdx[nCnt * 3];
				CKFUtility::GetStrToken(pFile, ",", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecNormalIdx[nCnt * 3 + 1];
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecNormalIdx[nCnt * 3 + 2];
				CKFUtility::GetStrToken(pFile, "\n", strBuf);
			}
		}

		if (strBuf.compare(" MeshTextureCoords {") == 0)
		{
			//UV数の読込
			CKFUtility::GetStrToken(pFile, ";", strBuf);
			stringstream ss;
			int nNumUV;
			ss << strBuf;
			ss >> nNumUV;
			if (nNumUV != vecVtx.size())
			{//エラー
				MessageBox(NULL, "CMeshManager:UV数と頂点数が違う！", "エラー", MB_OK | MB_ICONWARNING);
			}

			//UVデータの読込
			vecUV.resize(nNumUV);
			for (int nCnt = 0; nCnt < nNumUV; ++nCnt)
			{
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecUV[nCnt].m_fX;
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecUV[nCnt].m_fY;
				CKFUtility::GetStrToken(pFile, "\n", strBuf);
			}
		}
	}

	fclose(pFile);
	
#ifdef USING_DIRECTX
	if (!createBuffer(pMesh)) { return nullptr; }

	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < pMesh->m_nNumIdx; ++nCnt)
	{
		pVtx[nCnt].vPos = vecVtx[vecIdx[nCnt]];
		pVtx[nCnt].vNormal = vecNormal[vecNormalIdx[nCnt]];
		pVtx[nCnt].vUV = vecUV[vecIdx[nCnt]];
		pVtx[nCnt].ulColor = vecColor[vecColorIdx[nCnt]];
	}

	//仮想アドレス解放
	pMesh->m_pVtxBuffer->Unlock();

	//インデックス
	WORD *pIdx;
	pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < pMesh->m_nNumIdx; ++nCnt)
	{
		pIdx[nCnt] = nCnt;
	}

	pMesh->m_pIdxBuffer->Unlock();
#endif

	return pMesh;
}

//--------------------------------------------------------------------------------
//  Cubeの作成
//--------------------------------------------------------------------------------
void CMeshManager::createCube(CMesh* pMesh)
{
	pMesh->m_nNumVtx = 6 * 4;
	pMesh->m_nNumIdx = 6 * 4 + 5 * 2;
	pMesh->m_nNumPolygon = 6 * 2 + 5 * 4;
	if (!createBuffer(pMesh)) { return; }

#ifdef USING_DIRECTX
	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	CKFVec3 vHalfSize = CKFVec3(1.0f) * 0.5f;
	CKFColor cColor = CKFColor(1.0f);
	int nCntVtx = 0;

	//正面
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX + (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			-vHalfSize.m_fZ);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 0.0f, -1.0f);

		nCntVtx++;
	}

	//上
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX + (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			vHalfSize.m_fY,
			vHalfSize.m_fZ - (nCnt / 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 1.0f, 0.0f);

		nCntVtx++;
	}

	//左
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			vHalfSize.m_fZ - (nCnt % 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(-1.0f, 0.0f, 0.0f);

		nCntVtx++;
	}

	//下
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX + (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			-vHalfSize.m_fY,
			-vHalfSize.m_fZ + (nCnt / 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, -1.0f, 0.0f);
		nCntVtx++;
	}

	//右
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			vHalfSize.m_fX,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			-vHalfSize.m_fZ + (nCnt % 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(1.0f, 0.0f, 0.0f);
		nCntVtx++;
	}

	//後ろ
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			vHalfSize.m_fX - (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			vHalfSize.m_fZ);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 0.0f, 1.0f);
		nCntVtx++;
	}

	//仮想アドレス解放
	pMesh->m_pVtxBuffer->Unlock();

	//インデックス
	WORD *pIdx;
	pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < pMesh->m_nNumIdx; nCnt++)
	{
		if (nCnt % 6 < 4)
		{
			pIdx[nCnt] = (WORD)((nCnt / 6) * 4 + (nCnt % 6) % 4);
		}
		else
		{//縮退
			pIdx[nCnt] = (WORD)((nCnt / 6) * 4 + (nCnt % 2) + 3);
		}
	}

	pMesh->m_pIdxBuffer->Unlock();

	//Modelファイルの保存
	string strName = "data/MODEL/cube.model";
	FILE *pFile;

	//file open
	fopen_s(&pFile, strName.c_str(), "wb");

	//パーツ数の保存
	int nNumParentParts = 1;
	fwrite(&nNumParentParts, sizeof(int), 1, pFile);

	//ファイル名
	string strMeshName = "cube.mesh";
	int nSize = strMeshName.size();
	fwrite(&nSize, sizeof(int), 1, pFile);
	fwrite(&strMeshName[0], sizeof(char), nSize, pFile);

	//Offset
	auto vPos = CKFVec3(0.0f);
	auto vRot = CKFVec3(0.0f);
	auto vScale = CKFVec3(1.0f);
	fwrite(&vPos, sizeof(CKFVec3), 1, pFile);
	fwrite(&vRot, sizeof(CKFVec3), 1, pFile);
	fwrite(&vScale, sizeof(CKFVec3), 1, pFile);

	//Collider
	int nNumCollider = 1;
	fwrite(&nNumCollider, sizeof(int), 1, pFile);

	int nType = 1;
	fwrite(&nType, sizeof(int), 1, pFile);
	fwrite(&vPos, sizeof(CKFVec3), 1, pFile);
	fwrite(&vRot, sizeof(CKFVec3), 1, pFile);
	fwrite(&vScale, sizeof(CKFVec3), 1, pFile);

	fclose(pFile);

	//Mesh
	strName = "data/MESH/cube.mesh";

	//file open
	fopen_s(&pFile, strName.c_str(), "wb");

	//DrawType
	int nDrawType = (int)pMesh->m_drawType;
	fwrite(&nDrawType, sizeof(int), 1, pFile);

	//NumVtx
	fwrite(&pMesh->m_nNumVtx, sizeof(int), 1, pFile);

	//NumIdx
	fwrite(&pMesh->m_nNumIdx, sizeof(int), 1, pFile);

	//NumPolygon
	fwrite(&pMesh->m_nNumPolygon, sizeof(int), 1, pFile);

	//Vtx&Idx
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	fwrite(pVtx, sizeof(VERTEX_3D), pMesh->m_nNumVtx, pFile);
	pMesh->m_pVtxBuffer->Unlock();
	pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);
	fwrite(pIdx, sizeof(WORD), pMesh->m_nNumIdx, pFile);
	pMesh->m_pIdxBuffer->Unlock();

	//Texture
	string strTexture = "nomal_cube.jpg";
	nSize = strTexture.size();
	fwrite(&nSize, sizeof(int), 1, pFile);
	fwrite(&strTexture[0], sizeof(char), nSize, pFile);

	fclose(pFile);
#endif
}

//--------------------------------------------------------------------------------
//  Sphereの作成
//--------------------------------------------------------------------------------
void CMeshManager::createSphere(CMesh* pMesh)
{
}

//--------------------------------------------------------------------------------
//  SkyBoxの作成
//--------------------------------------------------------------------------------
void CMeshManager::createSkyBox(CMesh* pMesh)
{
	pMesh->m_nNumVtx = 6 * 4;
	pMesh->m_nNumIdx = 6 * 4 + 5 * 2;
	pMesh->m_nNumPolygon = 6 * 2 + 5 * 4;
	if (!createBuffer(pMesh)) { return; }

#ifdef USING_DIRECTX
	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	float fLength = (float)CCamera::DEFAULT_FAR * 0.5f;
	CKFColor cColor = CKFColor(1.0f);
	int nCntVtx = 0;
	float fUVtweens = 1.0f / 1024.0f;	//隙間を無くすためにUVを1px縮める

	//正面
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			fLength - (nCnt % 2) * fLength * 2.0f,
			fLength - (nCnt / 2) * fLength * 2.0f,
			-fLength);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 0.25f + 0.25f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 0.0f, 1.0f);
		nCntVtx++;
	}

	//上
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			fLength - (nCnt % 2) * fLength * 2.0f,
			fLength,
			fLength - (nCnt / 2) * fLength * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 0.25f + 0.25f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, -1.0f, 0.0f);
		nCntVtx++;
	}

	//左
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-fLength,
			fLength - (nCnt / 2) * fLength * 2.0f,
			-fLength + (nCnt % 2) * fLength * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 0.25f + 0.5f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(1.0f, 0.0f, 0.0f);
		nCntVtx++;
	}

	//下
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			fLength - (nCnt % 2) * fLength * 2.0f,
			-fLength,
			-fLength + (nCnt / 2) * fLength * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 0.25f + 0.25f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 2.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 1.0f, 0.0f);
		nCntVtx++;
	}

	//右
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			fLength,
			fLength - (nCnt / 2) * fLength * 2.0f,
			fLength - (nCnt % 2) * fLength * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 0.25f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(-1.0f, 0.0f, 0.0f);
		nCntVtx++;
	}

	//後ろ
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-fLength + (nCnt % 2) * fLength * 2.0f,
			fLength - (nCnt / 2) * fLength * 2.0f,
			fLength);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 0.25f + 0.75f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 0.0f, -1.0f);
		nCntVtx++;
	}

	//仮想アドレス解放
	pMesh->m_pVtxBuffer->Unlock();

	//インデックス
	WORD *pIdx;
	pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < 6 * 4 + 5 * 2; nCnt++)
	{
		if (nCnt % 6 < 4)
		{
			pIdx[nCnt] = (nCnt / 6) * 4 + (nCnt % 6) % 4;
		}
		else
		{//縮退
			pIdx[nCnt] = (nCnt / 6) * 4 + (nCnt % 2) + 3;
		}
	}

	pMesh->m_pIdxBuffer->Unlock();
#endif
}

//--------------------------------------------------------------------------------
//  バッファの作成
//--------------------------------------------------------------------------------
void CMeshManager::loadField(const string& strFileName, CMesh* pMesh)
{
#ifdef USING_DIRECTX
	//file open
	string strName = "data/MESH/" + strFileName + ".mesh";
	FILE *pFile;
	fopen_s(&pFile, strName.c_str(), "rb");
	if (!pFile) 
	{
		MessageBox(NULL, "CMeshManager : loadField ERROR!! ファイルが見つからない!!", "エラー", MB_OK | MB_ICONWARNING);
		return; 
	}

	//DrawType
	fread(&pMesh->m_drawType, sizeof(int), 1, pFile);

	//NumVtx
	fread(&pMesh->m_nNumVtx, sizeof(int), 1, pFile);

	//NumIdx
	fread(&pMesh->m_nNumIdx, sizeof(int), 1, pFile);

	//NumPolygon
	fread(&pMesh->m_nNumPolygon, sizeof(int), 1, pFile);

	if (!createBuffer(pMesh)) { return; }

	//Vtx&Idx
	//頂点データ
	VERTEX_3D* pVtx;
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	fread(pVtx, sizeof(VERTEX_3D), pMesh->m_nNumVtx, pFile);
	pMesh->m_pVtxBuffer->Unlock();

	//インデックス
	WORD* pIdx;
	pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);
	fread(pIdx, sizeof(WORD), pMesh->m_nNumIdx, pFile);
	pMesh->m_pIdxBuffer->Unlock();

	fclose(pFile);
#endif
}

//--------------------------------------------------------------------------------
//  バッファの作成
//--------------------------------------------------------------------------------
bool CMeshManager::createBuffer(CMesh* pMesh)
{
#ifdef USING_DIRECTX
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//頂点バッファ
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * pMesh->m_nNumVtx,	//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,						//頂点バッファの使用方法
		FVF_VERTEX_3D,							//書かなくても大丈夫
		D3DPOOL_MANAGED,						//メモリ管理方法(managed：デバイスにお任せ)
		&pMesh->m_pVtxBuffer,					//頂点バッファのポインタ
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CMeshManager : CreateVertexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}

	//インデックスバッファの作成
	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD) * pMesh->m_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pMesh->m_pIdxBuffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CMeshManager : CreateIndexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}
#endif
	return true;
}
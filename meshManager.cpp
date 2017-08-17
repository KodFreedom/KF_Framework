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

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  クラス
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
	}

	//メッシュの作成
	MESH mesh;
	mesh.usNumUsers = 1;
	mesh.pMesh = createMesh(strName);

	//保存
	m_umMesh.emplace(strName, mesh);
}

//--------------------------------------------------------------------------------
//  メッシュの追加
//--------------------------------------------------------------------------------
void CMeshManager::UseMesh(const string& strName, string& texName)
{
	auto itr = m_umMesh.find(strName);
	if (itr != m_umMesh.end())
	{ //すでに存在してる
		itr->second.usNumUsers++;
	}

	//メッシュの作成
	MESH mesh;
	mesh.usNumUsers = 1;
	mesh.pMesh = createMesh(strName, texName);

	//保存
	m_umMesh.emplace(strName, mesh);
}

//--------------------------------------------------------------------------------
//  メッシュの破棄
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
//  メッシュの作成
//--------------------------------------------------------------------------------
CMesh* CMeshManager::createMesh(const string& strName)
{
	CMesh* pMesh = new CMesh;

	//デフォルトのメッシュを作成
	if (strName == "cube") { createCube(pMesh); }
	else if (strName == "sphere") { createSphere(pMesh); }
	else if (strName == "skyBox") { createSkyBox(pMesh); }
	else if (strName == "field") { createField(pMesh); }
	return pMesh;
}

//--------------------------------------------------------------------------------
//  メッシュの作成
//--------------------------------------------------------------------------------
CMesh* CMeshManager::createMesh(const string& strName, string& texName)
{
	CMesh* pMesh = new CMesh;

	//メッシュの名前の最後の五文字が.meshかどうかをチェック
	if (strName.find(".mesh") != string::npos)
	{//ファイルから読み込む

	}

	else if (strName.find(".x") != string::npos)
	{//XFile
		createXFile(strName, pMesh, texName);
	}

	return pMesh;
}

//--------------------------------------------------------------------------------
//  Cubeの作成
//--------------------------------------------------------------------------------
void CMeshManager::createXFile(const string& strName, CMesh* pMesh, string& texName)
{
	LPDIRECT3DDEVICE9	pDevice = GetManager()->GetRenderer()->GetDevice();
	LPD3DXMESH			pDxMesh;		//メッシュ情報インターフェイス
	LPD3DXBUFFER		pBufferMaterial;//マテリアル情報　動的メモリ
	DWORD				dwNumMaterial;	//モデルのマテリアル数

	//ハードディスクからXファイルの読み込み
	HRESULT hr = D3DXLoadMeshFromX(
		strName.c_str(),	//ファイル名
		D3DXMESH_MANAGED,
		pDevice,
		NULL,				//隣接情報
		&pBufferMaterial,	//モデルのマテリアル情報
		NULL,
		&dwNumMaterial,		//モデルのマテリアル数
		&pDxMesh);			//メッシュ情報

	if (FAILED(hr))
	{
		MessageBox(NULL, "CModel : D3DXLoadMeshFromX ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return;
	}

	//Mesh
	pMesh->m_nNumIdx = pDxMesh->GetNumVertices();
	pMesh->m_nNumPolygon = pDxMesh->GetNumFaces();
	pMesh->m_nNumVtx = pDxMesh->GetNumVertices();
	pDxMesh->GetIndexBuffer(&pMesh->m_pIdxBuffer);
	pDxMesh->GetVertexBuffer(&pMesh->m_pVtxBuffer);

	//texture
	D3DXMATERIAL *pMat;//マテリアル情報を受け取れる用のポインタ

	pMat = (D3DXMATERIAL*)pBufferMaterial->GetBufferPointer();
	for (int nCnt = 0; nCnt < (int)dwNumMaterial; nCnt++)
	{
		if (pMat[nCnt].pTextureFilename != nullptr)
		{
			texName = pMat[nCnt].pTextureFilename;
			break;
		}
	}

	if (texName.empty())
	{//テクスチャがないの場合普通なポリゴンにする
		texName = "polygon.jpg";
	}
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

	//FILE *pFile;

	////file open
	//fopen_s(&pFile, "cube.mesh", "rb");
	//VERTEX_3D aV[100];
	//int aI[100];
	////総数保存
	//fread(&m_meshInfo.nNumVtx, sizeof(int), 1, pFile);
	//fread(&m_meshInfo.nNumIdx, sizeof(int), 1, pFile);
	//fread(&m_meshInfo.nNumPolygon, sizeof(int), 1, pFile);
	//fread(aV, sizeof(VERTEX_3D), m_meshInfo.nNumVtx, pFile);
	//fread(aI, sizeof(int), m_meshInfo.nNumIdx, pFile);

	//fclose(pFile);
	//int n = 1;
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
bool CMeshManager::createBuffer(CMesh* pMesh)
{
#ifdef USING_DIRECTX
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
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

//--------------------------------------------------------------------------------
//  SkyBoxの作成
//--------------------------------------------------------------------------------
void CMeshManager::createField(CMesh* pMesh)
{
	int nNumBlockX = 100;
	int nNumBlockZ = 100;
	pMesh->m_nNumVtx = (nNumBlockX + 1) * (nNumBlockZ + 1);
	pMesh->m_nNumIdx = ((nNumBlockX + 1) * 2 + 2) * nNumBlockZ - 1;
	pMesh->m_nNumPolygon = (nNumBlockX + 2) * 2 * nNumBlockZ - 4;
	if (!createBuffer(pMesh)) { return; }

#ifdef USING_DIRECTX

	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	float fHeight = 0.0f;
	CKFVec2 vSize = CKFVec2(2.0f, 2.0f);
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	CKFVec3 vStartPos = CKFVec3(-nNumBlockX * 0.5f * vSize.m_fX, 0.0f, nNumBlockZ * 0.5f * vSize.m_fY);
	for (int nCntZ = 0; nCntZ < nNumBlockZ + 1; nCntZ++)
	{
		if (nCntZ <= (nNumBlockZ + 1) / 4)
		{
			fHeight += 0.25f;
		}
		else if (nCntZ <= (nNumBlockZ + 1) / 2)
		{
			fHeight -= 0.25f;
		}
		else if (nCntZ <= (nNumBlockZ + 1) * 3 / 4)
		{
			fHeight += 0.5f;
		}
		else
		{
			fHeight -= 0.5f;
		}
		for (int nCntX = 0; nCntX < nNumBlockX + 1; nCntX++)
		{
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].vPos = vStartPos
				+ CKFVec3(nCntX * vSize.m_fX, fHeight, -nCntZ * vSize.m_fY);
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].vUV = CKFVec2(nCntX * 1.0f, nCntZ * 1.0f);
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].ulColor = CKFColor(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].vNormal = CKFVec3(0.0f, 1.0f, 0.0f);
		}
	}

	//法線計算
	for (int nCntZ = 0; nCntZ < nNumBlockX + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < nNumBlockX + 1; nCntX++)
		{
			CKFVec3 vNormal;
			CKFVec3 vPosThis = pVtx[nCntZ * (nNumBlockX + 1) + nCntX].vPos;
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

			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].vNormal = vNormal;
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
}
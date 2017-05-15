//--------------------------------------------------------------------------------
//
//　model.cpp
//	Author : Xu Wenjie
//	Date   : 2017-01-24
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
#include "model.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  XFileの読み込み
//--------------------------------------------------------------------------------
KFRESULT CModel::LoadXFile(XFILE* pXFile, const LPCSTR &pXFilePath)
{
	//チェックポインタ
	if (pXFile == NULL) { return KF_FAILED; }

	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//ハードディスクからXファイルの読み込み
	HRESULT hr = D3DXLoadMeshFromX(
		pXFilePath,					//ファイル名
		D3DXMESH_MANAGED,
		pDevice,
		NULL,						//隣接情報
		&pXFile->pBufferMaterial,	//モデルのマテリアル情報
		NULL,
		&pXFile->dwNumMaterial,		//モデルのマテリアル数
		&pXFile->pMesh);			//メッシュ情報

	if (FAILED(hr))
	{
		MessageBox(NULL, "CModel : D3DXLoadMeshFromX ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return KF_FAILED;
	}

	//texture
	D3DXMATERIAL *pMat;//マテリアル情報を受け取れる用のポインタ

	pMat = (D3DXMATERIAL*)pXFile->pBufferMaterial->GetBufferPointer();
	for (int nCnt = 0; nCnt < (int)pXFile->dwNumMaterial; nCnt++)
	{
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		if (pMat[nCnt].pTextureFilename != NULL)
		{
			//ハードディスクからTextureの読み込み
			//※エラーチェック必須
			hr = D3DXCreateTextureFromFile(pDevice,
				pMat[nCnt].pTextureFilename,
				&pTexture);

			if (FAILED(hr))
			{
				char aStr[128];
				wsprintf(aStr, "CModel %d番のテクスチャ ERROR!", nCnt);
				MessageBox(NULL, aStr, "エラー", MB_OK | MB_ICONWARNING);
			}
		}
		pXFile->vectorTexture.push_back(pTexture);
	}

	return KF_SUCCEEDED;
}

//--------------------------------------------------------------------------------
//  XFileの描画
//--------------------------------------------------------------------------------
void CModel::DrawXFile(const XFILE &XFile)
{
	//現在デバイスに設定されてるマテリアル情報を取得
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	//マテリアル情報を受け取る
	D3DXMATERIAL *pMat;
	pMat = (D3DXMATERIAL*)XFile.pBufferMaterial->GetBufferPointer();

	for (int nCnt = 0; nCnt < (int)XFile.dwNumMaterial; nCnt++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCnt].MatD3D);

		//Texture
		if (nCnt < (int)XFile.vectorTexture.size())
		{
			pDevice->SetTexture(0, XFile.vectorTexture[nCnt]);
		}

		//メッシュの描画
		XFile.pMesh->DrawSubset(nCnt);
	}

	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------------------------------------
//  XFileの描画(Material指定)
//--------------------------------------------------------------------------------
void CModel::DrawXFile(const XFILE &XFile, const CMM::MATERIAL &matType)
{
	//現在デバイスに設定されてるマテリアル情報を取得
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// マテリアルの設定
	D3DMATERIAL9 mat = GetManager()->GetMaterialManager()->GetMaterial(matType);
	pDevice->SetMaterial(&mat);

	for (int nCnt = 0; nCnt < (int)XFile.dwNumMaterial; nCnt++)
	{
		//Texture
		pDevice->SetTexture(0, XFile.vectorTexture[nCnt]);

		//メッシュの描画
		XFile.pMesh->DrawSubset(nCnt);
	}

	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------------------------------------
//  XFileの破棄
//--------------------------------------------------------------------------------
void CModel::ReleaseXFile(XFILE* pXFile)
{
	if (pXFile != NULL)
	{
		if (pXFile->pMesh != NULL)
		{
			pXFile->pMesh->Release();
			pXFile->pMesh = NULL;
		}

		if (pXFile->pBufferMaterial != NULL)
		{
			pXFile->pBufferMaterial->Release();
			pXFile->pBufferMaterial = NULL;
		}

		for (int nCnt = 0; nCnt < (int)pXFile->vectorTexture.size(); nCnt++)
		{
			if (pXFile->vectorTexture[nCnt] != NULL)
			{
				pXFile->vectorTexture[nCnt]->Release();
				pXFile->vectorTexture[nCnt] = NULL;
			}
		}

		pXFile->vectorTexture.clear();
	}
}
//--------------------------------------------------------------------------------
//	SkyBoxメッシュコンポネント
//　SkyBoxMeshComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "skyBoxMeshComponent.h"
#include "camera.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
KFRESULT CSkyBoxMeshComponent::Init(void)
{
	m_meshInfo.nNumVtx = 6 * 4;
	m_meshInfo.nNumIdx = 6 * 4 + 5 * 2;
	m_meshInfo.nNumPolygon = 6 * 2 + 5 * 4;

	if (C3DMeshComponent::CreateBuffer() == KF_FAILED)
	{
		return KF_FAILED;
	}

	MakeVertex();

	return KF_SUCCEEDED;
}

//--------------------------------------------------------------------------------
//  頂点生成処理
//--------------------------------------------------------------------------------
void CSkyBoxMeshComponent::MakeVertex(void)
{
#ifdef USING_DIRECTX9
	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	m_meshInfo.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	float fLength = (float)CCamera::DEFAULT_FAR * 0.5f;
	int nCntVtx = 0;
	float fUVtweens = 1.0f / 1024.0f;

	//正面
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			fLength - (nCnt % 2) * fLength * 2.0f,
			fLength - (nCnt / 2) * fLength * 2.0f,
			-fLength);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 0.25f + 0.25f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].ulColor = CKFColor(1.0f);
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
		pVtx[nCntVtx].ulColor = CKFColor(1.0f);
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
		pVtx[nCntVtx].ulColor = CKFColor(1.0f);
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
		pVtx[nCntVtx].ulColor = CKFColor(1.0f);
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
		pVtx[nCntVtx].ulColor = CKFColor(1.0f);
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
		pVtx[nCntVtx].ulColor = CKFColor(1.0f);
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 0.0f, -1.0f);
		nCntVtx++;
	}

	//仮想アドレス解放
	m_meshInfo.pVtxBuffer->Unlock();

	//インデックス
	WORD *pIdx;
	m_meshInfo.pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

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

	m_meshInfo.pIdxBuffer->Unlock();
#endif
}
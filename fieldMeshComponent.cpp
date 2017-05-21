//--------------------------------------------------------------------------------
//	フィールドメッシュコンポネント
//　fieldMeshComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "fieldMeshComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
KFRESULT CFieldMeshComponent::Init(void)
{
	m_meshInfo.nNumVtx = (m_nNumBlockX + 1) * (m_nNumBlockZ + 1);
	m_meshInfo.nNumIdx = ((m_nNumBlockX + 1) * 2 + 2) * m_nNumBlockZ - 1;
	m_meshInfo.nNumPolygon = (m_nNumBlockX + 2) * 2 * m_nNumBlockZ - 4;

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
void CFieldMeshComponent::MakeVertex(void)
{
#ifdef USING_DIRECTX9
	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	m_meshInfo.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	CKFVec3 vStartPos = CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);

	for (int nCntZ = 0; nCntZ < m_nNumBlockZ + 1; nCntZ++)
	{
		for (int nCntPos = 0; nCntPos < m_nNumBlockX + 1; nCntPos++)
		{
			pVtx[nCntZ * (m_nNumBlockX + 1) + nCntPos].vPos = vStartPos
				+ CKFVec3(nCntPos * m_vBlockSize.m_fX, 0.0f, -nCntZ * m_vBlockSize.m_fY);
			pVtx[nCntZ * (m_nNumBlockX + 1) + nCntPos].vUV = CKFVec2(nCntPos * 1.0f, nCntZ * 1.0f);
			pVtx[nCntZ * (m_nNumBlockX + 1) + nCntPos].ulColor = CKFColor(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[nCntZ * (m_nNumBlockX + 1) + nCntPos].vNormal = CKFVec3(0.0f, 1.0f, 0.0f);
		}
	}

	//仮想アドレス解放
	m_meshInfo.pVtxBuffer->Unlock();

	//インデックス
	WORD *pIdx;
	m_meshInfo.pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntZ = 0; nCntZ < m_nNumBlockZ; nCntZ++)
	{
		for (int nCntPos = 0; nCntPos < (m_nNumBlockX + 1) * 2 + 2; nCntPos++)
		{
			if (nCntPos < (m_nNumBlockX + 1) * 2)
			{
				pIdx[nCntZ * ((m_nNumBlockX + 1) * 2 + 2) + nCntPos] = nCntPos / 2 + (nCntPos + 1) % 2 * (m_nNumBlockX + 1) + nCntZ * (m_nNumBlockX + 1);
			}
			else if (nCntZ * ((m_nNumBlockX + 1) * 2 + 2) + nCntPos < (((m_nNumBlockX + 1) * 2 + 2) * m_nNumBlockZ - 1))//縮退ポリゴン
			{
				pIdx[nCntZ * ((m_nNumBlockX + 1) * 2 + 2) + nCntPos] = (nCntPos - 1) / 2 % (m_nNumBlockX + 1) + nCntPos % 2 * 2 * (m_nNumBlockX + 1) + nCntZ * (m_nNumBlockX + 1);
			}
		}
	}

	m_meshInfo.pIdxBuffer->Unlock();
#endif
}
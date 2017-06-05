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
	float fHeight = 0.0f;
	m_meshInfo.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	CKFVec3 vStartPos = CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
	for (int nCntZ = 0; nCntZ < m_nNumBlockZ + 1; nCntZ++)
	{
		if (nCntZ <= (m_nNumBlockZ + 1) / 4)
		{
			fHeight += 0.5f;
		}
		else if (nCntZ <= (m_nNumBlockZ + 1) / 2)
		{
			fHeight -= 1.0f;
		}
		else if (nCntZ <= (m_nNumBlockZ + 1) * 3 / 4)
		{
			fHeight += 1.0f;
		}
		else
		{
			fHeight -= 0.5f;
		}
		for (int nCntX = 0; nCntX < m_nNumBlockX + 1; nCntX++)
		{
			if (nCntX <= (m_nNumBlockX + 1) / 4)
			{
				fHeight += 0.1f;
			}
			else if (nCntX <= (m_nNumBlockX + 1) / 2)
			{
				fHeight -= 0.1f;
			}
			else if (nCntX <= (m_nNumBlockX + 1) * 3 / 4)
			{
				fHeight += 0.1f;
			}
			else
			{
				fHeight -= 0.1f;
			}
			//float fHeight = CKFMath::GetRandomFloat(-0.2f, 0.2f);

			pVtx[nCntZ * (m_nNumBlockX + 1) + nCntX].vPos = vStartPos
				+ CKFVec3(nCntX * m_vBlockSize.m_fX, fHeight, -nCntZ * m_vBlockSize.m_fY);
			pVtx[nCntZ * (m_nNumBlockX + 1) + nCntX].vUV = CKFVec2(nCntX * 1.0f, nCntZ * 1.0f);
			pVtx[nCntZ * (m_nNumBlockX + 1) + nCntX].ulColor = CKFColor(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[nCntZ * (m_nNumBlockX + 1) + nCntX].vNormal = CKFVec3(0.0f, 1.0f, 0.0f);
		}
	}

	//法線計算
	for (int nCntZ = 0; nCntZ < m_nNumBlockX + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nNumBlockX + 1; nCntX++)
		{
			CKFVec3 vNormal;
			CKFVec3 vPosThis = pVtx[nCntZ * (m_nNumBlockX + 1) + nCntX].vPos;
			CKFVec3 vPosLeft;
			CKFVec3 vPosRight;
			CKFVec3 vPosTop;
			CKFVec3 vPosBottom;

			if (nCntX == 0)
			{
				vPosLeft = vPosThis;
				vPosRight = pVtx[nCntZ * (m_nNumBlockX + 1) + nCntX + 1].vPos;
			}
			else if (nCntX < m_nNumBlockX)
			{
				vPosLeft = pVtx[nCntZ * (m_nNumBlockX + 1) + nCntX - 1].vPos;
				vPosRight = pVtx[nCntZ * (m_nNumBlockX + 1) + nCntX + 1].vPos;
			}
			else
			{
				vPosLeft = pVtx[nCntZ * (m_nNumBlockX + 1) + nCntX - 1].vPos;
				vPosRight = vPosThis;
			}

			if (nCntZ == 0)
			{
				vPosTop = vPosThis;
				vPosBottom = pVtx[(nCntZ + 1) * (m_nNumBlockX + 1) + nCntX].vPos;
			}
			else if (nCntZ < m_nNumBlockZ)
			{
				vPosTop = pVtx[(nCntZ - 1) * (m_nNumBlockX + 1) + nCntX].vPos;
				vPosBottom = pVtx[(nCntZ + 1) * (m_nNumBlockX + 1) + nCntX].vPos;
			}
			else
			{
				vPosTop = pVtx[(nCntZ - 1) * (m_nNumBlockX + 1) + nCntX].vPos;
				vPosBottom = vPosThis;
			}

			vNormal = (vPosRight - vPosLeft) * (vPosBottom - vPosTop);
			CKFMath::VecNormalize(&vNormal);

			pVtx[nCntZ * (m_nNumBlockX + 1) + nCntX].vNormal = vNormal;
		}
	}

	//仮想アドレス解放
	m_meshInfo.pVtxBuffer->Unlock();

	//インデックス
	WORD *pIdx;
	m_meshInfo.pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntZ = 0; nCntZ < m_nNumBlockZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX < (m_nNumBlockX + 1) * 2 + 2; nCntX++)
		{
			if (nCntX < (m_nNumBlockX + 1) * 2)
			{
				pIdx[nCntZ * ((m_nNumBlockX + 1) * 2 + 2) + nCntX] = nCntX / 2 + (nCntX + 1) % 2 * (m_nNumBlockX + 1) + nCntZ * (m_nNumBlockX + 1);
			}
			else if (nCntZ * ((m_nNumBlockX + 1) * 2 + 2) + nCntX < (((m_nNumBlockX + 1) * 2 + 2) * m_nNumBlockZ - 1))//縮退ポリゴン
			{
				pIdx[nCntZ * ((m_nNumBlockX + 1) * 2 + 2) + nCntX] = (nCntX - 1) / 2 % (m_nNumBlockX + 1) + nCntX % 2 * 2 * (m_nNumBlockX + 1) + nCntZ * (m_nNumBlockX + 1);
			}
		}
	}

	m_meshInfo.pIdxBuffer->Unlock();
#endif
}
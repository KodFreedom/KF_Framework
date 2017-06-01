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

float CFieldMeshComponent::GetHeight(const CKFVec3 &vPos)
{
#ifdef USING_DIRECTX9
	CKFVec3 vStartPos = CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
	int nXLeftUp = (int)(((vPos.m_fX - vStartPos.m_fX) / (m_vBlockSize.m_fX * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZLeftUp = -(int)(((vPos.m_fZ - vStartPos.m_fZ) / (m_vBlockSize.m_fY * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);

	if (nXLeftUp < 0 || nXLeftUp >= m_nNumBlockX || nZLeftUp < 0 || nZLeftUp >= m_nNumBlockZ)
	{
		return 0.0f;
	}

	int nXRightDown = nXLeftUp + 1;
	int nZRightDown = nZLeftUp + 1;

	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	m_meshInfo.pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	CKFVec3 vPosTarget = CKFVec3(vPos.m_fX, 0.0f, vPos.m_fY);
	CKFVec3 vPLeftUp = pVtx[nZLeftUp * (m_nNumBlockX + 1) + nXLeftUp].vPos;
	CKFVec3 vPRightDown = pVtx[nZRightDown * (m_nNumBlockX + 1) + nXRightDown].vPos;

	//Check Side
	CKFVec3 vMid = vPRightDown - vPLeftUp;
	CKFVec3 vTL = vPosTarget - vPLeftUp;
	CKFVec3 vCross = vTL * vMid;
	int nXSide, nZSide;
	if (vCross.m_fY >= 0.0f)
	{//LeftSide
		nXSide = nXLeftUp + 1;
		nZSide = nZLeftUp;
	}
	else
	{//RightSide
		nXSide = nXLeftUp;
		nZSide = nZLeftUp + 1;
	}
	CKFVec3 vPSide = pVtx[nZSide * (m_nNumBlockX + 1) + nXSide].vPos;
	CKFVec3 vLS = vPLeftUp - vPSide;
	CKFVec3 vRS = vPRightDown - vPSide;
	CKFVec3 vNormal = vLS * vRS;
	CKFMath::VecNormalize(&vNormal);

	float fY = vPSide.m_fY - ((vPos.m_fX - vPSide.m_fX) * vNormal.m_fX + (vPos.m_fZ - vPSide.m_fZ) * vNormal.m_fZ) / vNormal.m_fY;

	//仮想アドレス解放
	m_meshInfo.pVtxBuffer->Unlock();

	return fY;
#endif
}
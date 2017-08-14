//--------------------------------------------------------------------------------
//	fieldColliderコンポネント
//　fieldColliderComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "fieldColliderComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool CFieldColliderComponent::Init(void)
{
	MakeVertex();
	return true;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CFieldColliderComponent::Uninit(void)
{
	m_vectorVtx.clear();
	CColliderComponent::Uninit();
}

//--------------------------------------------------------------------------------
//  fieldの情報取得
//--------------------------------------------------------------------------------
CFieldColliderComponent::INFO CFieldColliderComponent::GetPointInfo(const CKFVec3& vPos)
{
	INFO info;
	info.bInTheField = false;
	info.fHeight = 0.0f;
	info.vFaceNormal = CKFVec3(0.0f);
	CKFVec3 vPosCenter = CKFVec3(m_mtxOffset.m_af[3][0], m_mtxOffset.m_af[3][1], m_mtxOffset.m_af[3][2]);
	CKFVec3 vStartPos = vPosCenter + CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
	int nXLeftUp = (int)(((vPos.m_fX - vStartPos.m_fX) / (m_vBlockSize.m_fX * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZLeftUp = -(int)(((vPos.m_fZ - vStartPos.m_fZ) / (m_vBlockSize.m_fY * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);

	//フィールドの範囲外だったら処理終了
	if (nXLeftUp < 0 || nXLeftUp >= m_nNumBlockX || nZLeftUp < 0 || nZLeftUp >= m_nNumBlockZ)
	{
		return info;
	}

	int nXRightDown = nXLeftUp + 1;
	int nZRightDown = nZLeftUp + 1;

	CKFVec3 vPosTarget = CKFVec3(vPos.m_fX, 0.0f, vPos.m_fY);
	CKFVec3 vPLeftUp = m_vectorVtx[nZLeftUp * (m_nNumBlockX + 1) + nXLeftUp];
	CKFVec3 vPRightDown = m_vectorVtx[nZRightDown * (m_nNumBlockX + 1) + nXRightDown];

	//Check Side
	CKFVec3 vMid = vPRightDown - vPLeftUp;
	CKFVec3 vTL = vPosTarget - vPLeftUp;
	CKFVec3 vCross = vTL * vMid;
	int nXSide, nZSide;
	int nSign = 0;
	if (vCross.m_fY >= 0.0f)
	{//LeftSide
		nXSide = nXLeftUp + 1;
		nZSide = nZLeftUp;
		nSign = -1;
	}
	else
	{//RightSide
		nXSide = nXLeftUp;
		nZSide = nZLeftUp + 1;
		nSign = 1;
	}
	CKFVec3 vPSide = m_vectorVtx[nZSide * (m_nNumBlockX + 1) + nXSide];
	CKFVec3 vLS = vPLeftUp - vPSide;
	CKFVec3 vRS = vPRightDown - vPSide;
	CKFVec3 vNormal = (vLS * vRS) * (float)nSign;
	CKFMath::VecNormalize(vNormal);

	info.bInTheField = true;
	info.fHeight = vPSide.m_fY - ((vPos.m_fX - vPSide.m_fX) * vNormal.m_fX + (vPos.m_fZ - vPSide.m_fZ) * vNormal.m_fZ) / vNormal.m_fY;
	info.vFaceNormal = vNormal;

	return info;
}

//--------------------------------------------------------------------------------
//  fieldの情報取得
//--------------------------------------------------------------------------------
bool CFieldColliderComponent::GetVtxByRange(const CKFVec3& vBegin, const CKFVec3& vEnd, int& nNumVtxXOut, int& nNumVtxZOut, vector<CKFVec3>& vecOut)
{
	CKFVec3 vMin = CKFVec3(min(vBegin.m_fX, vEnd.m_fX), min(vBegin.m_fY, vEnd.m_fY), min(vBegin.m_fZ, vEnd.m_fZ));
	CKFVec3 vMax = CKFVec3(max(vBegin.m_fX, vEnd.m_fX), max(vBegin.m_fY, vEnd.m_fY), max(vBegin.m_fZ, vEnd.m_fZ));

	//範囲を算出
	CKFVec3 vPosCenter = CKFVec3(m_mtxOffset.m_af[3][0], m_mtxOffset.m_af[3][1], m_mtxOffset.m_af[3][2]);
	CKFVec3 vStartPos = vPosCenter + CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
	int nXMin = (int)(((vMin.m_fX - vStartPos.m_fX) / (m_vBlockSize.m_fX * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZMin = -(int)(((vMin.m_fZ - vStartPos.m_fZ) / (m_vBlockSize.m_fY * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);
	int nXMax = 1 + (int)(((vMax.m_fX - vStartPos.m_fX) / (m_vBlockSize.m_fX * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZMax = 1 - (int)(((vMax.m_fZ - vStartPos.m_fZ) / (m_vBlockSize.m_fY * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);
	nXMin = nXMin < 0 ? 0 : nXMin;
	nZMin = nZMin < 0 ? 0 : nZMin;
	nXMax = nXMax > m_nNumBlockX + 1 ? m_nNumBlockX + 1 : nXMax;
	nZMax = nZMax > m_nNumBlockZ + 1 ? m_nNumBlockZ + 1 : nZMax;

	//フィールドの範囲外だったら処理終了
	if (nXMin >= m_nNumBlockX + 1 || nZMin >= m_nNumBlockZ + 1
		|| nXMax <= 0 || nZMax <= 0)
	{
		return false;
	}

	//範囲内の頂点を保存する
	nNumVtxXOut = nXMax - nXMin;
	nNumVtxZOut = nZMax - nZMin;
	vecOut.resize(nNumVtxXOut * nNumVtxZOut);
	int nCnt = 0;
	for (int nCntZ = nZMin; nCntZ <= nZMax; nCntZ++)
	{
		for (int nCntX = nXMin; nCntX <= nXMax; nCntX++)
		{
			vecOut[nCnt] = m_vectorVtx[nCntZ * (m_nNumBlockX + 1) + nCntX];
			nCnt++;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------
//  頂点生成
//--------------------------------------------------------------------------------
void CFieldColliderComponent::MakeVertex(void)
{
	float fHeight = 0.0f;
	CKFVec3 vPos = CKFVec3(m_mtxOffset.m_af[3][0], m_mtxOffset.m_af[3][1], m_mtxOffset.m_af[3][2]);
	CKFVec3 vStartPos = vPos + CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
	for (int nCntZ = 0; nCntZ < m_nNumBlockZ + 1; nCntZ++)
	{
		if (nCntZ <= (m_nNumBlockZ + 1) / 4)
		{
			fHeight += 0.25f;
		}
		else if (nCntZ <= (m_nNumBlockZ + 1) / 2)
		{
			fHeight -= 0.25f;
		}
		else if (nCntZ <= (m_nNumBlockZ + 1) * 3 / 4)
		{
			fHeight += 0.5f;
		}
		else
		{
			fHeight -= 0.5f;
		}
		for (int nCntX = 0; nCntX < m_nNumBlockX + 1; nCntX++)
		{
			CKFVec3 vPos = vStartPos
				+ CKFVec3(nCntX * m_vBlockSize.m_fX, fHeight, -nCntZ * m_vBlockSize.m_fY);
			m_vectorVtx.push_back(vPos);
		}
	}
}
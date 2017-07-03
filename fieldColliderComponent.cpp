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
KFRESULT CFieldColliderComponent::Init(void)
{
	MakeVertex();
	return KF_SUCCEEDED;
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
//  更新処理
//--------------------------------------------------------------------------------
void CFieldColliderComponent::Update(void)
{
	
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

	CKFVec3 vStartPos = m_vPos + CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
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
	CKFMath::VecNormalize(&vNormal);

	info.bInTheField = true;
	info.fHeight = vPSide.m_fY - ((vPos.m_fX - vPSide.m_fX) * vNormal.m_fX + (vPos.m_fZ - vPSide.m_fZ) * vNormal.m_fZ) / vNormal.m_fY;
	info.vFaceNormal = vNormal;

	return info;
}

//--------------------------------------------------------------------------------
//  fieldの情報取得
//--------------------------------------------------------------------------------
std::list<CFieldColliderComponent::INFO> CFieldColliderComponent::GetRangeInfo(const CKFVec3& vBegin, const CKFVec3& vEnd)
{
	std::list<INFO> listInfo;

	//書き出す範囲を算出する
	//CKFVec3 vStartPos = m_vPos + CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
	//int nXLeftUpBegin = (int)(((vBegin.m_fX - vStartPos.m_fX) / (m_vBlockSize.m_fX * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	//int nZLeftUpBegin = -(int)(((vBegin.m_fZ - vStartPos.m_fZ) / (m_vBlockSize.m_fY * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);
	//int nXLeftUpEnd = (int)(((vEnd.m_fX - vStartPos.m_fX) / (m_vBlockSize.m_fX * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	//int nZLeftUpEnd = -(int)(((vEnd.m_fZ - vStartPos.m_fZ) / (m_vBlockSize.m_fY * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);
	
	//範囲内フィールドがあるかどうかをチェック


	return listInfo;
}

//--------------------------------------------------------------------------------
//  頂点生成
//--------------------------------------------------------------------------------
void CFieldColliderComponent::MakeVertex(void)
{
	float fHeight = 0.0f;
	CKFVec3 vStartPos = m_vPos + CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
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
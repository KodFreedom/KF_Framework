//--------------------------------------------------------------------------------
//	boxColliderコンポネント
//　boxColliderComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-08-07
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "colliderComponent.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  boxColliderポネントクラス
//--------------------------------------------------------------------------------
class CBoxColliderComponent : public CColliderComponent
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CBoxColliderComponent(CGameObject* const pGameObj, const CS::COL_TYPE& type, const CS::COL_MODE& mode, const CKFVec3& vHalfSize)
		: CColliderComponent(pGameObj, type, mode)
		, m_vHalfSize(vHalfSize) {}
	~CBoxColliderComponent() {}

	//Get関数
	list<CKFVec3>	GetLocalVertexes(void) const
	{
		CKFVec3 aVtx[8];
		aVtx[0] = CKFVec3(-m_vHalfSize.m_fX, -m_vHalfSize.m_fY, m_vHalfSize.m_fZ);
		aVtx[1] = CKFVec3(m_vHalfSize.m_fX, -m_vHalfSize.m_fY, m_vHalfSize.m_fZ);
		aVtx[2] = CKFVec3(-m_vHalfSize.m_fX, -m_vHalfSize.m_fY, -m_vHalfSize.m_fZ);
		aVtx[3] = CKFVec3(m_vHalfSize.m_fX, -m_vHalfSize.m_fY, -m_vHalfSize.m_fZ);
		aVtx[4] = CKFVec3(-m_vHalfSize.m_fX, m_vHalfSize.m_fY, m_vHalfSize.m_fZ);
		aVtx[5] = CKFVec3(m_vHalfSize.m_fX, m_vHalfSize.m_fY, m_vHalfSize.m_fZ);
		aVtx[6] = CKFVec3(-m_vHalfSize.m_fX, m_vHalfSize.m_fY, -m_vHalfSize.m_fZ);
		aVtx[7] = CKFVec3(m_vHalfSize.m_fX, m_vHalfSize.m_fY, -m_vHalfSize.m_fZ);
		list<CKFVec3> listVtx;
		for (int nCnt = 0; nCnt < 8; nCnt++) { listVtx.push_back(aVtx[nCnt]); }
		return listVtx;
	}
	list<CKFVec3>	GetWorldVertexes(void);
	CKFVec3			GetHalfSize(void) const { return m_vHalfSize; }

private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	CKFVec3 m_vHalfSize;
};
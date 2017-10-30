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
	CBoxColliderComponent(CGameObject* const pGameObj, const CS::COL_TYPE& type, const CS::COL_MODE& mode, const Vector3& vHalfSize)
		: CColliderComponent(pGameObj, type, mode)
		, m_vHalfSize(vHalfSize) {}
	~CBoxColliderComponent() {}

	//Get関数
	list<Vector3>	GetLocalVertexes(void) const
	{
		Vector3 aVtx[8];
		aVtx[0] = Vector3(-m_vHalfSize.X, -m_vHalfSize.Y, m_vHalfSize.Z);
		aVtx[1] = Vector3(m_vHalfSize.X, -m_vHalfSize.Y, m_vHalfSize.Z);
		aVtx[2] = Vector3(-m_vHalfSize.X, -m_vHalfSize.Y, -m_vHalfSize.Z);
		aVtx[3] = Vector3(m_vHalfSize.X, -m_vHalfSize.Y, -m_vHalfSize.Z);
		aVtx[4] = Vector3(-m_vHalfSize.X, m_vHalfSize.Y, m_vHalfSize.Z);
		aVtx[5] = Vector3(m_vHalfSize.X, m_vHalfSize.Y, m_vHalfSize.Z);
		aVtx[6] = Vector3(-m_vHalfSize.X, m_vHalfSize.Y, -m_vHalfSize.Z);
		aVtx[7] = Vector3(m_vHalfSize.X, m_vHalfSize.Y, -m_vHalfSize.Z);
		list<Vector3> listVtx;
		for (int nCnt = 0; nCnt < 8; nCnt++) { listVtx.push_back(aVtx[nCnt]); }
		return listVtx;
	}
	list<Vector3>	GetWorldVertexes(void);
	Vector3			GetHalfSize(void) const { return m_vHalfSize; }

private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	Vector3 m_vHalfSize;
};
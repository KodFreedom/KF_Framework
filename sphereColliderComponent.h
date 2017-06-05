//--------------------------------------------------------------------------------
//	sphereColliderコンポネント
//　sphereColliderComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-06-04
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
//  sphereColliderポネントクラス
//--------------------------------------------------------------------------------
class CSphereColliderComponent : public CColliderComponent
{
public:
	CSphereColliderComponent(CGameObject* const pGameObj, const CM::MODE& mode, const float& fRadius)
		: CColliderComponent(pGameObj, CM::COL_SPHERE, mode)
		, m_fRadius(fRadius)
	{
	}

	~CSphereColliderComponent() {}

	KFRESULT	Init(void) override { return KF_SUCCEEDED; }
	void		Update(void) override {}

	//Get関数
	float		GetRadius(void) { return m_fRadius; }

private:
	float		m_fRadius;	//半径
};
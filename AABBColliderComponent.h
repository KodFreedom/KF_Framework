//--------------------------------------------------------------------------------
//	AABBColliderコンポネント
//　AABBColliderComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-28
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "boxColliderComponent.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  AABBColliderポネントクラス
//--------------------------------------------------------------------------------
class CAABBColliderComponent : public CBoxColliderComponent
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CAABBColliderComponent(CGameObject* const pGameObj, const CS::COL_MODE& mode, const CKFVec3& vHalfSize)
		: CBoxColliderComponent(pGameObj, CS::COL_AABB, mode, vHalfSize) {}
	~CAABBColliderComponent() {}

	void	Update(void) override
	{
		CBoxColliderComponent::Update();

		//回転を初期化する
		m_mtxWorld.m_af[0][0] = 1.0f;
		m_mtxWorld.m_af[0][1] = 0.0f;
		m_mtxWorld.m_af[0][2] = 0.0f;
		m_mtxWorld.m_af[1][0] = 0.0f;
		m_mtxWorld.m_af[1][1] = 1.0f;
		m_mtxWorld.m_af[1][2] = 0.0f;
		m_mtxWorld.m_af[2][0] = 0.0f;
		m_mtxWorld.m_af[2][1] = 0.0f;
		m_mtxWorld.m_af[2][2] = 1.0f;
	}

private:
};
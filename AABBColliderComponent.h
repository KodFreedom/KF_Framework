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
	CAABBColliderComponent(CGameObject* const pGameObj, const CS::COL_MODE& mode, const Vector3& vHalfSize)
		: CBoxColliderComponent(pGameObj, CS::COL_AABB, mode, vHalfSize) {}
	~CAABBColliderComponent() {}

	void	Update(void) override
	{
		CBoxColliderComponent::Update();

		//回転を初期化する
		m_mtxWorldNext.Elements[0][0] = 1.0f;
		m_mtxWorldNext.Elements[0][1] = 0.0f;
		m_mtxWorldNext.Elements[0][2] = 0.0f;
		m_mtxWorldNext.Elements[1][0] = 0.0f;
		m_mtxWorldNext.Elements[1][1] = 1.0f;
		m_mtxWorldNext.Elements[1][2] = 0.0f;
		m_mtxWorldNext.Elements[2][0] = 0.0f;
		m_mtxWorldNext.Elements[2][1] = 0.0f;
		m_mtxWorldNext.Elements[2][2] = 1.0f;
	}

private:
};
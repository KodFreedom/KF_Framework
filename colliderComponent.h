//--------------------------------------------------------------------------------
//	colliderコンポネント
//　colliderComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "component.h"
#include "colliderManager.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Colliderポネントクラス
//--------------------------------------------------------------------------------
class CColliderComponent : public CComponent
{
public:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CColliderComponent(CGameObject* const pGameObj, const CM::COL_TYPE& type, const CM::MODE& mode, const CKFVec3& vPos = CKFVec3(0.0f));
	~CColliderComponent() {}

	virtual KFRESULT	Init(void) override = 0;
	virtual void		Uninit(void) override;
	virtual void		Update(void) = 0;
	virtual void		OnCollision(CColliderComponent& collider) {}
	virtual void		OnTrigger(CColliderComponent& collider) {}

	//Set関数
	void				SetTrigger(const bool& bTrigger) { m_bTrigger = bTrigger; }

	//Get関数
	const CKFVec3		GetLocalPos(void) const { return m_vPos; }
	const CKFVec3		GetWorldPos(void) const;
	const CM::COL_TYPE	GetType(void) const { return m_type; }
	const bool			IsTrigger(void) { return m_bTrigger; }

protected:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CColliderComponent() : CComponent() , m_type(CM::COL_MAX) {}

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	CM::COL_TYPE	m_type;		//colliderのタイプ
	CM::COL_ITR		m_itr;		//マネージャlist中の位置
	CM::MODE		m_mode;		//Static/Dynamic mode
	bool			m_bTrigger;	//Trigger Flag
	CKFVec3			m_vPos;		//相対位置
};
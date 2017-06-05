
//--------------------------------------------------------------------------------
//
//　3DPhysicsComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-31
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "rigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  3D物理コンポネントクラス
//--------------------------------------------------------------------------------
class C3DRigidbodyComponent : public CRigidbodyComponent
{
public:
	C3DRigidbodyComponent(CGameObject* const pGameObj);
	~C3DRigidbodyComponent() {}

	KFRESULT	Init(void) override { return KF_SUCCEEDED; }
	void		Uninit(void) override {}
	void		Update(void) override;

	//Get関数
	CKFVec3		GetVelocity(void) const { return m_vVelocity; }

	//Set関数
	void		MovePos(const CKFVec3& vMovement) { m_vMovement = vMovement; }
	void		SetVelocity(const CKFVec3& vVelocity) { m_vVelocity = vVelocity; }

private:
	float	m_fMass;
	CKFVec3 m_vMovement;
	CKFVec3 m_vVelocity;
};
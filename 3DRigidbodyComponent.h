
//--------------------------------------------------------------------------------
//
//�@3DPhysicsComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-31
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "rigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  3D�����R���|�l���g�N���X
//--------------------------------------------------------------------------------
class C3DRigidbodyComponent : public CRigidbodyComponent
{
public:
	C3DRigidbodyComponent(CGameObject* const pGameObj);
	~C3DRigidbodyComponent() {}

	KFRESULT	Init(void) override { return KF_SUCCEEDED; }
	void		Uninit(void) override {}
	void		Update(void) override;

	//Get�֐�
	CKFVec3		GetVelocity(void) const { return m_vVelocity; }

	//Set�֐�
	void		MovePos(const CKFVec3& vMovement) { m_vMovement = vMovement; }
	void		SetVelocity(const CKFVec3& vVelocity) { m_vVelocity = vVelocity; }

private:
	float	m_fMass;
	CKFVec3 m_vMovement;
	CKFVec3 m_vVelocity;
};
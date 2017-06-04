//--------------------------------------------------------------------------------
//
//�@3DPhysicsComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-31
//--------------------------------------------------------------------------------
#ifndef _3D_PHYSICS_COMPONENT_H_
#define _3D_PHYSICS_COMPONENT_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "physicsComponent.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  3D�����R���|�l���g�N���X
//--------------------------------------------------------------------------------
class C3DPhysicsComponent : public CPhysicsComponent
{
public:
	C3DPhysicsComponent(CGameObject* const pGameObj);
	~C3DPhysicsComponent() {}

	KFRESULT	Init(void) override { return KF_SUCCEEDED; }
	void		Uninit(void) override {}
	void		Update(void) override;

	//Set�֐�
	void		MovePosByRot(const float &fMovement, const float &fRot) 
	{ 
		m_fMovement = fMovement; 
		m_fRot = fRot;
	}

private:
	float		m_fMovement;
	float		m_fRot;

};

#endif
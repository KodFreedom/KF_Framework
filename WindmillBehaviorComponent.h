//--------------------------------------------------------------------------------
//	���ԃr�w�C�r�A�R���|�l���g
//�@windmillBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-09-12
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "behaviorComponent.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CWindmillBehaviorComponent : public CBehaviorComponent
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CWindmillBehaviorComponent(CGameObject* const pGameObj)
		: CBehaviorComponent(pGameObj)
		, RotSpeed(0.01f)
		, m_pFan(nullptr)
	{}
	~CWindmillBehaviorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override {}
	void	Update(void) override;
	void	LateUpdate(void) override {}

	void	OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) override {}
	void	OnCollision(CCollisionInfo& collisionInfo) override {}

private:
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	CGameObject*	findFan(CGameObject* pParent);

	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	float			RotSpeed;
	CGameObject*	m_pFan;
};
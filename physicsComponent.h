//--------------------------------------------------------------------------------
//
//�@physicsComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _PHYSICS_COMPONENT_H_
#define _PHYSICS_COMPONENT_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "component.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CGameObject;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �����R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CPhysicsComponent : public CComponent
{
public:
	CPhysicsComponent() {}
	~CPhysicsComponent() {}
	
	virtual KFRESULT	Init(void) override = 0;
	virtual void		Uninit(void) override = 0;
	virtual void		Update(CGameObject &gameObj) = 0;
	virtual void		ReceiveMsg(const MESSAGE &msg) override = 0;
};

//--------------------------------------------------------------------------------
//  �k�������R���|�l���g
//--------------------------------------------------------------------------------
class CNullPhysicsComponent : public CPhysicsComponent
{
public:
	CNullPhysicsComponent() {}
	~CNullPhysicsComponent() {}

	KFRESULT	Init(void) override { return KF_SUCCEEDED; }
	void		Uninit(void) override {}
	void		Update(CGameObject &gameObj) override {}
	void		Release(void) override {}
	void		ReceiveMsg(const MESSAGE &msg) override {}
	
};

#endif

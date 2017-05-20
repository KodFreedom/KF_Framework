//--------------------------------------------------------------------------------
//
//�@physicComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _PHYSIC_COMPONENT_H_
#define _PHYSIC_COMPONENT_H_

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
class CPhysicComponent : public CComponent
{
public:
	CPhysicComponent() {}
	~CPhysicComponent() {}
	
	virtual KFRESULT	Init(void) = 0;
	virtual void		Uninit(void) = 0;
	virtual void		Update(CGameObject &gameObj) = 0;
	virtual void		Release(void) = 0;
	virtual void		ReceiveMsg(const MESSAGE &msg) override = 0;
};

//--------------------------------------------------------------------------------
//  �k�������R���|�l���g
//--------------------------------------------------------------------------------
class CNullPhysicComponent : public CPhysicComponent
{
public:
	CNullPhysicComponent() {}
	~CNullPhysicComponent() {}

	KFRESULT	Init(void) override {}
	void		Uninit(void) override {}
	void		Update(CGameObject &gameObj) override {}
	void		Release(void) override {}
	void		ReceiveMsg(const MESSAGE &msg) override {}
	
};

#endif

//--------------------------------------------------------------------------------
//
//　3DPhysicsComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-31
//--------------------------------------------------------------------------------
#ifndef _3D_PHYSICS_COMPONENT_H_
#define _3D_PHYSICS_COMPONENT_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "physicsComponent.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  3D物理コンポネントクラス
//--------------------------------------------------------------------------------
class C3DPhysicsComponent : public CPhysicsComponent
{
public:
	C3DPhysicsComponent(CGameObject* const pGameObj);
	~C3DPhysicsComponent() {}

	KFRESULT	Init(void) override { return KF_SUCCEEDED; }
	void		Uninit(void) override {}
	void		Update(void) override;

	//Set関数
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
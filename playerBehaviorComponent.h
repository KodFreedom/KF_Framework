//--------------------------------------------------------------------------------
//	プレイヤービヘイビアコンポネント
//　playerBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "actorBehaviorComponent.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
//class C3DRigidbodyComponent;
//class CActorMeshComponent;
class CStatus;
class CPlayerNormalStatus;
class CPlayerMoveStatus;
class CPlayerJumpStatus;
class CPlayerAttackStatus;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  プレイヤー行動コンポネントクラス
//--------------------------------------------------------------------------------
class CPlayerBehaviorComponent : public CActorBehaviorComponent
{
	//--------------------------------------------------------------------------------
	//  フレンドクラス
	//--------------------------------------------------------------------------------
	friend CPlayerNormalStatus;
	friend CPlayerMoveStatus;
	friend CPlayerJumpStatus;
	friend CPlayerAttackStatus;

public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CPlayerBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody);
	~CPlayerBehaviorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override;
			
	void	OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) override;
	void	OnCollision(CCollisionInfo& collisionInfo) override;

	//状態の切り替え
	void	ChangeStatus(CStatus* const pStatus);

private:
	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	CStatus*	m_pStatus;		//状態

	unsigned int	m_usCntWhosYourDaddy;
};
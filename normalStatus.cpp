//--------------------------------------------------------------------------------
//	ノーマルステータス
//　normalStatus.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "attackStatus.h"
#include "jumpStatus.h"
#include "moveStatus.h"
#include "normalStatus.h"
#include "gameObject.h"
#include "actorBehaviorComponent.h"
#include "actorMeshComponent.h"
#include "manager.h"
#include "inputManager.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  更新
//--------------------------------------------------------------------------------
void CNormalStatus::Update(CActorBehaviorComponent& actor)
{
	CInputManager* pInput = GetManager()->GetInputManager();

	//移動
	CKFVec2 vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
	if (fabsf(vAxis.m_fX) > 0.1f || fabsf(vAxis.m_fY) > 0.1f)
	{
		actor.ChangeStatus(new CMoveStatus);
		return;
	}

	//ジャンプ
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_JUMP))
	{
		actor.ChangeStatus(new CJumpStatus);
		return;
	}

	//攻撃
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_ATTACK))
	{
		actor.ChangeStatus(new CAttackStatus);
		return;
	}

	//ニュートラル
	CMeshComponent* pMesh = actor.m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	actor.Stay(pActor);
}

//--------------------------------------------------------------------------------
//  更新
//--------------------------------------------------------------------------------
void CNormalStatus::LateUpdate(CActorBehaviorComponent& actor)
{

}
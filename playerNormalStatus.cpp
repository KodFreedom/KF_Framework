//--------------------------------------------------------------------------------
//	プレイやノーマルステータス
//　playerNormalStatus.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "playerNormalStatus.h"
#include "playerMoveStatus.h"
#include "playerJumpStatus.h"
#include "playerAttackStatus.h"
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
void CPlayerNormalStatus::Update(CActorBehaviorComponent& actor)
{
	CInputManager* pInput = GetManager()->GetInputManager();

	//移動
	CKFVec2 vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
	if (fabsf(vAxis.m_fX) > 0.1f || fabsf(vAxis.m_fY) > 0.1f)
	{
		actor.ChangeStatus(new CPlayerMoveStatus);
		return;
	}

	//ジャンプ
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_JUMP))
	{
		actor.ChangeStatus(new CPlayerJumpStatus);
		return;
	}

	//攻撃
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_ATTACK))
	{
		actor.ChangeStatus(new CPlayerAttackStatus);
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
void CPlayerNormalStatus::LateUpdate(CActorBehaviorComponent& actor)
{

}
//--------------------------------------------------------------------------------
//	プレイヤー攻撃ステータス
//　playerAttackStatus.h
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "playerAttackStatus.h"
#include "playerJumpStatus.h"
#include "playerMoveStatus.h"
#include "playerNormalStatus.h"
#include "gameObject.h"
#include "playerBehaviorComponent.h"
#include "actorMeshComponent.h"
#include "sphereColliderComponent.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  更新
//--------------------------------------------------------------------------------
void CPlayerAttackStatus::Update(CPlayerBehaviorComponent& player)
{
	CMeshComponent* pMesh = player.m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;

	//攻撃
	if (m_bFirst)
	{
		m_bFirst = false;
		player.Attack(pActor);
	}

	if (pActor->GetMotionNow() == CActorMeshComponent::MOTION_ATTACK)
	{
		if (pActor->GetMotionInfo().nKeyNow == 3 && !player.m_pAttackCollider)
		{
			player.m_pAttackCollider = new CSphereColliderComponent(player.m_pGameObj, CS::DYNAMIC, 0.9f);
			player.m_pAttackCollider->SetOffset(CKFVec3(0.0f, 0.6f, 2.1f));
			player.m_pAttackCollider->SetTag("weapon");
			player.m_pAttackCollider->SetTrigger(true);
		}
	}
	else
	{//攻撃モーション終わったらノーマルステータスに戻る
		if (player.m_pAttackCollider)
		{
			player.m_pGameObj->DeleteCollider(player.m_pAttackCollider);
			player.m_pAttackCollider->Release();
			player.m_pAttackCollider = nullptr;
		}

		player.ChangeStatus(new CPlayerNormalStatus);
	}
}

//--------------------------------------------------------------------------------
//  更新
//--------------------------------------------------------------------------------
void CPlayerAttackStatus::LateUpdate(CPlayerBehaviorComponent& player)
{

}
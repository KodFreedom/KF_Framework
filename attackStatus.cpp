//--------------------------------------------------------------------------------
//	攻撃ステータス
//　attackStatus.h
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
#include "sphereColliderComponent.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  更新
//--------------------------------------------------------------------------------
void CAttackStatus::Update(CActorBehaviorComponent& actor)
{
	CMeshComponent* pMesh = actor.m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;

	//攻撃
	if (m_bFirst)
	{
		m_bFirst = false;
		actor.Attack(pActor);
	}

	if (pActor->GetMotionNow() == CActorMeshComponent::MOTION_ATTACK)
	{
		if (pActor->GetMotionInfo().nKeyNow == 3 && !actor.m_pAttackCollider)
		{
			actor.m_pAttackCollider = new CSphereColliderComponent(actor.m_pGameObj, CM::DYNAMIC, CKFVec3(0.0f, 0.6f, 2.1f), 0.9f);
			actor.m_pAttackCollider->SetTag("weapon");
			actor.m_pAttackCollider->SetTrigger(true);
		}
	}
	else
	{//攻撃モーション終わったらノーマルステータスに戻る
		if (actor.m_pAttackCollider)
		{
			actor.m_pGameObj->DeleteCollider(actor.m_pAttackCollider);
			actor.m_pAttackCollider->Release();
			actor.m_pAttackCollider = nullptr;
		}

		actor.ChangeStatus(new CNormalStatus);
	}
}

//--------------------------------------------------------------------------------
//  更新
//--------------------------------------------------------------------------------
void CAttackStatus::LateUpdate(CActorBehaviorComponent& actor)
{

}
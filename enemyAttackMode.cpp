//--------------------------------------------------------------------------------
//	エネミービヘイビアコンポネント
//　enemyBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "enemyAttackMode.h"
#include "enemyNormalMode.h"
#include "enemyBehaviorComponent.h"
#include "gameObjectActor.h"
#include "actorMeshComponent.h"
#include "3DRigidbodyComponent.h"
#include "sphereColliderComponent.h"
#include "colliderComponent.h"
#include "KF_CollisionSystem.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  攻撃状態
//--------------------------------------------------------------------------------
void CEnemyAttackMode::Update(CEnemyBehaviorComponent& enemy)
{
	if (!enemy.m_pGameObj)
	{//相手がいないなら普通な状態に戻る
		enemy.ChangeMode(new CEnemyNormalMode);
		return;
	}

	CMeshComponent* pMesh = enemy.m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	bool bCanControl = true;
	if (pActor->GetMotionNow() == CActorMeshComponent::MOTION::MOTION_ATTACK)
	{
		if (pActor->GetMotionInfo().nKeyNow == 3 && !enemy.m_pAttackCollider)
		{
			enemy.m_pAttackCollider = new CSphereColliderComponent(enemy.m_pGameObj, CS::DYNAMIC, 0.9f);
			enemy.m_pAttackCollider->SetOffset(CKFVec3(0.0f, 0.6f, 2.1f));
			enemy.m_pAttackCollider->SetTag("weapon");
			enemy.m_pAttackCollider->SetTrigger(true);
		}
		bCanControl = false;
	}
	if (!bCanControl) { return; }
	if (enemy.m_pAttackCollider)
	{
		enemy.m_pGameObj->DeleteCollider(enemy.m_pAttackCollider);
		enemy.m_pAttackCollider->Release();
		enemy.m_pAttackCollider = nullptr;
	}

	CTransformComponent* pTransform = enemy.GetGameObject()->GetTransformComponent();
	CKFVec3 vPosTarget = enemy.m_pTarget->GetTransformComponent()->GetPosNext();
	CKFVec3 vPosThis = pTransform->GetPosNext();
	CKFVec3 vForward = pTransform->GetForwardNext();
	CKFVec3 vDiff = vPosTarget - vPosThis;
	CKFVec3 vDir = vDiff;
	CKFMath::VecNormalize(vDir);

	//敵検知範囲を取得
	float fDisMax = 0.0f;
	auto listCollider = enemy.GetGameObject()->GetColliderComponent();
	for (auto itr = listCollider.begin(); itr != listCollider.end(); ++itr)
	{
		if ((*itr)->GetTag() == "detector")
		{
			fDisMax = ((CSphereColliderComponent*)(*itr))->GetRadius();
		}
	}

	float fDisSquare = CKFMath::VecMagnitudeSquare(vDiff);
	if (fDisSquare > fDisMax * fDisMax)
	{//相手との距離が長ければ普通な状態に戻る
		enemy.ChangeMode(new CEnemyNormalMode);
		return;
	}
	else if (fDisSquare <= 1.8f * 1.8f)
	{//距離が短いなら攻撃
		enemy.Attack(pActor);
	}
	else
	{//相手に向かって移動する
	 //相手に向く
	 //回転計算
		CKFVec3 vUp = pTransform->GetUpNext();
		vDir = (vUp * vDir) * vUp;
		enemy.Turn(vDir);

		//移動
		enemy.Move(pActor);
	}
}
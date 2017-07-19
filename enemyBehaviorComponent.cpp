//--------------------------------------------------------------------------------
//	エネミービヘイビアコンポネント
//　enemyBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "enemyBehaviorComponent.h"
#include "manager.h"
#include "mode.h"
#include "camera.h"
#include "inputManager.h"
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
//  コンストラクタ
//--------------------------------------------------------------------------------
CEnemyBehaviorComponent::CEnemyBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody, const float& fMoveSpeed, const float& fJumpForce)
	: CBehaviorComponent(pGameObj)
	, c_pRigidbody(pRigidbody)
	, c_fSpeed(fMoveSpeed)
	, c_fJumpForce(fJumpForce)
	, m_fLifeNow(c_fLifeMax)
	, m_pState(nullptr)
	, m_pAttackCollider(nullptr)
	, m_usCntWhosYourDaddy(0)
{}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool CEnemyBehaviorComponent::Init(void)
{
	m_pState = new CNormalEnemyState;
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CEnemyBehaviorComponent::Uninit(void)
{
	if (m_pState)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	if (m_pAttackCollider)
	{
		m_pGameObj->DeleteCollider(m_pAttackCollider);
		m_pAttackCollider->Release();
		m_pAttackCollider = nullptr;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CEnemyBehaviorComponent::Update(void)
{
	if (m_usCntWhosYourDaddy) { m_usCntWhosYourDaddy--; }
	m_pState->Update(*this);
}

//--------------------------------------------------------------------------------
//  OnTrigger
//--------------------------------------------------------------------------------
void CEnemyBehaviorComponent::OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider)
{
	if (collider.GetGameObject()->GetObjType() == CGameObject::OT_PLAYER)
	{//プレイヤー
		if (colliderThis.GetTag() == "detector")
		{//敵検知範囲
			m_pTarget = collider.GetGameObject();
			ChangeState(new CAttackEnemyState);
		}

		if (!m_usCntWhosYourDaddy && collider.GetTag() == "weapon" && colliderThis.GetTag() == "body")
		{
			m_usCntWhosYourDaddy = 30;
			m_fLifeNow -= 25.0f;
			if (m_fLifeNow <= 0.0f)
			{
				m_pGameObj->SetAlive(false);
				GetManager()->GetMode()->EndMode();
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  OnCollision
//--------------------------------------------------------------------------------
void CEnemyBehaviorComponent::OnCollision(CColliderComponent& colliderThis, CCollisionInfo& collisionInfo)
{

}

//--------------------------------------------------------------------------------
//  ChangeState
//--------------------------------------------------------------------------------
void CEnemyBehaviorComponent::ChangeState(CEnemyState* pEnemyState)
{
	if (!pEnemyState) { return; }
	delete m_pState;
	m_pState = pEnemyState;
}

//--------------------------------------------------------------------------------
//  普通状態
//--------------------------------------------------------------------------------
void CNormalEnemyState::Update(CEnemyBehaviorComponent& enemy)
{
	CMeshComponent* pMesh = enemy.m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	bool bCanControl = true;
	if (pActor->GetMotionNow() == CActorMeshComponent::MOTION::MOTION_ATTACK) { bCanControl = false; }
	if (!bCanControl) { return; }
	if (enemy.m_pAttackCollider)
	{
		enemy.m_pGameObj->DeleteCollider(enemy.m_pAttackCollider);
		enemy.m_pAttackCollider->Release();
		enemy.m_pAttackCollider = nullptr;
	}

	//ニュートラルモーション設定
	pActor->SetMotion(CActorMeshComponent::MOTION_NEUTAL);
}

//--------------------------------------------------------------------------------
//  攻撃状態
//--------------------------------------------------------------------------------
void CAttackEnemyState::Update(CEnemyBehaviorComponent& enemy)
{
	if (!enemy.m_pGameObj)
	{//相手がいないなら普通な状態に戻る
		enemy.ChangeState(new CNormalEnemyState);
		return;
	}

	CMeshComponent* pMesh = enemy.m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	bool bCanControl = true;
	if (pActor->GetMotionNow() == CActorMeshComponent::MOTION::MOTION_ATTACK)
	{
		if (pActor->GetMotionInfo().nKeyNow == 3 && !enemy.m_pAttackCollider)
		{
			enemy.m_pAttackCollider = new CSphereColliderComponent(enemy.m_pGameObj, CM::DYNAMIC, CKFVec3(0.0f, 0.6f, 2.1f), 0.9f);
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
		enemy.ChangeState(new CNormalEnemyState);
		return;
	}
	else if (fDisSquare <= 1.8f * 1.8f)
	{//距離が短いなら攻撃
		pActor->SetMotion(CActorMeshComponent::MOTION_ATTACK);
	}
	else
	{//相手に向かって移動する
		//相手に向く
		CKFVec3 vForwardNext = CKFMath::LerpNormal(vForward, vDir, 0.2f);
		CKFVec3 vUp = pTransform->GetUpNext();
		vForwardNext = (vUp * vForwardNext) * vUp;
		pTransform->RotByForward(vForwardNext);

		//移動
		//移動設定
		enemy.c_pRigidbody->MovePos(vForwardNext * enemy.c_fSpeed);

		//移動モーション設定
		pActor->SetMotion(CActorMeshComponent::MOTION_MOVE);
	}
}
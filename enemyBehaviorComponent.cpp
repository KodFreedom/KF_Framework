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
#include "gameObjectActor.h"
#include "actorMeshComponent.h"
#include "colliderComponent.h"
#include "enemyNormalMode.h"
#include "enemyAttackMode.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CEnemyBehaviorComponent::CEnemyBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody)
	: CActorBehaviorComponent(pGameObj, pRigidbody)
	, m_pTarget(nullptr)
	, m_pMode(nullptr)
	, m_usCntWhosYourDaddy(0)
{}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool CEnemyBehaviorComponent::Init(void)
{
	m_pMode = new CEnemyNormalMode;
	m_fMovementSpeed = 0.05f;
	m_fJumpForce = 20.0f;
	m_fTurnRate = 0.2f;
	m_fLifeMax = 100.0f;
	m_fLifeNow = 100.0f;
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CEnemyBehaviorComponent::Uninit(void)
{
	if (m_pMode)
	{
		delete m_pMode;
		m_pMode = nullptr;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CEnemyBehaviorComponent::Update(void)
{
	m_pMode->Update(*this);
	if (m_usCntWhosYourDaddy) { m_usCntWhosYourDaddy--; }
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
			ChangeMode(new CEnemyAttackMode);
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
void CEnemyBehaviorComponent::ChangeMode(CAIMode* pAIMode)
{
	if (!pAIMode) { return; }
	delete m_pMode;
	m_pMode = pAIMode;
}

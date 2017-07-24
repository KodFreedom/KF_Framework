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

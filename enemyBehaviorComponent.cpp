//--------------------------------------------------------------------------------
//	�G�l�~�[�r�w�C�r�A�R���|�l���g
//�@enemyBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "enemyBehaviorComponent.h"
#include "actorBehaviorComponent.h"
#include "manager.h"
#include "mode.h"
#include "gameObjectActor.h"
#include "actorMeshComponent.h"
#include "colliderComponent.h"
#include "enemyNormalMode.h"
#include "enemyAttackMode.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CEnemyBehaviorComponent::CEnemyBehaviorComponent(CGameObject* const pGameObj, CActorBehaviorComponent& actor)
	: CBehaviorComponent(pGameObj)
	, m_actor(actor)
	, m_pTarget(nullptr)
	, m_pMode(nullptr)
{}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool CEnemyBehaviorComponent::Init(void)
{
	m_pMode = new CEnemyNormalMode;
	m_actor.SetLifeMax(100.0f);
	m_actor.SetLifeNow(100.0f);
	m_actor.SetJumpSpeed(6.0f);
	m_actor.SetTurnSpeedMin(180.0f * DELTA_TIME);
	m_actor.SetTurnSpeedMax(360.0f * DELTA_TIME);
	m_actor.SetMoveSpeed(0.075f);
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
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
//  �X�V����
//--------------------------------------------------------------------------------
void CEnemyBehaviorComponent::Update(void)
{
	m_pMode->Update(*this);
}

//--------------------------------------------------------------------------------
//  OnTrigger
//--------------------------------------------------------------------------------
void CEnemyBehaviorComponent::OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider)
{
	if (collider.GetGameObject()->GetObjType() == CGameObject::OT_PLAYER)
	{//�v���C���[
		if (colliderThis.GetTag() == "detector")
		{//�G���m�͈�
			m_pTarget = collider.GetGameObject();
			ChangeMode(new CEnemyAttackMode);
		}

		if (collider.GetTag() == "weapon" && colliderThis.GetTag() == "body")
		{
			m_actor.Hit(10.0f);
			if (m_actor.GetLifeNow() <= 0.0f)
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
void CEnemyBehaviorComponent::OnCollision(CCollisionInfo& collisionInfo)
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

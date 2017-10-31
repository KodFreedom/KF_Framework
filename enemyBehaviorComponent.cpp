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
#include "colliderComponent.h"
#include "enemyNormalMode.h"
#include "enemyAttackMode.h"

#ifdef _DEBUG
#include "debugManager.h"
#endif

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
	m_actor.SetJumpSpeed(0.0f);
	m_actor.SetTurnSpeedMin(2.0f * KF_PI * DELTA_TIME);
	m_actor.SetTurnSpeedMax(4.0f * KF_PI * DELTA_TIME);
	m_actor.SetMoveSpeed(5.0f);
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CEnemyBehaviorComponent::Uninit(void)
{
	if (!m_pMode) { return; }
	delete m_pMode;
	m_pMode = nullptr;
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CEnemyBehaviorComponent::Update(void)
{
	m_pMode->Update(*this);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CEnemyBehaviorComponent::LateUpdate(void)
{
	if (m_actor.GetLifeNow() <= 0.0f)
	{
#ifdef _DEBUG
		Main::GetManager()->GetDebugManager()->DisplayScroll(GetGameObject()->GetName() + " is dead!\n");
#endif
		m_pGameObj->SetAlive(false);
	}
}

//--------------------------------------------------------------------------------
//  OnTrigger
//--------------------------------------------------------------------------------
void CEnemyBehaviorComponent::OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider)
{
	if (collider.GetGameObject()->GetTag()._Equal("Player"))
	{//�v���C���[
		if (colliderThis.GetTag()._Equal("detector") && !m_pTarget)
		{//�G���m�͈�
#ifdef _DEBUG
			Main::GetManager()->GetDebugManager()->DisplayScroll(GetGameObject()->GetName() + " find " + collider.GetGameObject()->GetName() + "!\n");
#endif
			m_pTarget = collider.GetGameObject();
			ChangeMode(new CEnemyAttackMode);
		}

		if (collider.GetTag()._Equal("weapon") && colliderThis.GetTag()._Equal("body"))
		{
#ifdef _DEBUG
			Main::GetManager()->GetDebugManager()->DisplayScroll(GetGameObject()->GetName() + " is hurted!\n");
#endif

			m_actor.Hit(25.0f);
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

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
#include "collider.h"
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
EnemyController::EnemyController(GameObject* const pGameObj, ActorController& actor)
	: Behavior(pGameObj)
	, m_actor(actor)
	, m_pTarget(nullptr)
	, m_pMode(nullptr)
{}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool EnemyController::Init(void)
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
void EnemyController::Uninit(void)
{
	if (!m_pMode) { return; }
	delete m_pMode;
	m_pMode = nullptr;
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void EnemyController::Update(void)
{
	m_pMode->Update(*this);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void EnemyController::LateUpdate(void)
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
void EnemyController::OnTrigger(Collider& colliderThis, Collider& collider)
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
void EnemyController::OnCollision(CollisionInfo& collisionInfo)
{

}

//--------------------------------------------------------------------------------
//  ChangeState
//--------------------------------------------------------------------------------
void EnemyController::ChangeMode(CAIMode* pAIMode)
{
	if (!pAIMode) { return; }
	delete m_pMode;
	m_pMode = pAIMode;
}

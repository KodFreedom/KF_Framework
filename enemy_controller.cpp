//--------------------------------------------------------------------------------
//	�G�l�~�[�r�w�C�r�A�R���|�l���g
//�@EnemyController.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "enemyController.h"
#include "gameObjectActor.h"
#include "collider.h"
#include "normalModeAI.h"
#include "neutralState.h"

#ifdef _DEBUG
#include "debugObserver.h"
#endif

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
EnemyController::EnemyController(GameObjectActor* const owner, Rigidbody3D& rigidbody)
	: ActorController(owner, "EnemyController", rigidbody)
	, target(nullptr)
	, currentMode(nullptr)
{}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool EnemyController::Init(void)
{
	ActorController::Change(new NeutralState);
	currentMode = new NormalModeAI;
	paramater.SetMaxLife(100.0f);
	paramater.SetCurrentLife(100.0f);
	paramater.SetJumpSpeed(0.0f);
	paramater.SetMinTurnSpeed(2.0f * Pi);
	paramater.SetMaxTurnSpeed(4.0f * Pi);
	paramater.SetMoveSpeed(5.0f);
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void EnemyController::Uninit(void)
{
	SAFE_DELETE(currentMode);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void EnemyController::Update(void)
{
	currentMode->Update(*this);
	currentState->Act(*this);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void EnemyController::LateUpdate(void)
{
	if (paramater.GetCurrentLife() <= 0.0f)
	{
#ifdef _DEBUG
		DebugObserver::Instance()->DisplayScroll(owner->GetName() + " is dead!\n");
#endif
		owner->SetAlive(false);
	}
}

//--------------------------------------------------------------------------------
//  OnTrigger
//--------------------------------------------------------------------------------
void EnemyController::OnTrigger(Collider& colliderThis, Collider& collider)
{
	if (collider.GetGameObject()->GetTag()._Equal("Player"))
	{//�v���C���[
		if (colliderThis.GetTag()._Equal("detector") && !target)
		{//�G���m�͈�
#ifdef _DEBUG
			DebugObserver::Instance()->DisplayScroll(owner->GetName() + " find " + collider.GetGameObject()->GetName() + "!\n");
#endif
			target = collider.GetGameObject();
		}

		if (collider.GetTag()._Equal("weapon") && colliderThis.GetTag()._Equal("body"))
		{
#ifdef _DEBUG
			DebugObserver::Instance()->DisplayScroll(owner->GetName() + " is hurted!\n");
#endif

			currentState->OnDamaged(*this, 25.0f);
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
//  ChangeMode
//--------------------------------------------------------------------------------
void EnemyController::Change(AI* mode)
{
	SAFE_DELETE(currentMode);
	currentMode = mode;
}

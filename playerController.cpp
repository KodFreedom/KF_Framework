//--------------------------------------------------------------------------------
//	�v���C���[�r�w�C�r�A�R���|�l���g
//�@playerController.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "playerController.h"
#include "input.h"
#include "camera.h"
#include "cameraManager.h"
#include "gameObjectActor.h"
#include "neutralState.h"

#ifdef _DEBUG
#include "debugObserver.h"
#endif

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
PlayerController::PlayerController(GameObjectActor* const owner, Rigidbody3D& rigidbody)
	: ActorController(owner, "PlayerController", rigidbody)
{}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool PlayerController::Init(void)
{
	Change(new NeutralState);
	paramater.SetMaxLife(100.0f);
	paramater.SetCurrentLife(100.0f);
	paramater.SetJumpSpeed(20.0f);
	paramater.SetMinTurnSpeed(2.0f * Pi);
	paramater.SetMaxTurnSpeed(4.0f * Pi);
	paramater.SetMoveSpeed(100.0f);
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerController::Uninit(void)
{
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerController::Update(void)
{
	auto input = Input::Instance();
	auto& axis = Vector2(input->MoveHorizontal(), input->MoveVertical());
	auto camera = CameraManager::Instance()->GetMainCamera();
	auto& cameraForward = Vector3::Scale(camera->GetForward(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();
	movement = (camera->GetRight() * axis.X + cameraForward * axis.Y).Normalized();
	isJump = input->GetKeyTrigger(Key::Jump);
	isAttack = input->GetKeyTrigger(Key::Attack);
	currentState->Act(*this);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerController::LateUpdate(void)
{
}

//--------------------------------------------------------------------------------
//  OnTrigger
//--------------------------------------------------------------------------------
void PlayerController::OnTrigger(Collider& colliderThis, Collider& collider)
{
	if (collider.GetGameObject()->GetTag()._Equal("Enemy"))
	{//����`�F�b�N
		if (collider.GetTag()._Equal("weapon") && colliderThis.GetTag()._Equal("body"))
		{
#ifdef _DEBUG
			DebugObserver::Instance()->DisplayScroll(owner->GetName() + " is hurted by" + collider.GetGameObject()->GetParentName() + "!");
#endif
			currentState->OnDamaged(*this, 5.0f);
		}
	}

	if (collider.GetGameObject()->GetTag()._Equal("Goal"))
	{
		if (colliderThis.GetTag()._Equal("body"))
		{
		}
	}
}

//--------------------------------------------------------------------------------
//  OnCollision
//--------------------------------------------------------------------------------
void PlayerController::OnCollision(CollisionInfo& collisionInfo)
{

}
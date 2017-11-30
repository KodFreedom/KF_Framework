//--------------------------------------------------------------------------------
//	プレイヤービヘイビアコンポネント
//　playerController.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
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
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
PlayerController::PlayerController(GameObjectActor* const owner, Rigidbody3D& rigidbody)
	: ActorController(owner, "PlayerController", rigidbody)
{}

//--------------------------------------------------------------------------------
//  初期化処理
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
//  終了処理
//--------------------------------------------------------------------------------
void PlayerController::Uninit(void)
{
}

//--------------------------------------------------------------------------------
//  更新処理
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
//  更新処理
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
	{//武器チェック
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
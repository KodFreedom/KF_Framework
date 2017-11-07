//--------------------------------------------------------------------------------
//	プレイヤービヘイビアコンポネント
//　playerBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "playerController.h"
#include "actorController.h"
#include "input.h"
#include "modeDemo.h"
#include "camera.h"
#include "cameraManager.h"
#include "gameObjectActor.h"
#include "collider.h"
#include "manager.h"

#ifdef _DEBUG
#include "debugObserver.h"
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
PlayerController::PlayerController(GameObject* const pGameObj, ActorController& actor)
	: Behavior(pGameObj)
	, actor(actor)
{}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool PlayerController::Init(void)
{
	actor.SetLifeMax(100.0f);
	actor.SetLifeNow(100.0f);
	actor.SetJumpSpeed(20.0f);
	actor.SetTurnSpeedMin(2.0f * Pi * DELTA_TIME);
	actor.SetTurnSpeedMax(4.0f * Pi * DELTA_TIME);
	actor.SetMoveSpeed(10.0f);
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
	auto& movement = (camera->GetRight() * axis.X + cameraForward * axis.Y).Normalized;
	auto isJump = input->GetKeyTrigger(Key::Jump);
	auto isAttack = input->GetKeyTrigger(Key::Attack);
	actor.Act(movement, isJump, isAttack);;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerController::LateUpdate(void)
{
	if (actor.GetLifeNow() <= 0.0f)
	{
	}
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
			actor.Hit(5.0f);
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
//--------------------------------------------------------------------------------
//	生き物コントローラ
//　ActorController.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "actor_controller.h"
#include "game_object.h"
#include "actor_state.h"
#include "animator.h"
#include "rigidbody3d.h"
#include "main_system.h"
#include "collision_detector.h"
#include "collision_system.h"
#include "time.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
ActorController::ActorController(GameObject& owner, Rigidbody3D& rigidbody, Animator& animator)
	: Behavior(owner, L"ActorController"), state_(nullptr), rigidbody_(rigidbody), animator_(animator)
{
	Change(MY_NEW NullActorState);
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool ActorController::Init(void)
{
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void ActorController::Uninit(void)
{
	if (state_)
	{
		state_->Uninit(*this);
		delete state_;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void ActorController::Update(void)
{
	state_->Update(*this);
}

//--------------------------------------------------------------------------------
//  後更新処理
//--------------------------------------------------------------------------------
void ActorController::LateUpdate(void)
{
	state_->LateUpdate(*this);
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void ActorController::OnTrigger(Collider& self, Collider& other)
{
	state_->OnTrigger(*this, self, other);
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void ActorController::OnCollision(CollisionInfo& info)
{
	state_->OnCollision(*this, info);
}

//--------------------------------------------------------------------------------
//	関数名：Change
//  関数説明：ステートの切り替え
//	引数：	state：最新のステート
//	戻り値：なし
//--------------------------------------------------------------------------------
void ActorController::Change(ActorState* state)
{
	if (state_)
	{
		state_->Uninit(*this);
		delete state_;
	}
	state_ = state;
	state_->Init(*this);
}

//--------------------------------------------------------------------------------
//  移動処理
//--------------------------------------------------------------------------------
void ActorController::Move(void)
{
	float move_amount = movement_.Magnitude();
	move_amount = min(move_amount, 1.0f) * parameter_.GetMovementMultiplier();
	if (move_amount > 0.0f)
	{
		auto transform = owner_.GetTransform();

		// 回転
		Vector3& turn_direction = movement_ / move_amount;
		turn_direction = transform->TransformDirectionToLocal(turn_direction).Normalized();
		float rotation_y = atan2f(turn_direction.x_, turn_direction.z_);
		float turn_speed = Math::Lerp(parameter_.GetMaxTurnSpeed(), parameter_.GetMaxTurnSpeed(), move_amount);
		transform->RotateByYaw(rotation_y * turn_speed * Time::Instance()->DeltaTime());

		//移動
		Vector3& move_direction = Vector3::kUp.Dot(current_ground_info_.normal) > CollisionDetector::kMaxFieldSlopeCos
			? Vector3::ProjectOnPlane(transform->GetForward(), current_ground_info_.normal).Normalized()
			: transform->GetForward();
		rigidbody_.Move(move_direction * move_amount * parameter_.GetMoveSpeed() * Time::Instance()->DeltaTime());
	}
	animator_.SetMovement(move_amount);
}

//--------------------------------------------------------------------------------
//  ジャンプ処理
//--------------------------------------------------------------------------------
void ActorController::Jump(void)
{
	Vector3 velocity = rigidbody_.GetVelocity();
	velocity.y_ = parameter_.GetJumpSpeed();
	rigidbody_.SetVelocity(velocity);
}

//--------------------------------------------------------------------------------
//  着地判定処理
//--------------------------------------------------------------------------------
void ActorController::CheckGrounded(void)
{
	auto& position = owner_.GetTransform()->GetPosition();
	auto ray_hit_info = MainSystem::Instance()->GetCollisionSystem()->
		RayCast(Ray(position, Vector3::kDown), parameter_.GetGroundCheckDistance(), &owner_);

	if (ray_hit_info)
	{
		current_ground_info_.is_grounded = true;
		current_ground_info_.normal = ray_hit_info->normal;
		delete ray_hit_info;
		return;
	}

	current_ground_info_.is_grounded = false;
	current_ground_info_.normal = Vector3::kUp;
}

//--------------------------------------------------------------------------------
//  今のステート名前を返す
//--------------------------------------------------------------------------------
const String& ActorController::GetCurrentStateName(void) const
{
	return state_->GetName();
}
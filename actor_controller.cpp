//--------------------------------------------------------------------------------
//	生き物コントローラ
//　ActorController.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "actor_controller.h"
#include "game_object.h"
#include "animator.h"
#include "rigidbody3d.h"
#include "main_system.h"
#include "collision_detector.h"
#include "collision_system.h"
#include "time.h"
#ifdef _DEBUG
#include "../main_system.h"
#include "../debug_observer.h"
#endif

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
ActorController::ActorController(GameObject& owner, const String& name, Rigidbody3D& rigidbody, Animator& animator)
	: Behavior(owner, name), rigidbody_(rigidbody)
    , animator_(animator)
{}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool ActorController::Init(void)
{
    list<Collider*>& colliders = owner_.GetCollidersFromChildren();
    for (auto collider : colliders)
    {
        collider->Register(this);
    }
    return true;
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
		transform->RotateByYaw(rotation_y * turn_speed * Time::Instance()->ScaledDeltaTime());

		//移動
		Vector3& move_direction = Vector3::kUp.Dot(current_ground_info_.normal) > CollisionDetector::kMaxFieldSlopeCos
			? Vector3::ProjectOnPlane(transform->GetForward(), current_ground_info_.normal).Normalized()
			: transform->GetForward();
		rigidbody_.Move(move_direction * move_amount * parameter_.GetMoveSpeed() * Time::Instance()->ScaledDeltaTime());
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
		MY_DELETE ray_hit_info;
		return;
	}

	current_ground_info_.is_grounded = false;
	current_ground_info_.normal = Vector3::kUp;
}

//--------------------------------------------------------------------------------
//  ダメージ計算処理
//--------------------------------------------------------------------------------
void ActorController::ReceiveDamage(const float& damage)
{
    float current_life = parameter_.GetCurrentLife();
    current_life = max(0.0f, current_life - damage);
    parameter_.SetCurrentLife(current_life);

#ifdef _DEBUG
    MainSystem::Instance()->GetDebugObserver()->Display(
        owner_.GetName() + L" が" + to_wstring(damage) + L"ダメージを受けた！ "
    "残りライフ：" + to_wstring(current_life));
#endif
}
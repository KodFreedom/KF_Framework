//--------------------------------------------------------------------------------
//  エネミーコントローラ
//　enemy controller.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_controller.h"
#include "actor_state\enemy_state.h"
#include "../../collider/sphere_collider.h"
#include "../../../game_object/game_object.h"
#include "../../../main_system.h"
#include "../../../observer/actor_observer.h"

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
EnemyController::EnemyController(GameObject& owner, Rigidbody3D& rigidbody, Animator& animator)
    : ActorController(owner, L"EnemyController", rigidbody, animator), target_(nullptr), current_state_(nullptr)
    , warning_range_(10.0f), patrol_range_(20.0f)
    , next_position_(Vector3::kZero), born_position_(Vector3::kZero)
{
    Change(MY_NEW NullEnemyState);
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool EnemyController::Init(void)
{
    auto collider = MY_NEW SphereCollider(owner_, kDynamic, 1.2f);
    collider->SetOffset(Vector3(0.0f, 1.5f, 0.0f));
    collider->SetTag(L"Body");
    collider->SetTrigger(true);
    owner_.AddCollider(collider);

    collider = MY_NEW SphereCollider(owner_, kDynamic, warning_range_);
    collider->SetTag(L"Detector");
    collider->SetTrigger(true);
    owner_.AddCollider(collider);
    next_position_ =
    born_position_ = owner_.GetTransform()->GetPosition();

    ActorController::Init();
    MainSystem::Instance().GetActorObserver().Register(this);
    return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void EnemyController::Uninit(void)
{
    if (current_state_)
    {
        current_state_->Uninit(*this);
        MY_DELETE current_state_;
    }
    ActorController::Uninit();
    MainSystem::Instance().GetActorObserver().Deregister(this);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EnemyController::Update(void)
{
    current_state_->Update(*this);
}

//--------------------------------------------------------------------------------
//  後更新処理
//--------------------------------------------------------------------------------
void EnemyController::LateUpdate(void)
{
    current_state_->LateUpdate(*this);
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void EnemyController::OnTrigger(Collider& self, Collider& other)
{
    current_state_->OnTrigger(*this, self, other);
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void EnemyController::OnCollision(CollisionInfo& info)
{
    current_state_->OnCollision(*this, info);
}

//--------------------------------------------------------------------------------
//  ステートの変換
//--------------------------------------------------------------------------------
void EnemyController::Change(EnemyState* state)
{
    if (current_state_)
    {
        current_state_->Uninit(*this);
        MY_DELETE current_state_;
    }
    current_state_ = state;
    current_state_->Init(*this);
}

//--------------------------------------------------------------------------------
//  今のステートの名前を返す
//--------------------------------------------------------------------------------
const String& EnemyController::GetCurrentStateName(void) const
{
    return current_state_->GetName();
}

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void EnemyController::Hit(const float& damage)
{
    current_state_->OnDamaged(*this, damage);
}
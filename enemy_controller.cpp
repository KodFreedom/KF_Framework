//--------------------------------------------------------------------------------
//  エネミーコントローラ
//　enemy controller.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_controller.h"
#include "actor_state_enemy\enemy_state.h"

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
EnemyController::EnemyController(GameObject& owner, Rigidbody3D& rigidbody, Animator& animator)
    : ActorController(owner, rigidbody, animator), target_(nullptr), current_state_(nullptr)
{
    Change(MY_NEW NullEnemyState);
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool EnemyController::Init(void)
{
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
//--------------------------------------------------------------------------------
//  プレイヤーコントローラ
//　player controller.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_controller.h"
#include "actor_state_player\player_state.h"

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
PlayerController::PlayerController(GameObject& owner, Rigidbody3D& rigidbody, Animator& animator)
    : ActorController(owner, rigidbody, animator), current_state_(nullptr)
{
    Change(MY_NEW NullPlayerState);
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool PlayerController::Init(void)
{
    return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void PlayerController::Uninit(void)
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
void PlayerController::Update(void)
{
    current_state_->Update(*this);
}

//--------------------------------------------------------------------------------
//  後更新処理
//--------------------------------------------------------------------------------
void PlayerController::LateUpdate(void)
{
    current_state_->LateUpdate(*this);
}

//--------------------------------------------------------------------------------
//  コライダートリガーの時呼ばれる
//--------------------------------------------------------------------------------
void PlayerController::OnTrigger(Collider& self, Collider& other)
{
    current_state_->OnTrigger(*this, self, other);
}

//--------------------------------------------------------------------------------
//  コライダー衝突の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerController::OnCollision(CollisionInfo& info)
{
    current_state_->OnCollision(*this, info);
}

//--------------------------------------------------------------------------------
//  ステートの変換
//--------------------------------------------------------------------------------
void PlayerController::Change(PlayerState* state)
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
const String& PlayerController::GetCurrentStateName(void) const
{
    return current_state_->GetName();
}
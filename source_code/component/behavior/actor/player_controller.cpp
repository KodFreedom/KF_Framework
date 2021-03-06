//--------------------------------------------------------------------------------
//  プレイヤーコントローラ
//　player controller.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_controller.h"
#include "actor_state\player_state.h"
#include "../../collider/sphere_collider.h"
#include "../../../game_object/game_object.h"
#include "../../../main_system.h"
#include "../../../observer/actor_observer.h"

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
PlayerController::PlayerController(GameObject& owner, Rigidbody3D& rigidbody, Animator& animator)
    : ActorController(owner, L"PlayerController", rigidbody, animator), current_state_(nullptr)
{
    Change(MY_NEW NullPlayerState);
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool PlayerController::Init(void)
{
    // HipsからBodyのコライダーを取得
    // Todo : ModelAnalyzer側でタグを設定する
    auto hips = owner_.GetTransform()->FindChildBy(L"Hips");
    if (hips)
    {
        auto& colliders = hips->GetGameObject().GetColliders();
        for (auto& collider : colliders)
        {
            collider->SetTag(L"Body");
            collider->SetMode(ColliderMode::kDynamic);
        }
    }
    else
    {// Bodyが設定されてない場合改めて設定する
        auto collider = MY_NEW SphereCollider(owner_, kDynamic, 1.2f);
        collider->SetOffset(Vector3(0.0f, 1.5f, 0.0f));
        collider->SetTag(L"Body");
        collider->SetTrigger(true);
        owner_.AddCollider(collider);
    }

    ActorController::Init();
    MainSystem::Instance().GetActorObserver().Register(this);
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
    ActorController::Uninit();
    MainSystem::Instance().GetActorObserver().Deregister(this);
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
//  モーション終了の時呼ばれる
//--------------------------------------------------------------------------------
void PlayerController::OnAnimationOver(void)
{
    current_state_->OnAnimationOver(*this);
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

//--------------------------------------------------------------------------------
//  ダメージ受けた処理
//--------------------------------------------------------------------------------
void PlayerController::Hit(const float& damage)
{
    current_state_->OnDamaged(*this, damage);
}
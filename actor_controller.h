//--------------------------------------------------------------------------------
//  生き物コントローラ
//　ActorController.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "behavior.h"
#include "kf_math.h"
using namespace kodfreedom;
#include "actor_parameter.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class GameObject;
class ActorState;
class Animator;
class Rigidbody3D;

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
struct GroundInfo
{
    Vector3 normal;
    bool    is_grounded;
};

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class ActorController : public Behavior
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    ActorController(GameObject& owner, Rigidbody3D& rigidbody, Animator& animator);
    ~ActorController() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    bool Init(void) override;

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    void Uninit(void) override;

    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    void Update(void) override;

    //--------------------------------------------------------------------------------
    //  後更新処理
    //--------------------------------------------------------------------------------
    void LateUpdate(void) override;

    //--------------------------------------------------------------------------------
    //  コライダートリガーの時呼ばれる
    //--------------------------------------------------------------------------------
    void OnTrigger(Collider& self, Collider& other) override;

    //--------------------------------------------------------------------------------
    //  コライダー衝突の時呼ばれる
    //--------------------------------------------------------------------------------
    void OnCollision(CollisionInfo& info) override;

    //--------------------------------------------------------------------------------
    //  ステートの変換
    //--------------------------------------------------------------------------------
    void Change(ActorState* state);

    //--------------------------------------------------------------------------------
    //  移動処理
    //--------------------------------------------------------------------------------
    void Move(void);

    //--------------------------------------------------------------------------------
    //  ジャンプ処理
    //--------------------------------------------------------------------------------
    void Jump(void);

    //--------------------------------------------------------------------------------
    //  着地判定処理
    //--------------------------------------------------------------------------------
    void CheckGrounded(void);

    //--------------------------------------------------------------------------------
    //  セッター
    //--------------------------------------------------------------------------------
    void SetMovement(const Vector3& value) { movement_ = value; }
    void SetIsJump(const bool& value) { is_jump_ = value; }
    void SetIsLightAttack(const bool& value) { is_light_attack_ = value; }
    void SetIsStrongAttack(const bool& value) { is_strong_attack_ = value; }
    void SetIsSkill(const bool& value) { is_skill_ = value; }
    void SetTarget(GameObject* target) { target_ = target; }

    //--------------------------------------------------------------------------------
    //  ゲッター
    //--------------------------------------------------------------------------------
    Animator& GetAnimator(void) const { return animator_; }
    ActorParameter& GetParameter(void) { return parameter_; }
    Rigidbody3D& GetRigidbody(void) const { return rigidbody_; }
    const Vector3& GetMovement(void) const { return movement_; }
    const GroundInfo& GetCurrentGroundInfo(void) const { return current_ground_info_; }
    const String& GetCurrentStateName(void) const;
    const auto& GetTarget(void) const { return target_; }

    //--------------------------------------------------------------------------------
    //  フラグの取得
    //--------------------------------------------------------------------------------
    const bool& IsJump(void) const { return is_jump_; }
    const bool& IsLightAttack(void) const { return is_light_attack_; }
    const bool& IsStrongAttack(void) const { return is_strong_attack_; }
    const bool& IsSkill(void) const { return is_skill_; }

protected:
    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    Animator&      animator_;
    Rigidbody3D&   rigidbody_;
    ActorState*    state_;
    ActorParameter parameter_;
    Vector3        movement_;
    bool           is_light_attack_;
    bool           is_strong_attack_;
    bool           is_skill_;
    bool           is_jump_;
    GroundInfo     current_ground_info_;
    GameObject*    target_;
};
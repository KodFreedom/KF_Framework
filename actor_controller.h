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
    //  移動量の設定
    //--------------------------------------------------------------------------------
    void SetMovement(const Vector3& value) { movement_ = value; }

    //--------------------------------------------------------------------------------
    //  ジャンプフラグの設定
    //--------------------------------------------------------------------------------
    void SetIsJump(const bool& value) { is_jump_ = value; }

    //--------------------------------------------------------------------------------
    //  軽攻撃フラグの設定
    //--------------------------------------------------------------------------------
    void SetIsLightAttack(const bool& value) { is_light_attack_ = value; }

    //--------------------------------------------------------------------------------
    //  重攻撃フラグの設定
    //--------------------------------------------------------------------------------
    void SetIsStrongAttack(const bool& value) { is_strong_attack_ = value; }

    //--------------------------------------------------------------------------------
    //  スキルフラグの設定
    //--------------------------------------------------------------------------------
    void SetIsSkill(const bool& value) { is_skill_ = value; }

    //--------------------------------------------------------------------------------
    //  アニメーターの取得
    //--------------------------------------------------------------------------------
    Animator& GetAnimator(void) const { return animator_; }

    //--------------------------------------------------------------------------------
    //  パラメーターの取得
    //--------------------------------------------------------------------------------
    ActorParameter& GetParameter(void) { return parameter_; }

    //--------------------------------------------------------------------------------
    //  リジッドボディの取得
    //--------------------------------------------------------------------------------
    Rigidbody3D& GetRigidbody(void) const { return rigidbody_; }

    //--------------------------------------------------------------------------------
    //  移動量の取得
    //--------------------------------------------------------------------------------
    const auto& GetMovement(void) const { return movement_; }

    //--------------------------------------------------------------------------------
    //  今の着地状態の取得
    //--------------------------------------------------------------------------------
    const auto& GetCurrentGroundInfo(void) const { return current_ground_info_; }

    //--------------------------------------------------------------------------------
    //  ジャンプフラグの取得
    //--------------------------------------------------------------------------------
    const auto& IsJump(void) const { return is_jump_; }

    //--------------------------------------------------------------------------------
    //  軽攻撃フラグの取得
    //--------------------------------------------------------------------------------
    const auto& IsLightAttack(void) const { return is_light_attack_; }

    //--------------------------------------------------------------------------------
    //  重攻撃フラグの取得
    //--------------------------------------------------------------------------------
    const auto& IsStrongAttack(void) const { return is_strong_attack_; }

    //--------------------------------------------------------------------------------
    //  スキルフラグの取得
    //--------------------------------------------------------------------------------
    const auto& IsSkill(void) const { return is_skill_; }

    //--------------------------------------------------------------------------------
    //  今のステート名前を返す
    //--------------------------------------------------------------------------------
    const String& GetCurrentStateName(void) const;

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
};
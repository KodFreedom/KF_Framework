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
    ActorController(GameObject& owner, const String& name, Rigidbody3D& rigidbody, Animator& animator);
    ~ActorController() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    virtual bool Init(void) override;

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    virtual void Uninit(void) override = 0;

    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    virtual void Update(void) override = 0;

    //--------------------------------------------------------------------------------
    //  後更新処理
    //--------------------------------------------------------------------------------
    virtual void LateUpdate(void) override = 0;

    //--------------------------------------------------------------------------------
    //  コライダートリガーの時呼ばれる
    //--------------------------------------------------------------------------------
    virtual void OnTrigger(Collider& self, Collider& other) override = 0;

    //--------------------------------------------------------------------------------
    //  コライダー衝突の時呼ばれる
    //--------------------------------------------------------------------------------
    virtual void OnCollision(CollisionInfo& info) override = 0;

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
    //  ダメージ受けた処理
    //--------------------------------------------------------------------------------
    virtual void Hit(const float& damage) = 0;

    //--------------------------------------------------------------------------------
    //  ダメージ計算処理
    //--------------------------------------------------------------------------------
    void ReceiveDamage(const float& damage);

    //--------------------------------------------------------------------------------
    //  セッター
    //--------------------------------------------------------------------------------
    void SetMovement(const Vector3& value) { movement_ = value; }
    void SetIsJump(const bool& value) { is_jump_ = value; }
    void SetIsLightAttack(const bool& value) { is_light_attack_ = value; }
    void SetIsStrongAttack(const bool& value) { is_strong_attack_ = value; }
    void SetIsSkill(const bool& value) { is_skill_ = value; }
    void SetParameter(const ActorParameter& parameter) { parameter_ = parameter; }

    //--------------------------------------------------------------------------------
    //  ゲッター
    //--------------------------------------------------------------------------------
    Animator&             GetAnimator(void) const { return animator_; }
    ActorParameter&       GetParameter(void) { return parameter_; }
    Rigidbody3D&          GetRigidbody(void) const { return rigidbody_; }
    const Vector3&        GetMovement(void) const { return movement_; }
    const GroundInfo&     GetCurrentGroundInfo(void) const { return current_ground_info_; }
    virtual const String& GetCurrentStateName(void) const = 0;

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
    ActorParameter parameter_;
    Vector3        movement_;
    bool           is_light_attack_;
    bool           is_strong_attack_;
    bool           is_skill_;
    bool           is_jump_;
    GroundInfo     current_ground_info_;
};
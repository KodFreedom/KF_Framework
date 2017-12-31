//--------------------------------------------------------------------------------
//
//　animator.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "component.h"
#include "kf_math.h"
using namespace kodfreedom;
#include "motion_state.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class Transform;

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum IKParts
{
    // 頭
    kHead,
    kNeck,

    // 右手
    kShoulderRight,
    kUpperArmRight,
    kLowerArmRight,
    kHandRight,

    // 左手
    kShoulderLeft,
    kUpperArmLeft,
    kLowerArmLeft,
    kHandLeft,

    // 右足
    kUpperLegRight,
    kLowerLegRight,
    kFootRight,
    kToesRight,

    // 左足
    kUpperLegLeft,
    kLowerLegLeft,
    kFootLeft,
    kToesLeft,

    kIKMax
};

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
struct BoneTexture
{// 骨の情報をテクスチャに書き込んでシェーダに渡す
    int size;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    LPDIRECT3DTEXTURE9 pointer;
#endif
};

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class Animator : public Component
{
#ifdef _DEBUG
    friend class DebugObserver;
#endif
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    Animator(GameObject& owner);
    ~Animator() {}

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
    void Update(void);

    //--------------------------------------------------------------------------------
    //  後更新処理
    //--------------------------------------------------------------------------------
    void LateUpdate(void);

    //--------------------------------------------------------------------------------
    //  今のアニメーションの名前の取得
    //--------------------------------------------------------------------------------
    const String& GetCurrentAnimationName(void);

    //--------------------------------------------------------------------------------
    //  今のアニメーションのタイプの取得
    //--------------------------------------------------------------------------------
    const auto& GetCurrentAnimationStateType(void) const { return state_->GetType(); }

    //--------------------------------------------------------------------------------
    //  ゲッター
    //--------------------------------------------------------------------------------
    const auto& GetAvatar(void) const { return avatar_; }
    const bool& GetIsGrounded(void) const { return is_grounded_; }
    const bool& GetIsDead(void) const { return is_dead_; }
    const bool& GetIsDamaged(void) const { return is_damaged_; }
    const bool& GetIsJump(void) const { return is_jump_; }
    const bool& GetIsRiseUp(void) const { return is_rise_up_; }
    const bool& GetIsLightAttack(void) const { return is_light_attack_; }
    const bool& GetIsStrongAttack(void) const { return is_strong_attack_; }
    const bool& GetIsSkill(void) const { return is_skill_; }
    const bool& GetIsSkillOver(void) const { return is_skill_over_; }
    const bool& GetIsStun(void) const { return is_stun_; }
    const bool& GetIsUltra(void) const { return is_ultra_; }
    const bool& GetIsAngry(void) const { return is_angry_; }
    const float& GetMovement(void) const { return movement_; }
    const float& GetTimeCounter(void) const { return time_counter_; }

    //--------------------------------------------------------------------------------
    //  セッター
    //--------------------------------------------------------------------------------
    void SetAvatar(const String& file_name);
    void SetLightAttack(const bool& value) { is_light_attack_ = value; }
    void SetStrongAttack(const bool& value) { is_strong_attack_ = value; }
    void SetGrounded(const bool& value) { is_grounded_ = value; }
    void SetJump(const bool& value) { is_jump_ = value; }
    void SetDamaged(const bool& value) { is_damaged_ = value; }
    void SetSkill(const bool& value) { is_skill_ = value; }
    void SetSkillOver(const bool& value) { is_skill_over_ = value; }
    void SetStun(const bool& value) { is_stun_ = value; }
    void SetUltra(const bool& value) { is_ultra_ = value; }
    void SetAngry(const bool& value) { is_angry_ = value; }
    void SetDead(const bool& value) { is_dead_ = value; }
    void SetEnableIK(const bool& value) { enable_ik_ = value; }
    void SetMovement(const float& value) { movement_ = value; }

    //--------------------------------------------------------------------------------
    //  モーションステートの変更
    //--------------------------------------------------------------------------------
    void Change(MotionState* new_motion_state);

    //--------------------------------------------------------------------------------
    //  テクスチャの取得
    //--------------------------------------------------------------------------------
    const auto& GetBoneTexture(void) const { return bone_texture_; }

private:
    //--------------------------------------------------------------------------------
    //  列挙型定義
    //--------------------------------------------------------------------------------
    enum IKGoals
    {
        kIKGoalLook = 0,
        kIKGoalLeftFoot,
        kIKGoalRightFoot,
        kIKGoalMax
    };

    //--------------------------------------------------------------------------------
    //  構造体定義
    //--------------------------------------------------------------------------------
    struct Bone
    {
        Transform*  transform;
        String      name;
        Matrix44    bind_pose_inverse;
    };

    struct IKController
    {
        int index;
        Vector3 rotation_limit_min;
        Vector3 rotation_limit_max;
    };

    struct IKGoal
    {
        float position_weight;
        float rotation_weight;
        Vector3 position;
        Quaternion rotation;
    };

    //--------------------------------------------------------------------------------
    //  ファイルから読み込む
    //--------------------------------------------------------------------------------
    void LoadFromFile(const String& file_name);

    //--------------------------------------------------------------------------------
    //  ボーンと子供を関連付ける
    //--------------------------------------------------------------------------------
    void AttachBonesToChildren(void);

    //--------------------------------------------------------------------------------
    //  テクスチャのサーフェイスにボーンマトリクスを書き込む
    //--------------------------------------------------------------------------------
    void UpdateBoneTexture(void);

    //--------------------------------------------------------------------------------
    //  インバースキネマティクス(IK)の初期化
    //--------------------------------------------------------------------------------
    void InitIK(void);

    //--------------------------------------------------------------------------------
    //  インバースキネマティクス(IK)計算
    //--------------------------------------------------------------------------------
    void UpdateIK(void);

    //--------------------------------------------------------------------------------
    //  foot ikの計算
    //--------------------------------------------------------------------------------
    void UpdateFootIK(void);

    //--------------------------------------------------------------------------------
    //  ik goalの計算
    //--------------------------------------------------------------------------------
    void ComputeIKGoal(const IKParts& goal_part, const IKGoals& ik_goal);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    vector<Bone>   avatar_;
    vector<String> motion_names_;
    MotionState*   state_;
    bool           is_grounded_;
    bool           is_light_attack_;
    bool           is_strong_attack_;
    bool           is_jump_;
    bool           is_damaged_;
    bool           is_dead_;
    bool           is_rise_up_;
    bool           is_skill_;
    bool           is_skill_over_;
    bool           is_stun_;
    bool           is_ultra_;
    bool           is_angry_;
    float          movement_;
    float          time_counter_;
    BoneTexture    bone_texture_;
    bool           enable_ik_;
    float          ik_ray_distance_;
    IKController   ik_controllers_[kIKMax];
    IKGoal         ik_goals[kIKGoalMax];
};
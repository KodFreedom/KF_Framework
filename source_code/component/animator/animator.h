//--------------------------------------------------------------------------------
//
//�@animator.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "component.h"
#include "kf_math.h"
using namespace kodfreedom;
#include "motion_state/motion_state.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class Transform;
class ActorController;

//--------------------------------------------------------------------------------
//  �񋓌^��`
//--------------------------------------------------------------------------------
enum IKParts
{
    // ��
    kHead,
    kNeck,

    // �E��
    kShoulderRight,
    kUpperArmRight,
    kLowerArmRight,
    kHandRight,

    // ����
    kShoulderLeft,
    kUpperArmLeft,
    kLowerArmLeft,
    kHandLeft,

    // �E��
    kUpperLegRight,
    kLowerLegRight,
    kFootRight,
    kToesRight,

    // ����
    kUpperLegLeft,
    kLowerLegLeft,
    kFootLeft,
    kToesLeft,

    kIKMax
};

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
struct BoneTexture
{// ���̏����e�N�X�`���ɏ�������ŃV�F�[�_�ɓn��
    int size;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    LPDIRECT3DTEXTURE9 pointer;
#endif
};

//--------------------------------------------------------------------------------
//  �N���X�錾
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
    //  ����������
    //--------------------------------------------------------------------------------
    bool Init(void) override;

    //--------------------------------------------------------------------------------
    //  �I������
    //--------------------------------------------------------------------------------
    void Uninit(void) override;

    //--------------------------------------------------------------------------------
    //  �X�V����
    //--------------------------------------------------------------------------------
    void Update(void);

    //--------------------------------------------------------------------------------
    //  ��X�V����
    //--------------------------------------------------------------------------------
    void LateUpdate(void);

    //--------------------------------------------------------------------------------
    //  ���̃A�j���[�V�����̖��O�̎擾
    //--------------------------------------------------------------------------------
    const String& GetCurrentAnimationName(void);

    //--------------------------------------------------------------------------------
    //  ���̃A�j���[�V�����̃^�C�v�̎擾
    //--------------------------------------------------------------------------------
    const auto& GetCurrentAnimationStateType(void) const { return state_->GetType(); }

    //--------------------------------------------------------------------------------
    //  �Q�b�^�[
    //--------------------------------------------------------------------------------
    const auto&  GetAvatar(void) const { return avatar_; }
    const bool&  GetIsGrounded(void) const { return is_grounded_; }
    const bool&  GetIsDead(void) const { return is_dead_; }
    const bool&  GetIsDamaged(void) const { return is_damaged_; }
    const bool&  GetIsJump(void) const { return is_jump_; }
    const bool&  GetIsRiseUp(void) const { return is_rise_up_; }
    const bool&  GetIsLightAttack(void) const { return is_light_attack_; }
    const bool&  GetIsStrongAttack(void) const { return is_strong_attack_; }
    const bool&  GetIsDushAttack(void) const { return is_dush_attack_; }
    const bool&  GetIsSkill(void) const { return is_skill_; }
    const bool&  GetIsSkillOver(void) const { return is_skill_over_; }
    const bool&  GetIsStun(void) const { return is_stun_; }
    const bool&  GetIsUltra(void) const { return is_ultra_; }
    const bool&  GetIsAngry(void) const { return is_angry_; }
    const bool&  GetIsDefence(void) const { return is_defence_; }
    const float& GetMovement(void) const { return movement_; }
    const float& GetTimeCounter(void) const { return time_counter_; }
    const int    GetCurrentFrame(void) const;

    //--------------------------------------------------------------------------------
    //  �Z�b�^�[
    //--------------------------------------------------------------------------------
    void SetAvatar(const String& file_name);
    void SetLightAttack(const bool& value) { is_light_attack_ = value; }
    void SetStrongAttack(const bool& value) { is_strong_attack_ = value; }
    void SetDushAttack(const bool& value) { is_dush_attack_ = value; }
    void SetGrounded(const bool& value) { is_grounded_ = value; }
    void SetJump(const bool& value) { is_jump_ = value; }
    void SetDamaged(const bool& value) { is_damaged_ = value; }
    void SetSkill(const bool& value) { is_skill_ = value; }
    void SetSkillOver(const bool& value) { is_skill_over_ = value; }
    void SetStun(const bool& value) { is_stun_ = value; }
    void SetUltra(const bool& value) { is_ultra_ = value; }
    void SetAngry(const bool& value) { is_angry_ = value; }
    void SetDead(const bool& value) { is_dead_ = value; }
    void SetDefence(const bool& value) { is_defence_ = value; }
    void SetEnableIK(const bool& value) { enable_ik_ = value; }
    void SetMovement(const float& value) { movement_ = value; }

    //--------------------------------------------------------------------------------
    //  ���[�V�����X�e�[�g�̕ύX
    //--------------------------------------------------------------------------------
    void Change(MotionState* new_motion_state);

    //--------------------------------------------------------------------------------
    //  �e�N�X�`���̎擾
    //--------------------------------------------------------------------------------
    const auto& GetBoneTexture(void) const { return bone_texture_; }

    //--------------------------------------------------------------------------------
    //  observer�֐�
    //--------------------------------------------------------------------------------
    void Register(ActorController* observer)
    {
        assert(!observer_);
        assert(observer);
        observer_ = observer;
    }
    void Deregister(ActorController* observer)
    {
        assert(observer == observer_);
        observer_ = nullptr;
    }

private:
    //--------------------------------------------------------------------------------
    //  �񋓌^��`
    //--------------------------------------------------------------------------------
    enum IKGoals
    {
        kIKGoalLook = 0,
        kIKGoalLeftFoot,
        kIKGoalRightFoot,
        kIKGoalMax
    };

    //--------------------------------------------------------------------------------
    //  �\���̒�`
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
    };

    struct IKGoal
    {
        float weight;
        Vector3 position;
        Vector3 up;
    };

    //--------------------------------------------------------------------------------
    //  �t�@�C������ǂݍ���
    //--------------------------------------------------------------------------------
    void LoadFromFile(const String& file_name);

    //--------------------------------------------------------------------------------
    //  �{�[���Ǝq�����֘A�t����
    //--------------------------------------------------------------------------------
    void AttachBonesToChildren(void);

    //--------------------------------------------------------------------------------
    //  �e�N�X�`���̃T�[�t�F�C�X�Ƀ{�[���}�g���N�X����������
    //--------------------------------------------------------------------------------
    void UpdateBoneTexture(void);

    //--------------------------------------------------------------------------------
    //  �C���o�[�X�L�l�}�e�B�N�X(IK)�̏�����
    //--------------------------------------------------------------------------------
    void InitIK(void);

    //--------------------------------------------------------------------------------
    //  �C���o�[�X�L�l�}�e�B�N�X(IK)�v�Z
    //--------------------------------------------------------------------------------
    void UpdateIK(void);

    //--------------------------------------------------------------------------------
    //  foot ik�̌v�Z
    //--------------------------------------------------------------------------------
    void UpdateFootIK(void);

    //--------------------------------------------------------------------------------
    //  ik goal�̌v�Z
    //--------------------------------------------------------------------------------
    void ComputeIKGoal(const IKParts& goal_part, const IKGoals& ik_goal);

    //--------------------------------------------------------------------------------
    //  foot ik�v�Z
    //--------------------------------------------------------------------------------
    void ComputeFootIK(const IKParts& end_part, const IKGoals& ik_goal);
    //void ComputeIK(const IKParts& end_part, const IKGoals& ik_goal, const int parent_number = 3);

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    ActorController* observer_ = nullptr;
    vector<Bone>     avatar_;
    vector<String>   motion_names_;
    MotionState*     state_ = nullptr;
    bool             is_grounded_ = false;
    bool             is_light_attack_ = false;
    bool             is_strong_attack_ = false;
    bool             is_dush_attack_ = false;
    bool             is_jump_ = false;
    bool             is_damaged_ = false;
    bool             is_dead_ = false;
    bool             is_rise_up_ = false;
    bool             is_skill_ = false;
    bool             is_skill_over_ = false;
    bool             is_stun_ = false;
    bool             is_ultra_ = false;
    bool             is_angry_ = false;
    bool             is_defence_ = false;
    float            movement_ = 0.0f;
    float            time_counter_ = 0.0f;
    BoneTexture      bone_texture_ = { 0 };
    bool             enable_ik_ = true;
    float            ik_ray_distance_ = 2.0f;
    float            ik_grounded_distance_ = 0.5f;
    float            ik_weight_increase_speed_ = 10.0f;
    float            ik_weight_decrease_speed_ = -10.0f;
    Vector3          ik_foot_position_offset_ = Vector3(0.0f, 0.3f, 0.0f);
    Vector3          ik_foot_rotation_offset_ = Vector3(0.0f, 0.0f, 0.0f);
    IKController     ik_controllers_[kIKMax] = { 0 };
    IKGoal           ik_goals_[kIKGoalMax] = { 0 };
};
//--------------------------------------------------------------------------------
//  �������R���g���[��
//�@ActorController.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "behavior.h"
#include "kf_math.h"
using namespace kodfreedom;
#include "actor_parameter.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class GameObject;
class ActorState;
class Animator;
class Rigidbody3D;

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
struct GroundInfo
{
    Vector3 normal;
    bool    is_grounded;
};

//--------------------------------------------------------------------------------
//  �N���X�錾
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
    void Update(void) override;

    //--------------------------------------------------------------------------------
    //  ��X�V����
    //--------------------------------------------------------------------------------
    void LateUpdate(void) override;

    //--------------------------------------------------------------------------------
    //  �R���C�_�[�g���K�[�̎��Ă΂��
    //--------------------------------------------------------------------------------
    void OnTrigger(Collider& self, Collider& other) override;

    //--------------------------------------------------------------------------------
    //  �R���C�_�[�Փ˂̎��Ă΂��
    //--------------------------------------------------------------------------------
    void OnCollision(CollisionInfo& info) override;

    //--------------------------------------------------------------------------------
    //  �X�e�[�g�̕ϊ�
    //--------------------------------------------------------------------------------
    void Change(ActorState* state);

    //--------------------------------------------------------------------------------
    //  �ړ�����
    //--------------------------------------------------------------------------------
    void Move(void);

    //--------------------------------------------------------------------------------
    //  �W�����v����
    //--------------------------------------------------------------------------------
    void Jump(void);

    //--------------------------------------------------------------------------------
    //  ���n���菈��
    //--------------------------------------------------------------------------------
    void CheckGrounded(void);

    //--------------------------------------------------------------------------------
    //  �Z�b�^�[
    //--------------------------------------------------------------------------------
    void SetMovement(const Vector3& value) { movement_ = value; }
    void SetIsJump(const bool& value) { is_jump_ = value; }
    void SetIsLightAttack(const bool& value) { is_light_attack_ = value; }
    void SetIsStrongAttack(const bool& value) { is_strong_attack_ = value; }
    void SetIsSkill(const bool& value) { is_skill_ = value; }
    void SetTarget(GameObject* target) { target_ = target; }

    //--------------------------------------------------------------------------------
    //  �Q�b�^�[
    //--------------------------------------------------------------------------------
    Animator& GetAnimator(void) const { return animator_; }
    ActorParameter& GetParameter(void) { return parameter_; }
    Rigidbody3D& GetRigidbody(void) const { return rigidbody_; }
    const Vector3& GetMovement(void) const { return movement_; }
    const GroundInfo& GetCurrentGroundInfo(void) const { return current_ground_info_; }
    const String& GetCurrentStateName(void) const;
    const auto& GetTarget(void) const { return target_; }

    //--------------------------------------------------------------------------------
    //  �t���O�̎擾
    //--------------------------------------------------------------------------------
    const bool& IsJump(void) const { return is_jump_; }
    const bool& IsLightAttack(void) const { return is_light_attack_; }
    const bool& IsStrongAttack(void) const { return is_strong_attack_; }
    const bool& IsSkill(void) const { return is_skill_; }

protected:
    //--------------------------------------------------------------------------------
    //  �ϐ���`
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
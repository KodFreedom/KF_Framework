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
    ActorController(GameObject& owner, const String& name, Rigidbody3D& rigidbody, Animator& animator);
    ~ActorController() {}

    //--------------------------------------------------------------------------------
    //  ����������
    //--------------------------------------------------------------------------------
    virtual bool Init(void) override;

    //--------------------------------------------------------------------------------
    //  �I������
    //--------------------------------------------------------------------------------
    virtual void Uninit(void) override = 0;

    //--------------------------------------------------------------------------------
    //  �X�V����
    //--------------------------------------------------------------------------------
    virtual void Update(void) override = 0;

    //--------------------------------------------------------------------------------
    //  ��X�V����
    //--------------------------------------------------------------------------------
    virtual void LateUpdate(void) override = 0;

    //--------------------------------------------------------------------------------
    //  �R���C�_�[�g���K�[�̎��Ă΂��
    //--------------------------------------------------------------------------------
    virtual void OnTrigger(Collider& self, Collider& other) override = 0;

    //--------------------------------------------------------------------------------
    //  �R���C�_�[�Փ˂̎��Ă΂��
    //--------------------------------------------------------------------------------
    virtual void OnCollision(CollisionInfo& info) override = 0;

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
    //  �_���[�W�󂯂�����
    //--------------------------------------------------------------------------------
    virtual void Hit(const float& damage) = 0;

    //--------------------------------------------------------------------------------
    //  �_���[�W�v�Z����
    //--------------------------------------------------------------------------------
    void ReceiveDamage(const float& damage);

    //--------------------------------------------------------------------------------
    //  �Z�b�^�[
    //--------------------------------------------------------------------------------
    void SetMovement(const Vector3& value) { movement_ = value; }
    void SetIsJump(const bool& value) { is_jump_ = value; }
    void SetIsLightAttack(const bool& value) { is_light_attack_ = value; }
    void SetIsStrongAttack(const bool& value) { is_strong_attack_ = value; }
    void SetIsSkill(const bool& value) { is_skill_ = value; }
    void SetParameter(const ActorParameter& parameter) { parameter_ = parameter; }

    //--------------------------------------------------------------------------------
    //  �Q�b�^�[
    //--------------------------------------------------------------------------------
    Animator&             GetAnimator(void) const { return animator_; }
    ActorParameter&       GetParameter(void) { return parameter_; }
    Rigidbody3D&          GetRigidbody(void) const { return rigidbody_; }
    const Vector3&        GetMovement(void) const { return movement_; }
    const GroundInfo&     GetCurrentGroundInfo(void) const { return current_ground_info_; }
    virtual const String& GetCurrentStateName(void) const = 0;

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
    ActorParameter parameter_;
    Vector3        movement_;
    bool           is_light_attack_;
    bool           is_strong_attack_;
    bool           is_skill_;
    bool           is_jump_;
    GroundInfo     current_ground_info_;
};
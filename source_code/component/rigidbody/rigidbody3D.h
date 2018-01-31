//--------------------------------------------------------------------------------
//�@rigidbody3d.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "rigidbody.h"
#include "kf_math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  3d���W�b�h�{�f�B
//--------------------------------------------------------------------------------
class Rigidbody3D : public Rigidbody
{
public:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    Rigidbody3D(GameObject& owner);
    ~Rigidbody3D() {}

    //--------------------------------------------------------------------------------
    //  ������
    //--------------------------------------------------------------------------------
    bool Init(void) override { return true; }

    //--------------------------------------------------------------------------------
    //  �I������
    //--------------------------------------------------------------------------------
    void Uninit(void) override {}

    //--------------------------------------------------------------------------------
    //  �X�V����
    //--------------------------------------------------------------------------------
    void Update(void) override;

    //--------------------------------------------------------------------------------
    //  ��X�V����
    //--------------------------------------------------------------------------------
    void LateUpdate(void) override;

    //--------------------------------------------------------------------------------
    //  �͂�^����
    //--------------------------------------------------------------------------------
    void AddForce(const Vector3& force) { force_accum_ += force; }

    //--------------------------------------------------------------------------------
    //  ���x��^����
    //--------------------------------------------------------------------------------
    void AddVelocity(const Vector3& value) { velocity_ += value; }

    //--------------------------------------------------------------------------------
    //  �ړ��ʂ�^����i���ʖ����j
    //--------------------------------------------------------------------------------
    void Move(const Vector3& value) { movement_ += value; }

    //--------------------------------------------------------------------------------
    //  �ړ��ʂ�^����i�������Z�p�j
    //--------------------------------------------------------------------------------
    void AddFixedMovement(const Vector3& value) { fixed_movement_ += value; }

    //--------------------------------------------------------------------------------
    //  Get�֐�
    //--------------------------------------------------------------------------------
    const auto& GetMass(void) const { return mass_; }
    const auto& GetInverseMass(void) const { return inverse_mass_; }
    const auto& GetDrag(void) const { return drag_; }
    const auto& GetFriction(void) const { return friction_; }
    const auto& GetBounciness(void) const { return bounciness_; }
    const auto& GetVelocity(void) const { return velocity_; }
    const auto& GetAcceleration(void) const { return acceleration_; }
    const auto& GetMovement(void) const { return movement_; }
    const auto& GetGravityMultiplier(void) const { return gravity_multiplier_; }

    //--------------------------------------------------------------------------------
    //  Set�֐�
    //--------------------------------------------------------------------------------
    void SetMass(const float& value);
    void SetDrag(const float& value) { drag_ = value; }
    void SetFriction(const float& value) { friction_ = value; }
    void SetBounciness(const float& value) { bounciness_ = value; }
    void SetGravityMultiplier(const float& value) { gravity_multiplier_ = value; }
    void SetVelocity(const Vector3& value) { velocity_ = value; }
    void SetMovement(const Vector3& value) { movement_ = value; }
    //void    SetInertiaTensor(Collider* pCollider);

private:
    //--------------------------------------------------------------------------------
    //  �萔��`
    //--------------------------------------------------------------------------------
    //enum AXIS
    //{
    //    X = 0x01,
    //    Y = 0x02,
    //    Z = 0x04,
    //    XYZ = 0x07
    //};

    //--------------------------------------------------------------------------------
    //  �֐���`
    //--------------------------------------------------------------------------------
    //void        calculateInertiaTensorWorld(Matrix44& mtxIitWorld);

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    float   mass_; // ����
    float   inverse_mass_; // ���ʂ̋t��
    float   drag_; // ��R�W��(��C��R)
    float   friction_; // ���C�W��
    float   bounciness_; // ���˕Ԃ�W��
    float   gravity_multiplier_; //�d�͌W��
    Vector3 movement_; // �ړ���
    Vector3 fixed_movement_; // �������Z�ɂ��␳
    Vector3 velocity_; // ���x
    Vector3 acceleration_; // �����x
    Vector3 force_accum_; // ���킹����p��
    //Vector3 angular_velocity_; // ��]���x
    //float angular_drag_; //��]��R�W��
    //Vector3 torque_accum_; // ��]��
    //Matrix44 inertis_tensor_; //�����e���\���̍s��
    //BYTE rotation_lock_; //��]�����̃t���O
};
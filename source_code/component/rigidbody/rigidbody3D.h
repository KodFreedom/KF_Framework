//--------------------------------------------------------------------------------
//　rigidbody3d.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "rigidbody.h"
#include "kf_math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  3dリジッドボディ
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
    //  初期化
    //--------------------------------------------------------------------------------
    bool Init(void) override { return true; }

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    void Uninit(void) override {}

    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    void Update(void) override;

    //--------------------------------------------------------------------------------
    //  後更新処理
    //--------------------------------------------------------------------------------
    void LateUpdate(void) override;

    //--------------------------------------------------------------------------------
    //  力を与える
    //--------------------------------------------------------------------------------
    void AddForce(const Vector3& force) { force_accum_ += force; }

    //--------------------------------------------------------------------------------
    //  速度を与える
    //--------------------------------------------------------------------------------
    void AddVelocity(const Vector3& value) { velocity_ += value; }

    //--------------------------------------------------------------------------------
    //  移動量を与える（質量無視）
    //--------------------------------------------------------------------------------
    void Move(const Vector3& value) { movement_ += value; }

    //--------------------------------------------------------------------------------
    //  移動量を与える（物理演算用）
    //--------------------------------------------------------------------------------
    void AddFixedMovement(const Vector3& value) { fixed_movement_ += value; }

    //--------------------------------------------------------------------------------
    //  Get関数
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
    //  Set関数
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
    //  定数定義
    //--------------------------------------------------------------------------------
    //enum AXIS
    //{
    //    X = 0x01,
    //    Y = 0x02,
    //    Z = 0x04,
    //    XYZ = 0x07
    //};

    //--------------------------------------------------------------------------------
    //  関数定義
    //--------------------------------------------------------------------------------
    //void        calculateInertiaTensorWorld(Matrix44& mtxIitWorld);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    float   mass_; // 質量
    float   inverse_mass_; // 質量の逆数
    float   drag_; // 抵抗係数(空気抵抗)
    float   friction_; // 摩擦係数
    float   bounciness_; // 跳ね返り係数
    float   gravity_multiplier_; //重力係数
    Vector3 movement_; // 移動量
    Vector3 fixed_movement_; // 物理演算による補正
    Vector3 velocity_; // 速度
    Vector3 acceleration_; // 加速度
    Vector3 force_accum_; // 合わせた作用力
    //Vector3 angular_velocity_; // 回転速度
    //float angular_drag_; //回転抵抗係数
    //Vector3 torque_accum_; // 回転力
    //Matrix44 inertis_tensor_; //慣性テンソルの行列
    //BYTE rotation_lock_; //回転制限のフラグ
};
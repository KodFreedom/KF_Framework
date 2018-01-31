//--------------------------------------------------------------------------------
//  �R���W�����T�m��
//�@collisionDetector.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "collision_detector.h"
#include "main_system.h"
#include "physics_system.h"
#include "sphere_collider.h"
#include "field_collider.h"
#include "aabb_collider.h"
#include "obb_collider.h"
#include "game_object.h"
#include "rigidbody3d.h"
#include "transform.h"

#ifdef _DEBUG
#include "debug_observer.h"
#endif

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
const float CollisionDetector::kMaxFieldSlopeCos = cosf(CollisionDetector::kMaxFieldSlope);
const float CollisionDetector::kMinWallSlopeCos = cosf(CollisionDetector::kMinWallSlope);
const float CollisionDetector::kMaxObbSlopeCos = cosf(CollisionDetector::kMaxObbSlope);

//--------------------------------------------------------------------------------
//
//  �Փ˔���֐�
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃX�t�B�A�̓����蔻��֐�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(SphereCollider& sphere_left, SphereCollider& sphere_right)
{
    const Matrix44& sphere_left_world = sphere_left.GetWorldMatrix();
    const Vector3& sphere_left_position = Vector3(sphere_left_world.rows_[3]);
    const float& sphere_left_radius = sphere_left.GetRadius() * Vector3(sphere_left_world.rows_[0]).Magnitude();

    const Matrix44& sphere_right_world = sphere_right.GetWorldMatrix();
    const Vector3& sphere_right_position = Vector3(sphere_right_world.rows_[3]);
    const float& sphere_right_radius = sphere_right.GetRadius() * Vector3(sphere_right_world.rows_[0]).Magnitude();

    Vector3& midline = sphere_left_position - sphere_right_position;
    float min_distance = sphere_left_radius + sphere_right_radius;
    float square_distance = midline.SquareMagnitude();
    if (square_distance >= min_distance * min_distance) return;

    if (sphere_left.IsTrigger() || sphere_right.IsTrigger())
    {//�g���K�[�������畨���������Ȃ�
        sphere_left.OnTrigger(sphere_right);
        sphere_right.OnTrigger(sphere_left);
        return;
    }

    auto collision = MY_NEW Collision;

    //�Փ˓_�̎Z�o
    collision->point = sphere_right_position + midline * 0.5f;

    //�Փː[�x�̎Z�o
    float distance = sqrtf(square_distance);
    collision->penetration = min_distance - distance;

    //�Փ˖@���̎Z�o
    collision->normal = midline / distance;

    //���W�b�h�{�f�B�̎擾
    auto left_rigidbody = sphere_left.GetGameObject().GetRigidbody();
    auto right_rigidbody = sphere_right.GetGameObject().GetRigidbody();
    if (left_rigidbody->GetType() == Rigidbody::kRigidbody3D)
    {
        collision->rigidbody_one = static_cast<Rigidbody3D*>(left_rigidbody);

        if (right_rigidbody->GetType() == Rigidbody::kRigidbody3D)
        {
            collision->rigidbody_two = static_cast<Rigidbody3D*>(right_rigidbody);
        }
    }
    else
    {//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
        collision->normal *= -1.0f;
        collision->rigidbody_one = static_cast<Rigidbody3D*>(right_rigidbody);
    }

    //�������Z�V�X�e���Ƀ��W�X�g��
    MainSystem::Instance().GetPhysicsSystem().Register(collision);

    //OnCollision
    CollisionInfo info;
    info.self = &sphere_left;
    info.other = &sphere_right;
    info.collisions.push_back(collision);
    sphere_left.OnCollision(info);

    info.self = &sphere_right;
    info.other = &sphere_left;
    sphere_right.OnCollision(info);
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��AABB�̓����蔻��֐�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(SphereCollider& sphere, AabbCollider& aabb)
{
    const Matrix44& sphere_world = sphere.GetWorldMatrix();
    const Vector3& sphere_position = Vector3(sphere_world.rows_[3]);
    const float& sphere_radius = sphere.GetRadius() * Vector3(sphere_world.rows_[0]).Magnitude();

    const Vector3& aabb_half_size = aabb.GetHalfSize();
    const Vector3& aabb_position = aabb.GetWorldPosition();
    Vector3& real_position = sphere_position - aabb_position;

    //�������`�F�b�N
    if (fabsf(real_position.x_) - sphere_radius > aabb_half_size.x_
        || fabsf(real_position.y_) - sphere_radius > aabb_half_size.y_
        || fabsf(real_position.z_) - sphere_radius > aabb_half_size.z_)
    {
        return;
    }

    Vector3 closest_position;
    float distance;

    //AABB�ƃX�t�B�A�̍ŋߓ_�̎Z�o
    distance = real_position.x_;
    if (distance > aabb_half_size.x_) { distance = aabb_half_size.x_; }
    else if (distance < -aabb_half_size.x_) { distance = -aabb_half_size.x_; }
    closest_position.x_ = distance;

    distance = real_position.y_;
    if (distance > aabb_half_size.y_) { distance = aabb_half_size.y_; }
    else if (distance < -aabb_half_size.y_) { distance = -aabb_half_size.y_; }
    closest_position.y_ = distance;

    distance = real_position.z_;
    if (distance > aabb_half_size.z_) { distance = aabb_half_size.z_; }
    else if (distance < -aabb_half_size.z_) { distance = -aabb_half_size.z_; }
    closest_position.z_ = distance;

    //�Փˌ��m
    float square_distance = (closest_position - real_position).SquareMagnitude();
    if (square_distance >= sphere_radius * sphere_radius) return;

    //OnTrigger
    if (sphere.IsTrigger() || aabb.IsTrigger())
    {//�g���K�[�������畨���������Ȃ�
        sphere.OnTrigger(aabb);
        aabb.OnTrigger(sphere);
        return;
    }

    //�Փˏ��
    closest_position = closest_position + aabb_position;
    auto collision = MY_NEW Collision;
    collision->normal = sphere_position - closest_position;
    if (collision->normal.SquareMagnitude() == 0.0f)
    {//���S��aabb�̒��ɂ���
        auto sphere_rigidbody = sphere.GetGameObject().GetRigidbody();
        if (sphere_rigidbody->GetType() == Rigidbody::kRigidbody3D)
        {
            auto rigidbody = static_cast<Rigidbody3D*>(sphere_rigidbody);
            collision->normal = rigidbody->GetAcceleration() * -1.0f;
        }
    }
    collision->normal.Normalize();
    collision->point = closest_position;
    collision->penetration = sphere_radius - sqrtf(square_distance);

    //���W�b�h�{�f�B�̎擾
    auto sphere_rigidbody = sphere.GetGameObject().GetRigidbody();
    auto aabb_rigidbody = aabb.GetGameObject().GetRigidbody();
    if (sphere_rigidbody->GetType() == Rigidbody::kRigidbody3D)
    {
        collision->rigidbody_one = static_cast<Rigidbody3D*>(sphere_rigidbody);

        if (aabb_rigidbody->GetType() == Rigidbody::kRigidbody3D)
        {
            collision->rigidbody_two = static_cast<Rigidbody3D*>(aabb_rigidbody);
        }
    }
    else
    {//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
        collision->normal *= -1.0f;
        collision->rigidbody_one = static_cast<Rigidbody3D*>(aabb_rigidbody);
    }

    //�������Z�V�X�e���Ƀ��W�X�g��
    MainSystem::Instance().GetPhysicsSystem().Register(collision);

    //OnCollision
    CollisionInfo info;
    info.self = &sphere;
    info.other = &aabb;
    info.collisions.push_back(collision);
    sphere.OnCollision(info);

    info.self = &aabb;
    info.other = &sphere;
    aabb.OnCollision(info);
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��OBB�̓����蔻��֐�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(SphereCollider& sphere, ObbCollider& obb)
{
    const Matrix44& sphere_world = sphere.GetWorldMatrix();
    const Vector3& sphere_position = Vector3(sphere_world.rows_[3]);
    const float& sphere_radius = sphere.GetRadius() * Vector3(sphere_world.rows_[0]).Magnitude();

    const Vector3& obb_half_size = obb.GetHalfSize();
    const Matrix44& obb_matrix = obb.GetWorldMatrix();
    Vector3& real_position = Vector3::TransformInverse(sphere_position, obb_matrix);

    //�������`�F�b�N
    if (fabsf(real_position.x_) - sphere_radius > obb_half_size.x_
        || fabsf(real_position.y_) - sphere_radius > obb_half_size.y_
        || fabsf(real_position.z_) - sphere_radius > obb_half_size.z_)
    {
        return;
    }

    Vector3 closest_position;
    float distance;

    //OBB�ƃX�t�B�A�̍ŋߓ_�̎Z�o
    distance = real_position.x_;
    if (distance > obb_half_size.x_) { distance = obb_half_size.x_; }
    else if (distance < -obb_half_size.x_) { distance = -obb_half_size.x_; }
    closest_position.x_ = distance;

    distance = real_position.y_;
    if (distance > obb_half_size.y_) { distance = obb_half_size.y_; }
    else if (distance < -obb_half_size.y_) { distance = -obb_half_size.y_; }
    closest_position.y_ = distance;

    distance = real_position.z_;
    if (distance > obb_half_size.z_) { distance = obb_half_size.z_; }
    else if (distance < -obb_half_size.z_) { distance = -obb_half_size.z_; }
    closest_position.z_ = distance;

    //�Փˌ��m
    float square_distance = (closest_position - real_position).SquareMagnitude();
    if (square_distance >= sphere_radius * sphere_radius) return;

    //OnTrigger
    if (sphere.IsTrigger() || obb.IsTrigger())
    {//�g���K�[�������畨���������Ȃ�
        sphere.OnTrigger(obb);
        obb.OnTrigger(sphere);
        return;
    }

    //�Փˏ��
    closest_position = Vector3::TransformCoord(closest_position, obb_matrix);
    auto collision = MY_NEW Collision;
    collision->normal = sphere_position - closest_position;
    if (collision->normal.SquareMagnitude() == 0.0f)
    {//���S��obb�̒��ɂ���
        auto sphere_rigidbody = sphere.GetGameObject().GetRigidbody();
        if (sphere_rigidbody->GetType() == Rigidbody::kRigidbody3D)
        {
            auto rigidbody = static_cast<Rigidbody3D*>(sphere_rigidbody);
            collision->normal = rigidbody->GetMovement() * -1.0f;
        }
    }
    collision->normal.Normalize();
    float dot = Vector3::kUp.Dot(collision->normal);
    if (dot > kMaxObbSlopeCos)
    {// Obb�̊p�x���o���ő�p�x�ȏ�̏ꍇ(�����ʂɑ΂���)
     // �Փ˖@���𐢊E������ɂ���
        collision->normal = Vector3::kUp;
    }
    collision->point = closest_position;
    collision->penetration = sphere_radius - sqrtf(square_distance);

    //���W�b�h�{�f�B�̎擾
    auto sphere_rigidbody = sphere.GetGameObject().GetRigidbody();
    auto obb_rigidbody = obb.GetGameObject().GetRigidbody();
    if (sphere_rigidbody->GetType() == Rigidbody::kRigidbody3D)
    {
        collision->rigidbody_one = static_cast<Rigidbody3D*>(sphere_rigidbody);

        if (obb_rigidbody->GetType() == Rigidbody::kRigidbody3D)
        {
            collision->rigidbody_two = static_cast<Rigidbody3D*>(obb_rigidbody);
        }
    }
    else
    {//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
        collision->normal *= -1.0f;
        collision->rigidbody_one = static_cast<Rigidbody3D*>(obb_rigidbody);
    }

    //�������Z�V�X�e���Ƀ��W�X�g��
    MainSystem::Instance().GetPhysicsSystem().Register(collision);

    //OnCollision
    CollisionInfo info;
    info.self = &sphere;
    info.other = &obb;
    info.collisions.push_back(collision);
    sphere.OnCollision(info);

    info.self = &obb;
    info.other = &sphere;
    obb.OnCollision(info);
}

//--------------------------------------------------------------------------------
//  AABB��AABB�̓����蔻��֐�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(AabbCollider& aabb_left, AabbCollider& aabb_right)
{
    if (!IsOverlap(aabb_left, aabb_right)) return;

    //OnTrigger
    if (aabb_left.IsTrigger() || aabb_right.IsTrigger())
    {//�g���K�[�������畨���������Ȃ�
        aabb_left.OnTrigger(aabb_right);
        aabb_right.OnTrigger(aabb_left);
        return;
    }

    //XYZ����Ԑ[�x���󂢂̎���􂢏o��
    const Vector3& left_position = aabb_left.GetWorldPosition();
    const Vector3& left_half_size = aabb_left.GetHalfSize();
    const Vector3& right_position = aabb_right.GetWorldPosition();
    const Vector3& right_half_size = aabb_right.GetHalfSize();
    Vector3& midline = left_position - right_position;
    Vector3& no_collision_distance = left_half_size + right_half_size;
    float penetration_x = no_collision_distance.x_ - fabsf(midline.x_);
    float penetration_y = no_collision_distance.y_ - fabsf(midline.y_);
    float penetration_z = no_collision_distance.z_ - fabsf(midline.z_);

    penetration_x = penetration_x > 0.0f ? penetration_x : no_collision_distance.x_;
    penetration_y = penetration_y > 0.0f ? penetration_y : no_collision_distance.y_;
    penetration_z = penetration_z > 0.0f ? penetration_z : no_collision_distance.z_;
    float penetration_min = min(penetration_x, min(penetration_y, penetration_z));
    
    auto collision = MY_NEW Collision;
    collision->penetration = penetration_min;
    collision->point = midline * 0.5f;
    if (penetration_x == penetration_min)
    {
        collision->normal = midline.x_ < 0.0f ? Vector3::kLeft : Vector3::kRight;
    }
    else if (penetration_y == penetration_min)
    {
        collision->normal = midline.y_ < 0.0f ? Vector3::kDown : Vector3::kUp;
    }
    else
    {
        collision->normal = midline.z_ < 0.0f ? Vector3::kBack : Vector3::kForward;
    }

    //���W�b�h�{�f�B�̎擾
    auto left_rigidbody = aabb_left.GetGameObject().GetRigidbody();
    auto right_rigidbody = aabb_right.GetGameObject().GetRigidbody();

    if (left_rigidbody->GetType() == Rigidbody::kRigidbody3D)
    {
        collision->rigidbody_one = static_cast<Rigidbody3D*>(left_rigidbody);

        if (right_rigidbody->GetType() == Rigidbody::kRigidbody3D)
        {
            collision->rigidbody_two = static_cast<Rigidbody3D*>(right_rigidbody);
        }
    }
    else
    {//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
        collision->normal *= -1.0f;
        collision->rigidbody_one = static_cast<Rigidbody3D*>(right_rigidbody);
    }

    //�������Z�V�X�e���Ƀ��W�X�g��
    MainSystem::Instance().GetPhysicsSystem().Register(collision);

    //OnCollision
    CollisionInfo info;
    info.collisions.push_back(collision);
    info.self = &aabb_left;
    info.other = &aabb_right;
    aabb_left.OnCollision(info);

    info.self = &aabb_right;
    info.other = &aabb_left;
    aabb_right.OnCollision(info);
}

//--------------------------------------------------------------------------------
//  box��box�̓����蔻��֐�(�����ꂩ���K��OBB)
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(BoxCollider& box_left, BoxCollider& box_right)
{
    //�{�b�N�X��xyz����ɏd�˂Ă邩�ǂ������`�F�b�N����
    if (!IsOverlapOnAxis(box_left, box_right, Vector3::kAxisX)
        && !IsOverlapOnAxis(box_left, box_right, Vector3::kAxisY)
        && !IsOverlapOnAxis(box_left, box_right, Vector3::kAxisZ))
    {
        return;
    }

    //box_left�̂��ׂĂ̒��_��box_right�Ɣ��肵�A�߂荞�݂���Ԑ[���̒��_��􂢏o��
    Collision* left_max_penetration_collision = nullptr;
    for (Vector3& vertex : box_left.GetWorldVertexes())
    {
        auto collision = Detect(vertex, box_right);
        left_max_penetration_collision = MaxPenetration(left_max_penetration_collision, collision);
    }

    //box_right�̂��ׂĂ̒��_��box_left�Ɣ��肵�A�߂荞�݂���Ԑ[���̒��_��􂢏o��
    Collision* right_max_penetration_collision = nullptr;
    for (Vector3& vertex : box_right.GetWorldVertexes())
    {
        auto collision = Detect(vertex, box_right);
        right_max_penetration_collision = MaxPenetration(right_max_penetration_collision, collision);
    }

    if (!left_max_penetration_collision && !right_max_penetration_collision) return;

    //OnTrigger
    if (box_left.IsTrigger() || box_right.IsTrigger())
    {//�g���K�[�������畨���������Ȃ�
        box_left.OnTrigger(box_right);
        box_right.OnTrigger(box_left);
        SAFE_DELETE(left_max_penetration_collision);
        SAFE_DELETE(right_max_penetration_collision);
        return;
    }

    CollisionInfo info;

    //���W�b�h�{�f�B�̎擾
    auto left_rigidbody = box_left.GetGameObject().GetRigidbody();
    auto right_rigidbody = box_right.GetGameObject().GetRigidbody();

    if (left_max_penetration_collision)
    {
        if (left_rigidbody->GetType() == Rigidbody::kRigidbody3D)
        {
            left_max_penetration_collision->rigidbody_one = static_cast<Rigidbody3D*>(left_rigidbody);

            if (right_rigidbody->GetType() == Rigidbody::kRigidbody3D)
            {
                left_max_penetration_collision->rigidbody_two = static_cast<Rigidbody3D*>(right_rigidbody);
            }
        }
        else
        {//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
            left_max_penetration_collision->normal *= -1.0f;
            left_max_penetration_collision->rigidbody_one = static_cast<Rigidbody3D*>(right_rigidbody);
        }

        //�������Z�V�X�e���Ƀ��W�X�g��
        MainSystem::Instance().GetPhysicsSystem().Register(left_max_penetration_collision);
        info.collisions.push_back(left_max_penetration_collision);
    }

    if (right_max_penetration_collision)
    {
        if (right_rigidbody->GetType() == Rigidbody::kRigidbody3D)
        {
            right_max_penetration_collision->rigidbody_one = static_cast<Rigidbody3D*>(right_rigidbody);

            if (left_rigidbody->GetType() == Rigidbody::kRigidbody3D)
            {
                right_max_penetration_collision->rigidbody_two = static_cast<Rigidbody3D*>(left_rigidbody);
            }
        }
        else
        {//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
            right_max_penetration_collision->normal *= -1.0f;
            right_max_penetration_collision->rigidbody_one = static_cast<Rigidbody3D*>(left_rigidbody);
        }

        //�������Z�V�X�e���Ƀ��W�X�g��
        MainSystem::Instance().GetPhysicsSystem().Register(right_max_penetration_collision);
        info.collisions.push_back(right_max_penetration_collision);
    }

    //OnCollision
    info.self = &box_left;
    info.other = &box_right;
    box_left.OnCollision(info);

    info.self = &box_right;
    info.other = &box_left;
    box_right.OnCollision(info);
}

//--------------------------------------------------------------------------------
//
//  �t�B�[���h
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃt�B�[���h�̓����蔻��֐�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(SphereCollider& sphere, FieldCollider& field)
{
    const Matrix44& sphere_world = sphere.GetWorldMatrix();
    const Vector3& sphere_position = Vector3(sphere_world.rows_[3]);
    const float& sphere_radius = sphere.GetRadius() * Vector3(sphere_world.rows_[0]).Magnitude();

    auto collision = Detect(sphere_position, field);
    if (!collision) return;

    float penetration = collision->penetration + sphere_radius;
    if (penetration <= 0.0f)
    {
        MY_DELETE collision;
        return;
    }

    if (sphere.IsTrigger())
    {
        sphere.OnTrigger(field);
        field.OnTrigger(sphere);
        MY_DELETE collision;
        return;
    }

    collision->rigidbody_one = static_cast<Rigidbody3D*>(sphere.GetGameObject().GetRigidbody());
    collision->rigidbody_two = nullptr;
    float dot = Vector3::kUp.Dot(collision->normal);
    if (dot < kMaxFieldSlopeCos)
    {// �n�ʖ@���̊p�x���o���ő�p�x�ȏ�̏ꍇ(�����ʂɑ΂���)
        // �o���Ȃ����ߖ@�������̈ړ��ʂ��Փː[�x�ɂ���
        if (dot < kMinWallSlopeCos)
        {// �p�x���傫������Əd�͂����˕Ԃ邱�Ƃ��ł��Ȃ����߁A�p�x���C������
            Vector3& right = Vector3::kUp * collision->normal;
            Quaternion& rotation = Quaternion::RotateAxis(right, kMinWallSlope);
            collision->normal = Vector3::Rotate(Vector3::kUp, rotation).Normalized();
        }

        collision->penetration = collision->normal.Dot(collision->rigidbody_one->GetMovement() * -1.0f);
    }
    else
    {// ��������Ȃ��Ȃ�Փ˖@���𐢊E������ɂ���
        collision->normal = Vector3::kUp;
        collision->penetration = penetration;
    }

    //�������Z�V�X�e���Ƀ��W�X�g��
    MainSystem::Instance().GetPhysicsSystem().Register(collision);

    //OnCollision
    CollisionInfo info;
    info.self = &sphere;
    info.other = &field;
    info.collisions.push_back(collision);
    sphere.OnCollision(info);

    info.self = &field;
    info.other = &sphere;
    field.OnCollision(info);
}

//--------------------------------------------------------------------------------
//  Box�ƃt�B�[���h�̓����蔻��֐�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(BoxCollider& box, FieldCollider& field)
{
    //��Ԑ[���̒��_���ďo����
    Collision* max_penetration_collision = nullptr;
    for (Vector3& vertex : box.GetWorldVertexes())
    {
        auto collision = Detect(vertex, field);
        max_penetration_collision = MaxPenetration(max_penetration_collision, collision);
    }

    if (!max_penetration_collision) return;

    if (box.IsTrigger())
    {
        box.OnTrigger(field);
        field.OnTrigger(box);
        MY_DELETE max_penetration_collision;
        return;
    }

    //���W�b�h�{�f�B�̎擾
    max_penetration_collision->rigidbody_one = static_cast<Rigidbody3D*>(box.GetGameObject().GetRigidbody());
    max_penetration_collision->rigidbody_two = nullptr;

    //�������Z�V�X�e���Ƀ��W�X�g��
    MainSystem::Instance().GetPhysicsSystem().Register(max_penetration_collision);

    //OnCollision
    CollisionInfo info;
    info.self = &box;
    info.other = &field;
    info.collisions.push_back(max_penetration_collision);
    box.OnCollision(info);

    info.self = &field;
    info.other = &box;
    field.OnCollision(info);
}

//--------------------------------------------------------------------------------
//
//  ���C
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//    ���C�ƃ{�b�N�X�̓����蔻��
//--------------------------------------------------------------------------------
//RayHitInfo* CollisionDetector::Detect(const Ray& ray, const float& distance, BoxCollider& box)
//{
//    auto collision = Detect(ray.origin_, box);
//    if (collision)
//    {
//        auto result = MY_NEW RayHitInfo;
//        result->normal = collision->normal;
//        result->position = ray.origin_;
//        result->other = &box;
//        result->distance = collision->penetration;
//        MY_DELETE collision;
//        return result;
//    }
//    
//    auto& ray_end = ray.origin_ + ray.direction_ * distance;
//    collision = Detect(ray_end, box);
//    if (collision)
//    {
//        auto result = MY_NEW RayHitInfo;
//        result->normal = collision->normal;
//        result->position = ray_end;
//        result->other = &box;
//        result->distance = collision->penetration;
//        MY_DELETE collision;
//        return result;
//    }
//
//    return nullptr;
//}

//--------------------------------------------------------------------------------
//  ray��AABB�̓����蔻��
//--------------------------------------------------------------------------------
RayHitInfo* CollisionDetector::Detect(const Ray& ray, const float& distance, AabbCollider& aabb)
{
    const Vector3& aabb_position = aabb.GetWorldPosition();
    const Vector3& half_size = aabb.GetHalfSize();
    Collision* collision = Detect(Ray(ray.origin_ - aabb_position, ray.direction_), distance, half_size);
    if (!collision) return nullptr;
    auto result = MY_NEW RayHitInfo;
    result->normal = collision->normal;
    result->position = collision->point + aabb_position;
    result->other = &aabb;
    result->distance = collision->penetration;
    MY_DELETE collision;
    return result;
}

//--------------------------------------------------------------------------------
//  ray��obb�̓����蔻��
//--------------------------------------------------------------------------------
RayHitInfo* CollisionDetector::Detect(const Ray& ray, const float& distance, ObbCollider& obb)
{
    Matrix44 obb_matrix = obb.GetWorldMatrix();
    const Vector3& half_size = obb.GetHalfSize();

    Ray real_ray = ray;
    real_ray.origin_ = Vector3::TransformInverse(real_ray.origin_, obb_matrix);
    real_ray.direction_ = Vector3::TransformNormal(real_ray.direction_, obb_matrix.Transpose()).Normalized();

    Collision* collision = Detect(real_ray, distance, half_size);
    if (!collision) return nullptr;
    auto result = MY_NEW RayHitInfo;
    result->normal = Vector3::TransformNormal(collision->normal, obb_matrix);
    result->position = Vector3::TransformCoord(collision->point, obb_matrix);
    result->other = &obb;
    result->distance = collision->penetration;
    MY_DELETE collision;
    return result;
}

//--------------------------------------------------------------------------------
//  ���C�ƃX�t�B�A�̓����蔻��
//--------------------------------------------------------------------------------
RayHitInfo* CollisionDetector::Detect(const Ray& ray, const float& distance, SphereCollider& sphere)
{
    const Matrix44& sphere_world = sphere.GetWorldMatrix();
    const Vector3& sphere_position = Vector3(sphere_world.rows_[3]);
    const float& sphere_radius = sphere.GetRadius() * Vector3(sphere_world.rows_[0]).Magnitude();

    Vector3& sphere_to_origin = ray.origin_ - sphere_position;
    float work_a = 2.0f * ray.direction_.Dot(sphere_to_origin);
    float work_b = sphere_to_origin.Dot(sphere_to_origin) - sphere_radius * sphere_radius;
    
    float dicriminant = work_a * work_a - 4.0f * work_b;
    if (dicriminant < 0.0f) return nullptr;
    dicriminant = sqrtf(dicriminant);

    float time_a = (-work_a + dicriminant) / 2.0f;
    float time_b = (-work_a - dicriminant) / 2.0f;
    if (time_a < 0.0f && time_b < 0.0f) return nullptr;
    
    //�ŒZ���Ԃ�ۑ�
    time_a = time_a < 0.0f ? time_b : time_a;
    time_b = time_b < 0.0f ? time_a : time_b;
    float min_time = time_a <= time_b ? time_a : time_b;

    if (min_time > distance) { return nullptr; }

    auto result = MY_NEW RayHitInfo;
    result->distance = min_time;
    result->other = &sphere;
    result->position = ray.origin_ + ray.direction_ * min_time;
    result->normal = (result->position - sphere_position).Normalized();
    return result;
}

//--------------------------------------------------------------------------------
//  ���C�ƃt�B�[���h�̓����蔻��
//--------------------------------------------------------------------------------
RayHitInfo* CollisionDetector::Detect(const Ray& ray, const float& distance, FieldCollider& field)
{
    auto collision = Detect(ray.origin_ + ray.direction_ * distance, field);
    if (!collision) return nullptr;
    if (collision->penetration < 0.0f) 
    {
        MY_DELETE collision;
        return nullptr;
    }
    auto result = MY_NEW RayHitInfo;
    result->distance = ray.origin_.y_ - collision->point.y_;
    result->normal = collision->normal;
    result->other = &field;
    result->position = collision->point;
    MY_DELETE collision;
    return result;
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �_��AABB�̓����蔻��
//--------------------------------------------------------------------------------
Collision* CollisionDetector::Detect(const Vector3& point, const AabbCollider& aabb)
{
    const Vector3& aabb_position = aabb.GetWorldPosition();
    const Vector3& half_size = aabb.GetHalfSize();
    Vector3& real_point = point - aabb_position;

    float min_depth = half_size.x_ - fabsf(real_point.x_);
    if (min_depth <= 0.0f) return nullptr;
    Vector3 normal = real_point.x_ < 0.0f ? Vector3::kLeft : Vector3::kRight;

    float depth = half_size.y_ - fabsf(real_point.y_);
    if (depth <= 0.0f) return nullptr;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = real_point.y_ < 0.0f ? Vector3::kDown : Vector3::kUp;
    }

    depth = half_size.z_ - fabsf(real_point.z_);
    if (depth <= 0.0f) return nullptr;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = real_point.z_ < 0.0f ? Vector3::kBack : Vector3::kForward;
    }

    auto result = MY_NEW Collision;
    result->normal = normal;
    result->penetration = min_depth;
    result->point = point;
    return result;
}

//--------------------------------------------------------------------------------
//  �_��Box�̓����蔻��
//--------------------------------------------------------------------------------
Collision* CollisionDetector::Detect(const Vector3& point, const BoxCollider& box)
{
    const Matrix44& box_matrix = box.GetWorldMatrix();
    const Vector3& half_size = box.GetHalfSize();
    Vector3& real_point = Vector3::TransformInverse(point, box_matrix);

    float min_depth = half_size.x_ - fabsf(real_point.x_);
    if (min_depth <= 0.0f) return nullptr;
    Vector3 normal = Vector3(box_matrix.m00_, box_matrix.m01_, box_matrix.m02_)
        * (real_point.x_ < 0.0f ? -1.0f : 1.0f);

    float depth = half_size.y_ - fabsf(real_point.y_);
    if (depth <= 0.0f) return nullptr;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = Vector3(box_matrix.m10_, box_matrix.m11_, box_matrix.m12_)
            * (real_point.y_ < 0.0f ? -1.0f : 1.0f);
    }

    depth = half_size.z_ - fabsf(real_point.z_);
    if (depth <= 0.0f) return nullptr;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = Vector3(box_matrix.m20_, box_matrix.m21_, box_matrix.m22_)
            * (real_point.z_ < 0.0f ? -1.0f : 1.0f);
    }

    auto result = MY_NEW Collision;
    result->normal = normal;
    result->penetration = min_depth;
    result->point = point;
    return result;
}

//--------------------------------------------------------------------------------
//  �_��field�̓����蔻��
//--------------------------------------------------------------------------------
Collision* CollisionDetector::Detect(const Vector3& point, const FieldCollider& field)
{
    auto polygon_info = field.GetPolygonAt(point);
    if (!polygon_info) return nullptr;
    Collision* result = nullptr;

    // �n�ʔ����xz���ʂȂ̂ŏՓ˖@���͕K�����E�̏�����ɂȂ�
    float point_y_on_field = polygon_info->side.y_ - ((point.x_ - polygon_info->side.x_) * polygon_info->normal.x_ + (point.z_ - polygon_info->side.z_) * polygon_info->normal.z_) / polygon_info->normal.y_;

    //// �n�ʂ̓��e�ʒu�̎Z�o
    //auto& center = (polygon_info->left_up + polygon_info->right_down) * 0.5f;
    //auto& right = (polygon_info->right_down - polygon_info->left_up).Normalized();
    //auto& forward = (right * polygon_info->normal).Normalized();
    //auto& transform = Matrix44::Transform(right, polygon_info->normal, forward, center);
    //auto& real_position = Vector3::TransformInverse(point, transform);
    //result->penetration = -real_position.y_;
    //// �o���Ȃ����ߖ@����������Ɛ��������ɂ���
    //result = MY_NEW Collision;
    //result->point = point;
    //result->normal = (Vector3::kUp * polygon_info->normal * Vector3::kUp).Normalized();
    //result->penetration = -real_position.y_;
    //MY_DELETE polygon_info;
    //return result;

    result = MY_NEW Collision;
    result->point = Vector3(point.x_, point_y_on_field, point.z_);
    result->normal = polygon_info->normal; // �ǃX���𔻒肷�邽�ߒn�ʖ@����Ԃ�
    result->penetration = point_y_on_field - point.y_;
    MY_DELETE polygon_info;
    return result;
}

//--------------------------------------------------------------------------------
//  ray��aabb�̓����蔻��
//--------------------------------------------------------------------------------
Collision* CollisionDetector::Detect(const Ray& ray, const float& distance, const Vector3& halfsize)
{
    float time_min = -FLT_MAX;
    float time_max = FLT_MAX;
    const Vector3& min = halfsize * -1.0f;
    const Vector3& max = halfsize;
    Vector3 normal;

    for (int count = 0; count < 3; ++count) 
    {
        if (fabsf(ray.direction_.m_[count]) < FLT_EPSILON)
        {
            if (ray.origin_.m_[count] < min.m_[count] || ray.origin_.m_[count] > max.m_[count])
            {// �������Ă��Ȃ�
                return nullptr;
            }
        }
        else 
        {
            // �ʂƂ̋������Z�o
            // t1���ߖʁAt2�����ʂƂ̋���
            float odd = 1.0f / ray.direction_.m_[count];
            float t1 = (min.m_[count] - ray.origin_.m_[count]) * odd;
            float t2 = (max.m_[count] - ray.origin_.m_[count]) * odd;

            float sign = -1.0f;
            if (t1 > t2) 
            {// ��������
                Math::Swap(t1, t2);
                sign = 1.0f;
            }

            if (t1 > time_min)
            {
                time_min = t1;
                normal = Vector3::kZero;
                normal.m_[count] = sign;
            }
            if (t2 < time_max) time_max = t2;

            // �ʌ����`�F�b�N
            if (time_min >= time_max)
            {// �������Ă��Ȃ�
                return nullptr;
            }
        }
    }

    // Distance�`�F�b�N
    if (distance < time_min || time_max <= 0.0f)
    {// �������Ă��Ȃ�
        return nullptr;
    }
    if (time_min < 0.0f) time_min = 0.0f;

    Collision* result = MY_NEW Collision;
    result->point = ray.origin_ + ray.direction_ * time_min;
    result->normal = normal;
    result->penetration = time_min;
    return result;
}

//--------------------------------------------------------------------------------
//  �����ƒ����̌�������(�񎟌�)
//--------------------------------------------------------------------------------
Vector2* CollisionDetector::Detect(const Vector2& begin_left, const Vector2& end_left, const Vector2& begin_right, const Vector2& end_right)
{
    Vector2& line_left = end_left - begin_left;
    Vector2& line_right = end_right - begin_right;

    //���F Y = slope * X + add
    float slope_left = (begin_left.x_ - end_left.x_) == 0.0f ? 0.0f
        : (begin_left.y_ - end_left.y_) / (begin_left.x_ - end_left.x_);
    float add_left = begin_left.y_ - slope_left * begin_left.x_;
    float slope_right = (begin_right.x_ - end_right.x_) == 0.0f ? 0.0f
        : (begin_right.y_ - end_right.y_) / (begin_right.x_ - end_right.x_);
    float add_right = begin_right.y_ - slope_left * begin_right.x_;

    //���s
    if (slope_left == slope_right) return nullptr;

    //��_�̎Z�o
    auto result = MY_NEW Vector2;
    result->x_ = (add_right - add_left) / (slope_left - slope_right);
    result->y_ = slope_left * result->x_ + add_left;

    //��_�����C���͈͓̔��ɂ��邩���`�F�b�N
    //line_left
    Vector2& begin_left_to_result = *result - begin_left;
    if (line_left.Dot(begin_left_to_result) < 0.0f // �����`�F�b�N
        || begin_left_to_result.SquareMagnitude() > line_left.SquareMagnitude()) //�����`�F�b�N
    {
        MY_DELETE result;
        return nullptr;
    }

    //line_right
    Vector2& begin_right_to_result = *result - begin_right;
    if (line_right.Dot(begin_right_to_result) < 0.0f // �����`�F�b�N
        || begin_right_to_result.SquareMagnitude() > line_right.SquareMagnitude()) //�����`�F�b�N
    {
        MY_DELETE result;
        return nullptr;
    }
    return result;
}

//--------------------------------------------------------------------------------
//  �����ƒ����̌�������(�O����)
//--------------------------------------------------------------------------------
Vector3* CollisionDetector::Detect(const Vector3& begin_left, const Vector3& end_left, const Vector3& begin_right, const Vector3& end_right)
{
    Vector3& line_left = begin_left - end_left;
    Vector3& line_right = begin_right - end_right;

    //���s�`�F�b�N
    if ((line_left * line_right) == Vector3::kZero) return nullptr;

    //XY���ʂ̌�_�̎Z�o
    auto point_on_xy = Detect(Vector2(begin_left.x_, begin_left.y_), Vector2(begin_left.x_, begin_left.y_), Vector2(begin_left.x_, begin_left.y_), Vector2(begin_left.x_, begin_left.y_));
    if (!point_on_xy) return nullptr;

    //������Z�l���Z�o����
    line_left.Normalize();
    line_right.Normalize();
    float rate_left = line_left.x_ != 0.0f ? (point_on_xy->x_ - begin_left.x_) / line_left.x_
        : line_left.y_ != 0.0f ? (point_on_xy->y_ - begin_left.y_) / line_left.y_
        : 0.0f;
    float rate_right = line_right.x_ != 0.0f ? (point_on_xy->x_ - begin_right.x_) / line_right.x_
        : line_right.y_ != 0.0f ? (point_on_xy->y_ - begin_right.y_) / line_right.y_
        : 0.0f;
    float z_left = begin_left.z_ + rate_left * line_left.z_;
    float z_right = begin_right.z_ + rate_right * line_right.z_;

    if (z_left != z_right)
    {
        MY_DELETE point_on_xy;
        return nullptr;
    }

    auto result = MY_NEW Vector3(point_on_xy->x_, point_on_xy->y_, z_left);
    MY_DELETE point_on_xy;
    return result;
}

//--------------------------------------------------------------------------------
//  box��^����ꂽ���ɓ��e���Ē������Z�o����֐�
//--------------------------------------------------------------------------------
float CollisionDetector::ProjectBoxToAxis(const BoxCollider& box, const Vector3& axis)
{
    const Vector3& half_size = box.GetHalfSize();
    const Matrix44& box_matrix = box.GetWorldMatrix();
    return half_size.x_ * fabsf(axis.Dot(Vector3(box_matrix.m00_, box_matrix.m01_, box_matrix.m02_)))
        + half_size.y_ * fabsf(axis.Dot(Vector3(box_matrix.m10_, box_matrix.m11_, box_matrix.m12_)))
        + half_size.z_ * fabsf(axis.Dot(Vector3(box_matrix.m20_, box_matrix.m21_, box_matrix.m22_)));
}

//--------------------------------------------------------------------------------
//  �{�b�N�X���m���^����ꂽ���ɏd�˂Ă邩�ǂ������`�F�b�N����
//--------------------------------------------------------------------------------
bool CollisionDetector::IsOverlapOnAxis(const BoxCollider& box_left, const BoxCollider& box_right, const Vector3& axis)
{
    //����̒������Z�o����
    float length_on_axis_left = ProjectBoxToAxis(box_left, axis);
    float length_on_axis_right = ProjectBoxToAxis(box_right, axis);

    //����̒��S�Ԃ̋������Z�o����
    auto& box_right_to_box_left = box_left.GetWorldPosition() - box_right.GetWorldPosition();
    float distance = fabsf(box_right_to_box_left.Dot(axis));

    //�d�˂Ă邩�ǂ������`�F�b�N����
    return distance < (length_on_axis_left + length_on_axis_right);
}

//--------------------------------------------------------------------------------
//  AABB���m��XYZ���ɏd�˂Ă邩�ǂ������`�F�b�N����(�O����)
//--------------------------------------------------------------------------------
bool CollisionDetector::IsOverlap(const AabbCollider& aabb_left, const AabbCollider& aabb_right)
{
    const Vector3& aabb_left_position = aabb_left.GetWorldPosition();
    const Vector3& aabb_left_half_size = aabb_left.GetHalfSize();
    const Vector3& aabb_right_position = aabb_right.GetWorldPosition();
    const Vector3& aabb_right_half_size = aabb_right.GetHalfSize();
    Vector3& min_left = aabb_left_position - aabb_left_half_size;
    Vector3& max_left = aabb_left_position + aabb_left_half_size;
    Vector3& min_right = aabb_right_position - aabb_right_half_size;
    Vector3& max_right = aabb_right_position + aabb_right_half_size;

    //AABB���m��xyz����ɏd�˂Ă邩�ǂ������`�F�b�N����
    return IsOverlap(Vector2(min_left.y_, min_left.z_), Vector2(max_left.y_, max_left.z_), Vector2(min_right.y_, min_right.z_), Vector2(max_right.y_, max_right.z_))    //X��
        && IsOverlap(Vector2(min_left.z_, min_left.x_), Vector2(max_left.z_, max_left.x_), Vector2(min_right.z_, min_right.x_), Vector2(max_right.z_, max_right.x_))    //Y��
        && IsOverlap(Vector2(min_left.x_, min_left.y_), Vector2(max_left.x_, max_left.y_), Vector2(min_right.x_, min_right.y_), Vector2(max_right.x_, max_right.y_));    //Z��;
}

//--------------------------------------------------------------------------------
//  AABB���m���^����ꂽ���ɏd�˂Ă邩�ǂ������`�F�b�N����(�񎟌�)
//--------------------------------------------------------------------------------
bool CollisionDetector::IsOverlap(const Vector2& min_left, const Vector2& max_left, const Vector2& min_right, const Vector2& max_right)
{
    return min_left.x_ < max_right.x_ && min_right.x_ < max_left.x_
        && min_left.y_ < max_right.y_ && min_right.y_ < max_left.y_;
}

//--------------------------------------------------------------------------------
//  �Փː[�x����Ԑ[���̃R���W������Ԃ��A�c��͔̂j������
//--------------------------------------------------------------------------------
Collision* CollisionDetector::MaxPenetration(Collision* current, Collision* next)
{
    if (!next) return current;
    if (!current) return next;
    if (next->penetration > current->penetration)
    {
        MY_DELETE current;
        return next;
    }
    MY_DELETE next;
    return current;
}
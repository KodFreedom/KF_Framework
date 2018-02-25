//--------------------------------------------------------------------------------
//  AABBCollider�R���|�l���g
//�@AABBCollider.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "aabb_collider.h"
#include "../../game_object/game_object.h"
#include "../transform/transform.h"

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void AabbCollider::Update(void)
{
    BoxCollider::Update();

    //��]������������
    const float& scale_x = Vector3(world_.m00_, world_.m01_, world_.m02_).Magnitude();
    const float& scale_y = Vector3(world_.m10_, world_.m11_, world_.m12_).Magnitude();
    const float& scale_z = Vector3(world_.m20_, world_.m21_, world_.m22_).Magnitude();
    world_.m_[0][0] = scale_x;
    world_.m_[0][1] = 0.0f;
    world_.m_[0][2] = 0.0f;
    world_.m_[1][0] = 0.0f;
    world_.m_[1][1] = scale_y;
    world_.m_[1][2] = 0.0f;
    world_.m_[2][0] = 0.0f;
    world_.m_[2][1] = 0.0f;
    world_.m_[2][2] = scale_z;
}
//--------------------------------------------------------------------------------
//  sphereColliderコンポネント
//　sphereCollider.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "sphere_collider.h"
#include "../../game_object/game_object.h"
#include "../transform/transform.h"

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
float SphereCollider::GetRadius(void) const
{
    return radius_;
}
//--------------------------------------------------------------------------------
//　animator.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "animator.h"
#include "game_object.h"
#include "game_object_spawner.h"
#include "transform.h"
#include "motion_data.h"
#include "main_system.h"
#include "resources.h"
#include "motion_manager.h"
#include "texture_manager.h"
#include "game_time.h"
#include "collision_detector.h"
#include "collision_system.h"
#include "actor_controller.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
Animator::Animator(GameObject& owner)
    : Component(owner)
{}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool Animator::Init(void)
{
    InitIK();
    return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void Animator::Uninit(void)
{
    avatar_.clear();
    SAFE_DELETE(state_);

    auto& motion_manager = MainSystem::Instance().GetResources().GetMotionManager();
    for (auto& motion_name : motion_names_)
    {
        motion_manager.Disuse(motion_name);
    }
    motion_names_.clear();

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    SAFE_RELEASE(bone_texture_.pointer);
#endif
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void Animator::Update(void)
{
    if (avatar_.empty()) return;
    state_->Update(*this);
    time_counter_ += GameTime::Instance().ScaledDeltaTime();
    UpdateIK();
}

//--------------------------------------------------------------------------------
//  後更新処理
//--------------------------------------------------------------------------------
void Animator::LateUpdate(void)
{
    UpdateBoneTexture();
}

//--------------------------------------------------------------------------------
//  今のアニメーション名を返す
//--------------------------------------------------------------------------------
const String& Animator::GetCurrentAnimationName(void)
{
    return state_->GetCurrentMotionName();
}

//--------------------------------------------------------------------------------
//  アバターの設定
//--------------------------------------------------------------------------------
void Animator::SetAvatar(const String& file_name)
{
    if (!avatar_.empty()) return;

    LoadFromFile(file_name);
    AttachBonesToChildren();

    // textureのサイズの算出（2の冪乗）
    int bone_number = static_cast<int>(avatar_.size());
    bone_texture_.size = 2;
    while (bone_texture_.size < bone_number)
    {
        bone_texture_.size *= 2;
    }

    // textureの生成
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    bone_texture_.pointer = MainSystem::Instance().GetResources().GetTextureManager().CreateEmptyTexture(bone_texture_.size);
#endif
}

//--------------------------------------------------------------------------------
//  モーションステータスの切り替え
//--------------------------------------------------------------------------------
void Animator::Change(MotionState* new_motion_state)
{
    assert(new_motion_state);
    if (observer_ && new_motion_state->GetType() == kNormalMotionState)
    {
        observer_->OnAnimationOver();
    }

    SAFE_DELETE(state_);
    state_ = new_motion_state;
    time_counter_ = 0.0f;
}

//--------------------------------------------------------------------------------
//  今のフレームを取得
//--------------------------------------------------------------------------------
const int Animator::GetCurrentFrame(void) const
{
    return state_->GetCurrentFrame();
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ファイルから読み込む
//--------------------------------------------------------------------------------
void Animator::LoadFromFile(const String& file_name)
{
    String& path = L"data/avatar/" + file_name + L".avatar";
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        assert(file.is_open());
        return;
    }
    BinaryInputArchive archive(file);

    // Avatar
    size_t bone_number;
    archive.loadBinary(&bone_number, sizeof(bone_number));
    avatar_.resize(bone_number);
    for (size_t count = 0; count < bone_number; ++count)
    {
        size_t name_size;
        archive.loadBinary(&name_size, sizeof(name_size));
        string name;
        name.resize(name_size);
        archive.loadBinary(&name[0], name_size);
        avatar_[count].name = String(name.begin(), name.end());
        archive.loadBinary(&avatar_[count].bind_pose_inverse, sizeof(avatar_[count].bind_pose_inverse));
    }

    // IKController
    for (auto& controller : ik_controllers_)
    {
        archive.loadBinary(&controller.index, sizeof(controller.index));
    }

    // Motion
    size_t motion_number;
    archive.loadBinary(&motion_number, sizeof(motion_number));
    motion_names_.resize(motion_number);

    auto& motion_manager = MainSystem::Instance().GetResources().GetMotionManager();
    for (size_t count = 0; count < motion_number; ++count)
    {
        size_t name_size;
        archive.loadBinary(&name_size, sizeof(name_size));

        string name;
        name.resize(name_size);
        archive.loadBinary(&name[0], name_size);

        motion_names_[count] = String(name.begin(), name.end());
        motion_manager.Use(motion_names_[count]);
    }
    file.close();
}

//--------------------------------------------------------------------------------
//  ボーンと子供を関連付ける
//--------------------------------------------------------------------------------
void Animator::AttachBonesToChildren(void)
{
    for (auto& bone : avatar_)
    {
        bone.transform = owner_.GetTransform()->FindChildBy(bone.name);
    }
}

//--------------------------------------------------------------------------------
//  テクスチャのサーフェイスにボーンマトリクスを書き込む
//--------------------------------------------------------------------------------
void Animator::UpdateBoneTexture(void)
{
    if (!bone_texture_.pointer) return;

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    // サーフェイスロック
    D3DLOCKED_RECT lock_rect;
    bone_texture_.pointer->LockRect(0, &lock_rect, NULL, D3DLOCK_DISCARD);

    // テクスチャサーフェイスの初期化
    FillMemory(lock_rect.pBits, lock_rect.Pitch * bone_texture_.size, 0);

    // 骨情報の書き込み、一行ずつ
    size_t bone_number = avatar_.size();
    int offset = lock_rect.Pitch / sizeof(Matrix44);
    for (size_t count = 0; count < bone_number; ++count)
    {
        const Matrix44& bone_world = avatar_[count].bind_pose_inverse * avatar_[count].transform->GetWorldMatrix();
        memcpy((Matrix44*)lock_rect.pBits + offset * count, &bone_world, sizeof(bone_world));
    }

    // サーフェイスアンロック
    bone_texture_.pointer->UnlockRect(0);
#endif
}

//--------------------------------------------------------------------------------
//  インバースキネマティクス(IK)の初期化
//--------------------------------------------------------------------------------
void Animator::InitIK(void)
{
}

//--------------------------------------------------------------------------------
//  インバースキネマティクス(IK)計算
//--------------------------------------------------------------------------------
void Animator::UpdateIK(void)
{
    if (!enable_ik_) return;
    UpdateFootIK();
}

//--------------------------------------------------------------------------------
//  foot ikの計算
//--------------------------------------------------------------------------------
void Animator::UpdateFootIK(void)
{
    // IK goal
    ComputeIKGoal(IKParts::kFootLeft, IKGoals::kIKGoalLeftFoot);
    ComputeIKGoal(IKParts::kFootRight, IKGoals::kIKGoalRightFoot);

    // Compute IK
    ComputeFootIK(IKParts::kFootLeft, IKGoals::kIKGoalLeftFoot);
    ComputeFootIK(IKParts::kFootRight, IKGoals::kIKGoalRightFoot);
}

//--------------------------------------------------------------------------------
//  ik goalの計算
//--------------------------------------------------------------------------------
void Animator::ComputeIKGoal(const IKParts& goal_part, const IKGoals& ik_goal)
{
    float ik_weight_change_speed = ik_weight_decrease_speed_;

    auto& collision_system = MainSystem::Instance().GetCollisionSystem();
    Ray ray(avatar_[ik_controllers_[goal_part].index].transform->GetCurrentWorldPosition(), Vector3::kDown);

    // 下向きに着地点を判定する
    RayHitInfo* info = collision_system.RayCast(ray, ik_ray_distance_, &owner_);
    if (info)
    {
        ik_goals_[ik_goal].position = info->position + ik_foot_position_offset_;
        ik_goals_[ik_goal].up = info->normal;
        ik_weight_change_speed = ik_grounded_distance_ >= info->distance ? ik_weight_increase_speed_ : ik_weight_change_speed;
        MY_DELETE info;
    }

    // Weight更新
    ik_goals_[ik_goal].weight += ik_weight_change_speed * GameTime::Instance().ScaledDeltaTime();
    ik_goals_[ik_goal].weight = Math::Clamp(ik_goals_[ik_goal].weight, 0.0f, 1.0f);
}

//--------------------------------------------------------------------------------
//  foot ik計算(Law of Cosines)
//--------------------------------------------------------------------------------
void Animator::ComputeFootIK(const IKParts& end_part, const IKGoals& ik_goal)
{
    assert((end_part == kFootLeft && ik_goal == kIKGoalLeftFoot)
        || (end_part == kFootRight && ik_goal == kIKGoalRightFoot));

    if (ik_goals_[ik_goal].weight == 0.0f) return;

    // 位置と方向の算出
    const auto& start_transform = avatar_[ik_controllers_[end_part - 2].index].transform;
    const auto& joint_transform = avatar_[ik_controllers_[end_part - 1].index].transform;
    const auto& end_transform = avatar_[ik_controllers_[end_part].index].transform;

    Matrix44& start_world = start_transform->GetCurrentWorldMatrix();
    Matrix44& joint_world = joint_transform->GetCurrentWorldMatrix(start_world);
    Matrix44& end_world = end_transform->GetCurrentWorldMatrix(joint_world);

    Vector3& start_position = Vector3(start_world.rows_[3]);
    Vector3& joint_position = Vector3(joint_world.rows_[3]);
    Vector3& end_position = Vector3(end_world.rows_[3]);

    Vector3& start_to_goal = ik_goals_[ik_goal].position - start_position;
    Vector3& start_to_joint = joint_position - start_position;
    Vector3& joint_to_end = end_position - joint_position;

    float start_to_goal_length = start_to_goal.Magnitude();
    float start_to_joint_length = start_to_joint.Magnitude();
    float joint_to_end_length = joint_to_end.Magnitude();

    // Jointの今の角度の算出
    Vector3& joint_to_start_direction = start_to_joint.Normalized() * -1.0f;
    Vector3& joint_to_end_direction = joint_to_end.Normalized();
    float dot = joint_to_start_direction.Dot(joint_to_end_direction);
    if (fabsf(dot) > 0.98f)
    {// 角度が小さすぎると正しい外積が算出できないので計算しない
        return;
    }

    float current_joint_radian = acosf(dot);

    // Jointを回転する(膝はx軸のみ回転できる)
    // Jointの目標角度の算出
    float wanted_joint_radian = 0.0f;
    if (start_to_goal_length >= start_to_joint_length + joint_to_end_length)
    {// 届かない場合計算しない
        wanted_joint_radian = kPi;
    }
    else
    {// C2 = A2 + B2 – 2ABcos(x) ==> cos(x) = (A2 + B2 - C2) / 2AB
        float x = (start_to_joint_length * start_to_joint_length
            + joint_to_end_length * joint_to_end_length
            - start_to_goal_length * start_to_goal_length)
            / (2.0f * start_to_joint_length * joint_to_end_length);
        wanted_joint_radian = acosf(x);
    }
    
    // 回転する(ウェイトをかける)
    float delta_joint_radian = (wanted_joint_radian - current_joint_radian) * ik_goals_[ik_goal].weight;

    Vector3& rotate_axis = (joint_to_start_direction * joint_to_end_direction).Normalized();
    joint_world.RemoveScale();
    rotate_axis = Vector3::TransformNormal(rotate_axis, joint_world.Transpose()).Normalized();
    Quaternion& joint_rotation = Quaternion::RotateAxis(rotate_axis, delta_joint_radian);
    joint_transform->SetRotation(joint_transform->GetRotation() * joint_rotation);

    // Startを回転する
    // 新しいEndの算出
    joint_world = joint_transform->GetCurrentWorldMatrix(start_world);
    end_world = end_transform->GetCurrentWorldMatrix(joint_world);
    end_position = Vector3(end_world.rows_[3]);

    // 回転の算出
    Vector3& start_to_end = (end_position - start_position).Normalized();
    start_to_goal.Normalize();
    dot = start_to_end.Dot(start_to_goal);
    if (fabsf(dot) > 0.98f)
    {// 角度が小さすぎると正しい外積が算出できないので計算しない
        return;
    }
    rotate_axis = (start_to_end * start_to_goal).Normalized();
    start_world.RemoveScale();
    rotate_axis = Vector3::TransformNormal(rotate_axis, start_world.Transpose()).Normalized();

    // ウェイトをかける
    float delta_start_radian = acosf(dot) * ik_goals_[ik_goal].weight;

    // 回転する
    Quaternion& start_rotation = Quaternion::RotateAxis(rotate_axis, delta_start_radian);
    start_transform->SetRotation(start_transform->GetRotation() * start_rotation);

    // 足元の回転
    start_world = start_transform->GetCurrentWorldMatrix();
    joint_world = joint_transform->GetCurrentWorldMatrix(start_world);
    end_world = end_transform->GetCurrentWorldMatrix(joint_world);
    Vector3& goal_up = Vector3::TransformNormal(ik_goals_[ik_goal].up, end_world.Transpose()).Normalized();
    Vector3& end_up = end_transform->GetUp();
    Quaternion& rotation = Quaternion::FromToRotation(end_up, goal_up, ik_goals_[ik_goal].weight);
    end_transform->SetRotation(end_transform->GetRotation()
        * rotation * (ik_foot_rotation_offset_ * ik_goals_[ik_goal].weight).ToQuaternion());
    end_up = end_transform->GetUp();
}

//--------------------------------------------------------------------------------
//  ik計算(cyclic coordinate decent)
//--------------------------------------------------------------------------------
//void Animator::ComputeIK(const IKParts& end_part, const IKGoals& ik_goal, const int parent_number)
//{
//    if (ik_goals[ik_goal].weight == 0.0f) return;
//    Transform* end_part_transform = avatar_[ik_controllers_[end_part].index].transform;
//
//    for (int count = 0, count_parent = 0; count < ik_loop_max_; ++count)
//    {
//        // 目標に限界まで近づくとループを終了する
//        Vector3& end_position = end_part_transform->GetCurrentWorldPosition();
//        if (Vector3::SquareDistanceBetween(end_position, ik_goals[ik_goal].position) <= ik_position_threshold_ * ik_position_threshold_)
//        {
//            break;
//        }
//
//        int current_part = end_part - (count_parent + 1); // 1からparent_numberまで
//        int current_end = current_part + 1;
//        Transform* current_part_transform = avatar_[ik_controllers_[current_part].index].transform;
//        Transform* current_end_transform = avatar_[ik_controllers_[current_end].index].transform;
//        Matrix44& current_part_world = current_part_transform->GetCurrentWorldMatrix();
//        current_part_world.RemoveScale();
//
//        // 先への方向
//        Vector3& part_to_end_local = current_end_transform->GetPosition().Normalized();
//
//        // 目標への方向の算出
//        Vector3& goal_local = Vector3::TransformInverse(ik_goals[ik_goal].position, current_part_world);
//        Vector3& part_to_goal_local = goal_local.Normalized();
//        
//        // 回転角度の算出
//        float dot = part_to_end_local.Dot(part_to_goal_local);
//        float delta_radian = acosf(Math::Clamp(dot, -1.0f, 1.0f));
//        if (std::isnan(delta_radian) ||
//            dot >= 1.0f - 0.01f)
//        {
//            // 角度小さすぎるの場合やめる
//            count_parent = (count_parent + 1) % parent_number; // 0からparent_number - 1まで
//            continue;
//        }
//
//        // 回転軸の算出
//        Vector3& rotate_axis = (part_to_end_local * part_to_goal_local).Normalized();
//
//        // 回転quaternionの算出
//        Quaternion& rotation = Quaternion::RotateAxis(rotate_axis, delta_radian);
//
//        // 回転Eulerの算出
//        Vector3& rotation_euler = Vector3::AxisRadianToEuler(rotate_axis, delta_radian);
//        
//        //Quaternion& qtest = Vector3(1.0f, 0.3f, 0.5f).ToQuaternion();
//        //Vector3& vtest = qtest.ToEuler();
//        
//        // 回転制限
//        rotation_euler = Math::Clamp(rotation_euler, ik_controllers_[current_part].rotation_limit_min, ik_controllers_[current_part].rotation_limit_max);
//
//        // ウェイト
//        rotation_euler *= ik_goals[ik_goal].weight;
//
//        // 回転する
//        current_part_transform->SetRotation(current_part_transform->GetRotation() * rotation_euler.ToQuaternion());
//
//        count_parent = (count_parent + 1) % parent_number; // 0からparent_number - 1まで
//    }
//    
//}

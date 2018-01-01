//--------------------------------------------------------------------------------
//
//�@animatorComponent.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "animator.h"
#include "game_object.h"
#include "game_object_spawner.h"
#include "transform.h"
#include "motion_data.h"
#include "main_system.h"
#include "motion_manager.h"
#include "texture_manager.h"
#include "time.h"
#include "collision_detector.h"
#include "collision_system.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
Animator::Animator(GameObject& owner)
	: Component(owner)
	, state_(nullptr)
	, is_grounded_(false)
	, is_light_attack_(false)
    , is_strong_attack_(false)
	, is_jump_(false)
	, is_damaged_(false)
	, is_rise_up_(false)
	, is_skill_(false)
	, is_skill_over_(false)
	, is_dead_(false)
	, is_stun_(false)
	, is_ultra_(false)
    , enable_ik_(true)
	, movement_(0.0f)
	, time_counter_(0.0f)
    , ik_ray_distance_(0.5f)
{
    ZeroMemory(ik_controllers_, sizeof(IKController) * kIKMax);
	bone_texture_.size = 0;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	bone_texture_.pointer = nullptr;
#endif
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool Animator::Init(void)
{
    InitIK();
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void Animator::Uninit(void)
{
	avatar_.clear();
	SAFE_DELETE(state_);

	auto motion_manager = MainSystem::Instance()->GetMotionManager();
	size_t motion_number = motion_names_.size();
	for (size_t count = 0; count < motion_number; ++count)
	{
		motion_manager->Disuse(motion_names_[count]);
	}
	motion_names_.clear();
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	SAFE_RELEASE(bone_texture_.pointer);
#endif
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void Animator::Update(void)
{
	if (avatar_.empty()) return;
	state_->Update(*this);
	time_counter_ += Time::Instance()->ScaledDeltaTime();
    UpdateIK();
}

//--------------------------------------------------------------------------------
//  ��X�V����
//--------------------------------------------------------------------------------
void Animator::LateUpdate(void)
{
    UpdateBoneTexture();
}

//--------------------------------------------------------------------------------
//	���̃A�j���[�V��������Ԃ�
//--------------------------------------------------------------------------------
const String& Animator::GetCurrentAnimationName(void)
{
	return state_->GetCurrentMotionName();
}

//--------------------------------------------------------------------------------
//  �A�o�^�[�̐ݒ�
//--------------------------------------------------------------------------------
void Animator::SetAvatar(const String& file_name)
{
	if (!avatar_.empty()) return;

	LoadFromFile(file_name);
	AttachBonesToChildren();

	// texture�̃T�C�Y�̎Z�o�i2�̙p��j
	int bone_number = static_cast<int>(avatar_.size());
	bone_texture_.size = 2;
	while (bone_texture_.size < bone_number)
	{
		bone_texture_.size *= 2;
	}

	// texture�̐���
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	bone_texture_.pointer = MainSystem::Instance()->GetTextureManager()->CreateEmptyTexture(bone_texture_.size);
#endif
}

//--------------------------------------------------------------------------------
//	���[�V�����X�e�[�^�X�̐؂�ւ�
//--------------------------------------------------------------------------------
void Animator::Change(MotionState* new_motion_state)
{
	SAFE_DELETE(state_);
	state_ = new_motion_state;
	time_counter_ = 0.0f;
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �t�@�C������ǂݍ���
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
	auto motion_manager = MainSystem::Instance()->GetMotionManager();
	size_t motion_number;
	archive.loadBinary(&motion_number, sizeof(motion_number));
	motion_names_.resize(motion_number);
	for (size_t count = 0; count < motion_number; ++count)
	{
		size_t name_size;
		archive.loadBinary(&name_size, sizeof(name_size));
		string name;
		name.resize(name_size);
		archive.loadBinary(&name[0], name_size);
		motion_names_[count] = String(name.begin(), name.end());
		motion_manager->Use(motion_names_[count]);
	}
	file.close();
}

//--------------------------------------------------------------------------------
//  �{�[���Ǝq�����֘A�t����
//--------------------------------------------------------------------------------
void Animator::AttachBonesToChildren(void)
{
	for (auto& bone : avatar_)
	{
		bone.transform = owner_.GetTransform()->FindChildBy(bone.name);
	}
}

//--------------------------------------------------------------------------------
//  �e�N�X�`���̃T�[�t�F�C�X�Ƀ{�[���}�g���N�X����������
//--------------------------------------------------------------------------------
void Animator::UpdateBoneTexture(void)
{
	if (!bone_texture_.pointer) return;

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	// �T�[�t�F�C�X���b�N
	D3DLOCKED_RECT lock_rect;
	bone_texture_.pointer->LockRect(0, &lock_rect, NULL, D3DLOCK_DISCARD);

	// �e�N�X�`���T�[�t�F�C�X�̏�����
	FillMemory(lock_rect.pBits, lock_rect.Pitch * bone_texture_.size, 0);

	// �����̏������݁A��s����
	size_t bone_number = avatar_.size();
	int offset = lock_rect.Pitch / sizeof(Matrix44);
	for (size_t count = 0; count < bone_number; ++count)
	{
		const Matrix44& bone_world = avatar_[count].bind_pose_inverse * avatar_[count].transform->GetWorldMatrix();
		memcpy((Matrix44*)lock_rect.pBits + offset * count, &bone_world, sizeof(bone_world));
	}

	// �T�[�t�F�C�X�A�����b�N
	bone_texture_.pointer->UnlockRect(0);
#endif
}

//--------------------------------------------------------------------------------
//  �C���o�[�X�L�l�}�e�B�N�X(IK)�̏�����
//--------------------------------------------------------------------------------
void Animator::InitIK(void)
{
    // TODO : �ʂ̉�]�����̐ݒ�
    for (auto& ik_controller : ik_controllers_)
    {
        ik_controller.rotation_limit_min = Vector3(-kPi);
        ik_controller.rotation_limit_max = Vector3(kPi);
    }
}

//--------------------------------------------------------------------------------
//  �C���o�[�X�L�l�}�e�B�N�X(IK)�v�Z
//--------------------------------------------------------------------------------
void Animator::UpdateIK(void)
{
    if (!enable_ik_) return;
    UpdateFootIK();
}

//--------------------------------------------------------------------------------
//  foot ik�̌v�Z
//--------------------------------------------------------------------------------
void Animator::UpdateFootIK(void)
{
    // IK goal
    ComputeIKGoal(IKParts::kFootLeft, IKGoals::kIKGoalLeftFoot);
    ComputeIKGoal(IKParts::kFootRight, IKGoals::kIKGoalRightFoot);

    // Compute IK
    ComputeIK(IKParts::kToesLeft, IKGoals::kIKGoalLeftFoot);
    ComputeIK(IKParts::kToesRight, IKGoals::kIKGoalRightFoot);
}

//--------------------------------------------------------------------------------
//  ik goal�̌v�Z
//--------------------------------------------------------------------------------
void Animator::ComputeIKGoal(const IKParts& goal_part, const IKGoals& ik_goal)
{
    auto my_transform = owner_.GetTransform();
    const Vector3& current_position = avatar_[ik_controllers_[goal_part].index].transform->GetCurrentWorldPosition();
    float ik_weight_interpolation_sign = 0.0f;

    auto collision_system = MainSystem::Instance()->GetCollisionSystem();
    Ray ray(current_position, Vector3::kDown);

    // �������ɒ��n�_�𔻒肷��
    RayHitInfo* info = collision_system->RayCast(ray, ik_ray_distance_, &owner_);
    if (info)
    {
        ik_goals[ik_goal].position = info->position;
        ik_goals[ik_goal].rotation = Quaternion::FromToRotation(my_transform->GetUp(), info->normal) * my_transform->GetRotation();
        ik_weight_interpolation_sign = 1.0f;
    }

    // ������ɒ��n�_�𔻒肷��
    // �������Ō�����Ȃ�������߂荞�܂Ȃ��悤�ɏ���������肷��
    //if (ik_weight_interpolation_sign != 1.0f)
    //{
    //    ray.direction_ = Vector3::kUp;
    //    info = collision_system->RayCast(ray, ik_ray_distance_, &owner_);
    //    if (info)
    //    {
    //        ik_goals[ik_goal].position = info->position;
    //        ik_goals[ik_goal].rotation = Quaternion::FromToRotation(my_transform->GetUp(), info->normal) * my_transform->GetRotation();
    //        ik_weight_interpolation_sign = 1.0f;
    //    }
    //}

    // Weight�X�V
    ik_goals[ik_goal].position_weight = ik_weight_interpolation_sign;
    ik_goals[ik_goal].rotation_weight = ik_weight_interpolation_sign;
}

//--------------------------------------------------------------------------------
//  ik�v�Z
//--------------------------------------------------------------------------------
void Animator::ComputeIK(const IKParts& end_part, const IKGoals& ik_goal, const int parent_number)
{
    if (ik_goals[ik_goal].position_weight == 0.0f) return;
    Transform* end_part_transform = avatar_[ik_controllers_[end_part].index].transform;

    for (int count = 0, count_parent = 0; count < kIkLoopsMax; ++count)
    {
        int current_part = end_part - (count_parent + 1); // 1����parent_number�܂�
        int current_end = current_part + 1;
        Transform* current_part_transform = avatar_[ik_controllers_[current_part].index].transform;
        Transform* current_end_transform = avatar_[ik_controllers_[current_end].index].transform;
        Matrix44& current_part_world = current_part_transform->GetCurrentWorldMatrix();
        
        // �����̑O�����ƖڕW�����̕����̎Z�o
        Vector3& part_to_end = Vector3::TransformNormal(current_end_transform->GetPosition().Normalized(), current_part_world).Normalized();
        Vector3& part_to_goal = (ik_goals[ik_goal].position - Vector3(current_part_world.m30_, current_part_world.m31_, current_part_world.m32_)).Normalized();
        
        // ��]�p�x�̎Z�o
        Vector3& rotation_euler = Vector3::EulerBetween(part_to_end, part_to_goal);
        
        // ��]����
        rotation_euler = Math::Clamp(rotation_euler, ik_controllers_[current_part].rotation_limit_min, ik_controllers_[current_part].rotation_limit_max);

        // �E�F�C�g
        rotation_euler *= ik_goals[ik_goal].position_weight;

        // ��]����
        current_part_transform->SetRotation(current_part_transform->GetRotation() * rotation_euler.ToQuaternion());

        // �ڕW�Ɍ��E�܂ŋ߂Â��ƃ��[�v���I������
        //if (Vector3::SquareDistanceBetween(end_part_transform->GetCurrentWorldPosition(), ik_goals[ik_goal].position) <= kIkPositionThreshold * kIkPositionThreshold)
        //{
        //    break;
        //}
        //
        count_parent = (count_parent + 1) % parent_number; // 0����parent_number - 1�܂�
    }
}

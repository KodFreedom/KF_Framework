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
#include "motion_state.h"
#include "main_system.h"
#include "motion_manager.h"
#include "texture_manager.h"

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
	, is_attack_(false)
	, is_jump_(false)
	, is_damaged_(false)
	, movement_(0.0f)
{
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
		Matrix44 bone_world = avatar_[count].bind_pose_inverse * avatar_[count].transform->GetWorldMatrix();
		memcpy((Matrix44*)lock_rect.pBits + offset * count, &bone_world, sizeof(bone_world));
	}

	// �T�[�t�F�C�X�A�����b�N
	bone_texture_.pointer->UnlockRect(0);
#endif
}
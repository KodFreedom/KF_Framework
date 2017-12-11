//--------------------------------------------------------------------------------
//
//�@animator.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"
#include "component.h"
#include "motion_state.h"
//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class Transform;

//--------------------------------------------------------------------------------
//  �񋓌^��`
//--------------------------------------------------------------------------------
enum IKParts
{
	kHead,
	kNeck,
	kShoulderLeft,
	kUpperArmLeft,
	kLowerArmLeft,
	kHandLeft,
	kShoulderRight,
	kUpperArmRight,
	kLowerArmRight,
	kHandRight,
	kUpperLegLeft,
	kLowerLegLeft,
	kFootLeft,
	kToesLeft,
	kUpperLegRight,
	kLowerLegRight,
	kFootRight,
	kToesRight,
	kIKMax
};

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
struct BoneTexture
{// ���̏����e�N�X�`���ɏ�������ŃV�F�[�_�ɓn��
	int size;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	LPDIRECT3DTEXTURE9 pointer;
#endif
};

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class Animator : public Component
{
public:
	//--------------------------------------------------------------------------------
	//  constructors for singleton
	//--------------------------------------------------------------------------------
	Animator(GameObject& owner);
	~Animator() {}

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
	void Update(void);

	//--------------------------------------------------------------------------------
	//  ��X�V����
	//--------------------------------------------------------------------------------
	void LateUpdate(void)
	{
		UpdateBoneTexture();
	}

	//--------------------------------------------------------------------------------
	//  ���̃A�j���[�V�����̖��O�̎擾
	//--------------------------------------------------------------------------------
	const String& GetCurrentAnimationName(void);

	//--------------------------------------------------------------------------------
	//  ���̃A�j���[�V�����̃^�C�v�̎擾
	//--------------------------------------------------------------------------------
	const auto& GetCurrentAnimationStateType(void) const { return state_->GetType(); }

	//--------------------------------------------------------------------------------
	//  �Q�b�^�[
	//--------------------------------------------------------------------------------
	const auto& GetAvatar(void) const { return avatar_; }
	const bool& GetIsGrounded(void) const { return is_grounded_; }
	const bool& GetIsDead(void) const { return is_dead_; }
	const bool& GetIsDamaged(void) const { return is_damaged_; }
	const bool& GetIsJump(void) const { return is_jump_; }
	const bool& GetIsRiseUp(void) const { return is_rise_up_; }
	const bool& GetIsAttack(void) const { return is_attack_; }
	const bool& GetIsSkill(void) const { return is_skill_; }
	const bool& GetIsSkillOver(void) const { return is_skill_over_; }
	const bool& GetIsStun(void) const { return is_stun_; }
	const bool& GetIsUltra(void) const { return is_ultra_; }
	const float& GetMovement(void) const { return movement_; }
	const float& GetTimeCounter(void) const { return time_counter_; }

	//--------------------------------------------------------------------------------
	//  �Z�b�^�[
	//--------------------------------------------------------------------------------
	void SetAvatar(const String& file_name);
	void SetAttack(const bool& value) { is_attack_ = value; }
	void SetGrounded(const bool& value) { is_grounded_ = value; }
	void SetJump(const bool& value) { is_jump_ = value; }
	void SetDamaged(const bool& value) { is_damaged_ = value; }
	void SetSkill(const bool& value) { is_skill_ = value; }
	void SetSkillOver(const bool& value) { is_skill_over_ = value; }
	void SetStun(const bool& value) { is_stun_ = value; }
	void SetUltra(const bool& value) { is_ultra_ = value; }
	void SetMovement(const float& value) { movement_ = value; }

	//--------------------------------------------------------------------------------
	//  ���[�V�����X�e�[�g�̕ύX
	//--------------------------------------------------------------------------------
	void Change(MotionState* new_motion_state);

	//--------------------------------------------------------------------------------
	//  �e�N�X�`���̎擾
	//--------------------------------------------------------------------------------
	const auto& GetBoneTexture(void) const { return bone_texture_; }

private:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct Bone
	{
		Transform*  transform;
		String      name;
		Matrix44    bind_pose_inverse;
	};

	//--------------------------------------------------------------------------------
	//  �t�@�C������ǂݍ���
	//--------------------------------------------------------------------------------
	void LoadFromFile(const String& file_name);

	//--------------------------------------------------------------------------------
	//  �{�[���Ǝq�����֘A�t����
	//--------------------------------------------------------------------------------
	void AttachBonesToChildren(void);

	//--------------------------------------------------------------------------------
	//  �e�N�X�`���̃T�[�t�F�C�X�Ƀ{�[���}�g���N�X����������
	//--------------------------------------------------------------------------------
	void UpdateBoneTexture(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	vector<Bone>   avatar_;
	vector<String> motion_names_;
	MotionState*   state_;
	bool           is_grounded_;
	bool           is_attack_;
	bool           is_jump_;
	bool           is_damaged_;
	bool           is_dead_;
	bool           is_rise_up_;
	bool           is_skill_;
	bool           is_skill_over_;
	bool           is_stun_;
	bool           is_ultra_;
	float          movement_;
	float          time_counter_;
	BoneTexture    bone_texture_;
	int            ik_controllers_[kIKMax];
};
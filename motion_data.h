//--------------------------------------------------------------------------------
//�@motion_data.h
//	���[�V�����f�[�^�N���X
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  BoneTransform�N���X
//--------------------------------------------------------------------------------
class BoneTransform
{
public:
	BoneTransform() : translation_(Vector3::kZero)
		, rotation_(Quaternion::kIdentity), scale_(Vector3::kOne) {}
	~BoneTransform() {}
	Vector3    translation_;
	Quaternion rotation_;
	Vector3    scale_;
};

//--------------------------------------------------------------------------------
//  Frame�N���X
//	���[�V�����̈�t���[���i�S�Ă�Bone���܂߂�j
//--------------------------------------------------------------------------------
class Frame
{
public:
	Frame() {}
	~Frame() { bone_transforms_.clear(); }
	vector<BoneTransform> bone_transforms_;
};

//--------------------------------------------------------------------------------
//  MotionData�N���X
//--------------------------------------------------------------------------------
class MotionData
{
public:
	MotionData(void) : is_loop_(false) {}
	~MotionData() { frames_.clear(); }
	bool          is_loop_;
	vector<Frame> frames_;
};
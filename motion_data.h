//--------------------------------------------------------------------------------
//　motion_data.h
//	モーションデータクラス
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  BoneTransformクラス
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
//  Frameクラス
//	モーションの一フレーム（全てのBoneを含める）
//--------------------------------------------------------------------------------
class Frame
{
public:
	Frame() {}
	~Frame() { bone_transforms_.clear(); }
	vector<BoneTransform> bone_transforms_;
};

//--------------------------------------------------------------------------------
//  MotionDataクラス
//--------------------------------------------------------------------------------
class MotionData
{
public:
	MotionData(void) : is_loop_(false) {}
	~MotionData() { frames_.clear(); }
	bool          is_loop_;
	vector<Frame> frames_;
};
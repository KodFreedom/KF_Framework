//--------------------------------------------------------------------------------
//
//�@motionInfo.h
//	Author : Xu Wenjie
//	Date   : 2017-07-06
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  BoneFrame�N���X
//--------------------------------------------------------------------------------
class BoneFrame
{
public:
	BoneFrame(const Vector3& position = Vector3::Zero, const Quaternion& rotation = Quaternion::Identity)
		: Position(position), Rotation(rotation) {}
	~BoneFrame() {}

	Vector3		Position;
	Quaternion	Rotation;
};

//--------------------------------------------------------------------------------
//  MotionFrame�N���X
//	���[�V�����̈�t���[���i�S�Ă�Bone���܂߂�j
//--------------------------------------------------------------------------------
class MotionFrame
{
public:
	MotionFrame(void) { BoneFrames.clear(); }
	~MotionFrame() { BoneFrames.clear(); }

	list<BoneFrame>	BoneFrames;
};

//--------------------------------------------------------------------------------
//  MotionInfo�N���X
//--------------------------------------------------------------------------------
class MotionInfo
{
public:
	MotionInfo(void)
		: IsLoop(false)
	{
		MotionFrames.clear();
	}
	~MotionInfo() { MotionFrames.clear(); }

	bool				IsLoop;
	vector<MotionFrame>	MotionFrames;
};
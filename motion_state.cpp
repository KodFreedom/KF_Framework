//--------------------------------------------------------------------------------
//　motion_state.cpp
//  classes for motion state
//	モーションステートのクラス
//  用于动作状态机的类
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "main.h"
#include "motion_state.h"
#include "motionManager.h"
#include "animator.h"
#include "motionInfo.h"
#include "gameObject.h"

//--------------------------------------------------------------------------------
//
//  MotionState
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  constructor / コンストラクタ / 构造函数
//--------------------------------------------------------------------------------
MotionState::MotionState(const String& motion_name, const int start_frame)
	: current_motion_name_(motion_name), current_motion_frame_counter_(start_frame)
{
	current_motion_info_ = MotionManager::Instance()->GetMotionInfoBy(current_motion_name_);
}

//--------------------------------------------------------------------------------
//
//  NormalMotionState
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  モーションの更新 / 更新动作
//--------------------------------------------------------------------------------
void NormalMotionState::UpdateMotion(Animator& animator)
{
	assert(current_motion_info_);
	assert(animator.bones.size() <= current_motion_info_->MotionFrames[current_motion_frame_counter_].BoneFrames.size());
	
	auto frameNumber = (int)current_motion_info_->MotionFrames.size();
	auto iterator = current_motion_info_->MotionFrames[current_motion_frame_counter_].BoneFrames.begin();

	for (auto born : animator.bones)
	{
		born->GetTransform()->SetNextMatrix(iterator->Matrix);
		++iterator;
	}

	if (++current_motion_frame_counter_ == frameNumber)
	{
		if (!current_motion_info_->IsLoop)
		{
			--current_motion_frame_counter_;
		}
		else current_motion_frame_counter_ = 0;
	}
}

//--------------------------------------------------------------------------------
//
//  BlendMotionState
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  constructor / コンストラクタ / 构造函数
//--------------------------------------------------------------------------------
BlendMotionState::BlendMotionState(const String& current_motion_name, const String& next_motion_name, const int current_motion_start_frame, const int blend_frame_number)
	: MotionState(current_motion_name, current_motion_start_frame)
	, next_motion_frame_counter_(0)
	, next_motion_name_(next_motion_name)
	, blend_frame_counter_(0)
	, blend_frame_number_(blend_frame_number)
{
	next_motion_info_ = MotionManager::Instance()->GetMotionInfoBy(next_motion_name_);
}

//--------------------------------------------------------------------------------
//  モーションの更新 / 更新动作
//--------------------------------------------------------------------------------
void BlendMotionState::UpdateMotion(Animator& animator)
{
	assert(current_motion_info_);
	assert(next_motion_info_);
	assert(animator.bones.size() <= current_motion_info_->MotionFrames[current_motion_frame_counter_].BoneFrames.size());
	assert(animator.bones.size() <= next_motion_info_->MotionFrames[current_motion_frame_counter_].BoneFrames.size());

	auto frameNumber = (int)current_motion_info_->MotionFrames.size();
	auto currentBoneFrameIterator = current_motion_info_->MotionFrames[current_motion_frame_counter_].BoneFrames.begin();

	auto nextMotionFrameNumber = next_motion_info_->MotionFrames.size();
	auto nextMotionCurrentBoneFrameIterator = next_motion_info_->MotionFrames[current_motion_frame_counter_].BoneFrames.begin();
	auto blendRate = (float)blend_frame_counter_ / blend_frame_number_;

	for (auto born : animator.bones)
	{
		const auto& currentMotionMatrix = currentBoneFrameIterator->Matrix;
		const auto& nextMotionMatrix = nextMotionCurrentBoneFrameIterator->Matrix;;
		const auto& blendMatrix = currentMotionMatrix * (1.0f - blendRate) + nextMotionMatrix * blendRate;
		born->GetTransform()->SetNextMatrix(blendMatrix);
		++currentBoneFrameIterator;
		++nextMotionCurrentBoneFrameIterator;
	}

	if (++current_motion_frame_counter_ == frameNumber)
	{
		if (!current_motion_info_->IsLoop) --current_motion_frame_counter_;
		else current_motion_frame_counter_ = 0;
	}

	if (++next_motion_frame_counter_ == nextMotionFrameNumber)
	{
		if (!next_motion_info_->IsLoop) --next_motion_frame_counter_;
		else next_motion_frame_counter_ = 0;
	}

	++blend_frame_counter_;
}

//--------------------------------------------------------------------------------
//  モーションの切り替え / 切换动作
//--------------------------------------------------------------------------------
void BlendMotionState::ChangeMotion(Animator& animator)
{
	if (blend_frame_counter_ < blend_frame_number_) return;

	// TODO: change motion
}

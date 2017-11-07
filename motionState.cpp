//--------------------------------------------------------------------------------
//	
//　motionState.cpp
//	Author : Xu Wenjie
//	Date   : 2017-09-20
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "motionState.h"
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
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	コンストラクタ
//--------------------------------------------------------------------------------
MotionState::MotionState(const string& motionName, const int currentFrame)
	: currentFrame(currentFrame), currentMotionName(motionName)
{
	currentMotionInfo = MotionManager::Instance()->GetMotionInfoBy(currentMotionName);
}

//--------------------------------------------------------------------------------
//
//  NormalMotionState
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void NormalMotionState::Update(Animator& animator)
{
	assert(currentMotionInfo);
	assert(animator.bones.size() <= currentMotionInfo->MotionFrames[currentFrame].BoneFrames.size());
	
	auto frameNumber = (int)currentMotionInfo->MotionFrames.size();
	auto iterator = currentMotionInfo->MotionFrames[currentFrame].BoneFrames.begin();

	for (auto born : animator.bones)
	{
		auto transform = born->GetTransform();
		transform->SetNextPosition(iterator->Position);
		transform->SetNextRotation(iterator->Rotation);
		++iterator;
	}

	if (++currentFrame == frameNumber)
	{
		if (!currentMotionInfo->IsLoop)
		{
			changeState(animator);
			--currentFrame;
		}
		else currentFrame = 0;
	}
}

//--------------------------------------------------------------------------------
//
//  BlendMotionState
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	コンストラクタ
//--------------------------------------------------------------------------------
BlendMotionState::BlendMotionState(const string& currentMotionName, const string& nextMotionName, const int currentFrame, const int blendFrameNumber)
	: MotionState(currentMotionName, currentFrame)
	, currentNextMotionFrame(0)
	, currentBlendFrame(0)
	, blendFrameNumber(blendFrameNumber)
{
	nextMotionInfo = MotionManager::Instance()->GetMotionInfoBy(nextMotionName);
}

//--------------------------------------------------------------------------------
//  Update
//--------------------------------------------------------------------------------
void BlendMotionState::Update(Animator& animator)
{
	assert(currentMotionInfo);
	assert(nextMotionInfo);
	assert(animator.bones.size() <= currentMotionInfo->MotionFrames[currentFrame].BoneFrames.size());
	assert(animator.bones.size() <= nextMotionInfo->MotionFrames[currentFrame].BoneFrames.size());

	auto frameNumber = (int)currentMotionInfo->MotionFrames.size();
	auto currentBoneFrameIterator = currentMotionInfo->MotionFrames[currentFrame].BoneFrames.begin();

	auto nextMotionFrameNumber = nextMotionInfo->MotionFrames.size();
	auto nextMotionCurrentBoneFrameIterator = nextMotionInfo->MotionFrames[currentFrame].BoneFrames.begin();
	auto blendRate = (float)currentBlendFrame / blendFrameNumber;

	for (auto born : animator.bones)
	{
		auto currentMotionPosition = currentBoneFrameIterator->Position;
		auto currentMotionRotation = currentBoneFrameIterator->Rotation;
		
		auto nextMotionPosition = nextMotionCurrentBoneFrameIterator->Position;
		auto nextMotionRotation = nextMotionCurrentBoneFrameIterator->Rotation;
		
		auto blendPosition = currentMotionPosition * (1.0f - blendRate) + nextMotionPosition * blendRate;
		auto blendRotation = currentMotionRotation * (1.0f - blendRate) + nextMotionRotation * blendRate;
		
		auto transform = born->GetTransform();
		transform->SetNextPosition(blendPosition);
		transform->SetNextRotation(blendRotation);
		++currentBoneFrameIterator;
		++nextMotionCurrentBoneFrameIterator;
	}

	if (++currentFrame == frameNumber)
	{
		if (!currentMotionInfo->IsLoop) --currentFrame;
		else currentFrame = 0;
	}

	if (++currentNextMotionFrame == nextMotionFrameNumber)
	{
		if (!nextMotionInfo->IsLoop) --currentNextMotionFrame;
		else currentNextMotionFrame = 0;
	}

	if (++currentBlendFrame == blendFrameNumber)
	{
		changeState(animator);
	}
}
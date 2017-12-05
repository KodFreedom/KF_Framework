﻿//--------------------------------------------------------------------------------
//　motion_state.h
//  classes for motion state
//	モーションステートのクラス
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"

//--------------------------------------------------------------------------------
//  forward declaration / 前方宣言
//--------------------------------------------------------------------------------
class Animator;
class MotionData;

//--------------------------------------------------------------------------------
//  enum / 列挙型定義
//--------------------------------------------------------------------------------
enum MotionStateType
{
	kNormalMotionState,
	kBlendMotionState
};

//--------------------------------------------------------------------------------
//  motion state super class / モーションステート基底クラス
//--------------------------------------------------------------------------------
class MotionState
{
public:
	//--------------------------------------------------------------------------------
	//  constructor / コンストラクタ
	//  motion_name : 今のモーション名
	//  start_frame : モーションの始めるフレーム
	//--------------------------------------------------------------------------------
	MotionState(const String& motion_name, const int start_frame, const MotionStateType& type);
	~MotionState() {};

	//--------------------------------------------------------------------------------
	//  update motion and check if we need change the motion
	//  モーションの更新と切り替えのチェック
	//  called by animator per frame / アニメーターから呼ばれる
	//  animator : アニメーター
	//--------------------------------------------------------------------------------
	void Update(Animator& animator)
	{
		UpdateMotion(animator);
		ChangeMotion(animator);
	}

	//--------------------------------------------------------------------------------
	//  今のモーション名を返す
	//  return：const string&
	//--------------------------------------------------------------------------------
	const String& GetCurrentMotionName(void) const 
	{
		return current_motion_name_; 
	}

	//--------------------------------------------------------------------------------
	//  今のモーションタイプを返す
	//  return：const MotionStateType&
	//--------------------------------------------------------------------------------
	const auto& GetType(void) const
	{
		return type_;
	}

	//--------------------------------------------------------------------------------
	//  モーションの始めるフレームを設定する
	//  current_motion_frame_counter：今のモーションのフレームカウンタ / 现在的动作的帧计数器
	//--------------------------------------------------------------------------------
	void Set(const int current_motion_frame_counter) 
	{ 
		current_frame_counter_ = current_motion_frame_counter;
	}

protected:
	//--------------------------------------------------------------------------------
	//  モーションの更新
	//  animator : アニメーター
	//--------------------------------------------------------------------------------
	virtual void UpdateMotion(Animator& animator) = 0;

	//--------------------------------------------------------------------------------
	//  モーションの切り替え
	//  animator : アニメーター
	//--------------------------------------------------------------------------------
	virtual void ChangeMotion(Animator& animator) = 0;

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	MotionData* current_motion_data_;
	int current_frame_counter_;
	const String current_motion_name_;
	const MotionStateType type_;
};

//--------------------------------------------------------------------------------
//  NormalMotionState
//  update one motion only / 一つのモーションを更新する
//--------------------------------------------------------------------------------
class NormalMotionState : public MotionState
{
public:
	//--------------------------------------------------------------------------------
	//  constructor / コンストラクタ
	//  motion_name : 今のモーション名
	//  start_frame : モーションの始めるフレーム
	//--------------------------------------------------------------------------------
	NormalMotionState(const String& motion_name, const int start_frame)
		: MotionState(motion_name, start_frame, kNormalMotionState) {}
	~NormalMotionState() {}

protected:
	//--------------------------------------------------------------------------------
	//  モーションの更新
	//  animator : アニメーター
	//--------------------------------------------------------------------------------
	void UpdateMotion(Animator& animator) override;

	//--------------------------------------------------------------------------------
	//  モーションの切り替え
	//  animator : アニメーター
	//--------------------------------------------------------------------------------
	virtual void ChangeMotion(Animator& animator) override = 0;
};

//--------------------------------------------------------------------------------
//  BlendMotionState
//  motionblend / モーションブレンド
//--------------------------------------------------------------------------------
class BlendMotionState : public MotionState
{
public:
	//--------------------------------------------------------------------------------
	//  constructor / コンストラクタ
	//  current_motion_name : 今のモーション名
	//  next_motion_pointer : 次のモーションのポインタ
	//  start_frame : モーションの始めるフレーム
	//  blend_frame_number : モーションブレンドのフレーム数
	//--------------------------------------------------------------------------------
	BlendMotionState(const String& current_motion_name, NormalMotionState* next_motion_pointer, const int current_motion_start_frame, const int blend_frame_number = 10);
	~BlendMotionState() {}

protected:
	//--------------------------------------------------------------------------------
	//  モーションの更新
	//  animator : アニメーター
	//--------------------------------------------------------------------------------
	void UpdateMotion(Animator& animator) override;

	//--------------------------------------------------------------------------------
	//  モーションの切り替え
	//  animator : アニメーター
	//--------------------------------------------------------------------------------
	void ChangeMotion(Animator& animator) override;

	//--------------------------------------------------------------------------------
	//  次のモーションのフレームカウンタ
	//--------------------------------------------------------------------------------
	int next_frame_counter_;

	//--------------------------------------------------------------------------------
	//  モーションブレンドのフレームカウンタ
	//--------------------------------------------------------------------------------
	int	blend_frame_counter_;

	//--------------------------------------------------------------------------------
	//  モーションブレンドのフレーム数
	//--------------------------------------------------------------------------------
	const int blend_frame_number_;

	//--------------------------------------------------------------------------------
	//  次のモーションデータ
	//--------------------------------------------------------------------------------
	MotionData* next_motion_data_;

	//--------------------------------------------------------------------------------
	//  次のモーションのポインタ
	//--------------------------------------------------------------------------------
	NormalMotionState* next_motion_pointer_;
};
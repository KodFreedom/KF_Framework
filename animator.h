//--------------------------------------------------------------------------------
//
//　animatorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "component.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class MotionState;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class Animator : public Component
{
	//--------------------------------------------------------------------------------
	//  フレンドクラス
	//--------------------------------------------------------------------------------
	friend class NormalMotionState;
	friend class BlendMotionState;

public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	Animator(GameObject* const owner, const string& filePath);
	~Animator() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void);

	//Set関数
	void	SetAttack(const bool& value) { isAttack = value; }
	void	SetGrounded(const bool& value) { isGrounded = value; }
	void	SetJump(const bool& value) { isJump = value; }
	void	SetMovement(const float& value) { movement = value; }

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void			analyzeFile(const string& filePath);
	//void			updateAttack(void);
	void			changeMotionStatus(MotionState* motionStatus);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	vector<GameObject*>		bones;
	MotionState*			curentMotionState;
	bool					isGrounded;
	bool					isAttack;
	bool					isJump;
	float					movement;
};

//--------------------------------------------------------------------------------
//
//�@animatorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "component.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class MotionState;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class Animator : public Component
{
	//--------------------------------------------------------------------------------
	//  �t�����h�N���X
	//--------------------------------------------------------------------------------
	friend class NormalMotionState;
	friend class BlendMotionState;

public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	Animator(GameObject* const owner, const string& filePath);
	~Animator() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void);

	//Set�֐�
	void	SetAttack(const bool& value) { isAttack = value; }
	void	SetGrounded(const bool& value) { isGrounded = value; }
	void	SetJump(const bool& value) { isJump = value; }
	void	SetMovement(const float& value) { movement = value; }

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void			analyzeFile(const string& filePath);
	//void			updateAttack(void);
	void			changeMotionStatus(MotionState* motionStatus);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	vector<GameObject*>		bones;
	MotionState*			curentMotionState;
	bool					isGrounded;
	bool					isAttack;
	bool					isJump;
	float					movement;
};

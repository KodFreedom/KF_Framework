//--------------------------------------------------------------------------------
//	���ԃr�w�C�r�A�R���|�l���g
//�@WindmillController.h
//	Author : Xu Wenjie
//	Date   : 2017-09-12
//--------------------------------------------------------------------------------
#pragma once
#include "behavior.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class WindmillController : public Behavior
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	WindmillController(GameObject& owner)
		: Behavior(owner, L"WindmillController")
		, rotate_speed_(0.01f)
		, fan_(nullptr)
	{}
	~WindmillController() {}

	bool Init(void) override;
	void Uninit(void) override {}
	void Update(void) override;
	void LateUpdate(void) override {}

private:
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	float		rotate_speed_;
	GameObject*	fan_;
};
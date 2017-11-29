//--------------------------------------------------------------------------------
//	風車ビヘイビアコンポネント
//　WindmillController.h
//	Author : Xu Wenjie
//	Date   : 2017-09-12
//--------------------------------------------------------------------------------
#pragma once
#include "behavior.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class WindmillController : public Behavior
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
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
	//  変数定義
	//--------------------------------------------------------------------------------
	float		rotate_speed_;
	GameObject*	fan_;
};
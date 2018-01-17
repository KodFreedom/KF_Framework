//--------------------------------------------------------------------------------
//
//　modeResult.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "mode.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class FlashButtonController;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class ModeResult : public Mode
{
public:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	ModeResult() : Mode(L"Result"), time_counter_(0.0f), flash_button_controller_(nullptr) {}
	~ModeResult() {}

	void Init(void) override;
    void Uninit(void) override;
	void Update(void) override;

private:
    //--------------------------------------------------------------------------------
    //  定数宣言
    //--------------------------------------------------------------------------------
    static constexpr float kWaitTime = 1.0f;

    //--------------------------------------------------------------------------------
    //  変数宣言
    //--------------------------------------------------------------------------------
    float                  time_counter_;
    FlashButtonController* flash_button_controller_;
};
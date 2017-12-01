//--------------------------------------------------------------------------------
//
//　modeDemo.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode_result.h"
#include "main_system.h"
#include "input.h"
#include "mode_title.h"
#include "camera.h"
#include "fade_system.h"
#include "background_ui.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void ModeResult::Init(void)
{
	//カメラの初期化
	auto camera = MY_NEW Camera;
	camera->Init();

	//UIの初期化
	BackgroundUI::Create(L"endingGood.jpg");
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void ModeResult::Update(void)
{
	Mode::Update();

	if (MainSystem::Instance()->GetInput()->GetKeyTrigger(Key::kSubmit))
	{
		MainSystem::Instance()->GetFadeSystem()->FadeTo(MY_NEW ModeTitle);
	}
}
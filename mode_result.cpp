//--------------------------------------------------------------------------------
//
//�@modeDemo.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode_result.h"
#include "main_system.h"
#include "input.h"
#include "mode_title.h"
#include "camera.h"
#include "fade_system.h"
#include "background_ui.h"
#include "light.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void ModeResult::Init(void)
{
	//�J�����̏�����
	auto camera = MY_NEW Camera;
	camera->Init();

    //���C�g�̏�����
    auto directional_light = MY_NEW DirectionalLight(Vector3(-1.0f, -4.0f, 1.0f).Normalized());

	//UI�̏�����
	BackgroundUI::Create(L"endingGood.jpg");
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void ModeResult::Update(void)
{
	Mode::Update();

	if (MainSystem::Instance()->GetInput()->GetKeyTrigger(Key::kSubmit))
	{
		MainSystem::Instance()->GetFadeSystem()->FadeTo(MY_NEW ModeTitle);
	}
}
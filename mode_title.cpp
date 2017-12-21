//--------------------------------------------------------------------------------
//
//�@modetitle.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
#include "mode_title.h"
#include "main_system.h"
#include "input.h"
#include "mode_demo.h"
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
void ModeTitle::Init(void)
{
	//�J�����̏�����
	auto camera = MY_NEW Camera;
	camera->Init();

    //���C�g�̏�����
    auto directional_light = MY_NEW DirectionalLight(Vector3(-1.0f, -4.0f, 1.0f).Normalized());

	//UI�̏�����
	BackgroundUI::Create(L"title.jpg");
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void ModeTitle::Update(void)
{
	Mode::Update();

	if (MainSystem::Instance()->GetInput()->GetKeyTrigger(Key::kSubmit))
	{
		MainSystem::Instance()->GetFadeSystem()->FadeTo(MY_NEW ModeDemo);
	}
}
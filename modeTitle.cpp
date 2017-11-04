//--------------------------------------------------------------------------------
//
//　modeDemo.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "lightManager.h"
#include "input.h"
#include "mode.h"
#include "modeTitle.h"
#include "modeDemo.h"
#include "camera.h"
#include "fadeSystem.h"
#include "BackgroundUI.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void ModeTitle::Init(void)
{
	LightManager::Instance()->CreateLight(LightType::Directional);

	//カメラの初期化
	m_pCamera = new CCamera;
	m_pCamera->Init();

	//UIの初期化
	BackgroundUI::Create("title.jpg");
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void ModeTitle::Update(void)
{
	Mode::Update();

	if (Input::Instance()->GetKeyTrigger(Key::Submit))
	{
		FadeSystem::Instance()->FadeTo(new ModeDemo);
	}
}
//--------------------------------------------------------------------------------
//
//　modetitle.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
#include "mode_title.h"
#include "main_system.h"
#include "input.h"
#include "mode_demo.h"
#include "camera.h"
#include "fade_system.h"
#include "light.h"
#include "material_manager.h"
#include "game_object_spawner.h"

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
	//カメラの初期化
	auto camera = MY_NEW Camera;
	camera->Init();

    //ライトの初期化
    auto directional_light = MY_NEW DirectionalLight(Vector3(-1.0f, -4.0f, 1.0f).Normalized());

	//UIの初期化
    MainSystem::Instance()->GetMaterialManager()->Use(L"title", Color::kWhite, L"title.jpg");
    GameObjectSpawner::CreateBasicPolygon2d(Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), kDefaultLayer, L"title", kDefault2dTextureShader, k2d);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void ModeTitle::Uninit(void)
{
    MainSystem::Instance()->GetMaterialManager()->Disuse(L"title");
    Mode::Uninit();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void ModeTitle::Update(void)
{
	Mode::Update();

	if (MainSystem::Instance()->GetInput()->GetKeyTrigger(Key::kSubmit))
	{
		MainSystem::Instance()->GetFadeSystem()->FadeTo(MY_NEW ModeDemo);
	}
}
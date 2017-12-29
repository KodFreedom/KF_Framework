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
void ModeResult::Init(void)
{
	//カメラの初期化
	auto camera = MY_NEW Camera;
	camera->Init();

    //ライトの初期化
    auto directional_light = MY_NEW DirectionalLight(Vector3(-1.0f, -4.0f, 1.0f).Normalized());

	//UIの初期化
    MainSystem::Instance()->GetMaterialManager()->Use(L"result", Color::kWhite, L"endingGood.jpg");
    GameObjectSpawner::CreateBasicPolygon2d(Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), kDefaultLayer, L"result", kDefault2dTextureShader, k2d);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void ModeResult::Uninit(void)
{
    MainSystem::Instance()->GetMaterialManager()->Disuse(L"result");
    Mode::Uninit();
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
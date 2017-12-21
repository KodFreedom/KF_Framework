//--------------------------------------------------------------------------------
//
//　modeEditor.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode_editor.h"
#if defined(EDITOR)
#include "main_system.h"
#include "light.h"
#include "input.h"
#include "editor_camera.h"

//gameobject
#include "game_object_spawner.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void ModeEditor::Init(void)
{
	auto main_system = MainSystem::Instance();

	//カメラの初期化
	auto camera = MY_NEW EditorCamera;
	camera->Init();

    //ライトの初期化
    auto directional_light = MY_NEW DirectionalLight(Vector3(-1.0f, -4.0f, 1.0f).Normalized());

	main_system->GetInput()->SetEditorMode(true);

	//ゲームオブジェクトの初期化
	GameObjectSpawner::CreateEditor();
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void ModeEditor::LateUpdate(void)
{
	Mode::LateUpdate();
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void ModeEditor::Uninit(void)
{
	Mode::Uninit();
	MainSystem::Instance()->GetInput()->SetEditorMode(false);
}
#endif // _DEBUG


//--------------------------------------------------------------------------------
//
//　modeEditor.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode_editor.h"
#if defined(EDITOR)
#include "main_system.h"
#include "light_manager.h"
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
	main_system->GetLightManager()->CreateDirectionLight(Vector3(-1.0f, -1.0f, 1.0f).Normalized());

	//カメラの初期化
	auto camera = MY_NEW EditorCamera;
	camera->Init();

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


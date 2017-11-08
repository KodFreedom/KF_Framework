//--------------------------------------------------------------------------------
//
//　modeEditor.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "versionSetting.h"
#if defined(_DEBUG) || defined(EDITOR)
#include "main.h"
#include "manager.h"
#include "lightManager.h"
#include "input.h"
#include "modeEditor.h"
#include "editorCamera.h"

//gameobject
#include "gameObjectSpawner.h"
#include "gameObjectActor.h"

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
	LightManager::Instance()->CreateLight(LightType::Directional, Vector3::Zero, Vector3(0.5f, -0.5f, 0.5f));

	//カメラの初期化
	auto camera = new EditorCamera;
	camera->Init();

	Input::Instance()->SetEditorMode(true);

	//ゲームオブジェクトの初期化
	auto fieldEditor = GameObjectSpawner::CreateFieldEditor();
	GameObjectSpawner::CreateStageEditor(fieldEditor);
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
void ModeEditor::uninit(void)
{
	Mode::uninit();
	Input::Instance()->SetEditorMode(false);
}
#endif // _DEBUG


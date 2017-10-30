//--------------------------------------------------------------------------------
//
//　modeEditor.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-19
//--------------------------------------------------------------------------------
#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "lightManager.h"
#include "inputManager.h"
#include "modeEditor.h"
#include "editorCamera.h"

//gameobject
#include "gameObjectSpawner.h"
#include "gameObjectActor.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CModeEditor::CModeEditor() : CMode()
{

}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CModeEditor::~CModeEditor()
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CModeEditor::Init(void)
{
	//ライトの初期化
	CMain::GetManager()->GetLightManager()->CreateDirectionalLight(Vector3(0.5f, -0.5f, 0.5f));

	//カメラの初期化
	m_pCamera = new CEditorCamera;
	m_pCamera->Init();

	CMain::GetManager()->GetInputManager()->SetEditorMode(true);

	//ゲームオブジェクトの初期化
	auto pEditorField = CGameObjectSpawner::CreateEditorField();
	CGameObjectSpawner::CreateEditorController(pEditorField);

	//CGameObject3D::CreateSkyBox(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f));
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CModeEditor::Update(void)
{
	CMode::Update();
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CModeEditor::LateUpdate(void)
{
	CMode::LateUpdate();
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CModeEditor::uninit(void)
{
	//カメラとゲームオブジェクトの破棄
	CMode::uninit();

	//ライトの破棄
	CMain::GetManager()->GetLightManager()->ReleaseAll();

	CMain::GetManager()->GetInputManager()->SetEditorMode(false);
}
#endif // _DEBUG


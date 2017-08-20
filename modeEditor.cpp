//--------------------------------------------------------------------------------
//
//　modeEditor.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-19
//--------------------------------------------------------------------------------
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
	CMain::GetManager()->GetLightManager()->CreateDirectionalLight(CKFVec3(0.5f, -0.5f, 0.5f));

	//カメラの初期化
	m_pCamera = new CEditorCamera;
	m_pCamera->Init();

	CMain::GetManager()->GetInputManager()->SetEditorMode(true);

	//ゲームオブジェクトの初期化
	auto pEditorField = CGameObjectSpawner::CreateEditorField();
	CGameObjectSpawner::CreateEditorController(pEditorField);
	//CGameObject3D::CreateSkyBox(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CModeEditor::Uninit(void)
{
	//カメラとゲームオブジェクトの破棄
	CMode::Uninit();

	//ライトの破棄
	CMain::GetManager()->GetLightManager()->ReleaseAll();

	CMain::GetManager()->GetInputManager()->SetEditorMode(false);
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

	//if (m_bEndMode || CMain::GetManager()->GetInputManager()->GetKeyTrigger(CInputManager::KEY::K_SUBMIT))
	//{
	//	CMain::GetManager()->GetFade()->FadeToMode(new CModeResult);
	//}
}
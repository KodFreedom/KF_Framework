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
#include "textureManager.h"
#include "lightManager.h"
#include "inputManager.h"
#include "mode.h"
#include "modeTitle.h"
#include "modeDemo.h"
#include "camera.h"
#include "fade.h"
#include "BGUIObject.h"

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
CModeTitle::CModeTitle() : CMode()
{

}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CModeTitle::~CModeTitle()
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CModeTitle::Init(void)
{
	//ライトの初期化
	Main::GetManager()->GetLightManager()->CreateDirectionalLight(Vector3(0.5f, -0.5f, 0.5f));

	//カメラの初期化
	m_pCamera = new CCamera;
	m_pCamera->Init();

	//UIの初期化
	CBGUIObject::Create("title.jpg");
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CModeTitle::Update(void)
{
	CMode::Update();

	if (Main::GetManager()->GetInputManager()->GetKeyTrigger(CInputManager::KEY::K_SUBMIT))
	{
		Main::GetManager()->GetFade()->FadeToMode(new CModeDemo);
	}
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CModeTitle::LateUpdate(void)
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
void CModeTitle::uninit(void)
{
	//カメラとゲームオブジェクトの破棄
	CMode::uninit();

	//ライトの破棄
	Main::GetManager()->GetLightManager()->ReleaseAll();
}
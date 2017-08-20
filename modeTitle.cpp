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

//gameobject
//#include "gameObject2D.h"

//--------------------------------------------------------------------------------
//  クラス
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
	CMain::GetManager()->GetLightManager()->CreateDirectionalLight(CKFVec3(0.5f, -0.5f, 0.5f));

	//カメラの初期化
	m_pCamera = new CCamera;
	m_pCamera->Init();

	//ゲームオブジェクトの初期化
	//CGameObject2D::Create(CKFVec3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0.0f, CKFVec3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), "title.jpg");
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CModeTitle::Uninit(void)
{
	//カメラとゲームオブジェクトの破棄
	CMode::Uninit();

	//ライトの破棄
	CMain::GetManager()->GetLightManager()->ReleaseAll();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CModeTitle::Update(void)
{
	CMode::Update();

	if (CMain::GetManager()->GetInputManager()->GetKeyTrigger(CInputManager::KEY::K_SUBMIT))
	{
		CMain::GetManager()->GetFade()->FadeToMode(new CModeDemo);
	}
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CModeTitle::LateUpdate(void)
{
	CMode::LateUpdate();
}
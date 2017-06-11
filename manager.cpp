//--------------------------------------------------------------------------------
//
//　manager.cpp
//	Author : Xu Wenjie
//	Date   : 2016-11-22
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "modelManager.h"
#include "lightManager.h"
#include "materialManager.h"
#include "gameObjectManager.h"
#include "colliderManager.h"
#include "rendererDX.h"
#include "inputDX.h"
#include "mode.h"
#include "modeDemo.h"
#include "fade.h"

//--------------------------------------------------------------------------------
//  静的メンバー変数宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CManager::CManager()
	: m_pRenderer(NULL)
	, m_pKeyboard(NULL)
	, m_pMouse(NULL)
	, m_pTextureManager(NULL)
	, m_pLightManager(NULL)
	, m_pMaterialManager(NULL)
	, m_pModelManager(NULL)
	, m_pGameObjectManager(NULL)
	, m_pColliderManager(NULL)
	, m_pMode(NULL)
	, m_pFade(NULL)
{
}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
KFRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr;

	//レンダラーの生成
	m_pRenderer = new CRendererDX;

	hr = m_pRenderer->Init(hWnd, bWindow);
	if (FAILED(hr))
	{
		MessageBox(NULL, "m_pRenderer->Init ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return KF_FAILED;
	}

	//ランダム
	CKFMath::InitRandom();

	//入力の生成
	m_pKeyboard = new CKeyboardDX;
	m_pKeyboard->Init(hInstance, hWnd);
	m_pMouse = new CMouseDX;
	m_pMouse->Init(hInstance, hWnd);

	//モデルマネージャの生成
	m_pModelManager = new CModelManager;
	m_pModelManager->Init();
	m_pModelManager->LoadAll();

	//テクスチャマネージャの生成
	m_pTextureManager = new CTextureManager;
	m_pTextureManager->Init();
	m_pTextureManager->LoadAll();

	//ライトマネージャの生成
	m_pLightManager = new CLightManager;
	m_pLightManager->Init();

	//マテリアルマネージャの生成
	m_pMaterialManager = new CMaterialManager;
	m_pMaterialManager->Init();

	//コリジョンマネージャの生成
	m_pColliderManager = new CColliderManager;
	m_pColliderManager->Init();

	//ゲームオブジェクトマネージャの生成
	m_pGameObjectManager = new CGameObjectManager;
	m_pGameObjectManager->Init();

	//Fadeの生成
	m_pFade = CFade::Create();

	//初期モード設定
	SetMode(new CModeDemo);

	return KF_SUCCEEDED;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CManager::Uninit(void)
{
	//モードの破棄
	if (m_pMode)
	{
		m_pMode->Release();
		m_pMode = NULL;
	}

	//Fadeの破棄
	if (m_pFade)
	{
		m_pFade->Release();
		m_pFade = NULL;
	}

	//ゲームオブジェクトマネージャの破棄
	if (m_pGameObjectManager != NULL)
	{
		m_pGameObjectManager->Uninit();
		delete m_pGameObjectManager;
		m_pGameObjectManager = NULL;
	}

	//コリジョンマネージャの破棄
	if (m_pColliderManager != NULL)
	{
		m_pColliderManager->Uninit();
		delete m_pColliderManager;
		m_pColliderManager = NULL;
	}

	//マテリアルマネージャの破棄
	if (m_pMaterialManager != NULL)
	{
		delete m_pMaterialManager;
		m_pMaterialManager = NULL;
	}

	//ライトマネージャの破棄
	if (m_pLightManager != NULL)
	{
		m_pLightManager->Uninit();
		delete m_pLightManager;
		m_pLightManager = NULL;
	}

	//テクスチャマネージャの破棄
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->UnloadAll();
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	//モデルマネージャの破棄
	if (m_pModelManager != NULL)
	{
		m_pModelManager->UnloadAll();
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	//キーボードの破棄
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}

	//マウスの破棄
	if (m_pMouse != NULL)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = NULL;
	}

	//レンダラーの破棄
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CManager::Update(void)
{
	//キーボード更新
	if (m_pKeyboard)
	{
		m_pKeyboard->Update();
	}

	//マウス更新
	if (m_pMouse)
	{
		m_pMouse->Update();
	}

	//レンダラー更新
	if (m_pRenderer)
	{
		m_pRenderer->Update();
	}

	//モード更新
	if (m_pMode)
	{
		m_pMode->Update();
	}

	//Fade更新
	if (m_pFade)
	{
		m_pFade->Update();
	}

	//コリジョン更新
	if (m_pColliderManager)
	{
		m_pColliderManager->Update();
	}
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CManager::LateUpdate(void)
{
	//モード更新
	if (m_pMode)
	{
		m_pMode->LateUpdate();
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CManager::Draw(void)
{
	if (m_pRenderer)
	{
		if (m_pRenderer->BeginDraw() == KF_SUCCEEDED)
		{
			//モード描画
			if (m_pMode)
			{
				m_pMode->Draw();
			}

			//Fade更新
			if (m_pFade)
			{
				m_pFade->Draw();
			}

			m_pRenderer->EndDraw();
		}
	}
}

//--------------------------------------------------------------------------------
//  モード切り替え
//--------------------------------------------------------------------------------
void CManager::SetMode(CMode* pMode)
{
	if (m_pMode)
	{
		m_pMode->Release();
	}

	m_pMode = pMode;

	if (m_pMode)
	{
		m_pMode->Init();
	}
}
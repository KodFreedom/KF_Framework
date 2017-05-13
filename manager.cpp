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
#include "rendererDX.h"
#include "inputDX.h"
#include "mode.h"
#include "modeDemo.h"

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
	, m_mode(MODE_DEMO)
{
	for (int nCntMode = 0; nCntMode < MODE_MAX; nCntMode++)
	{
		m_apMode[nCntMode] = NULL;
	}
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

	//ゲームオブジェクトマネージャの生成
	m_pGameObjectManager = new CGameObjectManager;
	m_pGameObjectManager->Init();

	//初期モード設定
	m_mode = MODE_DEMO;

	//各モード生成
	m_apMode[MODE_DEMO] = new CModeDemo;

	//初期モードの初期化
	m_apMode[m_mode]->Init();

	return KF_SUCCEEDED;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CManager::Uninit(void)
{
	//モードの破棄
	for (int nCntMode = 0; nCntMode < MODE_MAX; nCntMode++)
	{
		if (m_apMode[nCntMode] != NULL)
		{
			m_apMode[nCntMode]->Uninit();
			delete m_apMode[nCntMode];
			m_apMode[nCntMode] = NULL;
		}
	}

	//ゲームオブジェクトマネージャの破棄
	if (m_pGameObjectManager != NULL)
	{
		m_pGameObjectManager->Uninit();
		delete m_pGameObjectManager;
		m_pGameObjectManager = NULL;
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
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard->Update();
	}

	//マウス更新
	if (m_pMouse != NULL)
	{
		m_pMouse->Update();
	}

	//レンダラー更新
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	//モード更新
	if (m_apMode[m_mode] != NULL)
	{
		m_apMode[m_mode]->Update();
	}
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CManager::LateUpdate(void)
{
	//モード更新
	if (m_apMode[m_mode] != NULL)
	{
		m_apMode[m_mode]->LateUpdate();
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		if (m_pRenderer->BeginDraw() == KF_SUCCEEDED)
		{
			//モード描画
			if (m_apMode[m_mode] != NULL)
			{
				m_apMode[m_mode]->Draw();
			}

			m_pRenderer->EndDraw();
		}
	}
}

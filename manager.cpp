//--------------------------------------------------------------------------------
//
//　manager.cpp
//	Author : Xu Wenjie
//	Date   : 2016-11-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "inputManager.h"
#include "meshManager.h"
#include "textureManager.h"
#include "modelManager.h"
#include "lightManager.h"
#include "materialManager.h"
#include "gameObjectManager.h"
#include "colliderManager.h"
#include "soundManager.h"
#include "UISystem.h"
#include "rendererDX.h"
#include "mode.h"
#include "modeDemo.h"
#include "fade.h"
#include "KF_PhysicsSystem.h"

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
	: m_pRenderer(nullptr)
	, m_pInputManager(nullptr)
	, m_pMeshManager(nullptr)
	, m_pTextureManager(nullptr)
	, m_pLightManager(nullptr)
	, m_pMaterialManager(nullptr)
	, m_pModelManager(nullptr)
	, m_pGameObjectManager(nullptr)
	, m_pColliderManager(nullptr)
	, m_pSoundManager(nullptr)
	, m_pUISystem(nullptr)
	, m_pMode(nullptr)
	, m_pFade(nullptr)
	, m_pPhysicsSystem(nullptr)
{
}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr;

	//レンダラーの生成
	m_pRenderer = new CRendererDX;

	hr = m_pRenderer->Init(hWnd, bWindow);
	if (FAILED(hr))
	{
		MessageBox(NULL, "m_pRenderer->Init ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}

	//ランダム
	CKFMath::InitRandom();

	//物理演算システム
	m_pPhysicsSystem = new CKFPhysicsSystem;
	m_pPhysicsSystem->Init();

	//入力の生成
	m_pInputManager = new CInputManager;
	m_pInputManager->Init(hInstance, hWnd);

	//メッシュマネージャの生成
	m_pMeshManager = new CMeshManager;

	//モデルマネージャの生成
	m_pModelManager = new CModelManager;
	m_pModelManager->Init();
	m_pModelManager->LoadAll();

	//テクスチャマネージャの生成
	m_pTextureManager = new CTextureManager;

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

	//UIシステムの生成
	m_pUISystem = new CUISystem;

	//サウンドマネージャの生成
	m_pSoundManager = new CSoundManager;
	m_pSoundManager->LoadAll();

	//Fadeの生成
	m_pFade = CFade::Create();

	//初期モード設定
	SetMode(new CModeDemo);

	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CManager::Uninit(void)
{
	//モードの破棄
	SAFE_RELEASE(m_pMode);

	//Fadeの破棄
	SAFE_RELEASE(m_pFade);

	//サウンドマネージャの破棄
	if (m_pSoundManager)
	{
		m_pSoundManager->UnloadAll();
		delete m_pSoundManager;
		m_pSoundManager = nullptr;
	}

	//ゲームオブジェクトマネージャの破棄
	if (m_pGameObjectManager)
	{
		m_pGameObjectManager->Uninit();
		delete m_pGameObjectManager;
		m_pGameObjectManager = nullptr;
	}

	//UIシステムの破棄
	SAFE_RELEASE(m_pUISystem);

	//コリジョンマネージャの破棄
	if (m_pColliderManager)
	{
		m_pColliderManager->Uninit();
		delete m_pColliderManager;
		m_pColliderManager = nullptr;
	}

	//マテリアルマネージャの破棄
	if (m_pMaterialManager)
	{
		delete m_pMaterialManager;
		m_pMaterialManager = nullptr;
	}

	//ライトマネージャの破棄
	if (m_pLightManager)
	{
		m_pLightManager->Uninit();
		delete m_pLightManager;
		m_pLightManager = nullptr;
	}

	//テクスチャマネージャの破棄
	SAFE_RELEASE(m_pTextureManager);

	//モデルマネージャの破棄
	if (m_pModelManager)
	{
		m_pModelManager->UnloadAll();
		delete m_pModelManager;
		m_pModelManager = nullptr;
	}

	//メッシュマネージャの破棄
	SAFE_RELEASE(m_pMeshManager);

	//入力マネージャの破棄
	if (m_pInputManager)
	{
		m_pInputManager->Uninit();
		delete m_pInputManager;
		m_pInputManager = nullptr;
	}

	//物理演算システムの破棄
	SAFE_RELEASE(m_pPhysicsSystem);

	//レンダラーの破棄
	SAFE_RELEASE(m_pRenderer);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CManager::Update(void)
{
	//入力更新
	m_pInputManager->Update();

	//レンダラー更新
	m_pRenderer->Update();

	//モード更新
	m_pMode->Update();

	//コリジョン更新
	m_pColliderManager->Update();

	//物理演算更新
	m_pPhysicsSystem->Update();
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CManager::LateUpdate(void)
{
	//モード更新
	m_pMode->LateUpdate();

	//UI更新
	m_pUISystem->UpdateAll();

	//Fade更新
	m_pFade->Update();
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CManager::Draw(void)
{
	if (m_pRenderer)
	{
		if (m_pRenderer->BeginDraw() == true)
		{
			//モード描画
			m_pMode->Draw();

#ifdef _DEBUG
			m_pColliderManager->DrawCollider();
#endif
			//UI描画
			m_pUISystem->DrawAll();

			//Fade描画
			m_pFade->Draw();

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
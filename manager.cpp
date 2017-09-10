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
#include "lightManager.h"
#include "materialManager.h"
#include "gameObjectManager.h"
#include "soundManager.h"
#include "UISystem.h"
#include "rendererDX.h"
#include "mode.h"
#include "modeTitle.h"
#include "modeDemo.h"
#include "modeEditor.h"
#include "fade.h"

//物理演算処理
#include "KF_CollisionSystem.h"
#include "KF_PhysicsSystem.h"

//Debug処理
#ifdef _DEBUG
#include "debugManager.h"
#endif

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
	, m_pGameObjectManager(nullptr)
	, m_pSoundManager(nullptr)
	, m_pUISystem(nullptr)
	, m_pMode(nullptr)
	, m_pFade(nullptr)
	, m_pCollisionSystem(nullptr)
	, m_pPhysicsSystem(nullptr)
#ifdef _DEBUG
	, m_pDebugManager(nullptr)
#endif
{
}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr = E_FAIL;

	//ランダム
	CKFMath::InitRandom();

	//レンダラーの生成
	m_pRenderer = new CRendererDX;

	hr = m_pRenderer->Init(hWnd, bWindow);
	if (FAILED(hr))
	{
		MessageBox(NULL, "m_pRenderer->Init ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}

#ifdef _DEBUG
	//Debugマネージャの生成
	m_pDebugManager = CDebugManager::Create();
#endif

	//入力の生成
	m_pInputManager = new CInputManager;
	if (!m_pInputManager->Init(hInstance, hWnd))
	{
		MessageBox(NULL, "m_pInputManager->Init ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}

	//メッシュマネージャの生成
	m_pMeshManager = new CMeshManager;

	//テクスチャマネージャの生成
	m_pTextureManager = new CTextureManager;

	//ライトマネージャの生成
	m_pLightManager = new CLightManager;
	if (!m_pLightManager->Init())
	{
		MessageBox(NULL, "m_pLightManager->Init ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}

	//マテリアルマネージャの生成
	m_pMaterialManager = new CMaterialManager;
	m_pMaterialManager->Init();

	//コリジョンシステム
	m_pCollisionSystem = new CKFCollisionSystem;
	m_pCollisionSystem->Init();

	//物理演算システム
	m_pPhysicsSystem = new CKFPhysicsSystem;
	if (!m_pPhysicsSystem->Init())
	{
		MessageBox(NULL, "m_pPhysicsSystem->Init ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}

	//ゲームオブジェクトマネージャの生成
	m_pGameObjectManager = CGameObjectManager::Create();

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
	SAFE_RELEASE(m_pGameObjectManager);

	//UIシステムの破棄
	SAFE_RELEASE(m_pUISystem);

	//物理演算システムの破棄
	SAFE_RELEASE(m_pPhysicsSystem);

	//コリジョンシステムの破棄
	SAFE_RELEASE(m_pCollisionSystem);

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

	//メッシュマネージャの破棄
	SAFE_RELEASE(m_pMeshManager);

	//入力マネージャの破棄
	SAFE_RELEASE(m_pInputManager);

#ifdef _DEBUG
	//Debugマネージャの破棄
	SAFE_RELEASE(m_pDebugManager);
#endif

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
	m_pCollisionSystem->Update();

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

	//コリジョン更新
	m_pCollisionSystem->LateUpdate();

	//UI更新
	m_pUISystem->UpdateAll();

	//Fade更新
	m_pFade->Update();

#ifdef _DEBUG
	//Debugマネージャの更新
	m_pDebugManager->Update();
#endif
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CManager::Draw(void)
{
	if (m_pRenderer->BeginDraw())
	{
		//モード描画
		m_pMode->Draw();

#ifdef _DEBUG
		m_pCollisionSystem->DrawCollider();
#endif
		//UI描画
		m_pUISystem->DrawAll();

		//Fade描画
		m_pFade->Draw();

#ifdef _DEBUG
		//Debug表示
		m_pDebugManager->Draw();
#endif

		m_pRenderer->EndDraw();
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
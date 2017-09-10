//--------------------------------------------------------------------------------
//
//　rendererDX.cpp
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"

#ifdef _DEBUG
#include "manager.h"
#include "inputManager.h"
#include "inputDX.h"
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CRendererDX::CRendererDX()
	: m_pD3D(nullptr)
	, m_pD3DDevice(nullptr)
{
}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CRendererDX::~CRendererDX()
{
}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool CRendererDX::Init(HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return false;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return false;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));									// ワークをゼロクリア
	d3dpp.BackBufferCount				= 1;							// バックバッファの数
	d3dpp.BackBufferWidth				= SCREEN_WIDTH;					// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight				= SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat				= d3ddm.Format;					// カラーモードの指定
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil		= TRUE;							// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed						= bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;	// インターバル
	//抗劇歯
	//d3dpp.MultiSampleType				= D3DMULTISAMPLE_8_SAMPLES;		// アンチエイジングの使用

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return false;
			}
		}
	}

	//抗劇歯
	//if (FAILED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
	//	D3DDEVTYPE_HAL,
	//	D3DFMT_R8G8B8,
	//	FALSE,
	//	D3DMULTISAMPLE_2_SAMPLES,
	//	NULL)))
	//{
	//	//
	//}

	// レンダーステートの設定
	SetRenderMode(RM_NORMAL);
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	
	//抗劇歯
	//m_pD3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);			// アンチエイジング

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大時の補間設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小時の補間設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// 最初のアルファ引数(初期値はD3DTA_TEXTURE、テクスチャがない場合はD3DTA_DIFFUSE)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理(初期値はD3DTOP_SELECTARG1)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);		// ２番目のアルファ引数(初期値はD3DTA_CURRENT)

	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CRendererDX::Release(void)
{
	// デバイスの破棄
	SAFE_RELEASE(m_pD3DDevice);

	// Direct3Dオブジェクトの破棄
	SAFE_RELEASE(m_pD3D);

	delete this;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CRendererDX::Update(void)
{
#ifdef _DEBUG
	CKeyboardDX *pKeyboard = CMain::GetManager()->GetInputManager()->GetKeyboard();
	
	if (pKeyboard->GetKeyPress(DIK_F8))
	{
		SetRenderMode(RM_WIREFRAME);
	}
	else
	{
		SetRenderMode(RM_NORMAL);
	}

#endif
}

//--------------------------------------------------------------------------------
//  描画開始
//--------------------------------------------------------------------------------
bool CRendererDX::BeginDraw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(128, 0, 128, 255), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (FAILED(m_pD3DDevice->BeginScene())) { return false; }

	return true;
}

//--------------------------------------------------------------------------------
//  描画終了
//--------------------------------------------------------------------------------
void CRendererDX::EndDraw(void)
{
	// Direct3Dによる描画の終了
	m_pD3DDevice->EndScene();

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//--------------------------------------------------------------------------------
// レンダーモード設定
//--------------------------------------------------------------------------------
void CRendererDX::SetRenderMode(const RENDER_MODE &rm)
{
	switch (rm)
	{
	case RM_NORMAL:
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		//背面を左回りでカリングする
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//塗りつぶし
		break;
	case RM_WIREFRAME:
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);		//背面のカリングはしない
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);//ワイヤーフレーム
		break;
	default:
		break;
	}
}

#endif
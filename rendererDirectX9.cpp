//--------------------------------------------------------------------------------
//
//　rendererDirectX9.cpp
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "rendererDirectX9.h"

#ifdef _DEBUG
#include "manager.h"
#include "inputManager.h"
#include "inputDX.h"
#endif

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：BeginRender
//  関数説明：描画開始
//	引数：	なし
//	戻り値：なし
//--------------------------------------------------------------------------------
bool RendererDirectX9::BeginRender(void)
{
	// バックバッファ＆Ｚバッファのクリア
	lpD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), backgroundColor, 1.0f, 0);
	return (bool)lpD3DDevice->BeginScene();
}

//--------------------------------------------------------------------------------
//	関数名：EndRender
//  関数説明：描画終了
//	引数：	なし
//	戻り値：なし
//--------------------------------------------------------------------------------
void RendererDirectX9::EndRender(void)
{
	lpD3DDevice->EndScene();

	// バックバッファとフロントバッファの入れ替え
	lpD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//--------------------------------------------------------------------------------
//	関数名：SetWireFrameFlag
//  関数説明：レンダーモード設定
//	引数：	isEnable：WireFrameフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
void RendererDirectX9::SetWireFrameFlag(const bool& isEnable)
{
	Renderer::SetWireFrameFlag(isEnable);
	lpD3DDevice->SetRenderState(D3DRS_FILLMODE, isEnable ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool RendererDirectX9::init(HWND hWnd, BOOL isWindowMode)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	lpDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (nullptr == lpDirect3D9) return false;

	// 現在のディスプレイモードを取得
	if (FAILED(lpDirect3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) return false;

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));									// ワークをゼロクリア
	d3dpp.BackBuffer++count				= 1;							// バックバッファの数
	d3dpp.BackBufferWidth				= SCREEN_WIDTH;					// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight				= SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat				= d3ddm.Format;					// カラーモードの指定
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil		= TRUE;							// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed						= isWindowMode;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;	// インターバル
	
	// 抗劇歯
	//d3dpp.MultiSampleType				= D3DMULTISAMPLE_8_SAMPLES;		// アンチエイジングの使用

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(lpDirect3D9->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &lpD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(lpDirect3D9->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &lpD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(lpDirect3D9->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &lpD3DDevice)))
			{
				// 生成失敗
				return false;
			}
		}
	}

	// 抗劇歯
	//if (FAILED(lpDirect3D9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
	//	D3DDEVTYPE_HAL,
	//	D3DFMT_R8G8B8,
	//	FALSE,
	//	D3DMULTISAMPLE_2_SAMPLES,
	//	NULL)))
	//{
	//	//
	//}

	SetWireFrameFlag(false);
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void RendererDirectX9::uninit(void)
{
	SAFE_RELEASE(lpD3DDevice);
	SAFE_RELEASE(lpDirect3D9);
}
#endif
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
#include "textureManager.h"
#include "materialManager.h"
#include "meshManager.h"
#include "meshInfo.h"
#include "rendererDirectX9.h"
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
//	関数名：Render
//  関数説明：描画処理
//	引数：	meshName：メッシュ名前
//			textureName：テクスチャ名前
//			materialID：マテリアルID
//			worldMatrix：世界行列
//	戻り値：なし
//--------------------------------------------------------------------------------
void RendererDirectX9::Render(const string& meshName, const string& textureName, const unsigned short materialID, const Matrix44& worldMatrix)
{
	auto mesh = MeshManager::Instance()->GetMeshBy(meshName);
	auto texture = TextureManager::Instance()->GetTexture(textureName);
	auto material = MaterialManager::Instance()->GetMaterial(materialID);
	lpD3DDevice->SetTransform(D3DTS_WORLD, &(D3DXMATRIX)worldMatrix);
	lpD3DDevice->SetStreamSource(0, mesh->VertexBuffer, 0, sizeof(VERTEX_3D));
	lpD3DDevice->SetIndices(mesh->IndexBuffer);
	lpD3DDevice->SetFVF(FVF_VERTEX_3D);
	lpD3DDevice->SetTexture(0, texture);
	lpD3DDevice->SetMaterial(&(D3DMATERIAL9)material);
	lpD3DDevice->DrawIndexedPrimitive((_D3DPRIMITIVETYPE)mesh->CurrentType,
		0, 0, mesh->VertexNumber, 0, mesh->PolygonNumber);
}

//--------------------------------------------------------------------------------
//	関数名：SetPorjectionCamera
//  関数説明：projectionカメラの設定
//	引数：	view：view行列
//			projection：射影行列
//	戻り値：なし
//--------------------------------------------------------------------------------
void RendererDirectX9::SetPorjectionCamera(const Matrix44& view, const Matrix44& projection)
{
	lpD3DDevice->SetTransform(D3DTS_VIEW, &(D3DXMATRIX)view);
	lpD3DDevice->SetTransform(D3DTS_PROJECTION, &(D3DXMATRIX)projection);
}

//--------------------------------------------------------------------------------
//	関数名：SetRenderState
//  関数説明：ライティングの設定
//	引数：	value：ライティングの設定値
//	戻り値：なし
//--------------------------------------------------------------------------------
void RendererDirectX9::SetRenderState(const Lighting& value)
{
	lpD3DDevice->SetRenderState(D3DRS_LIGHTING, (DWORD)value);
}

//--------------------------------------------------------------------------------
//	関数名：SetRenderState
//  関数説明：ライティングの設定
//	引数：	value：ライティングの設定値
//	戻り値：なし
//--------------------------------------------------------------------------------
void RendererDirectX9::SetRenderState(const CullMode& value)
{
	lpD3DDevice->SetRenderState(D3DRS_CULLMODE, (_D3DCULL)(value + 1));
}

//--------------------------------------------------------------------------------
//	関数名：SetRenderState
//  関数説明：合成方法の設定
//	引数：	value：合成方法の設定値
//	戻り値：なし
//--------------------------------------------------------------------------------
void RendererDirectX9::SetRenderState(const Synthesis& value)
{

}

//--------------------------------------------------------------------------------
//	関数名：SetRenderState
//  関数説明：ライティングの設定
//	引数：	value：ライティングの設定値
//	戻り値：なし
//--------------------------------------------------------------------------------
void RendererDirectX9::SetRenderState(const FillMode& value)
{
	lpD3DDevice->SetRenderState(D3DRS_FILLMODE, (_D3DFILLMODE)(value + 1));
}

//--------------------------------------------------------------------------------
//	関数名：SetRenderState
//  関数説明：アルファテストの設定
//	引数：	value：アルファテストの設定値
//	戻り値：なし
//--------------------------------------------------------------------------------
void RendererDirectX9::SetRenderState(const Alpha& value)
{
	lpD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, value == Alpha::AlphaTest ? TRUE : FALSE);
}

//--------------------------------------------------------------------------------
//	関数名：SetRenderState
//  関数説明：フォグの設定
//	引数：	value：フォグの設定値
//	戻り値：なし
//--------------------------------------------------------------------------------
void RendererDirectX9::SetRenderState(const Fog& value)
{
	lpD3DDevice->SetRenderState(D3DRS_FOGENABLE, (DWORD)value);
	lpD3DDevice->SetRenderState(D3DRS_RANGEFOGENABLE, (DWORD)value);
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
	if (!createDevice(hWnd, isWindowMode))
	{
		return false;
	}

	initRenderSate();
	initSamplerState();
	initTextureStageStage();
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

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
bool RendererDirectX9::createDevice(HWND hWnd, BOOL isWindowMode)
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
	d3dpp.BackBufferCount				= 1;							// バックバッファの数
	d3dpp.BackBufferWidth				= SCREEN_WIDTH;					// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight				= SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat				= d3ddm.Format;					// カラーモードの指定
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil		= TRUE;							// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed						= isWindowMode;					// ウィンドウモード
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
}

//--------------------------------------------------------------------------------
//  レンダーステートの初期化
//--------------------------------------------------------------------------------
void RendererDirectX9::initRenderSate(void)
{
	lpD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	lpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// αソースカラーの指定
	lpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// αデスティネーションカラーの指定
	lpD3DDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);			// αテストに関する設定
	lpD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);		// αテストに関する設定
}

//--------------------------------------------------------------------------------
//  サンプラーステートの初期化
//--------------------------------------------------------------------------------
void RendererDirectX9::initSamplerState(void)
{
	lpD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャＵ値の繰り返し設定
	lpD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャＶ値の繰り返し設定
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// テクスチャ拡大時の補間設定
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// テクスチャ縮小時の補間設定
}

//--------------------------------------------------------------------------------
//  テクスチャステージステートの初期化
//--------------------------------------------------------------------------------
void RendererDirectX9::initTextureStageStage(void)
{
	lpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	lpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	lpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数(初期値はD3DTA_TEXTURE、テクスチャがない場合はD3DTA_DIFFUSE)
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理(初期値はD3DTOP_SELECTARG1)
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数(初期値はD3DTA_CURRENT)
}
#endif
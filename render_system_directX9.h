//--------------------------------------------------------------------------------
//  renderer_directX9.h
//　レンダーシステム(directx9)
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "render_system.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class RenderSystemDirectX9 : public RenderSystem
{
public:
	//--------------------------------------------------------------------------------
	//  return the instance if create successfully
	//  生成処理
	//  return : RenderSystemDirectX9*
	//--------------------------------------------------------------------------------
	static RenderSystemDirectX9* Create(HWND hwnd, BOOL is_window_mode)
	{
		auto instance = MY_NEW RenderSystemDirectX9;
		if (!instance->Init(hwnd, is_window_mode))
		{
			SAFE_RELEASE(instance);
		}
		return instance;
	}

	//--------------------------------------------------------------------------------
	//  ディバイスを返す
	//  return : LPDIRECT3DDEVICE9
	//--------------------------------------------------------------------------------
	const auto GetDevice(void) const { return device_; }

	//--------------------------------------------------------------------------------
	//  描画開始
	//--------------------------------------------------------------------------------
	bool BeginRender(void) override
	{
		device_->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), background_color_, 1.0f, 0);
		return static_cast<bool>(device_->BeginScene());
	}

	//--------------------------------------------------------------------------------
	//  描画終了
	//--------------------------------------------------------------------------------
	void EndRender(void) override
	{
		device_->EndScene();
	}

	//--------------------------------------------------------------------------------
	//  swap backbuffer / バックバッファの切り替え
	//--------------------------------------------------------------------------------
	void Present(void) override
	{
		device_->Present(NULL, NULL, NULL, NULL);
	}

	//--------------------------------------------------------------------------------
	//  描画処理(3D)
	//	mesh_name：メッシュ名前
	//--------------------------------------------------------------------------------
	void Render(const String& mesh_name) override;

	//--------------------------------------------------------------------------------
	//  カリングモードの設定
	//--------------------------------------------------------------------------------
	void SetRenderState(const CullMode& value) override;

	//--------------------------------------------------------------------------------
	//  フィルモードの設定
	//--------------------------------------------------------------------------------
	void SetRenderState(const FillMode& value) override;

	//--------------------------------------------------------------------------------
	//  フォグモードの設定
	//--------------------------------------------------------------------------------
	void SetRenderState(const FogMode& value) override;

	//--------------------------------------------------------------------------------
	//  αモードの設定
	//--------------------------------------------------------------------------------
	void SetRenderState(const AlphaMode& value) override;

private:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	RenderSystemDirectX9() : RenderSystem()
		, instance_(nullptr), device_(nullptr)
		, vertex_declaration_2d_(nullptr), vertex_declaration_3d_(nullptr) {}
	RenderSystemDirectX9(const RenderSystemDirectX9& value) {}
	RenderSystemDirectX9& operator=(const RenderSystemDirectX9& value) {}
	~RenderSystemDirectX9() {}

	//--------------------------------------------------------------------------------
	//  初期化
	//--------------------------------------------------------------------------------
	bool Init(HWND hwnd, BOOL is_window_mode);

	//--------------------------------------------------------------------------------
	//  終了処理
	//--------------------------------------------------------------------------------
	void Uninit(void) override;

	//--------------------------------------------------------------------------------
	//  ディバイスの生成
	//--------------------------------------------------------------------------------
	bool CreateDevice(HWND hwnd, BOOL is_window_mode);
	
	//--------------------------------------------------------------------------------
	//  バーテックスデクラレーションの初期化
	//--------------------------------------------------------------------------------
	void InitVertexDeclaration(void);

	//--------------------------------------------------------------------------------
	//  レンダーステートの初期化
	//--------------------------------------------------------------------------------
	void InitRenderSate(void);

	//--------------------------------------------------------------------------------
	//  サンプラーステートの初期化
	//--------------------------------------------------------------------------------
	void InitSamplerState(void);

	//--------------------------------------------------------------------------------
	//  Fogの初期化
	//--------------------------------------------------------------------------------
	void InitFog(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	LPDIRECT3D9	instance_; // Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 device_; // Deviceディバイス
	LPDIRECT3DVERTEXDECLARATION9 vertex_declaration_2d_; // 2dバーテックスデクラレーション
	LPDIRECT3DVERTEXDECLARATION9 vertex_declaration_3d_; // 3dバーテックスデクラレーション
};
#endif
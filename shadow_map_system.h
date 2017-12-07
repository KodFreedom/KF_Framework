//--------------------------------------------------------------------------------
//　shadow_map_system.h
//  render objects to shadow map
//	シャドウマップをレンダリングするシステム
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#include "shader_enum.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class MeshRenderer;
class MeshRenderer3d;
class MeshRenderer3dSkin;

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class ShadowMapSystem
{
public:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static constexpr float kShadowMapWidth = 2048.0f;
	static constexpr float kShadowMapHeight = 2048.0f;

	//--------------------------------------------------------------------------------
	//  生成処理
	//  return : TextureManager*
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	static ShadowMapSystem* Create(const LPDIRECT3DDEVICE9 device)
	{
		auto instance = MY_NEW ShadowMapSystem(device);
		instance->Init();
		return instance;
	}
#endif

	//--------------------------------------------------------------------------------
	//  破棄処理
	//--------------------------------------------------------------------------------
	void Release(void) { Uninit(); }

	//--------------------------------------------------------------------------------
	//  描画処理
	//--------------------------------------------------------------------------------
	void Render(void);

	//--------------------------------------------------------------------------------
	//  rendererを登録する
	//  renderer : レンダラー
	//--------------------------------------------------------------------------------
	void Register(MeshRenderer3d* renderer);

	//--------------------------------------------------------------------------------
	//  rendererを登録する
	//  renderer : レンダラー
	//--------------------------------------------------------------------------------
	void Register(MeshRenderer3dSkin* renderer);

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	//--------------------------------------------------------------------------------
	//  シャドウマップのポインタを取得
	//  return : const LPDIRECT3DTEXTURE9
	//--------------------------------------------------------------------------------
	const LPDIRECT3DTEXTURE9 GetShadowMap(void) const
	{
		return shadow_map_;
	}
#endif

private:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	ShadowMapSystem(const LPDIRECT3DDEVICE9 device) : device_(device)
	, shadow_map_(nullptr), shadow_map_surface_(nullptr) {}
	ShadowMapSystem() : device_(nullptr) {}
	ShadowMapSystem(const ShadowMapSystem& value) : device_(nullptr) {}
#else
	ShadowMapSystem() {}
	ShadowMapSystem(const ShadowMapSystem& value) {}
#endif
	ShadowMapSystem& operator=(const ShadowMapSystem& value) {}
	~ShadowMapSystem() {}

	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	void Init(void);

	//--------------------------------------------------------------------------------
	//  破棄処理
	//--------------------------------------------------------------------------------
	void Uninit(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	list<MeshRenderer*> renderers_array_[kShadowMapShaderMax]; // シャドウを表示するレンダラー
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	LPDIRECT3DTEXTURE9 shadow_map_;
	LPDIRECT3DSURFACE9 shadow_map_surface_;
	LPDIRECT3DSURFACE9 depth_stencil_surface_;
	const LPDIRECT3DDEVICE9 device_; // directx9のディバイス
#endif
};
//--------------------------------------------------------------------------------
//
//　rendererDirectX9.h
//	Author : Xu Wenjie
//	Date   : 2017-11-09
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "versionSetting.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "renderSystem.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class RenderSystemDirectX9 : public RenderSystem
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	RenderSystemDirectX9() : RenderSystem(), lpDirect3D9(nullptr), lpD3DDevice(nullptr) {}
	~RenderSystemDirectX9() {}

	bool BeginRender(void) override;
	void EndRender(void) override;
	void Render(const string& meshName, const string& materialName, const Matrix44& worldMatrix) override;
	void SetPorjectionCamera(const Matrix44& view, const Matrix44& projection) override;
	void SetRenderState(const Lighting& value) override;
	void SetRenderState(const CullMode& value) override;
	void SetRenderState(const Synthesis& value) override;
	void SetRenderState(const FillMode& value) override;
	void SetRenderState(const Fog& value) override;
	void SetRenderState(const Alpha& value) override;

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	bool init(HWND hWnd, BOOL isWindowMode) override;
	void uninit(void) override;
	bool createDevice(HWND hWnd, BOOL isWindowMode);
	void initRenderSate(void);
	void initSamplerState(void);
	void initTextureStageState(void);
	void initFog(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	LPDIRECT3D9			lpDirect3D9; // Direct3Dオブジェクト
	LPDIRECT3DDEVICE9	lpD3DDevice; // Deviceオブジェクト(描画に必要)
};
#endif
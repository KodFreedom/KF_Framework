//--------------------------------------------------------------------------------
//
//�@rendererDirectX9.h
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "renderer.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class RendererDirectX9 : public Renderer
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	RendererDirectX9() : Renderer(), lpDirect3D9(nullptr), lpD3DDevice(nullptr) {}
	~RendererDirectX9() {}

	bool BeginRender(void) override;
	void EndRender(void) override;
	void SetPorjectionCamera(const Matrix44& view, const Matrix44& projection) override;
	void SetRenderState(const Lighting& value) override;
	void SetRenderState(const CullMode& value) override;
	void SetRenderState(const Synthesis& value) override;
	void SetRenderState(const FillMode& value) override;
	void SetRenderState(const Alpha& value) override;
	void SetRenderState(const Fog& value) override;

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	bool init(HWND hWnd, BOOL isWindowMode) override;
	void uninit(void) override;
	bool createDevice(HWND hWnd, BOOL isWindowMode);
	void initRenderSate(void);
	void initSamplerState(void);
	void initTextureStageStage(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	LPDIRECT3D9			lpDirect3D9;	// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9	lpD3DDevice;	// Device�I�u�W�F�N�g(�`��ɕK�v)
};
#endif
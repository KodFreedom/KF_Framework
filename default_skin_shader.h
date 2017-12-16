//--------------------------------------------------------------------------------
//�@default_skin_shader.h
//	�V�F�[�_�[�N���X
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "shader_directX9.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class DefaultSkinShader : public ShaderDirectX9
{
public:
	DefaultSkinShader() : ShaderDirectX9() {}
	~DefaultSkinShader() {}

	void Init(const LPDIRECT3DDEVICE9 device) override;
	void Set(const LPDIRECT3DDEVICE9 device) override;
	void Reset(const LPDIRECT3DDEVICE9 device) override;
	void SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer) override;
};
#endif
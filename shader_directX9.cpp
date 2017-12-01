//--------------------------------------------------------------------------------
//�@shader_directX9.cpp
//	�V�F�[�_�[�N���X
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "shader_directX9.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "main.h"
//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void ShaderDirectX9::Uninit(void)
{
	SAFE_RELEASE(vertex_shader_);
	SAFE_RELEASE(vertex_shader_constant_table_);
	SAFE_RELEASE(pixel_shader_);
	SAFE_RELEASE(pixel_shader_constant_table_);
}

//--------------------------------------------------------------------------------
//  �g�p����
//--------------------------------------------------------------------------------
void ShaderDirectX9::Set(const LPDIRECT3DDEVICE9 device)
{
	device->SetVertexShader(vertex_shader_);
	device->SetPixelShader(pixel_shader_);
}
#endif
//--------------------------------------------------------------------------------
//�@shader_directX9.h
//	�V�F�[�_�[�N���X
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class MeshRenderer;

//--------------------------------------------------------------------------------
//  ���N���X
//--------------------------------------------------------------------------------
class ShaderDirectX9
{
public:
	ShaderDirectX9()
		: vertex_shader_(nullptr)
		, vertex_shader_constant_table_(nullptr)
		, pixel_shader_(nullptr)
		, pixel_shader_constant_table_(nullptr) {}
	~ShaderDirectX9() {}

	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Set(const LPDIRECT3DDEVICE9 device) = 0;
	virtual void SetConstantTable(const MeshRenderer& renderer) = 0;

protected:
	LPDIRECT3DVERTEXSHADER9	vertex_shader_;
	LPD3DXCONSTANTTABLE     vertex_shader_constant_table_;
	LPDIRECT3DPIXELSHADER9	pixel_shader_;
	LPD3DXCONSTANTTABLE		pixel_shader_constant_table_;
};
#endif
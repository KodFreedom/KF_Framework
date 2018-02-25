//--------------------------------------------------------------------------------
//�@shader_directX9.h
//  �V�F�[�_�[�N���X
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "../common_setting.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include <d3dx9.h>

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

    virtual void Init(const LPDIRECT3DDEVICE9 device) = 0;
    void Uninit(void);
    virtual void Set(const LPDIRECT3DDEVICE9 device);
    virtual void Reset(const LPDIRECT3DDEVICE9 device) = 0;
    virtual void SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer) = 0;

protected:
    void CompileShaderFrom(const String& vertex_shader, const String& pixel_shader, const LPDIRECT3DDEVICE9 device);

    LPDIRECT3DVERTEXSHADER9    vertex_shader_;
    LPD3DXCONSTANTTABLE     vertex_shader_constant_table_;
    LPDIRECT3DPIXELSHADER9    pixel_shader_;
    LPD3DXCONSTANTTABLE        pixel_shader_constant_table_;
};
#endif
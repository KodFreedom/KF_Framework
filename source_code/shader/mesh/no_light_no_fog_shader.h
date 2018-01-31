//--------------------------------------------------------------------------------
//　no_light_no_fog_shader.h
//  シェーダークラス
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "shader_directX9.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class NoLightNoFogShader : public ShaderDirectX9
{
public:
    NoLightNoFogShader() : ShaderDirectX9() {}
    ~NoLightNoFogShader() {}

    void Init(const LPDIRECT3DDEVICE9 device) override;
    void Set(const LPDIRECT3DDEVICE9 device) override;
    void Reset(const LPDIRECT3DDEVICE9 device) override;
    void SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer) override;
};
#endif
//--------------------------------------------------------------------------------
//　default_2d_texture_shader.h
//  2dシェーダークラス
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "shader_directX9.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class Default2dTextureShader : public ShaderDirectX9
{
public:
    Default2dTextureShader() : ShaderDirectX9() {}
    ~Default2dTextureShader() {}

    void Init(const LPDIRECT3DDEVICE9 device) override;
    void Set(const LPDIRECT3DDEVICE9 device) override;
    void Reset(const LPDIRECT3DDEVICE9 device) override;
    void SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer) override;
};
#endif
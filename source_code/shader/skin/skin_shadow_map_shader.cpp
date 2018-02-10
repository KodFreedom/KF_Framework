//--------------------------------------------------------------------------------
//　shadow_map_shader
//  シェーダークラス
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "skin_shadow_map_shader.h"
#include "main_system.h"
#include "render_system.h"
#include "camera.h"
#include "camera_manager.h"
#include "material_manager.h"
#include "texture_manager.h"
#include "game_object.h"
#include "mesh_renderer_3d_skin.h"
#include "animator.h"
#include "shadow_map_system.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void SkinShadowMapShader::Init(const LPDIRECT3DDEVICE9 device)
{
    CompileShaderFrom(L"basic_skin_shadow_map_vertex_shader", L"basic_skin_shadow_map_pixel_shader", device);
}

//--------------------------------------------------------------------------------
//  使用処理
//--------------------------------------------------------------------------------
void SkinShadowMapShader::Set(const LPDIRECT3DDEVICE9 device)
{
    ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  使用完了の後片付け
//--------------------------------------------------------------------------------
void SkinShadowMapShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  定数テーブルの設定
//--------------------------------------------------------------------------------
void SkinShadowMapShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
    assert(renderer.GetType() == kMeshRenderer3dSkin);

    auto& shadow_map_system = MainSystem::Instance().GetShadowMapSystem();
    vertex_shader_constant_table_->SetMatrix(device, "view_light", &static_cast<D3DXMATRIX>(shadow_map_system.GetLightView()));
    vertex_shader_constant_table_->SetMatrix(device, "projection_light", &static_cast<D3DXMATRIX>(shadow_map_system.GetLightProjection()));
    pixel_shader_constant_table_->SetFloat(device, "light_far", shadow_map_system.GetFar());

    // bone
    auto skin_mesh_renderer = (MeshRenderer3dSkin*)(&renderer);
    const auto& bone_texture = skin_mesh_renderer->GetAnimator().GetBoneTexture();
    assert(bone_texture.pointer);
    assert(bone_texture.size);
    vertex_shader_constant_table_->SetFloat(device, "texture_size", static_cast<FLOAT>(bone_texture.size));
    UINT bone_texture_index = vertex_shader_constant_table_->GetSamplerIndex("bone_texture");
    device->SetSamplerState(D3DVERTEXTEXTURESAMPLER0 + bone_texture_index, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    device->SetSamplerState(D3DVERTEXTEXTURESAMPLER0 + bone_texture_index, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    device->SetSamplerState(D3DVERTEXTEXTURESAMPLER0 + bone_texture_index, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    device->SetSamplerState(D3DVERTEXTEXTURESAMPLER0 + bone_texture_index, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    device->SetSamplerState(D3DVERTEXTEXTURESAMPLER0 + bone_texture_index, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    device->SetTexture(D3DVERTEXTEXTURESAMPLER0 + bone_texture_index, bone_texture.pointer);
}
#endif
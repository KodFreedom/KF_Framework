//--------------------------------------------------------------------------------
//　default_skin_shader.cpp
//  シェーダークラス
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "default_skin_shader.h"
#include "../../main_system.h"
#include "../../render_system/render_system.h"
#include "../../camera/camera.h"
#include "../../camera/camera_manager.h"
#include "../../resources/resources.h"
#include "../../resources/material_manager.h"
#include "../../resources/texture_manager.h"
#include "../../game_object/game_object.h"
#include "../../component/renderer/mesh_renderer_3d_skin.h"
#include "../../component/animator/animator.h"
#include "../../light/light_manager.h"
#include "../../render_system/shadow_map_system.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void DefaultSkinShader::Init(const LPDIRECT3DDEVICE9 device)
{
    CompileShaderFrom(L"default_skin_vertex_shader", L"default_skin_pixel_shader", device);
}

//--------------------------------------------------------------------------------
//  使用処理
//--------------------------------------------------------------------------------
void DefaultSkinShader::Set(const LPDIRECT3DDEVICE9 device)
{
    ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  使用完了の後片付け
//--------------------------------------------------------------------------------
void DefaultSkinShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  定数テーブルの設定
//--------------------------------------------------------------------------------
void DefaultSkinShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
    assert(renderer.GetType() == kMeshRenderer3dSkin);

    auto& main_system = MainSystem::Instance();

    // Camera
    auto camera = main_system.GetCameraManager().GetMainCamera();
    vertex_shader_constant_table_->SetMatrix(device, "view", (D3DXMATRIX*)&camera->GetView());
    vertex_shader_constant_table_->SetMatrix(device, "projection", (D3DXMATRIX*)&camera->GetProjection());
    vertex_shader_constant_table_->SetValue(device, "camera_position_world", &camera->GetWorldEyePosition(), sizeof(Vector3));

    // Light
    auto& light = main_system.GetLightManager().GetDirectionalLights().front();
    vertex_shader_constant_table_->SetValue(device, "light_direction_world", &light->direction_, sizeof(light->direction_));

    // Skin Bone Data
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

    // Material
    auto material = main_system.GetResources().GetMaterialManager().Get(renderer.GetMaterialName());
    if (material)
    {
        auto& texture_manager = main_system.GetResources().GetTextureManager();
        UINT index = pixel_shader_constant_table_->GetSamplerIndex("color_texture");
        device->SetSamplerState(index, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        device->SetSamplerState(index, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
        device->SetSamplerState(index, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        device->SetSamplerState(index, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        device->SetTexture(index, texture_manager.Get(material->color_texture_));

        index = pixel_shader_constant_table_->GetSamplerIndex("normal_texture");
        device->SetSamplerState(index, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
        device->SetSamplerState(index, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        device->SetSamplerState(index, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        device->SetSamplerState(index, D3DSAMP_MAXANISOTROPY, 8);
        device->SetTexture(index, texture_manager.Get(material->normal_texture_));
    }

    // Shadow Map
    auto& shadow_map_system = MainSystem::Instance().GetShadowMapSystem();
    vertex_shader_constant_table_->SetMatrix(device, "view_light", (D3DXMATRIX*)&shadow_map_system.GetLightView());
    vertex_shader_constant_table_->SetMatrix(device, "projection_light", (D3DXMATRIX*)&shadow_map_system.GetLightProjection());
    pixel_shader_constant_table_->SetFloat(device, "bias", shadow_map_system.GetBias());
    pixel_shader_constant_table_->SetFloat(device, "light_far", shadow_map_system.GetFar());
    UINT shadow_map_index = pixel_shader_constant_table_->GetSamplerIndex("shadow_map");
    device->SetSamplerState(shadow_map_index, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
    device->SetSamplerState(shadow_map_index, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
    device->SetSamplerState(shadow_map_index, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    device->SetSamplerState(shadow_map_index, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    device->SetSamplerState(shadow_map_index, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    device->SetTexture(shadow_map_index, shadow_map_system.GetShadowMap());
}
#endif
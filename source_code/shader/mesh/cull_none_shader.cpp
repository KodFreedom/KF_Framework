//--------------------------------------------------------------------------------
//　cull_none_shader.cpp
//  シェーダークラス
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "cull_none_shader.h"
#include "../../main_system.h"
#include "../../render_system/render_system.h"
#include "../../camera/camera.h"
#include "../../camera/camera_manager.h"
#include "../../light/light_manager.h"
#include "../../resources/resources.h"
#include "../../resources/material_manager.h"
#include "../../resources/texture_manager.h"
#include "../../game_object/game_object.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CullNoneShader::Init(const LPDIRECT3DDEVICE9 device)
{
    CompileShaderFrom(L"cull_none_vertex_shader", L"cull_none_pixel_shader", device);
}

//--------------------------------------------------------------------------------
//  使用処理
//--------------------------------------------------------------------------------
void CullNoneShader::Set(const LPDIRECT3DDEVICE9 device)
{
    ShaderDirectX9::Set(device);
    MainSystem::Instance().GetRenderSystem().SetRenderState(CullMode::KCullNone);
}

//--------------------------------------------------------------------------------
//  使用完了の後片付け
//--------------------------------------------------------------------------------
void CullNoneShader::Reset(const LPDIRECT3DDEVICE9 device)
{
    MainSystem::Instance().GetRenderSystem().SetRenderState(CullMode::kCcw);
}

//--------------------------------------------------------------------------------
//  定数テーブルの設定
//--------------------------------------------------------------------------------
void CullNoneShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
    auto& main_system = MainSystem::Instance();
    auto camera = main_system.GetCameraManager().GetMainCamera();
    auto& world = renderer.GetGameObject().GetTransform()->GetWorldMatrix();
    auto& world_inverse = world.Inverse();

    // Vertex
    D3DXMATRIX world_view_projection = world * camera->GetView() * camera->GetProjection();
    vertex_shader_constant_table_->SetMatrix(device, "world_view_projection", &world_view_projection);

    // Pixel
    auto& camera_position_local = Vector3::TransformCoord(camera->GetWorldEyePosition(), world_inverse);
    pixel_shader_constant_table_->SetValue(device, "camera_position_local", &camera_position_local, sizeof(camera_position_local));

    auto& light = main_system.GetLightManager().GetDirectionalLights().front();
    auto& light_direction_local = Vector3::TransformNormal(light->direction_, world_inverse);
    pixel_shader_constant_table_->SetValue(device, "light_direction_local", &light_direction_local, sizeof(light_direction_local));
    pixel_shader_constant_table_->SetValue(device, "light_diffuse", &light->diffuse_, sizeof(light->diffuse_));
    pixel_shader_constant_table_->SetValue(device, "light_ambient", &light->ambient_, sizeof(light->ambient_));

    // Material
    auto material = main_system.GetResources().GetMaterialManager().Get(renderer.GetMaterialName());
    if (material)
    {
        UINT index = pixel_shader_constant_table_->GetSamplerIndex("color_texture");
        device->SetSamplerState(index, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        device->SetSamplerState(index, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
        device->SetSamplerState(index, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        device->SetSamplerState(index, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        device->SetTexture(index, main_system.GetResources().GetTextureManager().Get(material->color_texture_));
        pixel_shader_constant_table_->SetValue(device, "material_diffuse", &material->diffuse_, sizeof(material->diffuse_));
        pixel_shader_constant_table_->SetValue(device, "material_ambient", &material->ambient_, sizeof(material->ambient_));
        pixel_shader_constant_table_->SetValue(device, "material_emissive", &material->emissive_, sizeof(material->emissive_));
        pixel_shader_constant_table_->SetValue(device, "material_specular", &material->specular_, sizeof(material->specular_));
        pixel_shader_constant_table_->SetValue(device, "material_power", &material->power_, sizeof(material->power_));
    }
}
#endif
//--------------------------------------------------------------------------------
//  default_shader.cpp
//  �V�F�[�_�[�N���X
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "default_shader.h"
#include "../../main_system.h"
#include "../../render_system/render_system.h"
#include "../../camera/camera.h"
#include "../../camera/camera_manager.h"
#include "../../light/light_manager.h"
#include "../../resources/resources.h"
#include "../../resources/material_manager.h"
#include "../../resources/texture_manager.h"
#include "../../game_object/game_object.h"
#include "../../render_system/shadow_map_system.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void DefaultShader::Init(const LPDIRECT3DDEVICE9 device)
{
    CompileShaderFrom(L"default_vertex_shader", L"default_pixel_shader", device);
}

//--------------------------------------------------------------------------------
//  �g�p����
//--------------------------------------------------------------------------------
void DefaultShader::Set(const LPDIRECT3DDEVICE9 device)
{
    ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  �g�p�����̌�Еt��
//--------------------------------------------------------------------------------
void DefaultShader::Reset(const LPDIRECT3DDEVICE9 device)
{
    
}

//--------------------------------------------------------------------------------
//  �萔�e�[�u���̐ݒ�
//--------------------------------------------------------------------------------
void DefaultShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
    auto& main_system = MainSystem::Instance();

    auto camera = main_system.GetCameraManager().GetMainCamera();
    const Matrix44& world = renderer.GetGameObject().GetTransform()->GetWorldMatrix();
    const Matrix44& world_inverse = world.Inverse();
    
    // Vertex
    const Matrix44& world_view_projection = world * camera->GetView() * camera->GetProjection();
    vertex_shader_constant_table_->SetValue(device, "world_view_projection", &world_view_projection, sizeof(world_view_projection));
    
    // Pixel
    auto& camera_position_local = Vector3::TransformCoord(camera->GetWorldEyePosition(), world_inverse);
    pixel_shader_constant_table_->SetValue(device, "camera_position_local", &camera_position_local, sizeof(camera_position_local));
    
    const auto& light = main_system.GetLightManager().GetDirectionalLights().front();
    Vector3& light_direction_local = Vector3::TransformNormal(light->direction_, world_inverse).Normalized();
    pixel_shader_constant_table_->SetValue(device, "light_direction_local", &light_direction_local, sizeof(light_direction_local));
    pixel_shader_constant_table_->SetValue(device, "light_diffuse", &light->diffuse_, sizeof(light->diffuse_));

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
        pixel_shader_constant_table_->SetValue(device, "material_emissive", &material->emissive_, sizeof(material->emissive_));
        pixel_shader_constant_table_->SetValue(device, "material_specular", &material->specular_, sizeof(material->specular_));
        pixel_shader_constant_table_->SetValue(device, "material_power", &material->power_, sizeof(material->power_));
    }

    // Shadow Map
    auto& shadow_map_system = main_system.GetShadowMapSystem();
    const Matrix44& world_view_projection_light = world * shadow_map_system.GetLightView() * shadow_map_system.GetLightProjection();
    vertex_shader_constant_table_->SetValue(device, "world_view_projection_light", &world_view_projection_light, sizeof(world_view_projection_light));
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
//--------------------------------------------------------------------------------
//  default_shader.cpp
//  �V�F�[�_�[�N���X
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "default_shader.h"
#include "main_system.h"
#include "render_system.h"
#include "camera.h"
#include "camera_manager.h"
#include "light_manager.h"
#include "material_manager.h"
#include "texture_manager.h"
#include "game_object.h"
#include "shadow_map_system.h"

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
    auto main_system = MainSystem::Instance();
    auto camera = main_system->GetCameraManager()->GetMainCamera();
    const Matrix44& world = renderer.GetGameObject().GetTransform()->GetWorldMatrix();
    const Matrix44& world_inverse = world.Inverse();
    
    // Vertex
    const Matrix44& world_view_projection = world * camera->GetView() * camera->GetProjection();
    vertex_shader_constant_table_->SetValue(device, "world_view_projection", &world_view_projection, sizeof(world_view_projection));
    
    // Pixel
    auto& camera_position_local = Vector3::TransformCoord(camera->GetWorldEyePosition(), world_inverse);
    pixel_shader_constant_table_->SetValue(device, "camera_position_local", &camera_position_local, sizeof(camera_position_local));
    
    const auto& light = main_system->GetLightManager()->GetDirectionalLights().front();
    Vector3& light_direction_local = Vector3::TransformNormal(light->direction_, world_inverse).Normalized();
    pixel_shader_constant_table_->SetValue(device, "light_direction_local", &light_direction_local, sizeof(light_direction_local));
    pixel_shader_constant_table_->SetValue(device, "light_diffuse", &light->diffuse_, sizeof(light->diffuse_));

    const auto& material = main_system->GetMaterialManager()->GetMaterial(renderer.GetMaterialName());
    UINT color_texture_index = pixel_shader_constant_table_->GetSamplerIndex("color_texture");
    device->SetTexture(color_texture_index, main_system->GetTextureManager()->Get(material->color_texture_));
    pixel_shader_constant_table_->SetValue(device, "material_diffuse", &material->diffuse_, sizeof(material->diffuse_));
    pixel_shader_constant_table_->SetValue(device, "material_emissive", &material->emissive_, sizeof(material->emissive_));
    pixel_shader_constant_table_->SetValue(device, "material_specular", &material->specular_, sizeof(material->specular_));
    pixel_shader_constant_table_->SetValue(device, "material_power", &material->power_, sizeof(material->power_));

    // Shadow Map
    auto shadow_map_system = main_system->GetShadowMapSystem();
    const Matrix44& world_view_projection_light = world * shadow_map_system->GetLightView() * shadow_map_system->GetLightProjection();
    vertex_shader_constant_table_->SetValue(device, "world_view_projection_light", &world_view_projection_light, sizeof(world_view_projection_light));
    pixel_shader_constant_table_->SetFloat(device, "bias", shadow_map_system->GetBias());
    UINT shadow_map_index = pixel_shader_constant_table_->GetSamplerIndex("shadow_map");
    device->SetTexture(shadow_map_index, main_system->GetShadowMapSystem()->GetShadowMap());
}
#endif
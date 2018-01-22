//--------------------------------------------------------------------------------
//�@shadow_map_shader
//  �V�F�[�_�[�N���X
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "shadow_map_shader.h"
#include "main_system.h"
#include "render_system.h"
#include "camera.h"
#include "camera_manager.h"
#include "material_manager.h"
#include "texture_manager.h"
#include "game_object.h"
#include "shadow_map_system.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void ShadowMapShader::Init(const LPDIRECT3DDEVICE9 device)
{
    CompileShaderFrom(L"basic_shadow_map_vertex_shader", L"basic_shadow_map_pixel_shader", device);
}

//--------------------------------------------------------------------------------
//  �g�p����
//--------------------------------------------------------------------------------
void ShadowMapShader::Set(const LPDIRECT3DDEVICE9 device)
{
    ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  �g�p�����̌�Еt��
//--------------------------------------------------------------------------------
void ShadowMapShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  �萔�e�[�u���̐ݒ�
//--------------------------------------------------------------------------------
void ShadowMapShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
    auto shadow_map_system = MainSystem::Instance()->GetShadowMapSystem();
    auto& world = renderer.GetGameObject().GetTransform()->GetWorldMatrix();
    D3DXMATRIX world_view_projection_light = world * shadow_map_system->GetLightView() * shadow_map_system->GetLightProjection();
    vertex_shader_constant_table_->SetMatrix(device, "world_view_projection_light", &world_view_projection_light);
    pixel_shader_constant_table_->SetFloat(device, "light_far", shadow_map_system->GetFar());
}
#endif
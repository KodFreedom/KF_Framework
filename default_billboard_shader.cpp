//--------------------------------------------------------------------------------
//�@default_billboard_shader.h
//  �V�F�[�_�[�N���X
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "default_billboard_shader.h"
#include "main_system.h"
#include "render_system.h"
#include "camera.h"
#include "camera_manager.h"
#include "material_manager.h"
#include "texture_manager.h"
#include "game_object.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void DefaultBillboardShader::Init(const LPDIRECT3DDEVICE9 device)
{
    CompileShaderFrom(L"default_billboard_vertex_shader", L"no_light_no_fog_pixel_shader", device);
}

//--------------------------------------------------------------------------------
//  �g�p����
//--------------------------------------------------------------------------------
void DefaultBillboardShader::Set(const LPDIRECT3DDEVICE9 device)
{
    ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  �g�p�����̌�Еt��
//--------------------------------------------------------------------------------
void DefaultBillboardShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  �萔�e�[�u���̐ݒ�
//--------------------------------------------------------------------------------
void DefaultBillboardShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
    // Vertex
    auto main_system = MainSystem::Instance();
    auto camera = main_system->GetCameraManager()->GetMainCamera();
    auto transform = renderer.GetGameObject().GetTransform();

    // �J�����Ɍ���
    Matrix44 world;
    world.rows_[3] = Vector4(transform->GetPosition(), 1.0f);
    world = camera->GetViewTranspose() * world;
    D3DXMATRIX world_view_projection = world * camera->GetView() * camera->GetProjection();
    vertex_shader_constant_table_->SetMatrix(device, "world_view_projection", &world_view_projection);
    
    const Vector3& scale = transform->GetScale();
    vertex_shader_constant_table_->SetValue(device, "scale", &scale, sizeof(scale));

    // Pixel
    const auto& material = main_system->GetMaterialManager()->GetMaterial(renderer.GetMaterialName());
    UINT color_texture_index = pixel_shader_constant_table_->GetSamplerIndex("color_texture");
    device->SetTexture(color_texture_index, main_system->GetTextureManager()->Get(material->color_texture_));
    pixel_shader_constant_table_->SetValue(device, "material_diffuse", &material->diffuse_, sizeof(material->diffuse_));
}
#endif
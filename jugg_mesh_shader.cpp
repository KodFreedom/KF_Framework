//--------------------------------------------------------------------------------
//　jugg_mesh_shader.cpp
//  シェーダークラス
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "jugg_mesh_shader.h"
#include "main_system.h"
#include "render_system.h"
#include "camera.h"
#include "camera_manager.h"
#include "material_manager.h"
#include "texture_manager.h"
#include "game_object.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void JuggMeshShader::Init(const LPDIRECT3DDEVICE9 device)
{
    CompileShaderFrom(L"jugg_mesh_vertex_shader", L"jugg_mesh_pixel_shader", device);
}

//--------------------------------------------------------------------------------
//  使用処理
//--------------------------------------------------------------------------------
void JuggMeshShader::Set(const LPDIRECT3DDEVICE9 device)
{
    ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  使用完了の後片付け
//--------------------------------------------------------------------------------
void JuggMeshShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  定数テーブルの設定
//--------------------------------------------------------------------------------
void JuggMeshShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
    // Vertex
    auto main_system = MainSystem::Instance();
    auto camera = main_system->GetCameraManager()->GetMainCamera();
    auto& world = renderer.GetGameObject().GetTransform()->GetWorldMatrix();
    D3DXMATRIX world_view_projection = world * camera->GetView() * camera->GetProjection();
    vertex_shader_constant_table_->SetMatrix(device, "world_view_projection", &world_view_projection);

    // Pixel
    const auto& material = main_system->GetMaterialManager()->GetMaterial(renderer.GetMaterialName());
    UINT color_texture_index = pixel_shader_constant_table_->GetSamplerIndex("color_texture");
    device->SetTexture(color_texture_index, main_system->GetTextureManager()->Get(material->color_texture_));
}
#endif
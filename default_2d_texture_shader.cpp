//--------------------------------------------------------------------------------
//　default_2d_texture_shader.h
//	2dシェーダークラス
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "default_2d_texture_shader.h"
#include "main_system.h"
#include "material_manager.h"
#include "texture_manager.h"
#include "game_object.h"
#include "render_system.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void Default2dTextureShader::Init(const LPDIRECT3DDEVICE9 device)
{
    CompileShaderFrom(L"default_2d_texture_vertex_shader", L"default_2d_texture_pixel_shader", device);
}

//--------------------------------------------------------------------------------
//  使用処理
//--------------------------------------------------------------------------------
void Default2dTextureShader::Set(const LPDIRECT3DDEVICE9 device)
{
    ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  使用完了の後片付け
//--------------------------------------------------------------------------------
void Default2dTextureShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  定数テーブルの設定
//--------------------------------------------------------------------------------
void Default2dTextureShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
    // Vertex
    const Matrix44& world = renderer.GetGameObject().GetTransform()->GetWorldMatrix();
    vertex_shader_constant_table_->SetValue(device, "world", &world, sizeof(world));
    vertex_shader_constant_table_->SetValue(device, "projection", &RenderSystem::kProjection2d, sizeof(RenderSystem::kProjection2d));
    vertex_shader_constant_table_->SetValue(device, "offset", &RenderSystem::kOffset2d, sizeof(RenderSystem::kOffset2d));

    // Pixel
    auto main_system = MainSystem::Instance();
    const auto& material = main_system->GetMaterialManager()->GetMaterial(renderer.GetMaterialName());
    pixel_shader_constant_table_->SetValue(device, "material_diffuse", &material->diffuse_, sizeof(material->diffuse_));
    UINT color_texture_index = pixel_shader_constant_table_->GetSamplerIndex("color_texture");
    device->SetTexture(color_texture_index, main_system->GetTextureManager()->Get(material->color_texture_));
}
#endif
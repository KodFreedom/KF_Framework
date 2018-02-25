//--------------------------------------------------------------------------------
//　default_2d_texture_shader.h
//  2dシェーダークラス
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "default_2d_texture_shader.h"
#include "../../main_system.h"
#include "../../resources/resources.h"
#include "../../resources/material_manager.h"
#include "../../resources/texture_manager.h"
#include "../../game_object/game_object.h"
#include "../../render_system/render_system.h"
#include "../../component/renderer/mesh_renderer_2d.h"

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
    assert(renderer.GetType() == kMeshRenderer2d);

    // Vertex
    const Matrix44& world = renderer.GetGameObject().GetTransform()->GetWorldMatrix();
    vertex_shader_constant_table_->SetValue(device, "world", &world, sizeof(world));
    vertex_shader_constant_table_->SetValue(device, "projection", &RenderSystem::kProjection2d, sizeof(RenderSystem::kProjection2d));
    vertex_shader_constant_table_->SetValue(device, "offset", &RenderSystem::kOffset2d, sizeof(RenderSystem::kOffset2d));
    
    auto mesh_renderer_2d = (MeshRenderer2d*)&renderer;
    const Vector2& uv_scale = mesh_renderer_2d->GetUvScale();
    const Vector2& uv_offset = mesh_renderer_2d->GetUvOffset();
    vertex_shader_constant_table_->SetValue(device, "uv_scale", &uv_scale, sizeof(uv_scale));
    vertex_shader_constant_table_->SetValue(device, "uv_offset", &uv_offset, sizeof(uv_offset));

    // Pixel
    auto& main_system = MainSystem::Instance();
    auto material = main_system.GetResources().GetMaterialManager().Get(renderer.GetMaterialName());
    if (material)
    {
        pixel_shader_constant_table_->SetValue(device, "material_diffuse", &material->diffuse_, sizeof(material->diffuse_));
        UINT index = pixel_shader_constant_table_->GetSamplerIndex("color_texture");
        device->SetSamplerState(index, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        device->SetSamplerState(index, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
        device->SetSamplerState(index, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
        device->SetSamplerState(index, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        device->SetTexture(index, main_system.GetResources().GetTextureManager().Get(material->color_texture_));
    }
}
#endif
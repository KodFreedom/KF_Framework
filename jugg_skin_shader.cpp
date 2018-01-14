//--------------------------------------------------------------------------------
//　jugg_skin_shader.cpp
//	シェーダークラス
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "jugg_skin_shader.h"
#include "main_system.h"
#include "render_system.h"
#include "camera.h"
#include "camera_manager.h"
#include "material_manager.h"
#include "texture_manager.h"
#include "game_object.h"
#include "mesh_renderer_3d_skin.h"
#include "animator.h"
#include "light_manager.h"
#include "shadow_map_system.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void JuggSkinShader::Init(const LPDIRECT3DDEVICE9 device)
{
    CompileShaderFrom(L"default_skin_vertex_shader", L"jugg_skin_pixel_shader", device);
}

//--------------------------------------------------------------------------------
//  使用処理
//--------------------------------------------------------------------------------
void JuggSkinShader::Set(const LPDIRECT3DDEVICE9 device)
{
	ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  使用完了の後片付け
//--------------------------------------------------------------------------------
void JuggSkinShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  定数テーブルの設定
//--------------------------------------------------------------------------------
void JuggSkinShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
	auto main_system = MainSystem::Instance();

	// Camera
	auto camera = main_system->GetCameraManager()->GetMainCamera();
	vertex_shader_constant_table_->SetMatrix(device, "view", &static_cast<D3DXMATRIX>(camera->GetView()));
	vertex_shader_constant_table_->SetMatrix(device, "projection", &static_cast<D3DXMATRIX>(camera->GetProjection()));
	vertex_shader_constant_table_->SetValue(device, "camera_position_world", &camera->GetWorldEyePosition(), sizeof(Vector3));

	// Light
    auto& light = main_system->GetLightManager()->GetDirectionalLights().front();
	vertex_shader_constant_table_->SetValue(device, "light_direction_world", &light->direction_, sizeof(light->direction_));

	// Skin Bone Data
	auto skin_mesh_renderer = (MeshRenderer3dSkin*)(&renderer);
	const auto& bone_texture = skin_mesh_renderer->GetAnimator().GetBoneTexture();
	assert(bone_texture.pointer);
	assert(bone_texture.size);
	vertex_shader_constant_table_->SetFloat(device, "texture_size", static_cast<FLOAT>(bone_texture.size));
	UINT bone_texture_index = vertex_shader_constant_table_->GetSamplerIndex("bone_texture");
	device->SetTexture(D3DVERTEXTEXTURESAMPLER0 + bone_texture_index, bone_texture.pointer);

	// Material
	const auto& material = main_system->GetMaterialManager()->GetMaterial(renderer.GetMaterialName());
	auto texture_manager = main_system->GetTextureManager();
	UINT index = pixel_shader_constant_table_->GetSamplerIndex("color_texture");
	device->SetTexture(index, texture_manager->Get(material->color_texture_));
	index = pixel_shader_constant_table_->GetSamplerIndex("diffuse_texture");
	device->SetTexture(index, texture_manager->Get(material->diffuse_texture_));
	index = pixel_shader_constant_table_->GetSamplerIndex("diffuse_texture_mask");
	device->SetTexture(index, texture_manager->Get(material->diffuse_texture_mask_));
	index = pixel_shader_constant_table_->GetSamplerIndex("specular_texture");
	device->SetTexture(index, texture_manager->Get(material->specular_texture_));
	index = pixel_shader_constant_table_->GetSamplerIndex("specular_texture_mask");
	device->SetTexture(index, texture_manager->Get(material->specular_texture_mask_));
	index = pixel_shader_constant_table_->GetSamplerIndex("normal_texture");
	device->SetTexture(index, texture_manager->Get(material->normal_texture_));
	index = pixel_shader_constant_table_->GetSamplerIndex("detail_texture");
	device->SetTexture(index, texture_manager->Get(material->detail_texture_));
	index = pixel_shader_constant_table_->GetSamplerIndex("detail_mask");
	device->SetTexture(index, texture_manager->Get(material->detail_mask_));
	index = pixel_shader_constant_table_->GetSamplerIndex("tint_by_base_mask");
	device->SetTexture(index, texture_manager->Get(material->tint_by_base_mask_));
	index = pixel_shader_constant_table_->GetSamplerIndex("rim_mask");
	device->SetTexture(index, texture_manager->Get(material->rim_mask_));
	index = pixel_shader_constant_table_->GetSamplerIndex("translucency");
	device->SetTexture(index, main_system->GetTextureManager()->Get(material->translucency_));
	index = pixel_shader_constant_table_->GetSamplerIndex("metalness_mask");
	device->SetTexture(index, main_system->GetTextureManager()->Get(material->metalness_mask_));
	index = pixel_shader_constant_table_->GetSamplerIndex("self_illum_mask");
	device->SetTexture(index, main_system->GetTextureManager()->Get(material->self_illum_mask_));
	//index = pixel_shader_constant_table_->GetSamplerIndex("fresnel_warp_color");
	//device->SetTexture(index, main_system->GetTextureManager()->Get(material->fresnel_warp_color_));
	//index = pixel_shader_constant_table_->GetSamplerIndex("fresnel_warp_rim");
	//device->SetTexture(index, main_system->GetTextureManager()->Get(material->fresnel_warp_rim_));
	//index = pixel_shader_constant_table_->GetSamplerIndex("fresnel_warp_specular");
	//device->SetTexture(index, main_system->GetTextureManager()->Get(material->fresnel_warp_specular_));
	
	// Shadow Map
    auto shadow_map_system = MainSystem::Instance()->GetShadowMapSystem();
	vertex_shader_constant_table_->SetMatrix(device, "view_light", &(D3DXMATRIX)shadow_map_system->GetLightView());
	vertex_shader_constant_table_->SetMatrix(device, "projection_light", &(D3DXMATRIX)shadow_map_system->GetLightProjection());
    pixel_shader_constant_table_->SetFloat(device, "bias", shadow_map_system->GetBias());
    UINT shadow_map_index = pixel_shader_constant_table_->GetSamplerIndex("shadow_map");
	device->SetTexture(shadow_map_index, main_system->GetShadowMapSystem()->GetShadowMap());
}
#endif
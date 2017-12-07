//--------------------------------------------------------------------------------
//　default_shader.cpp
//	シェーダークラス
//	Author : 徐文杰(KodFreedom)
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
//  初期化処理
//--------------------------------------------------------------------------------
void DefaultShader::Init(const LPDIRECT3DDEVICE9 device)
{
	// Vertex Shader
	LPD3DXBUFFER error_buffer = nullptr;
	LPD3DXBUFFER code_buffer = nullptr;
	HRESULT hr = D3DXCompileShaderFromFile(
		L"default_vertex_shader.hlsl",
		NULL,
		NULL,
		"main",	// FunctionNameエントリー関数名
		"vs_3_0", // シェーダーモデル
		0,
		&code_buffer, // バイナリコード
		&error_buffer, // コンパイルが無事済んだらnull、失敗したらエラーメッセージが入ってる
		&vertex_shader_constant_table_);

	if (FAILED(hr))
	{
		string buffer = static_cast<char*>(error_buffer->GetBufferPointer());
		MessageBox(NULL, String(buffer.begin(), buffer.end()).c_str(), L"エラー", MB_OK | MB_ICONWARNING);
		error_buffer->Release();
		return;
	}

	hr = device->CreateVertexShader(
		static_cast<LPDWORD>(code_buffer->GetBufferPointer()),
		&vertex_shader_); // シェーダー管理してくるところ
	
	SAFE_RELEASE(code_buffer);
	SAFE_RELEASE(error_buffer);

	// Pixel Shader
	hr = D3DXCompileShaderFromFile(
		L"default_pixel_shader.hlsl",
		NULL,
		NULL,
		"main",	// FunctionNameエントリー関数名
		"ps_3_0", // シェーダーモデル
		0,
		&code_buffer, // バイナリコード
		&error_buffer, // コンパイルが無事済んだらnull、失敗したらエラーメッセージが入ってる
		&pixel_shader_constant_table_);

	if (FAILED(hr))
	{
		string buffer = static_cast<char*>(error_buffer->GetBufferPointer());
		MessageBox(NULL, String(buffer.begin(), buffer.end()).c_str(), L"エラー", MB_OK | MB_ICONWARNING);
		error_buffer->Release();
		return;
	}

	hr = device->CreatePixelShader(
		static_cast<LPDWORD>(code_buffer->GetBufferPointer()),
		&pixel_shader_ // シェーダー管理してくるところ
	);

	SAFE_RELEASE(code_buffer);
	SAFE_RELEASE(error_buffer);
}

//--------------------------------------------------------------------------------
//  使用処理
//--------------------------------------------------------------------------------
void DefaultShader::Set(const LPDIRECT3DDEVICE9 device)
{
	ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  使用完了の後片付け
//--------------------------------------------------------------------------------
void DefaultShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  定数テーブルの設定
//--------------------------------------------------------------------------------
void DefaultShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
	auto main_system = MainSystem::Instance();
	auto camera = main_system->GetCameraManager()->GetMainCamera();
	auto& view = camera->GetView();
	auto& projection = camera->GetProjection();
	auto& world = renderer.GetGameObject().GetTransform()->GetWorldMatrix();
	auto& world_inverse = world.Inverse();
	
	// Vertex
	D3DXMATRIX world_view_projection = world * view * projection;
	vertex_shader_constant_table_->SetMatrix(device, "world_view_projection", &world_view_projection);
	
	// Pixel
	auto& camera_position_local = Vector3::TransformCoord(camera->GetWorldEyePosition(), world_inverse);
	pixel_shader_constant_table_->SetValue(device, "camera_position_local", &camera_position_local, sizeof(camera_position_local));
	
	auto& light = main_system->GetLightManager()->GetDirectionLights().front();
	auto& light_direction_local = Vector3::TransformNormal(light->direction_, world_inverse);
	pixel_shader_constant_table_->SetValue(device, "light_direction_local", &light_direction_local, sizeof(light_direction_local));
	pixel_shader_constant_table_->SetValue(device, "light_diffuse", &light->diffuse_, sizeof(light->diffuse_));
	pixel_shader_constant_table_->SetValue(device, "light_ambient", &light->ambient_, sizeof(light->ambient_));

	const auto& material = main_system->GetMaterialManager()->GetMaterial(renderer.GetMaterialName());
	auto texture_manager = main_system->GetTextureManager();
	UINT diffuse_texture_index = pixel_shader_constant_table_->GetSamplerIndex("diffuse_texture");
	UINT norma_texture_index = pixel_shader_constant_table_->GetSamplerIndex("normal_texture");
	UINT specular_texture_index = pixel_shader_constant_table_->GetSamplerIndex("specular_texture");
	device->SetTexture(diffuse_texture_index, texture_manager->Get(material->diffuse_texture));
	device->SetTexture(norma_texture_index, texture_manager->Get(material->normal_texture));
	device->SetTexture(specular_texture_index, texture_manager->Get(material->specular_texture));
	pixel_shader_constant_table_->SetValue(device, "material_diffuse", &material->diffuse, sizeof(material->diffuse));
	pixel_shader_constant_table_->SetValue(device, "material_ambient", &material->ambient, sizeof(material->ambient));
	pixel_shader_constant_table_->SetValue(device, "material_emissive", &material->emissive, sizeof(material->emissive));
	pixel_shader_constant_table_->SetValue(device, "material_specular", &material->specular, sizeof(material->specular));
	pixel_shader_constant_table_->SetValue(device, "material_power", &material->power, sizeof(material->power));

	// Shadow Map
	// View行列
	D3DXVECTOR3 light_up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX view_light, projection_light;
	D3DXMatrixLookAtLH(&view_light, &(D3DXVECTOR3)light->position_
		, &(D3DXVECTOR3)(light->at_), &light_up);
	D3DXMatrixOrthoLH(&projection_light, 2048, 2048, light->near_, light->far_);
	//D3DXMatrixPerspectiveFovLH(&projection_light, 75.0f / 180.0 * kPi, (float)2048 / 2048, light->near_, light->far_);
	D3DXMATRIX world_view_projection_light = (D3DXMATRIX)world * view_light * projection_light;
	vertex_shader_constant_table_->SetMatrix(device, "world_view_projection_light", &world_view_projection_light);

	D3DXVECTOR4 offset(
		0.5f / 2048,
		0.5f / 2048,
		0.0f,
		0.0f);
	pixel_shader_constant_table_->SetVector(device, "shadow_map_offset", &offset);
	UINT shadow_map_index = pixel_shader_constant_table_->GetSamplerIndex("shadow_map");
	device->SetTexture(shadow_map_index, main_system->GetShadowMapSystem()->GetShadowMap());
}
#endif
//--------------------------------------------------------------------------------
//　cull_none_shader.cpp
//	シェーダークラス
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "cull_none_shader.h"
#include "main_system.h"
#include "render_system.h"
#include "camera.h"
#include "camera_manager.h"
#include "light_manager.h"
#include "material_manager.h"
#include "texture_manager.h"
#include "game_object.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CullNoneShader::Init(const LPDIRECT3DDEVICE9 device)
{
	// Vertex Shader
	LPD3DXBUFFER error_buffer = nullptr;
	LPD3DXBUFFER code_buffer = nullptr;
	HRESULT hr = D3DXCompileShaderFromFile(
		L"cull_none_vertex_shader.hlsl",
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
		L"cull_none_pixel_shader.hlsl",
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
void CullNoneShader::Set(const LPDIRECT3DDEVICE9 device)
{
	ShaderDirectX9::Set(device);
	MainSystem::Instance()->GetRenderSystem()->SetRenderState(CullMode::KCullNone);
}

//--------------------------------------------------------------------------------
//  使用完了の後片付け
//--------------------------------------------------------------------------------
void CullNoneShader::Reset(const LPDIRECT3DDEVICE9 device)
{
	MainSystem::Instance()->GetRenderSystem()->SetRenderState(CullMode::kCcw);
}

//--------------------------------------------------------------------------------
//  定数テーブルの設定
//--------------------------------------------------------------------------------
void CullNoneShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
	auto main_system = MainSystem::Instance();
	auto camera = main_system->GetCameraManager()->GetMainCamera();
	auto& world = renderer.GetGameObject().GetTransform()->GetWorldMatrix();
	auto& world_inverse = world.Inverse();

	D3DXMATRIX world_view_projection = world * camera->GetView() * camera->GetProjection();
	vertex_shader_constant_table_->SetMatrix(device, "world_view_projection", &world_view_projection);

	auto& camera_position_local = Vector3::TransformCoord(camera->GetWorldEyePosition(), world_inverse);
	pixel_shader_constant_table_->SetValue(device, "camera_position_local", &camera_position_local, sizeof(camera_position_local));

	auto& light = main_system->GetLightManager()->GetShadowMapLight();
	auto& light_direction_local = Vector3::TransformNormal(light.GetDirection(), world_inverse);
	pixel_shader_constant_table_->SetValue(device, "light_direction_local", &light_direction_local, sizeof(light_direction_local));
	pixel_shader_constant_table_->SetValue(device, "light_diffuse", &light.diffuse_, sizeof(light.diffuse_));
	pixel_shader_constant_table_->SetValue(device, "light_ambient", &light.ambient_, sizeof(light.ambient_));

	const auto& material = main_system->GetMaterialManager()->GetMaterial(renderer.GetMaterialName());
	auto texture_manager = main_system->GetTextureManager();
	UINT color_texture_index = pixel_shader_constant_table_->GetSamplerIndex("color_texture");
	device->SetTexture(color_texture_index, texture_manager->Get(material->color_texture_));
	pixel_shader_constant_table_->SetValue(device, "material_diffuse", &material->diffuse_, sizeof(material->diffuse_));
	pixel_shader_constant_table_->SetValue(device, "material_ambient", &material->ambient_, sizeof(material->ambient_));
	pixel_shader_constant_table_->SetValue(device, "material_emissive", &material->emissive_, sizeof(material->emissive_));
	pixel_shader_constant_table_->SetValue(device, "material_specular", &material->specular_, sizeof(material->specular_));
	pixel_shader_constant_table_->SetValue(device, "material_power", &material->power_, sizeof(material->power_));
}
#endif
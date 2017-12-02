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
	
	D3DXMATRIX world_view_projection = world * view * projection;
	vertex_shader_constant_table_->SetMatrix(device, "world_view_projection", &world_view_projection);
	
	auto& camera_position_local = Vector3::TransformCoord(camera->GetWorldEyePosition(), world_inverse);
	pixel_shader_constant_table_->SetValue(device, "camera_position_local", &camera_position_local, sizeof(camera_position_local));
	
	auto& light = main_system->GetLightManager()->GetDirectionLights().front();
	auto& light_direction_local = Vector3::TransformNormal(light->direction_, world_inverse);
	pixel_shader_constant_table_->SetValue(device, "light_direction_local", &light_direction_local, sizeof(light_direction_local));
	pixel_shader_constant_table_->SetValue(device, "light_diffuse", &light->diffuse_, sizeof(light->diffuse_));
	pixel_shader_constant_table_->SetValue(device, "light_ambient", &light->ambient_, sizeof(light->ambient_));

	const auto& material = main_system->GetMaterialManager()->GetMaterial(renderer.GetMaterialName());
	if (material)
	{
		device->SetTexture(0, main_system->GetTextureManager()->Get(material->diffuse_texture));
		device->SetTexture(1, main_system->GetTextureManager()->Get(material->normal_texture));
		device->SetTexture(2, main_system->GetTextureManager()->Get(material->specular_texture));
		pixel_shader_constant_table_->SetValue(device, "material_diffuse", &material->diffuse, sizeof(material->diffuse));
		pixel_shader_constant_table_->SetValue(device, "material_ambient", &material->ambient, sizeof(material->ambient));
		pixel_shader_constant_table_->SetValue(device, "material_emissive", &material->emissive, sizeof(material->emissive));
		pixel_shader_constant_table_->SetValue(device, "material_specular", &material->specular, sizeof(material->specular));
		pixel_shader_constant_table_->SetValue(device, "material_power", &material->power, sizeof(material->power));
	}
	else
	{
		float power = 1.0f;
		device->SetTexture(0, nullptr);
		device->SetTexture(1, nullptr);
		device->SetTexture(2, nullptr);
		pixel_shader_constant_table_->SetValue(device, "material_diffuse", &Color::kWhite, sizeof(Color::kWhite));
		pixel_shader_constant_table_->SetValue(device, "material_ambient", &Color::kGray, sizeof(Color::kGray));
		pixel_shader_constant_table_->SetValue(device, "material_emissive", &Color::kBlack, sizeof(Color::kBlack));
		pixel_shader_constant_table_->SetValue(device, "material_specular", &Color::kBlack, sizeof(Color::kBlack));
		pixel_shader_constant_table_->SetValue(device, "material_power", &power, sizeof(power));
	}
}
#endif
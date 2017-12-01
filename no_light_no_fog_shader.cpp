//--------------------------------------------------------------------------------
//　no_light_no_fog_shader.cpp
//	シェーダークラス
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "no_light_no_fog_shader.h"
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
void NoLightNoFogShader::Init(const LPDIRECT3DDEVICE9 device)
{
	// Vertex Shader
	LPD3DXBUFFER error_buffer = nullptr;
	LPD3DXBUFFER code_buffer = nullptr;
	HRESULT hr = D3DXCompileShaderFromFile(
		L"perPixelLightingVertexShader.hlsl",
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
		assert("failed to compile");
		error_buffer->Release();
		return;
	}

	hr = device->CreateVertexShader(
		(LPDWORD)code_buffer->GetBufferPointer(),
		&vertex_shader_); // シェーダー管理してくるところ

	SAFE_RELEASE(code_buffer);
	SAFE_RELEASE(error_buffer);

	// Pixel Shader
	hr = D3DXCompileShaderFromFile(
		L"perPixelLightingPixelShader.hlsl",
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
		assert("failed to compile");
		error_buffer->Release();
		return;
	}

	hr = device->CreatePixelShader(
		(LPDWORD)code_buffer->GetBufferPointer(),
		&pixel_shader_ // シェーダー管理してくるところ
	);

	SAFE_RELEASE(code_buffer);
	SAFE_RELEASE(error_buffer);
}

//--------------------------------------------------------------------------------
//  使用処理
//--------------------------------------------------------------------------------
void NoLightNoFogShader::Set(const LPDIRECT3DDEVICE9 device)
{
	ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  使用完了の後片付け
//--------------------------------------------------------------------------------
void NoLightNoFogShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  定数テーブルの設定
//--------------------------------------------------------------------------------
void NoLightNoFogShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
	auto main_system = MainSystem::Instance();
	auto camera = main_system->GetCameraManager()->GetMainCamera();
	auto& view = camera->GetView();
	auto& projection = camera->GetProjection();
	auto& world = renderer.GetGameObject().GetTransform()->GetWorldMatrix();

	D3DXMATRIX world_view_projection = world * view * projection;
	vertex_shader_constant_table_->SetMatrix(device, "world_view_projection", &world_view_projection);

	const auto& material = main_system->GetMaterialManager()->GetMaterial(renderer.GetMaterialName());
	if (material)
	{
		device->SetTexture(0, main_system->GetTextureManager()->Get(material->diffuse_texture));
	}
	else
	{
		device->SetTexture(0, nullptr);
	}
}
#endif
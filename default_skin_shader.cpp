//--------------------------------------------------------------------------------
//　default_skin_shader.cpp
//	シェーダークラス
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "default_skin_shader.h"
#include "main_system.h"
#include "render_system.h"
#include "camera.h"
#include "camera_manager.h"
#include "material_manager.h"
#include "texture_manager.h"
#include "game_object.h"
#include "mesh_renderer_3d_skin.h"
#include "animator.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void DefaultSkinShader::Init(const LPDIRECT3DDEVICE9 device)
{
	// Vertex Shader
	LPD3DXBUFFER error_buffer = nullptr;
	LPD3DXBUFFER code_buffer = nullptr;
	HRESULT hr = D3DXCompileShaderFromFile(
		L"default_skin_vertex_shader.hlsl",
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
		L"default_skin_pixel_shader.hlsl",
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
void DefaultSkinShader::Set(const LPDIRECT3DDEVICE9 device)
{
	ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  使用完了の後片付け
//--------------------------------------------------------------------------------
void DefaultSkinShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  定数テーブルの設定
//--------------------------------------------------------------------------------
void DefaultSkinShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
	auto main_system = MainSystem::Instance();
	auto camera = main_system->GetCameraManager()->GetMainCamera();
	auto& world = Matrix44::kIdentity;
	auto& view = camera->GetView();
	auto& projection = camera->GetProjection();
	D3DXMATRIX world_view_projection = world * view * projection;
	vertex_shader_constant_table_->SetMatrix(device, "world_view_projection", &world_view_projection);
	
	auto skin_mesh_renderer = (MeshRenderer3dSkin*)(&renderer);
	const auto& bone_texture = skin_mesh_renderer->GetAnimator().GetBoneTexture();
	assert(bone_texture.pointer);
	assert(bone_texture.size);
	vertex_shader_constant_table_->SetFloat(device, "texture_size", static_cast<FLOAT>(bone_texture.size));
	UINT bone_texture_index = vertex_shader_constant_table_->GetSamplerIndex("bone_texture");
	device->SetTexture(D3DVERTEXTEXTURESAMPLER0 + bone_texture_index, bone_texture.pointer);

	const auto& material = main_system->GetMaterialManager()->GetMaterial(renderer.GetMaterialName());
	UINT diffuse_texture_index = pixel_shader_constant_table_->GetSamplerIndex("diffuse_texture");
	device->SetTexture(diffuse_texture_index, main_system->GetTextureManager()->Get(material->diffuse_texture));
}
#endif
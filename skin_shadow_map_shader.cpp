//--------------------------------------------------------------------------------
//　shadow_map_shader
//	シェーダークラス
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "skin_shadow_map_shader.h"
#include "main_system.h"
#include "render_system.h"
#include "camera.h"
#include "camera_manager.h"
#include "material_manager.h"
#include "texture_manager.h"
#include "game_object.h"
#include "mesh_renderer_3d_skin.h"
#include "animator.h"
#include "shadow_map_system.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void SkinShadowMapShader::Init(const LPDIRECT3DDEVICE9 device)
{
	// Vertex Shader
	LPD3DXBUFFER error_buffer = nullptr;
	LPD3DXBUFFER code_buffer = nullptr;
	HRESULT hr = D3DXCompileShaderFromFile(
		L"basic_skin_shadow_map_vertex_shader.hlsl",
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
		L"basic_shadow_map_pixel_shader.hlsl",
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
void SkinShadowMapShader::Set(const LPDIRECT3DDEVICE9 device)
{
	ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  使用完了の後片付け
//--------------------------------------------------------------------------------
void SkinShadowMapShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  定数テーブルの設定
//--------------------------------------------------------------------------------
void SkinShadowMapShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
    auto shadow_map_system = MainSystem::Instance()->GetShadowMapSystem();
	vertex_shader_constant_table_->SetMatrix(device, "view_light", &static_cast<D3DXMATRIX>(shadow_map_system->GetLightView()));
	vertex_shader_constant_table_->SetMatrix(device, "projection_light", &static_cast<D3DXMATRIX>(shadow_map_system->GetLightProjection()));

	// bone
	auto skin_mesh_renderer = (MeshRenderer3dSkin*)(&renderer);
	const auto& bone_texture = skin_mesh_renderer->GetAnimator().GetBoneTexture();
	assert(bone_texture.pointer);
	assert(bone_texture.size);
	vertex_shader_constant_table_->SetFloat(device, "texture_size", static_cast<FLOAT>(bone_texture.size));
	UINT bone_texture_index = vertex_shader_constant_table_->GetSamplerIndex("bone_texture");
	device->SetTexture(D3DVERTEXTEXTURESAMPLER0 + bone_texture_index, bone_texture.pointer);
}
#endif
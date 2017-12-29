//--------------------------------------------------------------------------------
//　shader_directX9.cpp
//	シェーダークラス
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "shader_directX9.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "kf_utility.h"
//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void ShaderDirectX9::Uninit(void)
{
	SAFE_RELEASE(vertex_shader_);
	SAFE_RELEASE(vertex_shader_constant_table_);
	SAFE_RELEASE(pixel_shader_);
	SAFE_RELEASE(pixel_shader_constant_table_);
}

//--------------------------------------------------------------------------------
//  使用処理
//--------------------------------------------------------------------------------
void ShaderDirectX9::Set(const LPDIRECT3DDEVICE9 device)
{
	device->SetVertexShader(vertex_shader_);
	device->SetPixelShader(pixel_shader_);
}

//--------------------------------------------------------------------------------
//  読込処理
//--------------------------------------------------------------------------------
void ShaderDirectX9::CompileShaderFrom(const String& shader_name, const LPDIRECT3DDEVICE9 device)
{
    // Vertex Shader
    LPD3DXBUFFER error_buffer = nullptr;
    LPD3DXBUFFER code_buffer = nullptr;
    String& file_path = L"data/shader/" + shader_name + L"_vertex_shader.hlsl";
    HRESULT hr = D3DXCompileShaderFromFile(
        file_path.c_str(),
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
    file_path = L"data/shader/" + shader_name + L"_pixel_shader.hlsl";
    hr = D3DXCompileShaderFromFile(
        file_path.c_str(),
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
#endif
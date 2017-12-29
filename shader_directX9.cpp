//--------------------------------------------------------------------------------
//�@shader_directX9.cpp
//	�V�F�[�_�[�N���X
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "shader_directX9.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "kf_utility.h"
//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void ShaderDirectX9::Uninit(void)
{
	SAFE_RELEASE(vertex_shader_);
	SAFE_RELEASE(vertex_shader_constant_table_);
	SAFE_RELEASE(pixel_shader_);
	SAFE_RELEASE(pixel_shader_constant_table_);
}

//--------------------------------------------------------------------------------
//  �g�p����
//--------------------------------------------------------------------------------
void ShaderDirectX9::Set(const LPDIRECT3DDEVICE9 device)
{
	device->SetVertexShader(vertex_shader_);
	device->SetPixelShader(pixel_shader_);
}

//--------------------------------------------------------------------------------
//  �Ǎ�����
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
        "main",	// FunctionName�G���g���[�֐���
        "vs_3_0", // �V�F�[�_�[���f��
        0,
        &code_buffer, // �o�C�i���R�[�h
        &error_buffer, // �R���p�C���������ς񂾂�null�A���s������G���[���b�Z�[�W�������Ă�
        &vertex_shader_constant_table_);

    if (FAILED(hr))
    {
        string buffer = static_cast<char*>(error_buffer->GetBufferPointer());
        MessageBox(NULL, String(buffer.begin(), buffer.end()).c_str(), L"�G���[", MB_OK | MB_ICONWARNING);
        error_buffer->Release();
        return;
    }

    hr = device->CreateVertexShader(
        static_cast<LPDWORD>(code_buffer->GetBufferPointer()),
        &vertex_shader_); // �V�F�[�_�[�Ǘ����Ă���Ƃ���

    SAFE_RELEASE(code_buffer);
    SAFE_RELEASE(error_buffer);

    // Pixel Shader
    file_path = L"data/shader/" + shader_name + L"_pixel_shader.hlsl";
    hr = D3DXCompileShaderFromFile(
        file_path.c_str(),
        NULL,
        NULL,
        "main",	// FunctionName�G���g���[�֐���
        "ps_3_0", // �V�F�[�_�[���f��
        0,
        &code_buffer, // �o�C�i���R�[�h
        &error_buffer, // �R���p�C���������ς񂾂�null�A���s������G���[���b�Z�[�W�������Ă�
        &pixel_shader_constant_table_);

    if (FAILED(hr))
    {
        string buffer = static_cast<char*>(error_buffer->GetBufferPointer());
        MessageBox(NULL, String(buffer.begin(), buffer.end()).c_str(), L"�G���[", MB_OK | MB_ICONWARNING);
        error_buffer->Release();
        return;
    }

    hr = device->CreatePixelShader(
        static_cast<LPDWORD>(code_buffer->GetBufferPointer()),
        &pixel_shader_ // �V�F�[�_�[�Ǘ����Ă���Ƃ���
    );

    SAFE_RELEASE(code_buffer);
    SAFE_RELEASE(error_buffer);
}
#endif
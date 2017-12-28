//--------------------------------------------------------------------------------
//�@default_2d_shader.h
//	2d�V�F�[�_�[�N���X
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "default_2d_shader.h"
#include "main_system.h"
#include "material_manager.h"
#include "game_object.h"
#include "render_system.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void Default2dShader::Init(const LPDIRECT3DDEVICE9 device)
{
    // Vertex Shader
    LPD3DXBUFFER error_buffer = nullptr;
    LPD3DXBUFFER code_buffer = nullptr;
    HRESULT hr = D3DXCompileShaderFromFile(
        L"data/shader/default_2d_vertex_shader.hlsl",
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
    hr = D3DXCompileShaderFromFile(
        L"data/shader/default_2d_pixel_shader.hlsl",
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

//--------------------------------------------------------------------------------
//  �g�p����
//--------------------------------------------------------------------------------
void Default2dShader::Set(const LPDIRECT3DDEVICE9 device)
{
    ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  �g�p�����̌�Еt��
//--------------------------------------------------------------------------------
void Default2dShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  �萔�e�[�u���̐ݒ�
//--------------------------------------------------------------------------------
void Default2dShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
    // Vertex
    const Matrix44& world = renderer.GetGameObject().GetTransform()->GetWorldMatrix();
    vertex_shader_constant_table_->SetValue(device, "world", &world, sizeof(world));
    vertex_shader_constant_table_->SetValue(device, "projection", &RenderSystem::kProjection2d, sizeof(RenderSystem::kProjection2d));
    vertex_shader_constant_table_->SetValue(device, "offset", &RenderSystem::kOffset2d, sizeof(RenderSystem::kOffset2d));

    // Pixel
    const auto& material = MainSystem::Instance()->GetMaterialManager()->GetMaterial(renderer.GetMaterialName());
    pixel_shader_constant_table_->SetValue(device, "material_diffuse", &material->diffuse_, sizeof(material->diffuse_));
}
#endif
//--------------------------------------------------------------------------------
//�@no_light_no_fog_shader.cpp
//	�V�F�[�_�[�N���X
//	Author : �����^(KodFreedom)
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
//  ����������
//--------------------------------------------------------------------------------
void NoLightNoFogShader::Init(const LPDIRECT3DDEVICE9 device)
{
	// Vertex Shader
	LPD3DXBUFFER error_buffer = nullptr;
	LPD3DXBUFFER code_buffer = nullptr;
	HRESULT hr = D3DXCompileShaderFromFile(
		L"no_light_no_fog_vertex_shader.hlsl",
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
		L"no_light_no_fog_pixel_shader.hlsl",
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
void NoLightNoFogShader::Set(const LPDIRECT3DDEVICE9 device)
{
	ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  �g�p�����̌�Еt��
//--------------------------------------------------------------------------------
void NoLightNoFogShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  �萔�e�[�u���̐ݒ�
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
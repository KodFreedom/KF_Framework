//--------------------------------------------------------------------------------
//�@jugg_skin_shader.cpp
//	�V�F�[�_�[�N���X
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "jugg_skin_shader.h"
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
//  ����������
//--------------------------------------------------------------------------------
void JuggSkinShader::Init(const LPDIRECT3DDEVICE9 device)
{
	// Vertex Shader
	LPD3DXBUFFER error_buffer = nullptr;
	LPD3DXBUFFER code_buffer = nullptr;
	HRESULT hr = D3DXCompileShaderFromFile(
		L"jugg_skin_vertex_shader.hlsl",
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
		L"jugg_skin_pixel_shader.hlsl",
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
void JuggSkinShader::Set(const LPDIRECT3DDEVICE9 device)
{
	ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  �g�p�����̌�Еt��
//--------------------------------------------------------------------------------
void JuggSkinShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  �萔�e�[�u���̐ݒ�
//--------------------------------------------------------------------------------
void JuggSkinShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
	auto main_system = MainSystem::Instance();
	auto camera = main_system->GetCameraManager()->GetMainCamera();
	vertex_shader_constant_table_->SetMatrix(device, "view", &static_cast<D3DXMATRIX>(camera->GetView()));
	vertex_shader_constant_table_->SetMatrix(device, "projection", &static_cast<D3DXMATRIX>(camera->GetProjection()));

	auto skin_mesh_renderer = (MeshRenderer3dSkin*)(&renderer);
	const auto& bone_texture = skin_mesh_renderer->GetAnimator().GetBoneTexture();
	assert(bone_texture.pointer);
	assert(bone_texture.size);
	vertex_shader_constant_table_->SetFloat(device, "texture_size", static_cast<FLOAT>(bone_texture.size));
	UINT bone_texture_index = vertex_shader_constant_table_->GetSamplerIndex("bone_texture");
	device->SetTexture(D3DVERTEXTEXTURESAMPLER0 + bone_texture_index, bone_texture.pointer);

	const auto& material = main_system->GetMaterialManager()->GetMaterial(renderer.GetMaterialName());
	UINT color_texture_index = pixel_shader_constant_table_->GetSamplerIndex("color_texture");
	device->SetTexture(color_texture_index, main_system->GetTextureManager()->Get(material->color_texture));
}
#endif
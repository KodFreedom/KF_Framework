//--------------------------------------------------------------------------------
//�@shadow_map_shader
//	�V�F�[�_�[�N���X
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "shadow_map_shader.h"
#include "main_system.h"
#include "render_system.h"
#include "camera.h"
#include "camera_manager.h"
#include "material_manager.h"
#include "texture_manager.h"
#include "game_object.h"
#include "light_manager.h"
#include "light.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void ShadowMapShader::Init(const LPDIRECT3DDEVICE9 device)
{
	// Vertex Shader
	LPD3DXBUFFER error_buffer = nullptr;
	LPD3DXBUFFER code_buffer = nullptr;
	HRESULT hr = D3DXCompileShaderFromFile(
		L"basic_shadow_map_vertex_shader.hlsl",
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
		L"basic_shadow_map_pixel_shader.hlsl",
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
void ShadowMapShader::Set(const LPDIRECT3DDEVICE9 device)
{
	ShaderDirectX9::Set(device);
}

//--------------------------------------------------------------------------------
//  �g�p�����̌�Еt��
//--------------------------------------------------------------------------------
void ShadowMapShader::Reset(const LPDIRECT3DDEVICE9 device)
{

}

//--------------------------------------------------------------------------------
//  �萔�e�[�u���̐ݒ�
//--------------------------------------------------------------------------------
void ShadowMapShader::SetConstantTable(const LPDIRECT3DDEVICE9 device, const MeshRenderer& renderer)
{
	// View�s��
	auto& light = MainSystem::Instance()->GetLightManager()->GetDirectionLights().front();
	D3DXVECTOR3 light_up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX view_light, projection_light;
	auto& world = renderer.GetGameObject().GetTransform()->GetWorldMatrix();
	//D3DXMatrixTranslation(&world_light, light_pos.x, light_pos.y, light_pos.z);
	D3DXMatrixLookAtLH(&view_light, &(D3DXVECTOR3)light->position_
		, &(D3DXVECTOR3)(light->at_) , &light_up);
	//D3DXMatrixOrthoLH(&projection_light, 2048, 2048, light->near_, light->far_);
	D3DXMatrixPerspectiveFovLH(&projection_light, 75.0f / 180.0 * kPi, (float)2048 / 2048, light->near_, light->far_);
	D3DXMATRIX world_view_projection_light = (D3DXMATRIX)world * view_light * projection_light;
	vertex_shader_constant_table_->SetMatrix(device, "world_view_projection_light", &world_view_projection_light);
}
#endif
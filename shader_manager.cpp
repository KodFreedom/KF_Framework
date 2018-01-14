//--------------------------------------------------------------------------------
//�@shader_manager.cpp
//	�V�F�[�_�[�Ǘ���
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "shader_manager.h"
#include "mesh_renderer.h"
#include "kf_utility.h"
using namespace kodfreedom;

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "default_shader.h"
#include "no_light_no_fog_shader.h"
#include "cull_none_shader.h"
#include "default_skin_shader.h"
#include "shadow_map_shader.h"
#include "jugg_mesh_shader.h"
#include "jugg_skin_shader.h"
#include "skin_shadow_map_shader.h"
#include "default_2d_shader.h"
#include "default_2d_texture_shader.h"
#include "zombie_skin_shader.h"
#endif

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �V�F�[�_�[�̎g�p
//--------------------------------------------------------------------------------
void ShaderManager::Set(const ShaderType& type)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	shaders_[type]->Set(device_);
#endif
}

//--------------------------------------------------------------------------------
//  �V�F�[�_�[�g�p�����̌�Ђ�
//--------------------------------------------------------------------------------
void ShaderManager::Reset(const ShaderType& type)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	shaders_[type]->Reset(device_);
#endif
}

//--------------------------------------------------------------------------------
//  �V�F�[�_�[�̒萔�e�[�u���̐ݒ�
//--------------------------------------------------------------------------------
void ShaderManager::SetConstantTable(const ShaderType& type, const MeshRenderer& renderer)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    shaders_[type]->SetConstantTable(device_, renderer);
#endif
}

//--------------------------------------------------------------------------------
//  �V�F�[�_�[�̎g�p
//--------------------------------------------------------------------------------
void ShaderManager::Set(const ShadowMapShaderType& type)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	shadow_map_shaders_[type]->Set(device_);
#endif
}

//--------------------------------------------------------------------------------
//  �V�F�[�_�[�g�p�����̌�Ђ�
//--------------------------------------------------------------------------------
void ShaderManager::Reset(const ShadowMapShaderType& type)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	shadow_map_shaders_[type]->Reset(device_);
#endif
}

//--------------------------------------------------------------------------------
//  �V�F�[�_�[�̒萔�e�[�u���̐ݒ�
//--------------------------------------------------------------------------------
void ShaderManager::SetConstantTable(const ShadowMapShaderType& type, const MeshRenderer& renderer)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    shadow_map_shaders_[type]->SetConstantTable(device_, renderer);
#endif
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void ShaderManager::Init(void)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	shaders_[kDefaultShader] = MY_NEW DefaultShader;
	shaders_[kDefaultShader]->Init(device_);
	shaders_[kNoLightNoFog] = MY_NEW NoLightNoFogShader;
	shaders_[kNoLightNoFog]->Init(device_);
	shaders_[kCullNone] = MY_NEW CullNoneShader;
	shaders_[kCullNone]->Init(device_);
	shaders_[kDefaultSkinShader] = MY_NEW DefaultSkinShader;
	shaders_[kDefaultSkinShader]->Init(device_);
	shaders_[kJuggernautMeshShader] = MY_NEW JuggMeshShader;
	shaders_[kJuggernautMeshShader]->Init(device_);
	shaders_[kJuggernautSkinShader] = MY_NEW JuggSkinShader;
	shaders_[kJuggernautSkinShader]->Init(device_);
    shaders_[kDefault2dShader] = MY_NEW Default2dShader;
    shaders_[kDefault2dShader]->Init(device_);
    shaders_[kDefault2dTextureShader] = MY_NEW Default2dTextureShader;
    shaders_[kDefault2dTextureShader]->Init(device_);
    shaders_[kZombieSkinShader] = MY_NEW ZombieSkinShader;
    shaders_[kZombieSkinShader]->Init(device_);
	shadow_map_shaders_[kBasicShadowMapShader] = MY_NEW ShadowMapShader;
	shadow_map_shaders_[kBasicShadowMapShader]->Init(device_);
	shadow_map_shaders_[kBasicSkinShadowMapShader] = MY_NEW SkinShadowMapShader;
	shadow_map_shaders_[kBasicSkinShadowMapShader]->Init(device_);
#endif
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void ShaderManager::Uninit(void)
{
	for (auto& shader : shaders_)
	{
		SAFE_UNINIT(shader);
	}

	for (auto& shader : shadow_map_shaders_)
	{
		SAFE_UNINIT(shader);
	}
}
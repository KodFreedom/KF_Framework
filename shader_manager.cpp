//--------------------------------------------------------------------------------
//　shader_manager.cpp
//	シェーダー管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "shader_manager.h"
#include "mesh_renderer.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "default_shader.h"
#include "no_light_no_fog_shader.h"
#include "cull_none_shader.h"
#endif

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  シェーダーの使用
//--------------------------------------------------------------------------------
void ShaderManager::Set(const ShaderType& type)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	shaders_[type]->Set(device_);
#endif
}

//--------------------------------------------------------------------------------
//  シェーダー使用完了の後片つけ
//--------------------------------------------------------------------------------
void ShaderManager::Reset(const ShaderType& type)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	shaders_[type]->Reset(device_);
#endif
}

//--------------------------------------------------------------------------------
//  シェーダーの定数テーブルの設定
//--------------------------------------------------------------------------------
void ShaderManager::SetConstantTable(const MeshRenderer& renderer)
{
	shaders_[renderer.GetShaderType()]->SetConstantTable(device_, renderer);
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void ShaderManager::Init(void)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	shaders_[kDefaultShader] = MY_NEW DefaultShader();
	shaders_[kDefaultShader]->Init(device_);
	shaders_[kNoLightNoFog] = MY_NEW NoLightNoFogShader();
	shaders_[kNoLightNoFog]->Init(device_);
	shaders_[kCullNone] = MY_NEW CullNoneShader();
	shaders_[kCullNone]->Init(device_);
#endif
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void ShaderManager::Uninit(void)
{
	for (auto& shader : shaders_)
	{
		SAFE_UNINIT(shader);
	}
}
//--------------------------------------------------------------------------------
//　shader_manager.h
//	シェーダー管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#include "shader_enum.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class MeshRenderer;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
class ShaderDirectX9;
#endif

//--------------------------------------------------------------------------------
//  クラス定義
//--------------------------------------------------------------------------------
class ShaderManager
{
public:
	//--------------------------------------------------------------------------------
	//  生成処理
	//  return : MaterialManager*
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	static ShaderManager* Create(const LPDIRECT3DDEVICE9 device)
	{
		auto instance = MY_NEW ShaderManager(device);
		instance->Init();
		return instance;
	}
#endif

	//--------------------------------------------------------------------------------
	//  破棄処理
	//--------------------------------------------------------------------------------
	void Release(void) 
    {
        Uninit();
        MY_DELETE this;
    }

	//--------------------------------------------------------------------------------
	//  シェーダーの使用
	//--------------------------------------------------------------------------------
	void Set(const ShaderType& type);

	//--------------------------------------------------------------------------------
	//  シェーダーの使用
	//--------------------------------------------------------------------------------
	void Set(const ShadowMapShaderType& type);

	//--------------------------------------------------------------------------------
	//  シェーダー使用完了の後片つけ
	//--------------------------------------------------------------------------------
	void Reset(const ShaderType& type);

	//--------------------------------------------------------------------------------
	//  シェーダー使用完了の後片つけ
	//--------------------------------------------------------------------------------
	void Reset(const ShadowMapShaderType& type);

	//--------------------------------------------------------------------------------
	//  シェーダーの定数テーブルの設定
	//--------------------------------------------------------------------------------
	void SetConstantTable(const ShaderType& type, const MeshRenderer& renderer);

	//--------------------------------------------------------------------------------
	//  シェーダーの定数テーブルの設定
	//--------------------------------------------------------------------------------
	void SetConstantTable(const ShadowMapShaderType& type, const MeshRenderer& renderer);

private:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	ShaderManager(const LPDIRECT3DDEVICE9 device) : device_(device)
	{
		for (auto& shader : shaders_) shader = nullptr;
		for (auto& shader : shadow_map_shaders_) shader = nullptr;
	}
	ShaderManager() : device_(nullptr) {}
	ShaderManager(const ShaderManager& value) : device_(nullptr) {}
#else
	ShaderManager() {}
	ShaderManager(const ShaderManager& value) {}
#endif
	ShaderManager& operator=(const ShaderManager& value) {}
	~ShaderManager() {}

	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	void Init(void);

	//--------------------------------------------------------------------------------
	//  終了処理
	//--------------------------------------------------------------------------------
	void Uninit(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	ShaderDirectX9* shaders_[ShaderType::kShaderMax];
	ShaderDirectX9* shadow_map_shaders_[ShadowMapShaderType::kShadowMapShaderMax];
	const LPDIRECT3DDEVICE9 device_; // directx9のディバイス
#endif
};
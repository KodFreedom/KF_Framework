//--------------------------------------------------------------------------------
//	マテリアルマネージャ
//　materialManager.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  クラス定義
//--------------------------------------------------------------------------------
class Material
{
public:
	Material()
		: Ambient(Color::White)
		, Diffuse(Color::White)
		, Specular(Color::Black)
		, Emissive(Color::Black)
		, Power(1.0f)
	{
		MainTexture.clear();
	}
	~Material() {}

	string	MainTexture;// テクスチャ
	Color	Ambient;	// 環境光の反射率
	Color	Diffuse;	// 漫射光の反射率
	Color	Specular;	// 鏡面光の反射率
	Color	Emissive;	// 自発光
	float	Power;		// ハイライトのシャープネス

	//キャスト
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	operator D3DMATERIAL9 () const;
#endif
};

//--------------------------------------------------------------------------------
//	マテリアルマネージャ
//--------------------------------------------------------------------------------
class MaterialManager
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	static auto	Create(void)
	{
		if (instance) return instance;
		instance = new MaterialManager;
		instance->init();
		return instance;
	}
	static void Release(void) { SAFE_UNINIT(instance); }
	static auto Instance(void) { return instance; }

	void		Use(const string& materialName);
	void		Disuse(const string& materialName);
	Material*	GetMaterial(const string& materialName)
	{
		auto iterator = materials.find(materialName);
		if (materials.end() == iterator) return nullptr;
		return iterator->second.Pointer;
	}
	void		CreateMaterialFileBy(const string& materialName, const string& mainTextureName, const Color& ambient = Color::Gray, const Color& diffuse = Color::White, const Color& specular = Color::Black, const Color& emissive = Color::Black, const float& power = 1.0f);

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct MaterialInfo
	{
		MaterialInfo() : UserNumber(1), Pointer(nullptr) {}
		int			UserNumber;
		Material*	Pointer;
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	MaterialManager();
	~MaterialManager() {}
	void		init(void) {}
	void		uninit(void) { materials.clear(); }
	Material*	loadFrom(const string& materialName);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	unordered_map<string, MaterialInfo>		materials;
	static MaterialManager*					instance;
};
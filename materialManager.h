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
	Material(const Color& ambient, const Color& diffuse, const Color& specular, const Color& emissive, const float& power)
		: ambient(ambient)
		, diffuse(diffuse)
		, specular(specular)
		, emissive(emissive)
		, power(power)
	{}
	~Material() {}

	Color	ambient;	// 環境光の反射率
	Color	diffuse;	// 漫射光の反射率
	Color	specular;	// 鏡面光の反射率
	Color	emissive;	// 自発光
	float	power;		// ハイライトのシャープネス

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
	void		Use(const string& materialName, const Color &ambient, const Color &diffuse, const Color &specular, const Color &emissive, const float &power);
	void		Disuse(const string& materialName);
	Material*	GetMaterial(const string& materialName)
	{
		auto iterator = materials.find(materialName);
		if (materials.end() == iterator) return nullptr;
		return iterator->second.Pointer;
	}

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct MaterialInfo
	{
		MaterialInfo()
			: UserNumber(1)
			, Pointer(nullptr) {}
		int			UserNumber;
		Material*	Pointer;
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	MaterialManager();
	~MaterialManager() {}
	void init(void) {}
	void uninit(void) { materials.clear(); }

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	unordered_map<string, MaterialInfo>		materials;
	static MaterialManager*					instance;
};
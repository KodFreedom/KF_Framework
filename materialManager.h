//--------------------------------------------------------------------------------
//	マテリアルマネージャ
//　materialManager.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  クラス定義
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	マテリアル
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

	//operator
	bool operator==(const Material& material) const;
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

	const unsigned short	SaveMaterial(const Color &ambient, const Color &diffuse, const Color &specular, const Color &emissive, const float &power);
	const Material&			GetMaterial(const unsigned short& usID);

private:
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
	unordered_map<unsigned short, Material> materials;
	static MaterialManager*					instance;
};
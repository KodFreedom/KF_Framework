//--------------------------------------------------------------------------------
//　material_manager.h
//  manage the materials' save, load
//	マテリアル管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  マテリアル
//--------------------------------------------------------------------------------
class Material
{
public:
	Material(const Color& ambient = Color::kGray
		, const Color& diffuse = Color::kWhite
		, const Color& specular = Color::kBlack
		, const Color& emissive = Color::kBlack
		, const float& power = 1.0f)
		: ambient(ambient), diffuse(diffuse)
		, specular(specular), emissive(emissive)
		, power(power) {}
	Material(const String& diffuse_texture
		, const String& specular_texture = String()
		, const String& normal_texture = String()
		, const Color& ambient = Color::kGray
		, const Color& diffuse = Color::kWhite
		, const Color& specular = Color::kBlack
		, const Color& emissive = Color::kBlack
		, const float& power = 1.0f)
		: diffuse_texture(diffuse_texture)
		, specular_texture(specular_texture)
		, normal_texture(normal_texture)
		, ambient(ambient), diffuse(diffuse)
		, specular(specular), emissive(emissive)
		, power(power) {}
	~Material() {}

	String diffuse_texture;
	String specular_texture;
	String normal_texture;
	Color ambient; // 環境光の反射率
	Color diffuse; // 漫射光の反射率
	Color specular; // 鏡面光の反射率
	Color emissive; // 自発光
	float power; // ハイライトのシャープネス
};

//--------------------------------------------------------------------------------
//	マテリアルマネージャ
//--------------------------------------------------------------------------------
class MaterialManager
{
public:
	//--------------------------------------------------------------------------------
	//  生成処理
	//  return : MaterialManager*
	//--------------------------------------------------------------------------------
	static MaterialManager* Create(void)
	{
		auto instance = MY_NEW MaterialManager();
		instance->Init();
		return instance;
	}

	//--------------------------------------------------------------------------------
	//  破棄処理
	//--------------------------------------------------------------------------------
	void Release(void) { Uninit(); }

	//--------------------------------------------------------------------------------
	//  与えられた名前のマテリアルを使う
	//  すでにあるの場合ユーザーを1たす、ないの場合ファイルから読み込む
	//  material_name : マテリアル名
	//--------------------------------------------------------------------------------
	void Use(const String& material_name);

	//--------------------------------------------------------------------------------
	//  与えられた名前のマテリアルを使う
	//  すでにあるの場合ユーザーを1たす、ないの場合与えられた値で生成する
	//  material_name : マテリアル名
	//  material : マテリアル情報
	//--------------------------------------------------------------------------------
	void Use(const String& material_name, Material* material);

	//--------------------------------------------------------------------------------
	//  与えられた名前のマテリアルを使わない
	//  ユーザーが-1になる、0になった場合メモリから破棄する
	//  material_name : マテリアル名
	//--------------------------------------------------------------------------------
	void Disuse(const String& material_name);

	//--------------------------------------------------------------------------------
	//  与えられた名前のマテリアルのポインタを取得
	//  material_name : テクスチャ名
	//  return : const Material*
	//--------------------------------------------------------------------------------
	const Material* GetMaterial(const String& material_name) const
	{
		auto iterator = materials_.find(material_name);
		if (materials_.end() == iterator)
		{// ないの場合デフォルトのマテリアルを返す
			return &kDefaultMaterial;
		}
		return iterator->second.pointer;
	}

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct MaterialInfo
	{
		MaterialInfo() : user_number(1), pointer(nullptr) {}
		int	user_number;
		Material* pointer;
	};

	//--------------------------------------------------------------------------------
	//  静的メンバ変数
	//--------------------------------------------------------------------------------
	static const Material kDefaultMaterial;

	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	MaterialManager() {}
	MaterialManager(const MaterialManager& value) {}
	MaterialManager& operator=(const MaterialManager& value) {}
	~MaterialManager() {}

	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	void Init(void);

	//--------------------------------------------------------------------------------
	//  終了処理
	//--------------------------------------------------------------------------------
	void Uninit(void);

	//--------------------------------------------------------------------------------
	//  ファイルからマテリアルの読み込み
	//	material_name：マテリアルの名前
	//	return：Material*
	//--------------------------------------------------------------------------------
	Material* LoadFromFile(const String& material_name);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	unordered_map<String, MaterialInfo> materials_; // マテリアルを保存するところ
};
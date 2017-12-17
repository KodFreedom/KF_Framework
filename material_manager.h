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
		: ambient_(ambient), diffuse_(diffuse)
		, specular_(specular), emissive_(emissive)
		, power_(power) {}
	Material(const String& color_texture
		, const String& diffuse_texture = String()
		, const String& diffuse_texture_mask = String()
		, const String& specular_texture = String()
		, const String& specular_texture_mask = String()
		, const String& normal_texture = String()
		, const String& detail_texture = String()
		, const String& detail_mask = String()
		, const String& tint_by_base_mask = String()
		, const String& rim_mask = String()
		, const String& translucency = String()
		, const String& metalness_mask = String()
		, const String& self_illum_mask = String()
		, const String& fresnel_warp_color = String()
		, const String& fresnel_warp_rim = String()
		, const String& fresnel_warp_specular = String()
		, const Color& ambient = Color::kGray
		, const Color& diffuse = Color::kWhite
		, const Color& specular = Color::kBlack
		, const Color& emissive = Color::kBlack
		, const float& power = 1.0f)
		: color_texture_(color_texture)
		, diffuse_texture_(diffuse_texture)
		, diffuse_texture_mask_(diffuse_texture_mask)
		, specular_texture_(specular_texture)
		, specular_texture_mask_(specular_texture_mask)
		, normal_texture_(normal_texture)
		, detail_texture_(detail_texture)
		, detail_mask_(detail_mask)
		, tint_by_base_mask_(tint_by_base_mask)
		, rim_mask_(rim_mask)
		, translucency_(translucency)
		, metalness_mask_(metalness_mask)
		, self_illum_mask_(self_illum_mask)
		, fresnel_warp_color_(fresnel_warp_color)
		, fresnel_warp_rim_(fresnel_warp_rim)
		, fresnel_warp_specular_(fresnel_warp_specular)
		, ambient_(ambient), diffuse_(diffuse)
		, specular_(specular), emissive_(emissive)
		, power_(power) {}
	~Material() {}

	String color_texture_;
	String diffuse_texture_;
	String diffuse_texture_mask_;
	String specular_texture_;
	String specular_texture_mask_;
	String normal_texture_;
	String detail_texture_;
	String detail_mask_;
	String tint_by_base_mask_;
	String rim_mask_;
	String translucency_;
	String metalness_mask_;
	String self_illum_mask_;
	String fresnel_warp_color_;
	String fresnel_warp_rim_;
	String fresnel_warp_specular_;
	Color ambient_; // 環境光の反射率
	Color diffuse_; // 漫射光の反射率
	Color specular_; // 鏡面光の反射率
	Color emissive_; // 自発光
	float power_; // ハイライトのシャープネス
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
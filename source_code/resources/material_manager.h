//--------------------------------------------------------------------------------
//　material_manager.h
//  manage the materials' save, load
//  マテリアル管理者
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "resource_manager.h"
#include "../kf_utility/kf_math.h"
using namespace kodfreedom;

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
//  マテリアルマネージャ
//--------------------------------------------------------------------------------
class MaterialManager final : public ResourceManager
{
public:
    //--------------------------------------------------------------------------------
    //  生成処理
    //  return : MaterialManager*
    //--------------------------------------------------------------------------------
    static MaterialManager* Create(void);

    //--------------------------------------------------------------------------------
    //  与えられた名前のマテリアルを使う
    //  すでにあるの場合ユーザーを1たす、ないの場合与えられた値で生成する
    //--------------------------------------------------------------------------------
    void Use(const String& material_name
        , const Color& diffuse
        , const String& color_texture = String()
        , const Color& ambient = Color::kGray
        , const Color& specular = Color::kBlack
        , const Color& emissive = Color::kBlack
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
        , const String& fresnel_warp_specular = String());

    //--------------------------------------------------------------------------------
    //  与えられた名前のマテリアルの取得
    //  material_name : マテリアル名
    //  return : Material*
    //--------------------------------------------------------------------------------
    Material* Get(const String& material_name) const;

private:
    //--------------------------------------------------------------------------------
    //  構造体定義
    //--------------------------------------------------------------------------------
    struct MaterialInfo
    {
        MaterialInfo() : user_number(1), pointer(nullptr) {}
        int    user_number;
        Material* pointer;
    };

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
    void Init(void) override;

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    void Uninit(void) override;

    //--------------------------------------------------------------------------------
    //  リソースの読込処理
    //--------------------------------------------------------------------------------
    void LoadResource(void) override;

    //--------------------------------------------------------------------------------
    //  リソースのリリース処理
    //--------------------------------------------------------------------------------
    void ReleaseResource(void) override;

    //--------------------------------------------------------------------------------
    //  テクスチャの使用
    //--------------------------------------------------------------------------------
    void UseTexture(const Material& material);

    //--------------------------------------------------------------------------------
    //  テクスチャの破棄
    //--------------------------------------------------------------------------------
    void DisuseTexture(const Material& material);

    //--------------------------------------------------------------------------------
    //  ファイルからマテリアルの読み込み
    //  material_name：マテリアルの名前
    //  return：Material*
    //--------------------------------------------------------------------------------
    Material* LoadFromFile(const String& material_name);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    unordered_map<size_t, MaterialInfo> materials_; // マテリアルを保存するところ
};
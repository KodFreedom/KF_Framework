//--------------------------------------------------------------------------------
//  メッシュ描画コンポネント
//　mesh_renderer.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "component.h"
#include "renderer_manager.h"
#include "kf_math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum MeshRendererType
{
    kMeshRenderer2d = 0,
    kMeshRenderer3d,
    kMeshRenderer3dSkin
};

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class RenderSystem;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class MeshRenderer : public Component
{
public:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    MeshRenderer(GameObject& owner, const RenderPriority& priority, const ShaderType& shader_type, const MeshRendererType& type)
        : Component(owner)
        , priority_(priority)
        , shader_type_(shader_type)
        , is_cast_shadow_(false)
        , is_registered_(false)
        , bounding_sphere_radius_(0.0f)
        , type_(type) {}
    ~MeshRenderer() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    virtual bool Init(void) override;

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    virtual void Uninit(void) override;

    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    virtual void Update(void);

    //--------------------------------------------------------------------------------
    //  描画処理
    //--------------------------------------------------------------------------------
    virtual void RenderBy(const RenderSystem& render_system) = 0;

    //--------------------------------------------------------------------------------
    //  セッター
    //--------------------------------------------------------------------------------
    void SetMesh(const String& name);
    void SetMaterial(const String& name);
    void SetRenderPriority(const RenderPriority& value) { priority_ = value; }
    void SetShaderType(const ShaderType& value) { shader_type_ = value; }
    void SetCastShadowFlag(const bool& value) { is_cast_shadow_ = value; }
    void SetBoundingSpherePosition(const Vector3& value) { bounding_sphere_position_ = value; }
    void SetBoundingSphereRadius(const float& value) { bounding_sphere_radius_ = value; }

    //--------------------------------------------------------------------------------
    //  ゲッター
    //--------------------------------------------------------------------------------
    const auto& GetMeshName(void) const { return mesh_name_; }
    const auto& GetMaterialName(void) const { return material_name_; }
    const auto& GetRenderPriority(void) const { return priority_; }
    const auto& GetShaderType(void) const { return shader_type_; }
    const auto& GetType(void) const { return type_; }

protected:
    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    String           mesh_name_;
    String           material_name_;
    RenderPriority   priority_;
    ShaderType       shader_type_;
    bool             is_cast_shadow_;
    bool             is_registered_;
    Vector3          bounding_sphere_position_;
    float            bounding_sphere_radius_;
    MeshRendererType type_;
};
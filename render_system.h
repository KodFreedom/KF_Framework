//--------------------------------------------------------------------------------
//　render_system.h
//  レンダーシステム
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "kf_math.h"
using namespace kodfreedom;
#include "shader_enum.h"

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
struct Vertex2d // 2D頂点情報構造体
{
    Vector3 position;
    Vector2 uv;
    Color   color;
};

struct Vertex3d // 3D頂点情報構造体
{
    Vector3 position;
    Vector3 normal;
    Vector2 uv;
    Color   color;
};

struct Vertex3dSkin // 3dスキン頂点情報構造体
{
    Vector3 position;
    Vector3 normal;
    Vector3 tangent;
    Vector3 binormal;
    Vector2 uv;
    Short2 bone_indexes_0;
    Short2 bone_indexes_1;
    Short2 bone_indexes_2;
    Short2 bone_indexes_3;
    Short2 bone_indexes_4;
    Vector2 bone_weights_0;
    Vector2 bone_weights_1;
    Vector2 bone_weights_2;
    Vector2 bone_weights_3;
    Vector2 bone_weights_4;
};

//--------------------------------------------------------------------------------
//  レンダーシステムクラス
//--------------------------------------------------------------------------------
class RenderSystem
{
public:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static const Vector2 kOffset2d; // 2d中心からのオフセット
    static const Matrix44 kProjection2d; // 2dプロジェクション行列

    //--------------------------------------------------------------------------------
    //  破棄処理
    //--------------------------------------------------------------------------------
    void Release(void);

    //--------------------------------------------------------------------------------
    //  背景色の取得
    //--------------------------------------------------------------------------------
    const auto& GetBackgroundColor(void) const { return background_color_; }

    //--------------------------------------------------------------------------------
    //  背景色の取得
    //--------------------------------------------------------------------------------
    void SetBackgroundColor(const Color& color) { background_color_ = color; }

    //--------------------------------------------------------------------------------
    //  描画開始
    //--------------------------------------------------------------------------------
    virtual bool BeginRender(void) = 0;

    //--------------------------------------------------------------------------------
    //  描画終了
    //--------------------------------------------------------------------------------
    virtual void EndRender(void) = 0;

    //--------------------------------------------------------------------------------
    //  swap backbuffer / バックバッファの切り替え
    //--------------------------------------------------------------------------------
    virtual void Present(void) = 0;

    //--------------------------------------------------------------------------------
    //  描画処理(2D)
    //  mesh_name：メッシュ名前
    //--------------------------------------------------------------------------------
    virtual void Render2dMesh(const String& mesh_name) const = 0;

    //--------------------------------------------------------------------------------
    //  描画処理(3D)
    //  mesh_name：メッシュ名前
    //--------------------------------------------------------------------------------
    virtual void Render3dMesh(const String& mesh_name) const = 0;

    //--------------------------------------------------------------------------------
    //  描画処理(3dSkin)
    //  skin_name：メッシュ名前
    //--------------------------------------------------------------------------------
    virtual void Render3dSkin(const String& skin_name) const = 0;

    //--------------------------------------------------------------------------------
    //  カリングモードの設定
    //--------------------------------------------------------------------------------
    virtual void SetRenderState(const CullMode& value) = 0;

    //--------------------------------------------------------------------------------
    //  フィルモードの設定
    //--------------------------------------------------------------------------------
    virtual void SetRenderState(const FillMode& value) = 0;

    //--------------------------------------------------------------------------------
    //  フォグモードの設定
    //--------------------------------------------------------------------------------
    virtual void SetRenderState(const FogMode& value) = 0;

    //--------------------------------------------------------------------------------
    //  αモードの設定
    //--------------------------------------------------------------------------------
    virtual void SetRenderState(const AlphaMode& value) = 0;

protected:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    RenderSystem()
        : background_color_(Color::kBlack) {}
    RenderSystem(const RenderSystem& value) {}
    RenderSystem& operator=(const RenderSystem& value) {}
    ~RenderSystem() {}

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    virtual void Uninit(void) = 0;

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    Color background_color_; // 背景色
};
//--------------------------------------------------------------------------------
//　shadow_map_system.h
//  render objects to shadow map
//    シャドウマップをレンダリングするシステム
//    Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"
#include "shader_enum.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class MeshRenderer;
class MeshRenderer3d;
class MeshRenderer3dSkin;
class Transform;

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class ShadowMapSystem
{
#ifdef _DEBUG
    friend class DebugObserver;
#endif

public:
    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static constexpr float kShadowMapWidth = 1024.0f;
    static constexpr float kShadowMapHeight = 1024.0f;

    //--------------------------------------------------------------------------------
    //  生成処理
    //  return : TextureManager*
    //--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    static ShadowMapSystem* Create(const LPDIRECT3DDEVICE9 device)
    {
        auto instance = MY_NEW ShadowMapSystem(device);
        instance->Init();
        return instance;
    }
#endif

    //--------------------------------------------------------------------------------
    //  破棄処理
    //--------------------------------------------------------------------------------
    void Release(void) { Uninit(); }

    //--------------------------------------------------------------------------------
    //  描画処理
    //--------------------------------------------------------------------------------
    void Render(void);

    //--------------------------------------------------------------------------------
    //  rendererを登録する
    //  renderer : レンダラー
    //--------------------------------------------------------------------------------
    void Register(MeshRenderer3d* renderer);

    //--------------------------------------------------------------------------------
    //  rendererを登録する
    //  renderer : レンダラー
    //--------------------------------------------------------------------------------
    void Register(MeshRenderer3dSkin* renderer);

    //--------------------------------------------------------------------------------
    //  クリア処理
    //--------------------------------------------------------------------------------
    void Clear(void);

    //--------------------------------------------------------------------------------
    //  Set関数
    //--------------------------------------------------------------------------------
    void SetTarget(Transform* target) { target_ = target; }

    //--------------------------------------------------------------------------------
    //  Get関数
    //--------------------------------------------------------------------------------
    const Matrix44& GetLightView(void) const { return view_; }
    const Matrix44& GetLightProjection(void) const { return projection_; }
    const float& GetBias(void) const { return bias_; }

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    //--------------------------------------------------------------------------------
    //  シャドウマップのポインタを取得
    //  return : const LPDIRECT3DTEXTURE9
    //--------------------------------------------------------------------------------
    const LPDIRECT3DTEXTURE9 GetShadowMap(void) const
    {
        return shadow_map_;
    }
#endif

private:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    ShadowMapSystem(const LPDIRECT3DDEVICE9 device)
        : device_(device)
        , shadow_map_(nullptr)
        , shadow_map_surface_(nullptr)
        , offset_(Vector3(20.0f, 80.0f, -20.0f))
        , range_(20.0f) 
        , near_(0.0f)
        , far_(200.0f)
        , bias_(0.00001f)
        , target_(nullptr) {}
    ShadowMapSystem() : device_(nullptr) {}
    ShadowMapSystem(const ShadowMapSystem& value) : device_(nullptr) {}
#else
    ShadowMapSystem() {}
    ShadowMapSystem(const ShadowMapSystem& value) {}
#endif
    ShadowMapSystem& operator=(const ShadowMapSystem& value) {}
    ~ShadowMapSystem() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    void Init(void);

    //--------------------------------------------------------------------------------
    //  破棄処理
    //--------------------------------------------------------------------------------
    void Uninit(void);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    list<MeshRenderer*> renderers_array_[kShadowMapShaderMax]; // シャドウを表示するレンダラー
    Vector3 offset_;
    float range_;
    float near_;
    float far_;
    float bias_;
    Matrix44 view_;
    Matrix44 projection_;
    Transform* target_;

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    LPDIRECT3DTEXTURE9 shadow_map_;
    LPDIRECT3DSURFACE9 shadow_map_surface_;
    LPDIRECT3DSURFACE9 depth_stencil_surface_;
    const LPDIRECT3DDEVICE9 device_; // directx9のディバイス
#endif
};
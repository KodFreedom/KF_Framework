//--------------------------------------------------------------------------------
//　resources.h
//  リソース統括者
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include <d3dx9.h>
#endif

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class TextureManager;
class MaterialManager;
class MotionManager;
class MeshManager;

//--------------------------------------------------------------------------------
//  リソース統括者クラス
//--------------------------------------------------------------------------------
class Resources final
{
public:
    //--------------------------------------------------------------------------------
    //  生成処理
    //  return : Resources*
    //--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    static Resources* Create(const LPDIRECT3DDEVICE9 device)
    {
        auto instance = MY_NEW Resources;
        instance->Init(device);
        return instance;
    }
#endif

    //--------------------------------------------------------------------------------
    //  破棄処理
    //--------------------------------------------------------------------------------
    void Release(void);

    //--------------------------------------------------------------------------------
    //  ゲッター
    //--------------------------------------------------------------------------------
    auto& GetTextureManager(void) const { return *texture_manager_; }
    auto& GetMeshManager(void) const { return *mesh_manager_; }
    auto& GetMaterialManager(void) const { return *material_manager_; }
    auto& GetMotionManager(void) const { return *motion_manager_; }
    bool  IsCompleteLoading(void) const;

private:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    Resources() {}
    Resources(const Resources& value) {}
    Resources& operator=(const Resources& value) {}
    ~Resources() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    void Init(const LPDIRECT3DDEVICE9 device);
#endif

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    void Uninit(void);

    //--------------------------------------------------------------------------------
    //  マルチスレッド処理
    //--------------------------------------------------------------------------------
    void Run(void);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    bool             is_running_ = true;
    thread*          thread_ = nullptr;
    TextureManager*  texture_manager_ = nullptr;
    MaterialManager* material_manager_ = nullptr;
    MotionManager*   motion_manager_ = nullptr;
    MeshManager*     mesh_manager_ = nullptr;
};
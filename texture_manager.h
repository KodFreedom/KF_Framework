//--------------------------------------------------------------------------------
//　texture_manager.h
//  manage the textures' save, load
//  テクスチャ管理者
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include <d3dx9.h>
#endif

//--------------------------------------------------------------------------------
//  テクスチャ管理者クラス
//--------------------------------------------------------------------------------
class TextureManager
{
public:
    //--------------------------------------------------------------------------------
    //  生成処理
    //  return : TextureManager*
    //--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    static TextureManager* Create(const LPDIRECT3DDEVICE9 device)
    {
        auto instance = MY_NEW TextureManager(device);
        instance->Init();
        return instance;
    }
#endif

    //--------------------------------------------------------------------------------
    //  破棄処理
    //--------------------------------------------------------------------------------
    void Release(void);

    //--------------------------------------------------------------------------------
    //  与えられた名前のテクスチャを使う
    //  すでにあるの場合ユーザーを1たす、ないの場合ファイルから読み込む
    //  texture_name : テクスチャ名
    //--------------------------------------------------------------------------------
    void Use(const String& texture_name);

    //--------------------------------------------------------------------------------
    //  与えられた名前のテクスチャを使わない
    //  ユーザーが-1になる、0になった場合メモリから破棄する
    //  texture_name : テクスチャ名
    //--------------------------------------------------------------------------------
    void Disuse(const String& texture_name);

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    //--------------------------------------------------------------------------------
    //  与えられた名前のテクスチャのポインタを取得
    //  texture_name : テクスチャ名
    //  return : LPDIRECT3DTEXTURE9
    //--------------------------------------------------------------------------------
    LPDIRECT3DTEXTURE9 Get(const String& texture_name)
    {
        auto iterator = textures_.find(texture_name);
        if (textures_.end() == iterator) return nullptr;
        return iterator->second.pointer;
    }

    //--------------------------------------------------------------------------------
    //  空のテクスチャの作成
    //  size : width と heightのサイズ
    //  return : LPDIRECT3DTEXTURE9
    //--------------------------------------------------------------------------------
    LPDIRECT3DTEXTURE9 CreateEmptyTexture(const UINT size)
    {
        LPDIRECT3DTEXTURE9 texture = nullptr;
        device_->CreateTexture(size, size, 1, D3DUSAGE_DYNAMIC
            , D3DFMT_A32B32G32R32F, D3DPOOL_DEFAULT, &texture, NULL);
        return texture;
    }
#endif

private:
    //--------------------------------------------------------------------------------
    //  構造体定義
    //--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    struct TextureInfo
    {
        TextureInfo() : user_number(1), pointer(nullptr) {}
        int user_number;
        LPDIRECT3DTEXTURE9 pointer;
    };
#else
    struct TextureInfo
    {
        TextureInfo() : user_number(1), id(-1) {}
        int user_number;
        int id;
    };
#endif

    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    TextureManager(const LPDIRECT3DDEVICE9 device) : device_(device) {}
    TextureManager() : device_(nullptr) {}
    TextureManager(const TextureManager& value) : device_(nullptr) {}
#else
    TextureManager() {}
    TextureManager(const TextureManager& value) {}
#endif
    TextureManager& operator=(const TextureManager& value) {}
    ~TextureManager() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    void Init(void);

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    void Uninit(void);

    //--------------------------------------------------------------------------------
    //  マルチスレッド処理
    //--------------------------------------------------------------------------------
    void Run(void);

    //--------------------------------------------------------------------------------
    //  テクスチャ読込処理
    //--------------------------------------------------------------------------------
    void LoadTexture(void);

    //--------------------------------------------------------------------------------
    //  テクスチャリリース処理
    //--------------------------------------------------------------------------------
    void ReleaseTexture(void);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    bool                               is_running_ = true;
    thread*                            thread_ = nullptr;
    queue<String>                      load_tasks_;
    queue<String>                      release_tasks_;
    unordered_map<String, TextureInfo> textures_; // テクスチャを保存するところ
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    const LPDIRECT3DDEVICE9            device_; // directx9のディバイス
#endif
};
//--------------------------------------------------------------------------------
//　texture_manager.h
//  manage the textures' save, load
//  テクスチャ管理者
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "resource_manager.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include <d3dx9.h>
#endif

//--------------------------------------------------------------------------------
//  テクスチャ管理者クラス
//--------------------------------------------------------------------------------
class TextureManager final : public ResourceManager
{
public:
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    //--------------------------------------------------------------------------------
    //  生成処理
    //  return : TextureManager*
    //--------------------------------------------------------------------------------
    static TextureManager* Create(const LPDIRECT3DDEVICE9 device);

    //--------------------------------------------------------------------------------
    //  与えられた名前のテクスチャのポインタを取得
    //  texture_name : テクスチャ名
    //  return : LPDIRECT3DTEXTURE9
    //--------------------------------------------------------------------------------
    LPDIRECT3DTEXTURE9 Get(const String& texture_name);

    //--------------------------------------------------------------------------------
    //  空のテクスチャの作成
    //  size : width と heightのサイズ
    //  return : LPDIRECT3DTEXTURE9
    //--------------------------------------------------------------------------------
    LPDIRECT3DTEXTURE9 CreateEmptyTexture(const UINT size);
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
    //  変数定義
    //--------------------------------------------------------------------------------
    unordered_map<size_t, TextureInfo> textures_; // テクスチャを保存するところ
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    const LPDIRECT3DDEVICE9            device_; // directx9のディバイス
#endif
};
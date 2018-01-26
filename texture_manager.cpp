//--------------------------------------------------------------------------------
//　texture_manager.cpp
//  manage the textures_' save, load
//  テクスチャ管理者
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "texture_manager.h"
#include "kf_utility.h"

//--------------------------------------------------------------------------------
//    
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void TextureManager::Release(void)
{
    Uninit();
    MY_DELETE this;
}

//--------------------------------------------------------------------------------
//  与えられた名前のテクスチャを使う
//--------------------------------------------------------------------------------
void TextureManager::Use(const String& texture_name)
{
    if (texture_name.empty()) return;
    load_tasks_.push(texture_name);
}

//--------------------------------------------------------------------------------
//  与えられた名前のテクスチャを使わない
//--------------------------------------------------------------------------------
void TextureManager::Disuse(const String& texture_name)
{
    if (texture_name.empty()) return;
    release_tasks_.push(texture_name);
}

//--------------------------------------------------------------------------------
//    
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void TextureManager::Init(void)
{
    // スレッドの生成
    thread_ = MY_NEW thread(&TextureManager::Run, this);
}

//--------------------------------------------------------------------------------
//  全てのテクスチャを破棄する
//--------------------------------------------------------------------------------
void TextureManager::Uninit(void)
{
    is_running_ = false;

    // スレッドの破棄
    if (thread_ && thread_->joinable())
    {
        thread_->join();
        MY_DELETE thread_;
        thread_ = nullptr;
    }

    for (auto iterator = textures_.begin(); iterator != textures_.end();)
    {
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
        SAFE_RELEASE(iterator->second.pointer);
#endif
        iterator = textures_.erase(iterator);
    }
}

//--------------------------------------------------------------------------------
//  マルチスレッド処理
//--------------------------------------------------------------------------------
void TextureManager::Run(void)
{
    while (is_running_)
    {
        LoadTexture();
        ReleaseTexture();
    }
}

//--------------------------------------------------------------------------------
//  テクスチャ読込処理
//--------------------------------------------------------------------------------
void TextureManager::LoadTexture(void)
{
    if (load_tasks_.empty()) return;
    String texture_name = load_tasks_.front();
    load_tasks_.pop();


    //すでに読み込んだら処理終了
    auto iterator = textures_.find(texture_name);
    if (textures_.end() != iterator)
    {
        ++iterator->second.user_number;
        return;
    }

    //テクスチャの読み込み
    TextureInfo info;
    String path = L"data/texture/" + texture_name;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    if (FAILED(D3DXCreateTextureFromFile(device_, path.c_str(), &info.pointer)))
    {
        String buffer = path + L"が見つからない！！！";
        MessageBox(NULL, buffer.c_str(), L"エラー", MB_OK | MB_ICONWARNING);
        info.pointer = nullptr;
    }
#endif
    textures_.emplace(texture_name, info);
}

//--------------------------------------------------------------------------------
//  テクスチャリリース処理
//--------------------------------------------------------------------------------
void TextureManager::ReleaseTexture(void)
{
    if (release_tasks_.empty()) return;
    String texture_name = release_tasks_.front();
    release_tasks_.pop();

    auto iterator = textures_.find(texture_name);
    if (textures_.end() == iterator) return;

    if (--iterator->second.user_number <= 0)
    {// 誰も使ってないので破棄する
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
        auto pointer = iterator->second.pointer;
        textures_.erase(iterator);
        SAFE_RELEASE(pointer);
#endif  
    }
}
//--------------------------------------------------------------------------------
//　texture_manager.cpp
//  manage the textures_' save, load
//	テクスチャ管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "main.h"
#include "texture_manager.h"

//--------------------------------------------------------------------------------
//	
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  与えられた名前のテクスチャを使う
//--------------------------------------------------------------------------------
void TextureManager::Use(const String& texture_name)
{
	if (texture_name.empty()) return;

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
//  与えられた名前のテクスチャを使わない
//--------------------------------------------------------------------------------
void TextureManager::Disuse(const String& texture_name)
{
	if (texture_name.empty()) return;
	auto iterator = textures_.find(texture_name);
	if (textures_.end() == iterator) return;

	if (--iterator->second.user_number <= 0)
	{// 誰も使ってないので破棄する
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		SAFE_RELEASE(iterator->second.pointer);
#endif
		textures_.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//	
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  全てのテクスチャを破棄する
//--------------------------------------------------------------------------------
void TextureManager::Uninit(void)
{
	for (auto iterator = textures_.begin(); iterator != textures_.end();)
	{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		SAFE_RELEASE(iterator->second.pointer);
#endif
		iterator = textures_.erase(iterator);
	}
}
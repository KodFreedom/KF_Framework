//--------------------------------------------------------------------------------
//
//　textureManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "textureManager.h"

//--------------------------------------------------------------------------------
//  静的メンバー変数宣言
//--------------------------------------------------------------------------------
TextureManager* TextureManager::instance = nullptr;

//--------------------------------------------------------------------------------
//	
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  テクスチャの読み込み
//--------------------------------------------------------------------------------
void TextureManager::Use(const string& textureName)
{
	if (textureName.empty()) return;

	//すでに読み込んだら処理終了
	auto iterator = textures.find(textureName);
	if (iterator != textures.end())
	{
		++iterator->second.UserNumber;
		return;
	}

	//テクスチャの読み込み
	TextureInfo info;
	string filePath = "data/TEXTURE/" + textureName;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	if (FAILED(D3DXCreateTextureFromFile(device, filePath.c_str(), &info.Pointer)))
	{
		string buffer = filePath + "が見つからない！！！";
		MessageBox(NULL, buffer.c_str(), "エラー", MB_OK | MB_ICONWARNING);
		info.Pointer = nullptr;
	}
#endif
	textures.emplace(textureName, info);
}

//--------------------------------------------------------------------------------
//  テクスチャの破棄
//--------------------------------------------------------------------------------
void TextureManager::Disuse(const string& textureName)
{
	if (textureName.empty()) return;

	auto iterator = textures.find(textureName);
	if (textures.end() == iterator) return;

	if (--iterator->second.UserNumber <= 0)
	{// 誰も使ってないので破棄する
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		SAFE_RELEASE(iterator->second.Pointer);
#endif
		textures.erase(iterator);
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
void TextureManager::uninit(void)
{
	for (auto iterator = textures.begin(); iterator != textures.end();)
	{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		SAFE_RELEASE(iterator->second.Pointer);
#endif
		iterator = textures.erase(iterator);
	}
}
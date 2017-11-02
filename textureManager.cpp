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
#include "manager.h"
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
		++iterator->second.userNumber;
		return;
	}

	//テクスチャの読み込み
#ifdef USING_DIRECTX
	TextureInfo info;
	info.userNumber = 1;
	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	string filePath = "data/TEXTURE/" + textureName;
	HRESULT hr = D3DXCreateTextureFromFile(pDevice, filePath.c_str(), &info.texturePointer);
	if (FAILED(hr))
	{
		string str = filePath + "が見つからない！！！";
		MessageBox(NULL, str.c_str(), "エラー", MB_OK | MB_ICONWARNING);
		info.texturePointer = nullptr;
	}
	textures.emplace(textureName, info);
#endif
}

//--------------------------------------------------------------------------------
//  テクスチャの破棄
//--------------------------------------------------------------------------------
void TextureManager::Disuse(const string& textureName)
{
	if (textureName.empty()) return;
	auto iterator = textures.find(textureName);
	if (iterator == textures.end()) return;
	--iterator->second.userNumber;
	if (iterator->second.userNumber == 0)
	{// 誰も使ってないので破棄する
#ifdef USING_DIRECTX
		SAFE_RELEASE(iterator->second.texturePointer);
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
#ifdef USING_DIRECTX
		SAFE_RELEASE(iterator->second.texturePointer);
#endif
		iterator = textures.erase(iterator);
	}
}

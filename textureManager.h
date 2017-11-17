//--------------------------------------------------------------------------------
//
//　textureManager.h
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class TextureManager
{
public:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	static void	Create(const LPDIRECT3DDEVICE9 device)
	{
		if (instance) return;
		instance = new TextureManager;
		instance->device = device;
	}
#endif
	static auto Instance(void) { return instance; }
	static void Release(void) { SAFE_UNINIT(instance); }

	void Use(const string& textureName);
	void Disuse(const string& textureName);

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	LPDIRECT3DTEXTURE9 GetTexture(const string& textureName)
	{
		auto iterator = textures.find(textureName);
		if (textures.end() == iterator) return nullptr;
		return iterator->second.Pointer;
	}
#endif

protected:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct TextureInfo
	{
		TextureInfo()
			: UserNumber(1)
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
			, Pointer(nullptr)
#endif
		{}
		int					UserNumber;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		LPDIRECT3DTEXTURE9	Pointer;
#endif
	};

	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	TextureManager() { textures.clear(); }
	~TextureManager() {}
	void uninit(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	static TextureManager* instance;
	unordered_map<string, TextureInfo> textures;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	LPDIRECT3DDEVICE9 device;
#endif
};
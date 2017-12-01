//--------------------------------------------------------------------------------
//　texture_manager.h
//  manage the textures' save, load
//	テクスチャ管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"

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
		return instance;
	}
#endif

	//--------------------------------------------------------------------------------
	//  破棄処理
	//--------------------------------------------------------------------------------
	void Release(void) { Uninit(); }

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
	//  破棄処理
	//--------------------------------------------------------------------------------
	void Uninit(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	unordered_map<String, TextureInfo> textures_; // テクスチャを保存するところ
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	const LPDIRECT3DDEVICE9 device_; // directx9のディバイス
#endif
};
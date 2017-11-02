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

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class TextureManager
{
#ifdef _DEBUG
	friend class DebugObserver;
#endif // _DEBUG

public:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	static auto	Create(void)
	{
		if (instance) return instance;
		instance = new TextureManager;
		instance->init();
		return instance;
	}
	static void Release(void) { SAFE_UNINIT(instance); }
	static auto Instance(void) { return instance; }

	void		Use(const string& textureName);
	void		Disuse(const string& textureName);

#ifdef USING_DIRECTX
	LPDIRECT3DTEXTURE9	GetTexture(const string& textureName)
	{
		if (textureName.empty()) return nullptr;
		return textures.at(textureName).texturePointer;
	}
#endif

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct TextureInfo
	{
		unsigned short		userNumber;	//今使ってるオブジェクト数
#ifdef USING_DIRECTX
		LPDIRECT3DTEXTURE9	texturePointer;	//テクスチャ
#endif
	};

	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	TextureManager() { textures.clear(); }
	~TextureManager() {}
	void init(void) {};
	void uninit(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	unordered_map<string, TextureInfo>	textures;
	static TextureManager*				instance;
};
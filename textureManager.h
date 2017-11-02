//--------------------------------------------------------------------------------
//
//�@textureManager.h
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class TextureManager
{
#ifdef _DEBUG
	friend class DebugObserver;
#endif // _DEBUG

public:
	//--------------------------------------------------------------------------------
	//  �֐��錾
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
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct TextureInfo
	{
		unsigned short		userNumber;	//���g���Ă�I�u�W�F�N�g��
#ifdef USING_DIRECTX
		LPDIRECT3DTEXTURE9	texturePointer;	//�e�N�X�`��
#endif
	};

	//--------------------------------------------------------------------------------
	//  �֐��錾
	//--------------------------------------------------------------------------------
	TextureManager() { textures.clear(); }
	~TextureManager() {}
	void init(void) {};
	void uninit(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	unordered_map<string, TextureInfo>	textures;
	static TextureManager*				instance;
};
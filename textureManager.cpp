//--------------------------------------------------------------------------------
//
//�@textureManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "textureManager.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�[�ϐ��錾
//--------------------------------------------------------------------------------
TextureManager* TextureManager::instance = nullptr;

//--------------------------------------------------------------------------------
//	
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �e�N�X�`���̓ǂݍ���
//--------------------------------------------------------------------------------
void TextureManager::Use(const string& textureName)
{
	if (textureName.empty()) return;

	//���łɓǂݍ��񂾂珈���I��
	auto iterator = textures.find(textureName);
	if (iterator != textures.end())
	{
		++iterator->second.UserNumber;
		return;
	}

	//�e�N�X�`���̓ǂݍ���
	TextureInfo info;
	string filePath = "data/TEXTURE/" + textureName;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	if (FAILED(D3DXCreateTextureFromFile(device, filePath.c_str(), &info.Pointer)))
	{
		string buffer = filePath + "��������Ȃ��I�I�I";
		MessageBox(NULL, buffer.c_str(), "�G���[", MB_OK | MB_ICONWARNING);
		info.Pointer = nullptr;
	}
#endif
	textures.emplace(textureName, info);
}

//--------------------------------------------------------------------------------
//  �e�N�X�`���̔j��
//--------------------------------------------------------------------------------
void TextureManager::Disuse(const string& textureName)
{
	if (textureName.empty()) return;

	auto iterator = textures.find(textureName);
	if (textures.end() == iterator) return;

	if (--iterator->second.UserNumber <= 0)
	{// �N���g���ĂȂ��̂Ŕj������
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
//  �S�Ẵe�N�X�`����j������
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
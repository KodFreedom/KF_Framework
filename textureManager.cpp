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
#include "manager.h"
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
		++iterator->second.userNumber;
		return;
	}

	//�e�N�X�`���̓ǂݍ���
#ifdef USING_DIRECTX
	TextureInfo info;
	info.userNumber = 1;
	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	string filePath = "data/TEXTURE/" + textureName;
	HRESULT hr = D3DXCreateTextureFromFile(pDevice, filePath.c_str(), &info.texturePointer);
	if (FAILED(hr))
	{
		string str = filePath + "��������Ȃ��I�I�I";
		MessageBox(NULL, str.c_str(), "�G���[", MB_OK | MB_ICONWARNING);
		info.texturePointer = nullptr;
	}
	textures.emplace(textureName, info);
#endif
}

//--------------------------------------------------------------------------------
//  �e�N�X�`���̔j��
//--------------------------------------------------------------------------------
void TextureManager::Disuse(const string& textureName)
{
	if (textureName.empty()) return;
	auto iterator = textures.find(textureName);
	if (iterator == textures.end()) return;
	--iterator->second.userNumber;
	if (iterator->second.userNumber == 0)
	{// �N���g���ĂȂ��̂Ŕj������
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
//  �S�Ẵe�N�X�`����j������
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

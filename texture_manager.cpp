//--------------------------------------------------------------------------------
//�@texture_manager.cpp
//  manage the textures_' save, load
//	�e�N�X�`���Ǘ���
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "main.h"
#include "texture_manager.h"

//--------------------------------------------------------------------------------
//	
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃e�N�X�`�����g��
//--------------------------------------------------------------------------------
void TextureManager::Use(const String& texture_name)
{
	if (texture_name.empty()) return;

	//���łɓǂݍ��񂾂珈���I��
	auto iterator = textures_.find(texture_name);
	if (textures_.end() != iterator)
	{
		++iterator->second.user_number;
		return;
	}

	//�e�N�X�`���̓ǂݍ���
	TextureInfo info;
	String path = L"data/texture/" + texture_name;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	if (FAILED(D3DXCreateTextureFromFile(device_, path.c_str(), &info.pointer)))
	{
		String buffer = path + L"��������Ȃ��I�I�I";
		MessageBox(NULL, buffer.c_str(), L"�G���[", MB_OK | MB_ICONWARNING);
		info.pointer = nullptr;
	}
#endif
	textures_.emplace(texture_name, info);
}

//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃e�N�X�`�����g��Ȃ�
//--------------------------------------------------------------------------------
void TextureManager::Disuse(const String& texture_name)
{
	if (texture_name.empty()) return;
	auto iterator = textures_.find(texture_name);
	if (textures_.end() == iterator) return;

	if (--iterator->second.user_number <= 0)
	{// �N���g���ĂȂ��̂Ŕj������
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
//  �S�Ẵe�N�X�`����j������
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
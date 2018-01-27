//--------------------------------------------------------------------------------
//�@texture_manager.cpp
//  manage the textures_' save, load
//  �e�N�X�`���Ǘ���
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "texture_manager.h"
#include "kf_utility.h"
#ifdef _DEBUG
#include "main_system.h"
#include "debug_observer.h"
#endif // _DEBUG

//--------------------------------------------------------------------------------
//    
//  Public
//
//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
TextureManager* TextureManager::Create(const LPDIRECT3DDEVICE9 device)
{
    auto instance = MY_NEW TextureManager(device);
    instance->Init();
    return instance;
}

//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃e�N�X�`���̃|�C���^���擾
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 TextureManager::Get(const String& texture_name)
{
    auto iterator = textures_.find(hash<String>()(texture_name));
    if (textures_.end() == iterator) return nullptr;
    return iterator->second.pointer;
}

//--------------------------------------------------------------------------------
//  ��̃e�N�X�`���̍쐬
//--------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 TextureManager::CreateEmptyTexture(const UINT size)
{
    LPDIRECT3DTEXTURE9 texture = nullptr;
    device_->CreateTexture(size, size, 1, D3DUSAGE_DYNAMIC
        , D3DFMT_A32B32G32R32F, D3DPOOL_DEFAULT, &texture, NULL);
    return texture;
}
#endif

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

//--------------------------------------------------------------------------------
//  �e�N�X�`���Ǎ�����
//--------------------------------------------------------------------------------
void TextureManager::LoadResource(void)
{
    const String& texture_name = load_tasks_.front();
    size_t key = hash<String>()(texture_name);

    //���łɓǂݍ��񂾂珈���I��
    auto iterator = textures_.find(key);
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
#ifdef _DEBUG
        MainSystem::Instance().GetDebugObserver().Display(L"TextureManager::LoadResource : " + path + L"��������Ȃ��I�I�I");
#endif // _DEBUG
        info.pointer = nullptr;
    }
#endif
    textures_.emplace(key, info);
}

//--------------------------------------------------------------------------------
//  �e�N�X�`�������[�X����
//--------------------------------------------------------------------------------
void TextureManager::ReleaseResource(void)
{
    auto iterator = textures_.find(release_tasks_.front());
    if (textures_.end() == iterator) return;

    if (--iterator->second.user_number <= 0)
    {// �N���g���ĂȂ��̂Ŕj������
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
        auto pointer = iterator->second.pointer;
        textures_.erase(iterator);
        SAFE_RELEASE(pointer);
#endif  
    }
}
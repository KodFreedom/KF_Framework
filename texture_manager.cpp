//--------------------------------------------------------------------------------
//�@texture_manager.cpp
//  manage the textures_' save, load
//  �e�N�X�`���Ǘ���
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "texture_manager.h"
#include "kf_utility.h"

//--------------------------------------------------------------------------------
//    
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �j������
//--------------------------------------------------------------------------------
void TextureManager::Release(void)
{
    Uninit();
    MY_DELETE this;
}

//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃e�N�X�`�����g��
//--------------------------------------------------------------------------------
void TextureManager::Use(const String& texture_name)
{
    if (texture_name.empty()) return;
    load_tasks_.push(texture_name);
}

//--------------------------------------------------------------------------------
//  �^����ꂽ���O�̃e�N�X�`�����g��Ȃ�
//--------------------------------------------------------------------------------
void TextureManager::Disuse(const String& texture_name)
{
    if (texture_name.empty()) return;
    release_tasks_.push(texture_name);
}

//--------------------------------------------------------------------------------
//    
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void TextureManager::Init(void)
{
    // �X���b�h�̐���
    thread_ = MY_NEW thread(&TextureManager::Run, this);
}

//--------------------------------------------------------------------------------
//  �S�Ẵe�N�X�`����j������
//--------------------------------------------------------------------------------
void TextureManager::Uninit(void)
{
    is_running_ = false;

    // �X���b�h�̔j��
    if (thread_ && thread_->joinable())
    {
        thread_->join();
        MY_DELETE thread_;
        thread_ = nullptr;
    }

    for (auto iterator = textures_.begin(); iterator != textures_.end();)
    {
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
        SAFE_RELEASE(iterator->second.pointer);
#endif
        iterator = textures_.erase(iterator);
    }
}

//--------------------------------------------------------------------------------
//  �}���`�X���b�h����
//--------------------------------------------------------------------------------
void TextureManager::Run(void)
{
    while (is_running_)
    {
        LoadTexture();
        ReleaseTexture();
    }
}

//--------------------------------------------------------------------------------
//  �e�N�X�`���Ǎ�����
//--------------------------------------------------------------------------------
void TextureManager::LoadTexture(void)
{
    if (load_tasks_.empty()) return;
    String texture_name = load_tasks_.front();
    load_tasks_.pop();


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
//  �e�N�X�`�������[�X����
//--------------------------------------------------------------------------------
void TextureManager::ReleaseTexture(void)
{
    if (release_tasks_.empty()) return;
    String texture_name = release_tasks_.front();
    release_tasks_.pop();

    auto iterator = textures_.find(texture_name);
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
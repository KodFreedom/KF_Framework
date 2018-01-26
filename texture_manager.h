//--------------------------------------------------------------------------------
//�@texture_manager.h
//  manage the textures' save, load
//  �e�N�X�`���Ǘ���
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include <d3dx9.h>
#endif

//--------------------------------------------------------------------------------
//  �e�N�X�`���Ǘ��҃N���X
//--------------------------------------------------------------------------------
class TextureManager
{
public:
    //--------------------------------------------------------------------------------
    //  ��������
    //  return : TextureManager*
    //--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    static TextureManager* Create(const LPDIRECT3DDEVICE9 device)
    {
        auto instance = MY_NEW TextureManager(device);
        instance->Init();
        return instance;
    }
#endif

    //--------------------------------------------------------------------------------
    //  �j������
    //--------------------------------------------------------------------------------
    void Release(void);

    //--------------------------------------------------------------------------------
    //  �^����ꂽ���O�̃e�N�X�`�����g��
    //  ���łɂ���̏ꍇ���[�U�[��1�����A�Ȃ��̏ꍇ�t�@�C������ǂݍ���
    //  texture_name : �e�N�X�`����
    //--------------------------------------------------------------------------------
    void Use(const String& texture_name);

    //--------------------------------------------------------------------------------
    //  �^����ꂽ���O�̃e�N�X�`�����g��Ȃ�
    //  ���[�U�[��-1�ɂȂ�A0�ɂȂ����ꍇ����������j������
    //  texture_name : �e�N�X�`����
    //--------------------------------------------------------------------------------
    void Disuse(const String& texture_name);

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    //--------------------------------------------------------------------------------
    //  �^����ꂽ���O�̃e�N�X�`���̃|�C���^���擾
    //  texture_name : �e�N�X�`����
    //  return : LPDIRECT3DTEXTURE9
    //--------------------------------------------------------------------------------
    LPDIRECT3DTEXTURE9 Get(const String& texture_name)
    {
        auto iterator = textures_.find(texture_name);
        if (textures_.end() == iterator) return nullptr;
        return iterator->second.pointer;
    }

    //--------------------------------------------------------------------------------
    //  ��̃e�N�X�`���̍쐬
    //  size : width �� height�̃T�C�Y
    //  return : LPDIRECT3DTEXTURE9
    //--------------------------------------------------------------------------------
    LPDIRECT3DTEXTURE9 CreateEmptyTexture(const UINT size)
    {
        LPDIRECT3DTEXTURE9 texture = nullptr;
        device_->CreateTexture(size, size, 1, D3DUSAGE_DYNAMIC
            , D3DFMT_A32B32G32R32F, D3DPOOL_DEFAULT, &texture, NULL);
        return texture;
    }
#endif

private:
    //--------------------------------------------------------------------------------
    //  �\���̒�`
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
    //  ����������
    //--------------------------------------------------------------------------------
    void Init(void);

    //--------------------------------------------------------------------------------
    //  �I������
    //--------------------------------------------------------------------------------
    void Uninit(void);

    //--------------------------------------------------------------------------------
    //  �}���`�X���b�h����
    //--------------------------------------------------------------------------------
    void Run(void);

    //--------------------------------------------------------------------------------
    //  �e�N�X�`���Ǎ�����
    //--------------------------------------------------------------------------------
    void LoadTexture(void);

    //--------------------------------------------------------------------------------
    //  �e�N�X�`�������[�X����
    //--------------------------------------------------------------------------------
    void ReleaseTexture(void);

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    bool                               is_running_ = true;
    thread*                            thread_ = nullptr;
    queue<String>                      load_tasks_;
    queue<String>                      release_tasks_;
    unordered_map<String, TextureInfo> textures_; // �e�N�X�`����ۑ�����Ƃ���
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    const LPDIRECT3DDEVICE9            device_; // directx9�̃f�B�o�C�X
#endif
};
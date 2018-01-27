//--------------------------------------------------------------------------------
//�@texture_manager.h
//  manage the textures' save, load
//  �e�N�X�`���Ǘ���
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "resource_manager.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include <d3dx9.h>
#endif

//--------------------------------------------------------------------------------
//  �e�N�X�`���Ǘ��҃N���X
//--------------------------------------------------------------------------------
class TextureManager final : public ResourceManager
{
public:
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    //--------------------------------------------------------------------------------
    //  ��������
    //  return : TextureManager*
    //--------------------------------------------------------------------------------
    static TextureManager* Create(const LPDIRECT3DDEVICE9 device);

    //--------------------------------------------------------------------------------
    //  �^����ꂽ���O�̃e�N�X�`���̃|�C���^���擾
    //  texture_name : �e�N�X�`����
    //  return : LPDIRECT3DTEXTURE9
    //--------------------------------------------------------------------------------
    LPDIRECT3DTEXTURE9 Get(const String& texture_name);

    //--------------------------------------------------------------------------------
    //  ��̃e�N�X�`���̍쐬
    //  size : width �� height�̃T�C�Y
    //  return : LPDIRECT3DTEXTURE9
    //--------------------------------------------------------------------------------
    LPDIRECT3DTEXTURE9 CreateEmptyTexture(const UINT size);
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
    //  �I������
    //--------------------------------------------------------------------------------
    void Uninit(void) override;

    //--------------------------------------------------------------------------------
    //  ���\�[�X�̓Ǎ�����
    //--------------------------------------------------------------------------------
    void LoadResource(void) override;

    //--------------------------------------------------------------------------------
    //  ���\�[�X�̃����[�X����
    //--------------------------------------------------------------------------------
    void ReleaseResource(void) override;

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    unordered_map<size_t, TextureInfo> textures_; // �e�N�X�`����ۑ�����Ƃ���
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    const LPDIRECT3DDEVICE9            device_; // directx9�̃f�B�o�C�X
#endif
};
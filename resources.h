//--------------------------------------------------------------------------------
//�@resources.h
//  ���\�[�X������
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include <d3dx9.h>
#endif

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class TextureManager;
class MaterialManager;
class MotionManager;
class MeshManager;

//--------------------------------------------------------------------------------
//  ���\�[�X�����҃N���X
//--------------------------------------------------------------------------------
class Resources final
{
public:
    //--------------------------------------------------------------------------------
    //  ��������
    //  return : Resources*
    //--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    static Resources* Create(const LPDIRECT3DDEVICE9 device)
    {
        auto instance = MY_NEW Resources;
        instance->Init(device);
        return instance;
    }
#endif

    //--------------------------------------------------------------------------------
    //  �j������
    //--------------------------------------------------------------------------------
    void Release(void);

    //--------------------------------------------------------------------------------
    //  �Q�b�^�[
    //--------------------------------------------------------------------------------
    auto& GetTextureManager(void) const { return *texture_manager_; }
    auto& GetMeshManager(void) const { return *mesh_manager_; }
    auto& GetMaterialManager(void) const { return *material_manager_; }
    auto& GetMotionManager(void) const { return *motion_manager_; }
    bool  IsCompleteLoading(void) const;

private:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    Resources() {}
    Resources(const Resources& value) {}
    Resources& operator=(const Resources& value) {}
    ~Resources() {}

    //--------------------------------------------------------------------------------
    //  ����������
    //--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    void Init(const LPDIRECT3DDEVICE9 device);
#endif

    //--------------------------------------------------------------------------------
    //  �I������
    //--------------------------------------------------------------------------------
    void Uninit(void);

    //--------------------------------------------------------------------------------
    //  �}���`�X���b�h����
    //--------------------------------------------------------------------------------
    void Run(void);

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    bool             is_running_ = true;
    thread*          thread_ = nullptr;
    TextureManager*  texture_manager_ = nullptr;
    MaterialManager* material_manager_ = nullptr;
    MotionManager*   motion_manager_ = nullptr;
    MeshManager*     mesh_manager_ = nullptr;
};
//--------------------------------------------------------------------------------
//�@resources.cpp
//  ���\�[�X������
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "resources.h"
#include "texture_manager.h"
#include "mesh_manager.h"
#include "material_manager.h"
#include "motion_manager.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �j������
//--------------------------------------------------------------------------------
void Resources::Release(void)
{
    Uninit();
    MY_DELETE this;
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
void Resources::Init(const LPDIRECT3DDEVICE9 device)
{
    texture_manager_ = TextureManager::Create(device);
    mesh_manager_ = MeshManager::Create(device);
    material_manager_ = MaterialManager::Create();
    motion_manager_ = MotionManager::Create();

    // �X���b�h�̐���
    thread_ = MY_NEW thread(&Resources::Run, this);
}
#endif

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void Resources::Uninit(void)
{
    is_running_ = false;

    // �X���b�h�̔j��
    if (thread_ && thread_->joinable())
    {
        thread_->join();
        MY_DELETE thread_;
        thread_ = nullptr;
    }

    SAFE_RELEASE(motion_manager_);
    SAFE_RELEASE(material_manager_);
    SAFE_RELEASE(mesh_manager_);
    SAFE_RELEASE(texture_manager_);
}

//--------------------------------------------------------------------------------
//  �}���`�X���b�h����
//--------------------------------------------------------------------------------
void Resources::Run(void)
{
    while (is_running_)
    {
        material_manager_->Run();
        texture_manager_->Run();
        mesh_manager_->Run();
        motion_manager_->Run();
    }
}
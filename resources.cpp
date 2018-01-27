//--------------------------------------------------------------------------------
//　resources.cpp
//  リソース統括者
//  Author : 徐文杰(KodFreedom)
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
//  破棄処理
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
//  初期化処理
//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
void Resources::Init(const LPDIRECT3DDEVICE9 device)
{
    texture_manager_ = TextureManager::Create(device);
    mesh_manager_ = MeshManager::Create(device);
    material_manager_ = MaterialManager::Create();
    motion_manager_ = MotionManager::Create();

    // スレッドの生成
    thread_ = MY_NEW thread(&Resources::Run, this);
}
#endif

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void Resources::Uninit(void)
{
    is_running_ = false;

    // スレッドの破棄
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
//  マルチスレッド処理
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
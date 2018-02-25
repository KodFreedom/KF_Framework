//--------------------------------------------------------------------------------
//  ���b�V���`��R���|�l���g
//�@MeshRenderer.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mesh_renderer.h"
#include "../../game_object/game_object.h"
#include "../../main_system.h"
#include "../../resources/resources.h"
#include "../../resources/mesh_manager.h"
#include "../../resources/material_manager.h"
#include "../../camera/camera.h"
#include "../../camera/camera_manager.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool MeshRenderer::Init(void)
{
    return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void MeshRenderer::Uninit(void)
{
    auto& resources = MainSystem::Instance().GetResources();
    if (!mesh_name_.empty())
    {
        resources.GetMeshManager().Disuse(mesh_name_);
        mesh_name_.clear();
    }

    if (!material_name_.empty())
    {
        resources.GetMaterialManager().Disuse(material_name_);
        material_name_.clear();
    }
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void MeshRenderer::Update(void)
{
    // FrustumCulling
    auto& main_system = MainSystem::Instance();
    const Vector3& position = Vector3::TransformCoord(bounding_sphere_position_, owner_.GetTransform()->GetWorldMatrix());
    if (main_system.GetCameraManager().GetMainCamera()
        ->FrustumCulling(position, bounding_sphere_radius_))
    {
        main_system.GetRendererManager().Register(this);
        is_registered_ = true;
        return;
    }

    is_registered_ = false;
}

//--------------------------------------------------------------------------------
//  ���b�V���̐ݒ�
//--------------------------------------------------------------------------------
void MeshRenderer::SetMesh(const String& name)
{
    auto& mesh_manager = MainSystem::Instance().GetResources().GetMeshManager();
    if (!mesh_name_.empty())
    {
        mesh_manager.Disuse(mesh_name_);
        mesh_name_.clear();
    }
    mesh_name_ = name;
    mesh_manager.ResourceManager::Use(name);
}

//--------------------------------------------------------------------------------
//  �}�e���A���̐ݒ�
//--------------------------------------------------------------------------------
void MeshRenderer::SetMaterial(const String& name)
{
    auto& material_manager = MainSystem::Instance().GetResources().GetMaterialManager();
    if (!material_name_.empty())
    {
        material_manager.Disuse(material_name_);
        material_name_.clear();
    }
    material_name_ = name;
    material_manager.ResourceManager::Use(name);
}
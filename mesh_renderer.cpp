//--------------------------------------------------------------------------------
//	メッシュ描画コンポネント
//　MeshRenderer.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mesh_renderer.h"
#include "game_object.h"
#include "main_system.h"
#include "mesh_manager.h"
#include "material_manager.h"
#include "camera.h"
#include "camera_manager.h"

//--------------------------------------------------------------------------------
//
//	Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool MeshRenderer::Init(void)
{
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void MeshRenderer::Uninit(void)
{
	auto main_system = MainSystem::Instance();
	if (!mesh_name_.empty())
	{
		main_system->GetMeshManager()->Disuse(mesh_name_);
		mesh_name_.clear();
	}

	if (!material_name_.empty())
	{
		main_system->GetMaterialManager()->Disuse(mesh_name_);
		material_name_.clear();
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void MeshRenderer::Update(void)
{
	auto main_system = MainSystem::Instance();
	if (main_system->GetCameraManager()->
		GetMainCamera()->IsInRange(
		owner_.GetTransform()->GetPosition()))
	{
		main_system->GetRendererManager()->Register(this);
	}
}

//--------------------------------------------------------------------------------
//  メッシュの設定
//--------------------------------------------------------------------------------
void MeshRenderer::SetMesh(const String& name)
{
	auto mesh_manager = MainSystem::Instance()->GetMeshManager();
	if (!mesh_name_.empty())
	{
		mesh_manager->Disuse(mesh_name_);
		mesh_name_.clear();
	}
	mesh_name_ = name;
	mesh_manager->Use(name);
}

//--------------------------------------------------------------------------------
//  マテリアルの設定
//--------------------------------------------------------------------------------
void MeshRenderer::SetMaterial(const String& name)
{
	auto material_manager = MainSystem::Instance()->GetMaterialManager();
	if (!material_name_.empty())
	{
		material_manager->Disuse(material_name_);
		material_name_.clear();
	}
	material_name_ = name;
	material_manager->Use(name);
}
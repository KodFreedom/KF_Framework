//--------------------------------------------------------------------------------
//	�`��R���|�l���g
//�@MeshRenderer.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-18	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "meshRenderer.h"
#include "gameObject.h"
#include "meshManager.h"
#include "materialManager.h"
#include "rendererManager.h"
#include "camera.h"
#include "cameraManager.h"

//--------------------------------------------------------------------------------
//
//	Public
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
	if (!meshName.empty())
	{
		MeshManager::Instance()->Disuse(meshName);
		meshName.clear();
	}

	if (!materialName.empty())
	{
		MaterialManager::Instance()->Disuse(meshName);
		materialName.clear();
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void MeshRenderer::Update(void)
{
	if (CameraManager::Instance()->GetMainCamera()->IsInRange(
		owner->GetTransform()->GetCurrentPosition()))
	{
		RendererManager::Instance()->Register(this);
	}
}

//--------------------------------------------------------------------------------
//  ���b�V���̐ݒ�
//--------------------------------------------------------------------------------
void MeshRenderer::SetMeshName(const string& name)
{
	if (!meshName.empty())
	{
		MeshManager::Instance()->Disuse(meshName);
		meshName.clear();
	}

	meshName = name;
	MeshManager::Instance()->Use(name);
}

//--------------------------------------------------------------------------------
//  �}�e���A���̐ݒ�
//--------------------------------------------------------------------------------
void MeshRenderer::SetMaterialName(const string& name)
{
	if (!materialName.empty())
	{
		MaterialManager::Instance()->Disuse(materialName);
		materialName.clear();
	}

	materialName = name;
	MaterialManager::Instance()->Use(name);
}
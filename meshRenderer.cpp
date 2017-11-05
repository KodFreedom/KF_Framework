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
#include "meshComponent.h"
#include "meshManager.h"
#include "textureManager.h"
#include "materialManager.h"
#include "camera.h"
#include "cameraManager.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//	�N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool MeshRenderer::Init(void)
{
	auto mesh = owner->GetMesh();
	auto& renderInfo = MeshManager::Instance()->GetRenderInfoBy(mesh->GetMeshName());
	if (!renderInfo.TextureName.empty()) Set(renderInfo.TextureName);
	lighting = renderInfo.CurrentLighting;
	cullMode = renderInfo.CurrentCullMode;
	synthesis = renderInfo.CurrentSynthesis;
	fillMode = renderInfo.CurrentFillMode;
	alpha = renderInfo.CurrentAlpha;
	fog = renderInfo.CurrentFog;
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void MeshRenderer::Uninit(void)
{
	if (!textureName.empty())
	{
		TextureManager::Instance()->Disuse(textureName);
		textureName.clear();
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void MeshRenderer::Update(void)
{
	if (CameraManager::Instance()->GetMainCamera()->
		IsInRange(owner->GetTransform()->GetCurrentPosition()))
	{
		RenderManager::Instance()->Register(this);
	}
}

//--------------------------------------------------------------------------------
//  �e�N�X�`���ݒ�
//--------------------------------------------------------------------------------
void MeshRenderer::Set(const string& texture)
{
	if (!textureName.empty())
	{
		TextureManager::Instance()->Disuse(textureName);
		textureName.clear();
	}
	textureName = texture;
	TextureManager::Instance()->Use(textureName);
}
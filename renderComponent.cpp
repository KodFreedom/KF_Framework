//--------------------------------------------------------------------------------
//	�`��R���|�l���g
//�@renderComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-18	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "meshRenderer.h"
#include "manager.h"
#include "gameObject.h"
#include "meshComponent.h"
#include "meshManager.h"
#include "textureManager.h"
#include "materialManager.h"

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
	auto mesh = m_pGameObj->GetMeshComponent();
	auto meshManager = Main::GetManager()->GetMeshManager();
	auto renderInfo = meshManager->GetRenderInfo(mesh->GetMeshName());
	
	if (!renderInfo.strTex.empty()) { SetTexName(renderInfo.strTex); }
	m_renderPriority = renderInfo.renderPriority;
	m_renderState = renderInfo.renderState;
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void MeshRenderer::Uninit(void)
{
	if (!m_texture.empty())
	{
		Main::GetManager()->GetTextureManager()->DisuseTexture(m_texture);
		m_texture.clear();
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void MeshRenderer::Update(void)
{
	Main::GetManager()->GetRenderManager()->Register(this, m_renderPriority, m_renderState);
}

//--------------------------------------------------------------------------------
//  �e�N�X�`���ݒ�
//--------------------------------------------------------------------------------
void MeshRenderer::SetTexName(const string& texture)
{
	if (!m_texture.empty())
	{
		Main::GetManager()->GetTextureManager()->DisuseTexture(m_texture);
		m_texture.clear();
	}

	m_texture = texture;
	Main::GetManager()->GetTextureManager()->UseTexture(m_texture);
}
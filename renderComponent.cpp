//--------------------------------------------------------------------------------
//	�`��R���|�l���g
//�@renderComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-18	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "renderComponent.h"
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
bool CRenderComponent::Init(void)
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
void CRenderComponent::Uninit(void)
{
	if (!m_strTexName.empty())
	{
		Main::GetManager()->GetTextureManager()->DisuseTexture(m_strTexName);
		m_strTexName.clear();
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CRenderComponent::Update(void)
{
	Main::GetManager()->GetRenderManager()->Register(this, m_renderPriority, m_renderState);
}

//--------------------------------------------------------------------------------
//  �e�N�X�`���ݒ�
//--------------------------------------------------------------------------------
void CRenderComponent::SetTexName(const string& strTexName)
{
	if (!m_strTexName.empty())
	{
		Main::GetManager()->GetTextureManager()->DisuseTexture(m_strTexName);
		m_strTexName.clear();
	}

	m_strTexName = strTexName;
	Main::GetManager()->GetTextureManager()->UseTexture(m_strTexName);
}
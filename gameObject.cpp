//--------------------------------------------------------------------------------
//
//�@gameObject.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "gameObject.h"

#ifdef _DEBUG
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
CNullRigidbodyComponent	CGameObject::s_nullRigidbody;
CNullMeshComponent		CGameObject::s_nullMesh;
CNullRenderComponent	CGameObject::s_nullRender;

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObject::CGameObject(const GOMLAYER& layer)
	: m_layer(layer)
	, m_bActive(true)
	, m_bAlive(true)
	, m_pRigidbody(&s_nullRigidbody)
	, m_mesh(&s_nullMesh)
	, m_pRender(&s_nullRender)
{
	m_strName.clear();
	m_strTag.clear();
	m_listpBehavior.clear();
	m_listpCollider.clear();
	m_pTransform = new CTransformComponent(this);
	Main::GetManager()->GetGameObjectManager()->Register(this, m_layer);
}

//--------------------------------------------------------------------------------
//  SetActive
//--------------------------------------------------------------------------------
void CGameObject::SetActive(const bool& bActive)
{
	m_bActive = bActive;

	//Children
	auto& listChildren = m_pTransform->GetChildren();
	for (auto pChild : listChildren)
	{
		pChild->GetGameObject()->SetActive(bActive);
	}
}

//--------------------------------------------------------------------------------
//  SetAlive
//--------------------------------------------------------------------------------
void CGameObject::SetAlive(const bool& bAlive)
{
	m_bAlive = bAlive;

	//Children
	auto& listChildren = m_pTransform->GetChildren();
	for (auto pChild : listChildren)
	{
		pChild->GetGameObject()->SetAlive(bAlive);
	}
}

//--------------------------------------------------------------------------------
//
//  Protected
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �p�����[�^�[��������
//--------------------------------------------------------------------------------
void CGameObject::swapParam(void)
{
	m_pTransform->SwapParam();
}
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

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
CNullRigidbodyComponent	GameObject::s_nullRigidbody;
CNullMeshComponent		GameObject::s_nullMesh;
CNullRenderComponent	GameObject::s_nullRender;

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
GameObject::GameObject(const Layer& layer)
	: m_layer(layer)
	, m_bActive(true)
	, m_bAlive(true)
	, m_pRigidbody(&s_nullRigidbody)
	, m_mesh(&s_nullMesh)
	, m_pRender(&s_nullRender)
{
	m_strName.clear();
	tag.clear();
	m_listpBehavior.clear();
	m_listpCollider.clear();
	m_pTransform = new Transform(this);
	Main::GetManager()->GetGameObjectManager()->Register(this, m_layer);
}

//--------------------------------------------------------------------------------
//  SetActive
//--------------------------------------------------------------------------------
void GameObject::SetActive(const bool& bActive)
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
void GameObject::SetAlive(const bool& bAlive)
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
void GameObject::swapParam(void)
{
	m_pTransform->SwapParam();
}
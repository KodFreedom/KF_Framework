//--------------------------------------------------------------------------------
//	�Q�[���I�u�W�F�N�g�X�[�p�[�N���X
//�@gameObject.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObjectManager.h"
#include "transformComponent.h"
#include "behaviorComponent.h"
#include "drawComponent.h"
#include "rigidbodyComponent.h"
#include "meshComponent.h"
#include "colliderComponent.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �Q�[���I�u�W�F�N�g�N���X
//--------------------------------------------------------------------------------
class CGameObject
{
	friend CGameObjectManager;

public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CGameObject(const GOM::PRIORITY& pri);
	~CGameObject() {}
	
	virtual bool	Init(void) 
	{ 
		m_pTransform->Init();
		for (auto pBehavior : m_listpBehavior) { pBehavior->Init(); }
		m_pRigidbody->Init();
		for (auto pCollider : m_listpCollider) { pCollider->Init(); }
		m_pMesh->Init();
		m_pDraw->Init();
		return true;
	}
	virtual void	Uninit(void)
	{
		m_pTransform->Release();
		for (auto itr = m_listpBehavior.begin(); itr != m_listpBehavior.end();)
		{
			(*itr)->Release();
			itr = m_listpBehavior.erase(itr);
		}
		m_pRigidbody->Release();
		for (auto itr = m_listpCollider.begin(); itr != m_listpCollider.end();) 
		{ 
			(*itr)->Release(); 
			itr = m_listpCollider.erase(itr);
		}
		m_pMesh->Release();
		m_pDraw->Release();
	}
	virtual void	Update(void)
	{
		if (!m_bActive) { return; }
		swapParam();
		for (auto pBehavior : m_listpBehavior) { pBehavior->Update(); }
		m_pRigidbody->Update();
		for (auto pCollider : m_listpCollider) { pCollider->Update(); }
	}
	virtual void	LateUpdate(void)
	{
		if (!m_bActive) { return; }
		m_pRigidbody->LateUpdate();
		for (auto pBehavior : m_listpBehavior) { pBehavior->LateUpdate(); }
		m_pMesh->Update();
		m_pTransform->UpdateMatrix();
	}
	virtual void	Draw(void)
	{
		if (!m_bActive) { return; }
		m_pDraw->Draw();

#ifdef _DEBUG
		DrawNormal();
#endif
	}
	void			Release(void);

	//Get�֐�
	auto		GetTransformComponent(void) const { return m_pTransform; }
	auto&		GetBehaviorComponent(void) { return m_listpBehavior; }
	auto		GetMeshComponent(void) const { return m_pMesh; }
	auto		GetRigidbodyComponent(void) const { return m_pRigidbody; }
	auto&		GetColliderComponent(void) const { return m_listpCollider; }
	const auto&	GetName(void) const { return m_strName; }
	const auto&	GetParentName(void) const
	{
		auto pParent = m_pTransform;
		while (pParent->GetParent()) { pParent = pParent->GetParent(); }
		return pParent->GetGameObject()->GetName();
	}
	const auto&	GetTag(void) const { return m_strTag; }
	bool		IsActive(void) const { return m_bActive; }

	//Set�֐�
	void		SetMeshComponent(CMeshComponent* pMesh) { m_pMesh = pMesh; }
	void		SetDrawComponent(CDrawComponent* pDraw) { m_pDraw = pDraw; }
	void		SetRigidbodyComponent(CRigidbodyComponent* pRb) { m_pRigidbody = pRb; }
	void		SetActive(const bool& bActive);
	void		SetAlive(const bool& bAlive);
	void		SetName(const string& strName) { m_strName = strName; }
	void		SetTag(const string& strTag) { m_strTag = strTag; }

	void		AddBehavior(CBehaviorComponent* pBehavior);
	void		AddCollider(CColliderComponent* pCollider);
	void		DeleteCollider(CColliderComponent* pCollider);

protected:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	virtual void				swapParam(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	//�R���|�l���g
	CTransformComponent*		m_pTransform;	//�ʒu�֌W�p�[�c
	list<CBehaviorComponent*>	m_listpBehavior;//�s���R���|�l���g
	CRigidbodyComponent*		m_pRigidbody;	//���������p�[�c
	list<CColliderComponent*>	m_listpCollider;//�R���W�����p�[�c
	CMeshComponent*				m_pMesh;		//���b�V���p�[�c
	CDrawComponent*				m_pDraw;		//�`�揈���p�[�c

private:
#ifdef _DEBUG
	void		DrawNormal(void);
#endif

	//�p�����[�^�[(�Ǘ��p)
	bool				m_bActive;		//�����t���O
	bool				m_bAlive;		//������t���O
	GOM::PRIORITY		m_pri;			//�D��x
	string				m_strName;		//�I�u�W�F�N�g�̖��O
	string				m_strTag;		//�I�u�W�F�N�g�̃^�O

	//�k���R���|�l���g
	static CNullRigidbodyComponent	s_nullRigidbody;
	static CNullMeshComponent		s_nullMesh;
	static CNullDrawComponent		s_nullDraw;
};
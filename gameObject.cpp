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
CNullDrawComponent		CGameObject::s_nullDraw;

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
CGameObject::CGameObject(const GOM::PRIORITY &pri)
	: m_pri(pri)
	, m_bActive(true)
	, m_bAlive(true)
	, m_pRigidbody(&s_nullRigidbody)
	, m_pMesh(&s_nullMesh)
	, m_pDraw(&s_nullDraw)
{
	m_strName.clear();
	m_strTag.clear();
	m_listpBehavior.clear();
	m_listpCollider.clear();
	m_pTransform = new CTransformComponent(this);
	CMain::GetManager()->GetGameObjectManager()->RegisterGameObj(m_pri, this);
}

//--------------------------------------------------------------------------------
//  �j������
//--------------------------------------------------------------------------------
void CGameObject::Release(void)
{
	CMain::GetManager()->GetGameObjectManager()->DeregisterGameObj(m_pri, this);
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�̒ǉ�
//--------------------------------------------------------------------------------
void CGameObject::AddBehavior(CBehaviorComponent* pBehavior)
{
	m_listpBehavior.push_back(pBehavior);
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�̒ǉ�
//--------------------------------------------------------------------------------
void CGameObject::AddCollider(CColliderComponent* pCollider)
{
	m_listpCollider.push_back(pCollider);
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�̍폜
//--------------------------------------------------------------------------------
void CGameObject::DeleteCollider(CColliderComponent* pCollider)
{
	m_listpCollider.remove(pCollider);
}

//--------------------------------------------------------------------------------
//  SetActive
//--------------------------------------------------------------------------------
void CGameObject::SetActive(const bool& bActive)
{
	m_bActive = bActive;
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

//--------------------------------------------------------------------------------
//  �@���̕`��
//--------------------------------------------------------------------------------
#ifdef _DEBUG
void CGameObject::DrawNormal(void)
{
	//LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	//D3DXMATRIX mtx, mtxPos;
	//D3DXMatrixIdentity(&mtx);
	//pDevice->SetTransform(D3DTS_WORLD, &mtx);

	//VERTEX_3D vtx[2];
	//vtx[0].vPos = m_pTransform->GetPos();
	//vtx[0].ulColor = CKFColor(1.0f, 0.0f, 0.0f, 1.0f);
	//vtx[1].vPos = m_pTransform->GetPos() + m_pTransform->GetUp() * 2.0f;
	//vtx[1].ulColor = CKFColor(1.0f, 0.0f, 0.0f, 1.0f);
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	////�v���~�e�B�u�`��
	//pDevice->DrawPrimitiveUP(D3DPT_LINELIST,//�v���~�e�B�u�̎��
	//	1,//�v���~�e�B�u�̐�
	//	&vtx[0],//���_�f�[�^�̐擪�A�h���X
	//	sizeof(VERTEX_3D)//����̃T�C�Y
	//);
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
#endif
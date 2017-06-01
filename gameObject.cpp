//--------------------------------------------------------------------------------
//
//�@gameObject.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
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
CNullInputComponent		CGameObject::s_nullInput;
CNullPhysicsComponent	CGameObject::s_nullPhysics;
CNullCollisionComponent	CGameObject::s_nullCollision;
CNullMeshComponent		CGameObject::s_nullMesh;
CNullDrawComponent		CGameObject::s_nullDraw;

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
CGameObject::CGameObject(const GOM::PRIORITY &pri)
	: m_pri(pri)
	, m_bActive(true)
	, m_bAlive(true)
	, m_pInput(&s_nullInput)
	, m_pPhysics(&s_nullPhysics)
	, m_pCollision(&s_nullCollision)
	, m_pMesh(&s_nullMesh)
	, m_pDraw(&s_nullDraw)
	, m_vPos(CKFVec3(0.0f))
	, m_vPosNext(CKFVec3(0.0f))
	, m_vRot(CKFVec3(0.0f))
	, m_vRotNext(CKFVec3(0.0f))
	, m_vScale(CKFVec3(0.0f))
	, m_vScaleNext(CKFVec3(0.0f))
{
	CKFMath::MtxIdentity(&m_mtxThis);
	GetManager()->GetGameObjectManager()->SaveGameObj(m_pri, this);
}

//--------------------------------------------------------------------------------
//  �j������
//--------------------------------------------------------------------------------
void CGameObject::Release(void)
{
	GetManager()->GetGameObjectManager()->ReleaseGameObj(m_pri, this);
}

//--------------------------------------------------------------------------------
//  �p�����[�^�[��������
//--------------------------------------------------------------------------------
void CGameObject::SwapParam(void)
{
	m_vPos = m_vPosNext;
	m_vRot = m_vRotNext;
	m_vScale = m_vScaleNext;
}
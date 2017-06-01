//--------------------------------------------------------------------------------
//
//�@3DPhysicsComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-31
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "3DPhysicsComponent.h"
#include "gameObject3D.h"
#include "manager.h"
#include "modeDemo.h"
#include "camera.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
C3DPhysicsComponent::C3DPhysicsComponent() : CPhysicsComponent()
	, m_fMovement(0.0f)
	, m_fRot(0.0f)
{
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void C3DPhysicsComponent::Update(CGameObject &gameObj)
{
	CGameObject3D* pObj = (CGameObject3D*)&gameObj;
	CCamera* pCamera = GetManager()->GetModeNow()->GetCamera();

	CKFVec3 vPos = pObj->GetPosNext();
	CKFVec3 vRot = pObj->GetRotNext();
	CKFVec3 vForwardNext = pObj->GetForwardNext();
	CKFVec3 vForward = pCamera->GetVecLook();
	CKFVec3 vUp = pObj->GetUpNext();

	//�����J�n
	vForward.m_fY = 0.0f;
	CKFMath::VecNormalize(&vForward);

	//�����Z�o
	if (m_fRot != 0.0f)
	{
		CKFMtx44 mtxYaw;
		CKFMath::MtxRotAxis(&mtxYaw, vUp, m_fRot);
		CKFMath::Vec3TransformNormal(&vForward, mtxYaw);
	}

	vPos += vForward * m_fMovement;
	//������Meshfield�Ɠ����蔻��
	vPos.m_fY = ((CModeDemo*)GetManager()->GetModeNow())->GetHeight(vPos);

	//��]�v�Z
	float fRotChanged = CKFMath::RadianBetweenVec3(vForward, vForwardNext);
	fRotChanged *= 0.2f;
	if (fRotChanged != 0.0f)
	{
		CKFMtx44 mtxYaw;
		CKFMath::MtxRotAxis(&mtxYaw, vUp, fRotChanged);
		CKFMath::Vec3TransformNormal(&vForwardNext, mtxYaw);
	}
	vRot.m_fY += fRotChanged;
	CKFMath::NormalizeRotInTwoPi(&vRot);

	//��������
	m_fMovement = 0.0f;
	m_fRot = 0.0f;
	pObj->SetPosNext(vPos);
	pObj->SetRotNext(vRot);
	pObj->SetForwardNext(vForwardNext);
	pObj->SetUpNext(vUp);
}
//--------------------------------------------------------------------------------
//
//�@playerInputComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-31
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "playerInputComponent.h"
#include "manager.h"
#include "mode.h"
#include "camera.h"
#include "inputManager.h"
#include "gameObjectActor.h"
#include "actorMeshComponent.h"
#include "3DRigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CPlayerInputComponent::CPlayerInputComponent(CGameObject* const pGameObj, const float& fMoveSpeed, const float& fJumpForce)
	: CInputComponent(pGameObj)
	, c_fSpeed(fMoveSpeed)
	, c_fJumpForce(fJumpForce)
{}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CPlayerInputComponent::Update(void)
{
	//�R���|�l���g
	CRigidbodyComponent* pRB = m_pGameObj->GetRigidbodyComponent();
	CMeshComponent* pMesh = m_pGameObj->GetMeshComponent();
	if (pRB->GetType() != CRigidbodyComponent::RB_3D) { return; }

	CInputManager* pInput = GetManager()->GetInputManager();
	C3DRigidbodyComponent* p3DRB = (C3DRigidbodyComponent*)pRB;

	//�ړ�
	CKFVec2 vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());

	if (fabsf(vAxis.m_fX) > 0.1f || fabsf(vAxis.m_fY) > 0.1f)
	{
		float fRot = CKFMath::Vec2Radian(vAxis) + KF_PI * 0.5f;

		//��]�v�Z
		CKFVec3 vUp = m_pGameObj->GetUpNext();
		CKFVec3 vForward = m_pGameObj->GetForwardNext();
		CKFVec3 vRight = m_pGameObj->GetRightNext();

		//�J�����������Z�o����
		CCamera* pCamera = GetManager()->GetMode()->GetCamera();
		CKFVec3 vForwardCamera = pCamera->GetVecLook();
		CKFVec3 vForwardNext = (vUp * vForwardCamera) * vUp;

		if (fRot != 0.0f)
		{//������s����������]����
			CKFMtx44 mtxYaw;
			CKFMath::MtxRotAxis(&mtxYaw, vUp, fRot);
			CKFMath::Vec3TransformNormal(&vForwardNext, mtxYaw);
		}

		CKFMath::VecNormalize(&vForwardNext);
		vForwardNext = CKFMath::LerpNormal(vForward, vForwardNext, 0.2f);
		m_pGameObj->RotByForward(vForwardNext);

		//�ړ��ݒ�
		p3DRB->MovePos(vForwardNext * c_fSpeed);

		//�ړ����[�V�����ݒ�
		if (pMesh->GetType() == CMeshComponent::MESH_ACTOR)
		{
			CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
			pActor->SetMotionNext(CActorMeshComponent::MOTION_MOVE);
		}
	}
	else
	{
		//�j���[�g�������[�V�����ݒ�
		if (pMesh->GetType() == CMeshComponent::MESH_ACTOR)
		{
			CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
			pActor->SetMotionNext(CActorMeshComponent::MOTION_NEUTAL);
		}
	}

	//�W�����v
	//if (pKeyboard->GetKeyPress(DIK_SPACE) && p3DRB->IsOnGround())
	//{
	//	//������ɃW�����v
	//	p3DRB->AddForce(CKFVec3(0.0f, 1.0f, 0.0f) * c_fJumpForce * DELTA_TIME);
	//	p3DRB->SetOnGround(false);

	//	//�W�����v���[�V�����ݒ�
	//	if (pMesh->GetType() == CMeshComponent::MESH_ACTOR)
	//	{
	//		CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	//		pActor->SetMotionNext(CActorMeshComponent::MOTION_JUMP);
	//	}
	//}
}
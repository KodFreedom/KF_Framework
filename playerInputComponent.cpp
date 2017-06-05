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
#include "inputDX.h"
#include "gameObjectActor.h"
#include "actorMeshComponent.h"
#include "3DRigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CPlayerInputComponent::Update(void)
{
	CKeyboardDX* pKeyboard = GetManager()->GetKeyboard();

	//�R���|�l���g
	CRigidbodyComponent* pRB = m_pGameObj->GetRigidbodyComponent();
	CMeshComponent* pMesh = m_pGameObj->GetMeshComponent();
	if (pRB->GetType() != CRigidbodyComponent::RB_3D) { return; }

	bool bMove = false;		//�ړ��t���b�O
	bool bInverse = false;	//���L�[�t���b�O
	bool bFb = false;		//�O��ړ��t���b�O
	float fRot = 0.0f;		//��]���

	//�ړ�
	if (pKeyboard->GetKeyPress(DIK_W))
	{
		bMove = true;
		bFb = true;
	}

	if (pKeyboard->GetKeyPress(DIK_S))
	{
		bMove = true;
		bFb = true;
		bInverse = true;
		fRot += KF_PI;
	}

	if (pKeyboard->GetKeyPress(DIK_A))
	{
		bMove = true;

		float fRotLR = -KF_PI * 0.5f;

		if (bInverse) { fRotLR = -fRotLR; }
		if (bFb) { fRotLR *= 0.5f; }

		fRot += fRotLR;
	}

	if (pKeyboard->GetKeyPress(DIK_D))
	{
		bMove = true;

		float fRotLR = KF_PI * 0.5f;

		if (bInverse) { fRotLR = -fRotLR; }
		if (bFb) { fRotLR *= 0.5f; }

		fRot += fRotLR;
	}

	//��]�v�Z
	CGameObject3D* pObj = (CGameObject3D*)m_pGameObj;
	CKFVec3 vUp = pObj->GetUpNext();
	CKFVec3 vForward = pObj->GetForwardNext();
	CKFVec3 vRot = pObj->GetRotNext();

	//�J������������v���C���[�̎��̌������Z�o����
	CCamera* pCamera = GetManager()->GetModeNow()->GetCamera();
	CKFVec3 vForwardCamera = pCamera->GetVecLook();
	vForwardCamera.m_fY = 0.0f;
	CKFMtx44 mtxRot;
	CKFMath::MtxRotationYawPitchRoll(&mtxRot, CKFVec3(vRot.m_fX, 0.0f, vRot.m_fZ));
	CKFMath::Vec3TransformNormal(&vForwardCamera, mtxRot);
	CKFMath::VecNormalize(&vForwardCamera);

	if (fRot != 0.0f)
	{//������s����������]����
		CKFMtx44 mtxYaw;
		CKFMath::MtxRotAxis(&mtxYaw, vUp, fRot);
		CKFMath::Vec3TransformNormal(&vForwardCamera, mtxYaw);
	}

	CKFVec3 vForwardNext = CKFMath::LerpNormal(vForward, vForwardCamera, 0.2f);
	CKFVec3 vRotChanged = CKFMath::EulerBetweenVec3(vForward, vForwardNext);
	vRot += vRotChanged;
	CKFMath::NormalizeRotInTwoPi(&vRot);
	pObj->SetRotNext(vRot);
	pObj->SetForwardNext(vForwardNext);

	if (bMove)
	{
		//�ړ��ݒ�
		C3DRigidbodyComponent *p3DRB = (C3DRigidbodyComponent*)pRB;
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
		//���[�V�����ݒ�
		if (pMesh->GetType() == CMeshComponent::MESH_ACTOR)
		{
			CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
			pActor->SetMotionNext(CActorMeshComponent::MOTION_NEUTAL);
		}
	}
}
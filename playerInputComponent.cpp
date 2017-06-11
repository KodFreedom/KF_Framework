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
	CKFVec3 vUp = m_pGameObj->GetUpNext();
	CKFVec3 vForward = m_pGameObj->GetForwardNext();
	CKFVec3 vRight = m_pGameObj->GetRightNext();

	//�J������������v���C���[�̎��̌������Z�o����
	CCamera* pCamera = GetManager()->GetMode()->GetCamera();
	CKFVec3 vForwardCamera = pCamera->GetVecLook();
	CKFVec3 vForwardFinal = (vUp * vForwardCamera) * vUp;

	if (fRot != 0.0f)
	{//������s����������]����
		CKFMtx44 mtxYaw;
		CKFMath::MtxRotAxis(&mtxYaw, vUp, fRot);
		CKFMath::Vec3TransformNormal(&vForwardFinal, mtxYaw);
	}

	CKFMath::VecNormalize(&vForwardFinal);
	CKFVec3 vForwardNext = CKFMath::LerpNormal(vForward, vForwardFinal, 0.2f);
	m_pGameObj->RotByForward(vForwardNext);

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
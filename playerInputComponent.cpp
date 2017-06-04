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
#include "inputDX.h"
#include "gameObjectActor.h"
#include "actorMeshComponent.h"
#include "3DPhysicsComponent.h"

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
	CPhysicsComponent* pPhysics = m_pGameObj->GetPhysicsComponent();
	CMeshComponent* pMesh = m_pGameObj->GetMeshComponent();
	if (pPhysics->GetType() != CPhysicsComponent::PSY_3D) { return; }

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

	if (bMove)
	{
		//�ړ��ݒ�
		C3DPhysicsComponent *p3DPhysics = (C3DPhysicsComponent*)pPhysics;
		p3DPhysics->MovePosByRot(c_fSpeed, fRot);

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
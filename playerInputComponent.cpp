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
void CPlayerInputComponent::Update(CGameObject &gameObj)
{
	CKeyboardDX* pKeyboard = GetManager()->GetKeyboard();
	CGameObjectActor *pObjActor = (CGameObjectActor*)&gameObj;

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
		C3DPhysicsComponent *pPhysicsComponent = (C3DPhysicsComponent*)pObjActor->GetPhysicsComponent();
		pPhysicsComponent->MovePosByRot(c_fSpeed, fRot);

		//�ړ����[�V�����ݒ�
		CActorMeshComponent *pActorComponent = (CActorMeshComponent*)pObjActor->GetMeshComponent();
		pActorComponent->SetMotionNext(CActorMeshComponent::MOTION_MOVE);
	}
	else
	{
		//�ړ����[�V�����ݒ�
		CActorMeshComponent *pActorComponent = (CActorMeshComponent*)pObjActor->GetMeshComponent();
		pActorComponent->SetMotionNext(CActorMeshComponent::MOTION_NEUTAL);
	}
}
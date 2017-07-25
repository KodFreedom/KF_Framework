//--------------------------------------------------------------------------------
//	�ړ����
//�@moveStatus.h
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "attackStatus.h"
#include "jumpStatus.h"
#include "moveStatus.h"
#include "normalStatus.h"
#include "gameObject.h"
#include "actorBehaviorComponent.h"
#include "actorMeshComponent.h"
#include "mode.h"
#include "camera.h"
#include "manager.h"
#include "inputManager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void CMoveStatus::Update(CActorBehaviorComponent& actor)
{
	CMeshComponent* pMesh = actor.m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	CInputManager* pInput = GetManager()->GetInputManager();

	//�ړ�
	CKFVec2 vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
	if (fabsf(vAxis.m_fX) > 0.1f || fabsf(vAxis.m_fY) > 0.1f)
	{
		float fRot = CKFMath::Vec2Radian(vAxis) + KF_PI * 0.5f;
		CTransformComponent* pTrans = actor.m_pGameObj->GetTransformComponent();

		//��]�v�Z
		CKFVec3 vUp = pTrans->GetUpNext();
		CKFVec3 vForward = pTrans->GetForwardNext();
		CKFVec3 vRight = pTrans->GetRightNext();

		//�J�����������Z�o����
		//CCamera* pCamera = GetManager()->GetMode()->GetCamera();
		//CKFVec3 vForwardCamera = pCamera->GetVecLook();
		//CKFVec3 vForwardNext = (vUp * vForwardCamera) * vUp;

		//if (fRot != 0.0f)
		//{//������s����������]����
		//	CKFMtx44 mtxYaw;
		//	CKFMath::MtxRotAxis(mtxYaw, vUp, fRot);
		//	CKFMath::Vec3TransformNormal(vForwardNext, mtxYaw);
		//}

		//CKFMath::VecNormalize(vForwardNext);

		//��]
		actor.Turn(pActor);

		//�ړ��ݒ�
		actor.Move(pActor);
	}
	else
	{
		actor.ChangeStatus(new CNormalStatus);
	}

	//�W�����v
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_JUMP))
	{
		actor.ChangeStatus(new CJumpStatus);
		return;
	}

	//�U��
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_ATTACK))
	{
		actor.ChangeStatus(new CAttackStatus);
		return;
	}
}

//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void CMoveStatus::LateUpdate(CActorBehaviorComponent& actor)
{

}
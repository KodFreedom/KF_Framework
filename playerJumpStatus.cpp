//--------------------------------------------------------------------------------
//	�v���C���[�W�����v�X�e�[�^�X
//�@playerJumpStatus.h
//	Author : Xu Wenjie
//	Date   : 2017-07-24
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "playerJumpStatus.h"
#include "playerMoveStatus.h"
#include "playerNormalStatus.h"
#include "playerAttackStatus.h"
#include "gameObject.h"
#include "actorBehaviorComponent.h"
#include "actorMeshComponent.h"
#include "3DRigidbodyComponent.h"
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
void CPlayerJumpStatus::Update(CActorBehaviorComponent& actor)
{
	//�W�����v
	if (m_bFirst)
	{
		m_bFirst = false;
		CMeshComponent* pMesh = actor.m_pGameObj->GetMeshComponent();
		CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
		actor.Jump(pActor);
		return;
	}

	//�ړ�
	CInputManager* pInput = GetManager()->GetInputManager();
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
		CCamera* pCamera = GetManager()->GetMode()->GetCamera();
		CKFVec3 vForwardCamera = pCamera->GetVecLook();
		CKFVec3 vForwardNext = (vUp * vForwardCamera) * vUp;

		if (fRot != 0.0f)
		{//������s����������]����
			CKFMtx44 mtxYaw;
			CKFMath::MtxRotAxis(mtxYaw, vUp, fRot);
			CKFMath::Vec3TransformNormal(vForwardNext, mtxYaw);
		}

		CKFMath::VecNormalize(vForwardNext);

		//��]
		actor.Turn(vForwardNext);

		//�ړ��ݒ�
		actor.Move();
	}

	//�����V����m�[�}���X�e�[�^�X�ɖ߂�
	if (actor.m_pRigidbody->IsOnGround())
	{
		actor.ChangeStatus(new CPlayerNormalStatus);
	}
}

//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void CPlayerJumpStatus::LateUpdate(CActorBehaviorComponent& actor)
{
	
}
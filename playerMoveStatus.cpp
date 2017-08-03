//--------------------------------------------------------------------------------
//	Player�ړ����
//�@playerMoveStatus.h
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "playerMoveStatus.h"
#include "playerNormalStatus.h"
#include "playerJumpStatus.h"
#include "playerAttackStatus.h"
#include "gameObject.h"
#include "playerBehaviorComponent.h"
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
void CPlayerMoveStatus::Update(CPlayerBehaviorComponent& player)
{
	CMeshComponent* pMesh = player.m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	CInputManager* pInput = GetManager()->GetInputManager();

	//�ړ�
	CKFVec2 vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
	if (fabsf(vAxis.m_fX) > 0.1f || fabsf(vAxis.m_fY) > 0.1f)
	{
		float fRot = CKFMath::Vec2Radian(vAxis) + KF_PI * 0.5f;
		CTransformComponent* pTrans = player.m_pGameObj->GetTransformComponent();

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
			vForwardNext = CKFMath::Vec3TransformNormal(vForwardNext, mtxYaw);
		}

		CKFMath::VecNormalize(vForwardNext);

		//��]
		player.Turn(vForwardNext, pActor);

		//�ړ��ݒ�
		player.Move(pActor);
	}
	else
	{
		player.ChangeStatus(new CPlayerNormalStatus);
	}

	//�W�����v
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_JUMP))
	{
		player.ChangeStatus(new CPlayerJumpStatus);
		return;
	}

	//�U��
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_ATTACK))
	{
		player.ChangeStatus(new CPlayerAttackStatus);
		return;
	}
}

//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void CPlayerMoveStatus::LateUpdate(CPlayerBehaviorComponent& player)
{

}
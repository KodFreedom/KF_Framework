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
#include "sphereColliderComponent.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CPlayerInputComponent::CPlayerInputComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody, const float& fMoveSpeed, const float& fJumpForce)
	: CInputComponent(pGameObj)
	, m_pRigidbody(pRigidbody)
	, c_fSpeed(fMoveSpeed)
	, c_fJumpForce(fJumpForce)
{}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CPlayerInputComponent::Update(void)
{
	CMeshComponent* pMesh = m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	CInputManager* pInput = GetManager()->GetInputManager();
	bool bCanControl = true;

	if (pActor->GetMotionNow() == CActorMeshComponent::MOTION::MOTION_ATTACK) { bCanControl = false; }

	if (bCanControl)
	{
		//�ړ�
		CKFVec2 vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
		if (fabsf(vAxis.m_fX) > 0.1f || fabsf(vAxis.m_fY) > 0.1f)
		{
			float fRot = CKFMath::Vec2Radian(vAxis) + KF_PI * 0.5f;
			CTransformComponent* pTrans = m_pGameObj->GetTransformComponent();

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
			vForwardNext = CKFMath::LerpNormal(vForward, vForwardNext, 0.2f);
			pTrans->RotByForward(vForwardNext);

			//�ړ��ݒ�
			m_pRigidbody->MovePos(vForwardNext * c_fSpeed);

			//�ړ����[�V�����ݒ�
			pActor->SetMotion(CActorMeshComponent::MOTION_MOVE);
		}
		else
		{
			//�j���[�g�������[�V�����ݒ�
			pActor->SetMotion(CActorMeshComponent::MOTION_NEUTAL);
		}

		//�W�����v
		if (pInput->GetKeyTrigger(CInputManager::KEY::K_JUMP) && m_pRigidbody->IsOnGround())
		{
			//������ɃW�����v
			m_pRigidbody->AddForce(CKFVec3(0.0f, 1.0f, 0.0f) * c_fJumpForce * DELTA_TIME);
			m_pRigidbody->SetOnGround(false);

			//�W�����v���[�V�����ݒ�
			pActor->SetMotion(CActorMeshComponent::MOTION_JUMP);
		}

		//�U��
		if (pInput->GetKeyTrigger(CInputManager::KEY::K_ATTACK))
		{
			//�W�����v���[�V�����ݒ�
			pActor->SetMotion(CActorMeshComponent::MOTION_ATTACK);
		}
	}
}
//--------------------------------------------------------------------------------
//	�v���C���[�r�w�C�r�A�R���|�l���g
//�@playerBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "playerBehaviorComponent.h"
#include "actorBehaviorComponent.h"
#include "manager.h"
#include "inputManager.h"
#include "modeDemo.h"
#include "camera.h"
#include "gameObjectActor.h"
#include "colliderComponent.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CPlayerBehaviorComponent::CPlayerBehaviorComponent(CGameObject* const pGameObj, CActorBehaviorComponent& actor)
	: CBehaviorComponent(pGameObj)
	, m_actor(actor)
{}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool CPlayerBehaviorComponent::Init(void)
{
	m_actor.SetLifeMax(100.0f);
	m_actor.SetLifeNow(100.0f);
	m_actor.SetJumpSpeed(5.0f);
	m_actor.SetTurnSpeedMin(2.0f * KF_PI * DELTA_TIME);
	m_actor.SetTurnSpeedMax(4.0f * KF_PI * DELTA_TIME);
	m_actor.SetMoveSpeed(0.15f);
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::Uninit(void)
{
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::Update(void)
{
	auto pInput = CMain::GetManager()->GetInputManager();
	auto vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
	auto pCamera = CMain::GetManager()->GetMode()->GetCamera();
	auto vCamForward = CKFMath::Vec3Scale(pCamera->GetVecLook(), CKFMath::VecNormalize(CKFVec3(1.0f, 0.0f, 1.0f)));
	auto vMove = pCamera->GetVecRight() * vAxis.m_fX + vCamForward * vAxis.m_fY;
	if (CKFMath::VecMagnitudeSquare(vMove) > 1.0f) { CKFMath::VecNormalize(vMove); }
	auto bJump = pInput->GetKeyTrigger(CInputManager::K_JUMP);
	auto bAttack = pInput->GetKeyTrigger(CInputManager::K_ATTACK);
	m_actor.Act(vMove, bJump, bAttack);;

	/*if (m_usCntWhosYourDaddy) { m_usCntWhosYourDaddy--; }
	CMeshComponent* pMesh = m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	CInputManager* pInput = CMain::GetManager()->GetInputManager();
	bool bCanControl = true;

	if (pActor->GetMotionNow() == CActorMeshComponent::MOTION::MOTION_ATTACK) 
	{
		if (pActor->GetMotionInfo().nKeyNow == 3 && !m_pAttackCollider)
		{
			m_pAttackCollider = new CSphereColliderComponent(m_pGameObj, CM::DYNAMIC, CKFVec3(0.0f, 0.6f, 2.1f), 0.9f);
			m_pAttackCollider->SetTag("weapon");
			m_pAttackCollider->SetTrigger(true);
		}
		bCanControl = false; 
	}

	if (!bCanControl) { return; }

	if (m_pAttackCollider)
	{
		m_pGameObj->DeleteCollider(m_pAttackCollider);
		m_pAttackCollider->Release();
		m_pAttackCollider = nullptr;
	}

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
		CCamera* pCamera = CMain::GetManager()->GetMode()->GetCamera();
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
		c_pRigidbody->MovePos(vForwardNext * c_fSpeed);

		//�ړ����[�V�����ݒ�
		pActor->SetMotion(CActorMeshComponent::MOTION_MOVE);
	}
	else
	{
		//�j���[�g�������[�V�����ݒ�
		pActor->SetMotion(CActorMeshComponent::MOTION_NEUTAL);
	}

	//�W�����v
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_JUMP) && c_pRigidbody->IsOnGround())
	{
		//������ɃW�����v
		c_pRigidbody->AddForce(CKFVec3(0.0f, 1.0f, 0.0f) * c_fJumpForce * DELTA_TIME);
		c_pRigidbody->SetOnGround(false);

		//�W�����v���[�V�����ݒ�
		pActor->SetMotion(CActorMeshComponent::MOTION_JUMP);
	}

	//�U��
	if (pInput->GetKeyTrigger(CInputManager::KEY::K_ATTACK))
	{
		//�W�����v���[�V�����ݒ�
		pActor->SetMotion(CActorMeshComponent::MOTION_ATTACK);
	}*/
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::LateUpdate(void)
{
	if (m_actor.GetLifeNow() <= 0.0f)
	{
		auto pMode = CMain::GetManager()->GetMode();
		auto pModeDemo = dynamic_cast<CModeDemo*>(pMode);
		pModeDemo->EndMode(true);
	}
}

//--------------------------------------------------------------------------------
//  OnTrigger
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider)
{
	if (collider.GetGameObject()->GetObjType() == CGameObject::OT_ENEMY)
	{//����`�F�b�N
		if (collider.GetTag() == "weapon" && colliderThis.GetTag() == "body")
		{
			m_actor.Hit(5.0f);
		}
	}

	if (collider.GetGameObject()->GetObjType() == CGameObject::OT_GOAL)
	{
		if (colliderThis.GetTag() == "body")
		{
			auto pMode = CMain::GetManager()->GetMode();
			auto pModeDemo = dynamic_cast<CModeDemo*>(pMode);
			pModeDemo->EndMode(false);
		}
	}
}

//--------------------------------------------------------------------------------
//  OnCollision
//--------------------------------------------------------------------------------
void CPlayerBehaviorComponent::OnCollision(CCollisionInfo& collisionInfo)
{

}
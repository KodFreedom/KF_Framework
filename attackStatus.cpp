//--------------------------------------------------------------------------------
//	�U���X�e�[�^�X
//�@attackStatus.h
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
#include "sphereColliderComponent.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void CAttackStatus::Update(CActorBehaviorComponent& actor)
{
	CMeshComponent* pMesh = actor.m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;

	//�U��
	if (m_bFirst)
	{
		m_bFirst = false;
		actor.Attack(pActor);
	}

	if (pActor->GetMotionNow() == CActorMeshComponent::MOTION_ATTACK)
	{
		if (pActor->GetMotionInfo().nKeyNow == 3 && !actor.m_pAttackCollider)
		{
			actor.m_pAttackCollider = new CSphereColliderComponent(actor.m_pGameObj, CM::DYNAMIC, CKFVec3(0.0f, 0.6f, 2.1f), 0.9f);
			actor.m_pAttackCollider->SetTag("weapon");
			actor.m_pAttackCollider->SetTrigger(true);
		}
	}
	else
	{//�U�����[�V�����I�������m�[�}���X�e�[�^�X�ɖ߂�
		if (actor.m_pAttackCollider)
		{
			actor.m_pGameObj->DeleteCollider(actor.m_pAttackCollider);
			actor.m_pAttackCollider->Release();
			actor.m_pAttackCollider = nullptr;
		}

		actor.ChangeStatus(new CNormalStatus);
	}
}

//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void CAttackStatus::LateUpdate(CActorBehaviorComponent& actor)
{

}
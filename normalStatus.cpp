//--------------------------------------------------------------------------------
//	�m�[�}���X�e�[�^�X
//�@normalStatus.cpp
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
#include "manager.h"
#include "inputManager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void CNormalStatus::Update(CActorBehaviorComponent& actor)
{
	CInputManager* pInput = GetManager()->GetInputManager();

	//�ړ�
	CKFVec2 vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
	if (fabsf(vAxis.m_fX) > 0.1f || fabsf(vAxis.m_fY) > 0.1f)
	{
		actor.ChangeStatus(new CMoveStatus);
		return;
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

	//�j���[�g����
	CMeshComponent* pMesh = actor.m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	actor.Stay(pActor);
}

//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void CNormalStatus::LateUpdate(CActorBehaviorComponent& actor)
{

}
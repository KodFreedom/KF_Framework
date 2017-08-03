//--------------------------------------------------------------------------------
//	�v���C��m�[�}���X�e�[�^�X
//�@playerNormalStatus.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "playerNormalStatus.h"
#include "playerMoveStatus.h"
#include "playerJumpStatus.h"
#include "playerAttackStatus.h"
#include "gameObject.h"
#include "playerBehaviorComponent.h"
#include "actorMeshComponent.h"
#include "manager.h"
#include "inputManager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void CPlayerNormalStatus::Update(CPlayerBehaviorComponent& player)
{
	CInputManager* pInput = GetManager()->GetInputManager();

	//�ړ�
	CKFVec2 vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
	if (fabsf(vAxis.m_fX) > 0.1f || fabsf(vAxis.m_fY) > 0.1f)
	{
		player.ChangeStatus(new CPlayerMoveStatus);
		return;
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

	//�j���[�g����
	CMeshComponent* pMesh = player.m_pGameObj->GetMeshComponent();
	CActorMeshComponent *pActor = (CActorMeshComponent*)pMesh;
	player.Stay(pActor);
}

//--------------------------------------------------------------------------------
//  �X�V
//--------------------------------------------------------------------------------
void CPlayerNormalStatus::LateUpdate(CPlayerBehaviorComponent& player)
{

}
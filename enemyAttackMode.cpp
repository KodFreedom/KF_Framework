//--------------------------------------------------------------------------------
//	�G�l�~�[�r�w�C�r�A�R���|�l���g
//�@enemyBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "enemyAttackMode.h"
#include "enemyNormalMode.h"
#include "enemyBehaviorComponent.h"
#include "actorBehaviorComponent.h"
#include "gameObjectActor.h"
#include "sphereColliderComponent.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �U�����
//--------------------------------------------------------------------------------
void CEnemyAttackMode::Update(CEnemyBehaviorComponent& enemy)
{
	if (!enemy.m_pTarget)
	{//���肪���Ȃ��Ȃ畁�ʂȏ�Ԃɖ߂�
		enemy.ChangeMode(new CEnemyNormalMode);
		return;
	}

	auto pTransform = enemy.GetGameObject()->GetTransformComponent();
	auto PositionTarget = enemy.m_pTarget->GetTransformComponent()->GetPosNext();
	auto PositionThis = pTransform->GetPosNext();
	auto vForward = pTransform->GetForwardNext();
	auto vDiff = PositionTarget - PositionThis;
	auto vDir = vDiff;
	CKFMath::VecNormalize(vDir);

	//�G���m�͈͂��擾
	float fDisMax = 0.0f;
	auto listCollider = enemy.GetGameObject()->GetColliderComponent();
	for (auto itr = listCollider.begin(); itr != listCollider.end(); ++itr)
	{
		if ((*itr)->GetTag()._Equal("detector"))
		{
			auto pSphereCollider = dynamic_cast<CSphereColliderComponent*>(*itr);
			fDisMax = pSphereCollider->GetRadius();
			break;
		}
	}

	bool bAttack = false;
	float fDisSquare = CKFMath::VecMagnitudeSquare(vDiff);
	if (fDisSquare > fDisMax * fDisMax)
	{//����Ƃ̋�����������Ε��ʂȏ�Ԃɖ߂�
		enemy.ChangeMode(new CEnemyNormalMode);
		enemy.m_pTarget = nullptr;
		return;
	}

	if(m_nCntWait > 0)
	{
		--m_nCntWait;
		return;
	}

	if (fDisSquare <= 2.6f * 2.6f)
	{//�������Z���Ȃ�U��
		bAttack = true;
		m_nCntWait = 120;
	}
	else
	{//����Ɍ������Ĉړ�����
	 //����Ɍ���
	 //��]�v�Z
		Vector3 vUp = pTransform->GetUpNext();
		vDir = (vUp * vDir) * vUp;
	}

	enemy.m_actor.Act(vDir, false, bAttack);
}
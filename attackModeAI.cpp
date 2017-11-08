//--------------------------------------------------------------------------------
//	�G�l�~�[�r�w�C�r�A�R���|�l���g
//�@enemyBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "attackModeAI.h"
#include "normalModeAI.h"
#include "enemyController.h"
#include "actorState.h"
#include "gameObjectActor.h"
#include "sphereCollider.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
const float coolDownTime = 2.0f;
const float attackRange = 2.6f;

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
AttackModeAI::AttackModeAI() : AI("AttackMode"), coolDown(0.0f) {}

//--------------------------------------------------------------------------------
//  �U�����
//--------------------------------------------------------------------------------
void AttackModeAI::Update(EnemyController& enemy)
{
	auto target = enemy.GetTarget();
	if (!target)
	{//���肪���Ȃ��Ȃ畁�ʂȏ�Ԃɖ߂�
		enemy.Change(new NormalModeAI);
		return;
	}

	auto transform = enemy.GetGameObject()->GetTransform();
	auto& targetPosition = target->GetTransform()->GetNextPosition();
	auto& position = transform->GetNextPosition();
	auto& forward = transform->GetNextForward();
	auto& difference = targetPosition - position;
	auto& direction = difference.Normalized();

	//�G���m�͈͂��擾
	float maxDistance = 0.0f;
	auto colliders = enemy.GetGameObject()->GetColliders();
	for (auto collider : colliders)
	{
		if (collider->GetTag()._Equal("detector"))
		{
			maxDistance = dynamic_cast<SphereCollider*>(collider)->GetRadius();
		}
	}

	float squareDistance = difference.SquareMagnitude();
	if (squareDistance > maxDistance * maxDistance)
	{//����Ƃ̋�����������Ε��ʂȏ�Ԃɖ߂�
		enemy.Change(new NormalModeAI);
		enemy.SetTarget(nullptr);
		return;
	}

	if(coolDown > 0.0f)
	{
		coolDown -= DELTA_TIME;
		return;
	}

	bool isAttack = false;
	if (squareDistance <= attackRange * attackRange)
	{//�������Z���Ȃ�U��
		isAttack = true;
		coolDown = coolDownTime;
	}
	else
	{//����Ɍ������Ĉړ�����
	 //����Ɍ���
	 //��]�v�Z
		Vector3 up = transform->GetNextUp();
		direction = (up * direction) * up;
	}

	enemy.SetMovement(direction);
	enemy.SetAttack(isAttack);
}
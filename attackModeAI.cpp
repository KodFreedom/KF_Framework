//--------------------------------------------------------------------------------
//	エネミービヘイビアコンポネント
//　enemyBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "attackModeAI.h"
#include "normalModeAI.h"
#include "enemyController.h"
#include "actorState.h"
#include "gameObjectActor.h"
#include "sphereCollider.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const float coolDownTime = 2.0f;
const float attackRange = 2.6f;

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
AttackModeAI::AttackModeAI() : AI("AttackMode"), coolDown(0.0f) {}

//--------------------------------------------------------------------------------
//  攻撃状態
//--------------------------------------------------------------------------------
void AttackModeAI::Update(EnemyController& enemy)
{
	auto target = enemy.GetTarget();
	if (!target)
	{//相手がいないなら普通な状態に戻る
		enemy.Change(new NormalModeAI);
		return;
	}

	auto transform = enemy.GetGameObject()->GetTransform();
	auto& targetPosition = target->GetTransform()->GetNextPosition();
	auto& position = transform->GetNextPosition();
	auto& forward = transform->GetNextForward();
	auto& difference = targetPosition - position;
	auto& direction = difference.Normalized();

	//敵検知範囲を取得
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
	{//相手との距離が長ければ普通な状態に戻る
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
	{//距離が短いなら攻撃
		isAttack = true;
		coolDown = coolDownTime;
	}
	else
	{//相手に向かって移動する
	 //相手に向く
	 //回転計算
		Vector3 up = transform->GetNextUp();
		direction = (up * direction) * up;
	}

	enemy.SetMovement(direction);
	enemy.SetAttack(isAttack);
}
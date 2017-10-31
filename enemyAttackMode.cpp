//--------------------------------------------------------------------------------
//	エネミービヘイビアコンポネント
//　enemyBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "enemyAttackMode.h"
#include "enemyNormalMode.h"
#include "enemyBehaviorComponent.h"
#include "actorBehaviorComponent.h"
#include "gameObjectActor.h"
#include "sphereColliderComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  攻撃状態
//--------------------------------------------------------------------------------
void CEnemyAttackMode::Update(CEnemyBehaviorComponent& enemy)
{
	if (!enemy.m_pTarget)
	{//相手がいないなら普通な状態に戻る
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

	//敵検知範囲を取得
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
	{//相手との距離が長ければ普通な状態に戻る
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
	{//距離が短いなら攻撃
		bAttack = true;
		m_nCntWait = 120;
	}
	else
	{//相手に向かって移動する
	 //相手に向く
	 //回転計算
		Vector3 vUp = pTransform->GetUpNext();
		vDir = (vUp * vDir) * vUp;
	}

	enemy.m_actor.Act(vDir, false, bAttack);
}
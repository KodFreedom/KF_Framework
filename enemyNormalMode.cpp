//--------------------------------------------------------------------------------
//	エネミービヘイビアコンポネント
//　enemyBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "enemyNormalMode.h"
#include "enemyBehaviorComponent.h"
#include "gameObjectActor.h"
#include "actorBehaviorComponent.h"
//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  普通状態
//--------------------------------------------------------------------------------
void CEnemyNormalMode::Update(CEnemyBehaviorComponent& enemy)
{
	enemy.m_actor.Act(Vector3(0.0f), false, false);
}

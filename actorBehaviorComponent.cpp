//--------------------------------------------------------------------------------
//	生き物ビヘイビアコンポネント
//　actorBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "actorBehaviorComponent.h"
#include "3DRigidbodyComponent.h"
#include "gameObject.h"
#include "transformComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CActorBehaviorComponent::CActorBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent* const pRigidbody)
	: CBehaviorComponent(pGameObj)
	, m_pRigidbody(pRigidbody)
	, m_nLevel(0)
	, m_fLifeMax(0.0f)
	, m_fLifeNow(0.0f)
	, m_fAttack(0.0f)
	, m_fDefence(0.0f)
	, m_fMovementSpeed(0.0f)
	, m_fJumpForce(0.0f)
	, m_fTurnRate(0.0f)
{
}

//--------------------------------------------------------------------------------
//  移動
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Move(void)
{

}

//--------------------------------------------------------------------------------
//  跳ぶ
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Jump(void)
{

}

//--------------------------------------------------------------------------------
//  攻撃
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Attack(void)
{

}

//--------------------------------------------------------------------------------
//  回る
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Turn(const CKFVec3& vForward)
{
	CKFVec3 vForwardNext = CKFMath::LerpNormal(m_pGameObj->GetTransformComponent()->GetForwardNext(), vForward, m_fTurnRate);
	m_pGameObj->GetTransformComponent()->RotByForward(vForwardNext);
}
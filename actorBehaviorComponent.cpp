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
#include "actorMeshComponent.h"

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
	, m_pAttackCollider(nullptr)
{
}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CActorBehaviorComponent::~CActorBehaviorComponent()
{
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Uninit(void)
{
}

//--------------------------------------------------------------------------------
//  更新
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Update(void)
{
}

//--------------------------------------------------------------------------------
//  更新
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::LateUpdate(void)
{
}

//--------------------------------------------------------------------------------
//  ニュートラルに戻る
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Stay(CActorMeshComponent *pActor)
{
	//ニュートラルモーション設定
	pActor->SetMotion(CActorMeshComponent::MOTION_NEUTAL);
}

//--------------------------------------------------------------------------------
//  移動
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Move(void)
{
	m_pRigidbody->MovePos(m_pGameObj->GetTransformComponent()->GetForwardNext() * m_fMovementSpeed);
}

//--------------------------------------------------------------------------------
//  移動
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Move(CActorMeshComponent *pActor)
{
	//移動設定
	Move();

	//移動モーション設定
	pActor->SetMotion(CActorMeshComponent::MOTION_MOVE);
}

//--------------------------------------------------------------------------------
//  跳ぶ
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Jump(void)
{
	if (!m_pRigidbody->IsOnGround()) { return; }

	//上方向にジャンプ
	m_pRigidbody->AddForce(CKFVec3(0.0f, 1.0f, 0.0f) * m_fJumpForce * DELTA_TIME);
	m_pRigidbody->SetOnGround(false);
}

//--------------------------------------------------------------------------------
//  跳ぶ
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Jump(CActorMeshComponent *pActor)
{
	if (!m_pRigidbody->IsOnGround()) { return; }

	//ジャンプ処理
	Jump();

	//ジャンプモーション設定
	pActor->SetMotion(CActorMeshComponent::MOTION_JUMP);
}

//--------------------------------------------------------------------------------
//  攻撃
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Attack(void)
{

}

//--------------------------------------------------------------------------------
//  攻撃
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Attack(CActorMeshComponent *pActor)
{
	//攻撃処理
	Attack();

	//モーション
	pActor->SetMotion(CActorMeshComponent::MOTION_ATTACK);
}

//--------------------------------------------------------------------------------
//  回る
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Turn(const CKFVec3& vForward)
{
	CKFVec3 vForwardNext = CKFMath::LerpNormal(m_pGameObj->GetTransformComponent()->GetForwardNext(), vForward, m_fTurnRate);
	m_pGameObj->GetTransformComponent()->RotByForward(vForwardNext);
}

//--------------------------------------------------------------------------------
//  回る
//--------------------------------------------------------------------------------
void  CActorBehaviorComponent::Turn(const CKFVec3& vForward, CActorMeshComponent *pActor)
{
	//回る処理
	Turn(vForward);

	//モーション

}

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
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CActorBehaviorComponent::CActorBehaviorComponent(CGameObject* const pGameObj, C3DRigidbodyComponent& rigidbody, CActorMeshComponent* const pMesh)
	: CBehaviorComponent(pGameObj)
	, m_rigidbody(rigidbody)
	, m_pActor(pMesh)
	, m_nLevel(0)
	, m_fLifeMax(0.0f)
	, m_fLifeNow(0.0f)
	, m_fAttack(0.0f)
	, m_fDefence(0.0f)
	, m_fMoveSpeed(0.0f)
	, m_fJumpSpeed(0.0f)
	, m_fTurnSpeedMin(0.0f)
	, m_fTurnSpeedMax(0.0f)
	, m_fGroundCheckDistance(0.0f)
	, m_fAnimSpeed(1.0f)
	, m_bEnabled(true)
	, m_bIsGrounded(false)
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
//  初期化処理
//--------------------------------------------------------------------------------
bool CActorBehaviorComponent::Init(void)
{
	return true;
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
//	関数名：Act
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Act(CKFVec3& vMovement, bool& bJump, bool& bAttack)
{
	if (!m_bEnabled) { return; }
	if (!checkCanAction())
	{
		vMovement = CKFVec3(0.0f);
		bJump = false;
		bAttack = false;
	}

	//方向の長さが1より大きいの場合ノーマライズする
	auto fMovement = CKFMath::VecMagnitude(vMovement);
	if (fMovement > 1.0f)
	{//処理速度向上のため手で計算する
		vMovement /= fMovement;
		fMovement = 1.0f;
	}

	//相対回転方向を算出する
	auto vTurnDir = m_pGameObj->GetTransformComponent()->TransformDirectionToLocal(vMovement);

	//地面の表面法線を取得
	auto vGroundNormal = checkGroundStatus();

	//移動方向を地面の表面に投影する
	vMovement = CKFMath::ProjectOnPlane(vMovement, vGroundNormal);

	//回転角度の算出
	auto fTurnAngle = atan2f(vTurnDir.m_fX, vTurnDir.m_fZ);

	//回転
	turn(fTurnAngle, fMovement);

	//移動
	move(vMovement);

	//跳ぶ                                                              
	jump(bJump);

	//Animation
	updateAnimation(fMovement, bAttack);
}

void CActorBehaviorComponent::Hit(const float& fDamage)
{

}

//--------------------------------------------------------------------------------
//
//  Protected
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：move
//  関数説明：移動関数
//	引数：	vMovement：移動方向と移動量
//	戻り値：なし
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::move(const CKFVec3& vMovement)
{
	auto vVelocity = vMovement * m_fMoveSpeed;
	vVelocity.m_fY = m_rigidbody.GetVelocity().m_fY;
	m_rigidbody.SetVelocity(vVelocity);
}

//--------------------------------------------------------------------------------
//	関数名：jump
//  関数説明：跳ぶ関数
//	引数：	bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::jump(const bool& bJump)
{
	if (!bJump) { return; }
	auto vVelocity = m_rigidbody.GetVelocity();
	vVelocity.m_fY = m_fJumpSpeed;
	m_bIsGrounded = false;
	m_fGroundCheckDistance = 0.1f;
}

//--------------------------------------------------------------------------------
//	関数名：turn
//  関数説明：回転関数
//	引数：	fTurnAngle：回転角度
//			fMoveRate：移動率
//	戻り値：なし
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::turn(const float& fTurnAngle, const float& fMoveRate)
{
	auto fTurnSpeed = CKFMath::LerpFloat(m_fTurnSpeedMin, m_fTurnSpeedMax, fMoveRate);
	m_pGameObj->GetTransformComponent()->RotByYaw(fTurnAngle * fTurnSpeed);
}

//--------------------------------------------------------------------------------
//	関数名：updateAnimation
//  関数説明：アニメーション更新
//	引数：	fTurnAngle：回転角度
//			fMoveRate：移動率
//	戻り値：なし
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::updateAnimation(const float& fMovement, const bool& bAttack)
{
	if (!m_pActor) { return; }

	if (bAttack && m_pActor->GetMotionNow() != CActorMeshComponent::MOTION_ATTACK)
	{
		m_pActor->SetMotion(CActorMeshComponent::MOTION_ATTACK);
		return;
	}

	if (!m_bIsGrounded)
	{
		m_pActor->SetMotion(CActorMeshComponent::MOTION_JUMP);
		return;
	}

	if (fMovement != 0.0f)
	{
		m_pActor->SetMotion(CActorMeshComponent::MOTION_MOVE);
	}
	else
	{
		m_pActor->SetMotion(CActorMeshComponent::MOTION_NEUTAL);
	}
}

//--------------------------------------------------------------------------------
//	関数名：checkGroundStatus
//  関数説明：着陸チェック
//	引数：	なし
//	戻り値：地面の表面法線
//--------------------------------------------------------------------------------
CKFVec3 CActorBehaviorComponent::checkGroundStatus(void)
{
	m_bIsGrounded = false;
	return CKFVec3(0.0f, 1.0f, 0.0f);
}

//--------------------------------------------------------------------------------
//	関数名：checkCanAction
//  関数説明：行動できるかどうかをチェックする関数
//	引数：	なし
//	戻り値：フラグ
//--------------------------------------------------------------------------------
bool CActorBehaviorComponent::checkCanAction(void)
{
	if (m_pActor && m_pActor->GetMotionNow() == CActorMeshComponent::MOTION_ATTACK)
	{
		return false;
	}

	return true;
}
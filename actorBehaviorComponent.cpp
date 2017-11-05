//--------------------------------------------------------------------------------
//	生き物ビヘイビアコンポネント
//　actorBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "collisionSystem.h"
#include "collisionDetector.h"
#include "actorBehaviorComponent.h"
#include "rigidbody3D.h"
#include "gameObject.h"
#include "transform.h"
#include "animatorComponent.h"

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
CActorBehaviorComponent::CActorBehaviorComponent(GameObject* const pGameObj, Rigidbody3D& rigidbody, CAnimatorComponent* const pAnimator)
	: CBehaviorComponent(pGameObj)
	, m_rigidbody(rigidbody)
	, m_pAnimator(pAnimator)
	, m_nLevel(0)
	, m_countInvincible(0)
	, m_fLifeMax(0.0f)
	, m_fLifeNow(0.0f)
	, Attack(0.0f)
	, m_fDefence(0.0f)
	, m_fMoveSpeed(0.0f)
	, m_fJumpSpeed(0.0f)
	, m_fTurnSpeedMin(0.0f)
	, m_fTurnSpeedMax(0.0f)
	, GroundCheckDistance(0.1f)
	, AnimSpeed(1.0f)
	, m_fMaxPosY(0.0f)
	, m_bEnabled(true)
	, m_bIsGrounded(false)
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
	if (m_countInvincible > 0) { --m_countInvincible; }
	
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
void CActorBehaviorComponent::Act(Vector3& vMovement, const bool& bJump, const bool& bAttack)
{
	if (!m_bEnabled) { return; }
	if (m_pAnimator && !m_pAnimator->CanAct()) { return; }

	//方向の長さが1より大きいの場合ノーマライズする
	auto fMovement = CKFMath::VecMagnitude(vMovement);
	if (fMovement > 1.0f)
	{//処理速度向上のため手で計算する
		vMovement /= fMovement;
		fMovement = 1.0f;
	}

	//相対回転方向を算出する
	auto vTurnDir = m_pGameObj->GetTransform()->TransformDirectionToLocal(vMovement);

	//地面の表面法線を取得
	auto vGroundNormal = checkGroundStatus();

	//移動方向を地面の表面に投影する
	vMovement = CKFMath::ProjectOnPlane(vMovement, vGroundNormal);

	//回転角度の算出
	auto fTurnAngle = atan2f(vTurnDir.X, vTurnDir.Z);

	//回転
	turn(fTurnAngle, fMovement);

	//移動
	move(vMovement);

	//跳ぶ                                                              
	jump(bJump);

	//Animation
	updateAnimation(fMovement, bJump, bAttack);
}

//--------------------------------------------------------------------------------
//	関数名：Act
//  関数説明：アクション（移動、跳ぶ、攻撃）
//	引数：	vDirection：移動方向
//			bJump：跳ぶフラグ
//	戻り値：なし
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::Hit(const float& fDamage)
{
	if (m_countInvincible > 0) { return; }
	m_countInvincible = 60;
	m_fLifeNow -= fDamage;
	m_fLifeNow = m_fLifeNow < 0.0f ? 0.0f : m_fLifeNow;
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
void CActorBehaviorComponent::move(const Vector3& vMovement)
{
	auto vVelocity = vMovement * m_fMoveSpeed;
	vVelocity.Y = m_rigidbody.GetVelocity().Y;
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
	if (!bJump || !m_bIsGrounded) { return; }
	auto vVelocity = m_rigidbody.GetVelocity();
	vVelocity.Y = m_fJumpSpeed;
	m_rigidbody.SetVelocity(vVelocity);
	//GroundCheckDistance = 0.3f;
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
	m_pGameObj->GetTransform()->RotByYaw(fTurnAngle * fTurnSpeed);
}

//--------------------------------------------------------------------------------
//	関数名：updateAnimation
//  関数説明：アニメーション更新
//	引数：	fTurnAngle：回転角度
//			fMoveRate：移動率
//	戻り値：なし
//--------------------------------------------------------------------------------
void CActorBehaviorComponent::updateAnimation(const float& fMovement, const bool& bJump, const bool& bAttack)
{
	if (!m_pAnimator) { return; }

	m_pAnimator->SetGrounded(m_bIsGrounded);
	m_pAnimator->SetAttack(bAttack);
	m_pAnimator->SetJump(bJump);
	m_pAnimator->SetMove(fMovement);
}

//--------------------------------------------------------------------------------
//	関数名：checkGroundStatus
//  関数説明：着陸チェック
//	引数：	なし
//	戻り値：地面の表面法線
//--------------------------------------------------------------------------------
Vector3 CActorBehaviorComponent::checkGroundStatus(void)
{
	RayHitInfo rayHit;
	auto Position = m_pGameObj->GetTransform()->GetPos();
	auto pCollisionSystem = CollisionSystem::Instance();
	if (pCollisionSystem->RayCast(Position, CKFMath::sc_vDown, GroundCheckDistance, rayHit, m_pGameObj))
	{
		//To do : Jump Damage
		if (!m_bIsGrounded)
		{
			float fFallDis = m_fMaxPosY - Position.Y;
		}
		m_fMaxPosY = Position.Y;
		m_bIsGrounded = true;
		return rayHit.Normal;
	}

	m_fMaxPosY = m_fMaxPosY < Position.Y ? Position.Y : m_fMaxPosY;
	m_bIsGrounded = false;
	return CKFMath::sc_vUp;
}

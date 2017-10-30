//--------------------------------------------------------------------------------
//	物理演算システム
//　KF_PhysicsSystem.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-25
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "KF_PhysicsSystem.h"
#include "KF_CollisionUtility.h"
#include "gameObject.h"
#include "transformComponent.h"
#include "3DRigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const Vector3 CKFPhysicsSystem::sc_vGravity = Vector3(0.0f, -9.8f, 0.0f);

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::Update(void)
{
	for (auto itr = m_listCollision.begin(); itr != m_listCollision.end();)
	{
		resolve(**itr);
		delete *itr;
		itr = m_listCollision.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::Clear(void)
{
	for (auto itr = m_listCollision.begin(); itr != m_listCollision.end();)
	{
		delete *itr;
		itr = m_listCollision.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//  衝突情報のレジストリ
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::RegisterCollision(CCollision* pCollision)
{
	m_listCollision.push_back(pCollision);
}

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::uninit(void)
{
	if (!m_listCollision.empty())
	{
		for (auto itr = m_listCollision.begin(); itr != m_listCollision.end();)
		{
			delete *itr;
			itr = m_listCollision.erase(itr);
		}
	}
}

//--------------------------------------------------------------------------------
//  衝突処理
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::resolve(CCollision& collision)
{
	resolveVelocity(collision);
	resolveInterpenetration(collision);
}

//--------------------------------------------------------------------------------
//  反発速度の算出
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::resolveVelocity(CCollision& collision)
{
	//分離速度計算
	float fSeparatingVelocity = calculateSeparatingVelocity(collision);

	//分離速度チェック
	//衝突法線の逆方向になれば計算しない
	if (fSeparatingVelocity > 0.0f) { return; }

	//跳ね返り係数の算出
	float fBounciness = collision.m_pRigidBodyOne->Bounciness;
	if (collision.m_pRigidBodyTwo)
	{
		fBounciness += collision.m_pRigidBodyTwo->Bounciness;
		fBounciness *= 0.5f;
	}

	//跳ね返り速度の算出
	float fNewSeparatingVelocity = -fSeparatingVelocity * fBounciness;

	//衝突方向に作用力を計算する
	auto vAccCausedVelocity = collision.m_pRigidBodyOne->m_vAcceleration;
	if (collision.m_pRigidBodyTwo)
	{
		vAccCausedVelocity -= collision.m_pRigidBodyTwo->m_vAcceleration;
	}
	float fAccCausedSeparatingVelocity = CKFMath::Vec3Dot(vAccCausedVelocity, collision.m_vCollisionNormal);

	//衝突法線の逆方向になれば
	if (fAccCausedSeparatingVelocity < 0.0f)
	{
		fNewSeparatingVelocity += fAccCausedSeparatingVelocity * fBounciness;
		if (fNewSeparatingVelocity < 0.0f) { fNewSeparatingVelocity = 0.0f; }
	}

	//速度差分計算
	float fDeltaVelocity = fNewSeparatingVelocity - fSeparatingVelocity;

	//逆質量取得
	float fTotalInverseMass = collision.m_pRigidBodyOne->m_fInverseMass;
	if (collision.m_pRigidBodyTwo)
	{
		fTotalInverseMass += collision.m_pRigidBodyTwo->m_fInverseMass;
	}

	//質量が無限大の場合計算しない
	if (fTotalInverseMass <= 0.0f) { return; }

	//衝突力計算
	float fImpulse = fDeltaVelocity / fTotalInverseMass;

	//単位逆質量の衝突力
	auto vImpulsePerInverseMass = collision.m_vCollisionNormal * fImpulse;

	//速度計算
	auto vVelocity = vImpulsePerInverseMass * collision.m_pRigidBodyOne->m_fInverseMass;
	collision.m_pRigidBodyOne->m_vVelocity += vVelocity;

	if (collision.m_pRigidBodyTwo)
	{
		vVelocity = vImpulsePerInverseMass * -1.0f * collision.m_pRigidBodyTwo->m_fInverseMass;
		collision.m_pRigidBodyTwo->m_vVelocity += vVelocity;
	}
}

//--------------------------------------------------------------------------------
//  めり込みの解決
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::resolveInterpenetration(CCollision& collision)
{
	//衝突しない
	if (collision.m_fPenetration <= 0.0f) { return; }

	//逆質量計算
	float fTotalInverseMass = collision.m_pRigidBodyOne->m_fInverseMass;
	if (collision.m_pRigidBodyTwo)
	{
		fTotalInverseMass += collision.m_pRigidBodyTwo->m_fInverseMass;
	}

	//質量が無限大の場合計算しない
	if (fTotalInverseMass <= 0.0f) { return; }

	//質量単位戻り量計算
	auto vMovePerInverseMass = collision.m_vCollisionNormal * collision.m_fPenetration / fTotalInverseMass;

	//各粒子戻り位置計算
	auto pTrans = collision.m_pRigidBodyOne->m_pGameObj->GetTransformComponent();
	collision.m_pRigidBodyOne->m_vMovement += vMovePerInverseMass * collision.m_pRigidBodyOne->m_fInverseMass;

	if (collision.m_pRigidBodyTwo)
	{
		pTrans = collision.m_pRigidBodyTwo->m_pGameObj->GetTransformComponent();
		collision.m_pRigidBodyTwo->m_vMovement -= vMovePerInverseMass * collision.m_pRigidBodyTwo->m_fInverseMass;
	}
}

//--------------------------------------------------------------------------------
//  分離速度の算出
//--------------------------------------------------------------------------------
float CKFPhysicsSystem::calculateSeparatingVelocity(CCollision& collision)
{
	auto vRelativeVelocity = collision.m_pRigidBodyOne->m_vVelocity;
	if (collision.m_pRigidBodyTwo) { vRelativeVelocity -= collision.m_pRigidBodyTwo->m_vVelocity; }
	return CKFMath::Vec3Dot(vRelativeVelocity, collision.m_vCollisionNormal);
}

//--------------------------------------------------------------------------------
//  衝突法線をX軸として、衝突座標係の算出
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::calculateCollisionBasis(CCollision& collision)
{
	Vector3 vAxisY, vAxisZ;

	//衝突法線が世界X軸と世界Y軸どっちとの角度が近い
	if (fabsf(collision.m_vCollisionNormal.X) > fabsf(collision.m_vCollisionNormal.Y))
	{//Y
		float fScale = 1.0f / sqrtf(collision.m_vCollisionNormal.X * collision.m_vCollisionNormal.X
			+ collision.m_vCollisionNormal.Z * collision.m_vCollisionNormal.Z);

		vAxisZ.X = collision.m_vCollisionNormal.Z * fScale;
		vAxisZ.Y = 0.0f;
		vAxisZ.Z = collision.m_vCollisionNormal.X * fScale;

		vAxisY.X = collision.m_vCollisionNormal.Y * vAxisZ.X;
		vAxisY.Y = collision.m_vCollisionNormal.Z * vAxisZ.X
			- collision.m_vCollisionNormal.X * vAxisZ.Z;
		vAxisY.Z = -collision.m_vCollisionNormal.Y * vAxisZ.X;
	}
	else
	{//X
		float fScale = 1.0f / sqrtf(collision.m_vCollisionNormal.Y * collision.m_vCollisionNormal.Y
			+ collision.m_vCollisionNormal.Z * collision.m_vCollisionNormal.Z);

		vAxisZ.X = 0.0f;
		vAxisZ.Y = -collision.m_vCollisionNormal.Z * fScale;
		vAxisZ.Z = collision.m_vCollisionNormal.Y * fScale;

		vAxisY.X = collision.m_vCollisionNormal.Y * vAxisZ.Z
			- collision.m_vCollisionNormal.Z * vAxisZ.Y;
		vAxisY.Y = -collision.m_vCollisionNormal.X * vAxisZ.Z;
		vAxisY.Z = collision.m_vCollisionNormal.X * vAxisZ.Y;
	}

	collision.m_mtxToWorld.Elements[0][0] = collision.m_vCollisionNormal.X;
	collision.m_mtxToWorld.Elements[0][1] = collision.m_vCollisionNormal.Y;
	collision.m_mtxToWorld.Elements[0][2] = collision.m_vCollisionNormal.Z;
	collision.m_mtxToWorld.Elements[1][0] = vAxisY.X;
	collision.m_mtxToWorld.Elements[1][1] = vAxisY.Y;
	collision.m_mtxToWorld.Elements[1][2] = vAxisY.Z;
	collision.m_mtxToWorld.Elements[2][0] = vAxisZ.X;
	collision.m_mtxToWorld.Elements[2][1] = vAxisZ.Y;
	collision.m_mtxToWorld.Elements[2][2] = vAxisZ.Z;
}

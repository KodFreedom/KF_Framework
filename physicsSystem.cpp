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
#include "physicsSystem.h"
#include "collisionDetector.h"
#include "gameObject.h"
#include "transformComponent.h"
#include "3DRigidbodyComponent.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const Vector3 PhysicsSystem::Gravity = Vector3(0.0f, -9.8f, 0.0f);

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
void PhysicsSystem::Update(void)
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
void PhysicsSystem::Clear(void)
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
void PhysicsSystem::RegisterCollision(Collision* pCollision)
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
void PhysicsSystem::uninit(void)
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
void PhysicsSystem::resolve(Collision& collision)
{
	resolveVelocity(collision);
	resolveInterpenetration(collision);
}

//--------------------------------------------------------------------------------
//  反発速度の算出
//--------------------------------------------------------------------------------
void PhysicsSystem::resolveVelocity(Collision& collision)
{
	//分離速度計算
	float fSeparatingVelocity = calculateSeparatingVelocity(collision);

	//分離速度チェック
	//衝突法線の逆方向になれば計算しない
	if (fSeparatingVelocity > 0.0f) { return; }

	//跳ね返り係数の算出
	float fBounciness = collision.RigidbodyOne->Bounciness;
	if (collision.RigidbodyTwo)
	{
		fBounciness += collision.RigidbodyTwo->Bounciness;
		fBounciness *= 0.5f;
	}

	//跳ね返り速度の算出
	float fNewSeparatingVelocity = -fSeparatingVelocity * fBounciness;

	//衝突方向に作用力を計算する
	auto vAccCausedVelocity = collision.RigidbodyOne->m_vAcceleration;
	if (collision.RigidbodyTwo)
	{
		vAccCausedVelocity -= collision.RigidbodyTwo->m_vAcceleration;
	}
	float fAccCausedSeparatingVelocity = CKFMath::Vec3Dot(vAccCausedVelocity, collision.Normal);

	//衝突法線の逆方向になれば
	if (fAccCausedSeparatingVelocity < 0.0f)
	{
		fNewSeparatingVelocity += fAccCausedSeparatingVelocity * fBounciness;
		if (fNewSeparatingVelocity < 0.0f) { fNewSeparatingVelocity = 0.0f; }
	}

	//速度差分計算
	float fDeltaVelocity = fNewSeparatingVelocity - fSeparatingVelocity;

	//逆質量取得
	float fTotalInverseMass = collision.RigidbodyOne->m_fInverseMass;
	if (collision.RigidbodyTwo)
	{
		fTotalInverseMass += collision.RigidbodyTwo->m_fInverseMass;
	}

	//質量が無限大の場合計算しない
	if (fTotalInverseMass <= 0.0f) { return; }

	//衝突力計算
	float fImpulse = fDeltaVelocity / fTotalInverseMass;

	//単位逆質量の衝突力
	auto vImpulsePerInverseMass = collision.Normal * fImpulse;

	//速度計算
	auto vVelocity = vImpulsePerInverseMass * collision.RigidbodyOne->m_fInverseMass;
	collision.RigidbodyOne->m_vVelocity += vVelocity;

	if (collision.RigidbodyTwo)
	{
		vVelocity = vImpulsePerInverseMass * -1.0f * collision.RigidbodyTwo->m_fInverseMass;
		collision.RigidbodyTwo->m_vVelocity += vVelocity;
	}
}

//--------------------------------------------------------------------------------
//  めり込みの解決
//--------------------------------------------------------------------------------
void PhysicsSystem::resolveInterpenetration(Collision& collision)
{
	//衝突しない
	if (collision.Penetration <= 0.0f) { return; }

	//逆質量計算
	float fTotalInverseMass = collision.RigidbodyOne->m_fInverseMass;
	if (collision.RigidbodyTwo)
	{
		fTotalInverseMass += collision.RigidbodyTwo->m_fInverseMass;
	}

	//質量が無限大の場合計算しない
	if (fTotalInverseMass <= 0.0f) { return; }

	//質量単位戻り量計算
	auto vMovePerInverseMass = collision.Normal * collision.Penetration / fTotalInverseMass;

	//各粒子戻り位置計算
	auto pTrans = collision.RigidbodyOne->m_pGameObj->GetTransformComponent();
	collision.RigidbodyOne->m_vMovement += vMovePerInverseMass * collision.RigidbodyOne->m_fInverseMass;

	if (collision.RigidbodyTwo)
	{
		pTrans = collision.RigidbodyTwo->m_pGameObj->GetTransformComponent();
		collision.RigidbodyTwo->m_vMovement -= vMovePerInverseMass * collision.RigidbodyTwo->m_fInverseMass;
	}
}

//--------------------------------------------------------------------------------
//  分離速度の算出
//--------------------------------------------------------------------------------
float PhysicsSystem::calculateSeparatingVelocity(Collision& collision)
{
	auto vRelativeVelocity = collision.RigidbodyOne->m_vVelocity;
	if (collision.RigidbodyTwo) { vRelativeVelocity -= collision.RigidbodyTwo->m_vVelocity; }
	return CKFMath::Vec3Dot(vRelativeVelocity, collision.Normal);
}

//--------------------------------------------------------------------------------
//  衝突法線をX軸として、衝突座標係の算出
//--------------------------------------------------------------------------------
void PhysicsSystem::calculateCollisionBasis(Collision& collision)
{
	Vector3 vAxisY, vAxisZ;

	//衝突法線が世界X軸と世界Y軸どっちとの角度が近い
	if (fabsf(collision.Normal.X) > fabsf(collision.Normal.Y))
	{//Y
		float fScale = 1.0f / sqrtf(collision.Normal.X * collision.Normal.X
			+ collision.Normal.Z * collision.Normal.Z);

		vAxisZ.X = collision.Normal.Z * fScale;
		vAxisZ.Y = 0.0f;
		vAxisZ.Z = collision.Normal.X * fScale;

		vAxisY.X = collision.Normal.Y * vAxisZ.X;
		vAxisY.Y = collision.Normal.Z * vAxisZ.X
			- collision.Normal.X * vAxisZ.Z;
		vAxisY.Z = -collision.Normal.Y * vAxisZ.X;
	}
	else
	{//X
		float fScale = 1.0f / sqrtf(collision.Normal.Y * collision.Normal.Y
			+ collision.Normal.Z * collision.Normal.Z);

		vAxisZ.X = 0.0f;
		vAxisZ.Y = -collision.Normal.Z * fScale;
		vAxisZ.Z = collision.Normal.Y * fScale;

		vAxisY.X = collision.Normal.Y * vAxisZ.Z
			- collision.Normal.Z * vAxisZ.Y;
		vAxisY.Y = -collision.Normal.X * vAxisZ.Z;
		vAxisY.Z = collision.Normal.X * vAxisZ.Y;
	}

	collision.m_mtxToWorld.Elements[0][0] = collision.Normal.X;
	collision.m_mtxToWorld.Elements[0][1] = collision.Normal.Y;
	collision.m_mtxToWorld.Elements[0][2] = collision.Normal.Z;
	collision.m_mtxToWorld.Elements[1][0] = vAxisY.X;
	collision.m_mtxToWorld.Elements[1][1] = vAxisY.Y;
	collision.m_mtxToWorld.Elements[1][2] = vAxisY.Z;
	collision.m_mtxToWorld.Elements[2][0] = vAxisZ.X;
	collision.m_mtxToWorld.Elements[2][1] = vAxisZ.Y;
	collision.m_mtxToWorld.Elements[2][2] = vAxisZ.Z;
}

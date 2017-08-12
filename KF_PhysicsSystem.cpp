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
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool CKFPhysicsSystem::Init(void)
{
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::Uninit(void)
{
	if (!m_listCollision.empty())
	{
		for (auto itr = m_listCollision.begin(); itr != m_listCollision.end();)
		{
			auto pCollision = *itr;
			delete pCollision;
			itr = m_listCollision.erase(itr);
		}
	}
}

//--------------------------------------------------------------------------------
//  リリース処理
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::Release(void)
{
	Uninit();
	delete this;
}

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
//  衝突情報のレジストリ
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::RegisterCollision(CCollision* pCollision)
{
	m_listCollision.push_back(pCollision);
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
	//変数宣言
	//CKFVec3	vAccCausedVelocity;				//衝突方向の作用力(加速度)
	CKFVec3	vImpulsePerInverseMass;			//単位逆質量の衝突力
	CKFVec3	vVelocity;						//変化した速度
	CKFVec3	vForce;							//速度あたりの作用力
	CKFVec3	vPosToCollisionPos;				//中心点と衝突点のベクトル
	CKFVec3	vWork;							//計算用ベクトル
	CKFVec3	vRotationForce;					//回転作用力
	float	fSeparatingVelocity;			//分離速度
	float	fNewSeparatingVelocity;			//新しい分離速度
	//float	fAccCausedSeparatingVelocity;	//衝突法線上の加速度
	float	fDeltaVelocity;					//速度の変化量
	float	fTotalInverseMass;				//質量の総和
	float	fImpulse;						//衝突力
	//float	fWorldAngle;					//全体的にとっての角度
	float	fBounciness;					//跳ね返り係数

	//分離速度計算
	fSeparatingVelocity = calculateSeparatingVelocity(collision);

	//分離速度チェック
	//衝突法線の逆方向になれば計算しない
	if (fSeparatingVelocity > 0.0f) { return; }

	//跳ね返り係数の算出
	fBounciness = collision.m_pRigidBodyOne->m_fBounciness;
	if (collision.m_pRigidBodyTwo)
	{
		fBounciness += collision.m_pRigidBodyTwo->m_fBounciness;
		fBounciness *= 0.5f;
	}

	//跳ね返り速度の算出
	fNewSeparatingVelocity = -fSeparatingVelocity * fBounciness;

	//衝突方向に作用力を計算する
	//vAccCausedVelocity = collision.m_pRigidBodyOne->m_vAcceleration;
	//if (collision.m_pRigidBodyTwo)
	//{
	//	vAccCausedVelocity -= collision.m_pRigidBodyTwo->m_vAcceleration;
	//}
	//fAccCausedSeparatingVelocity = CKFMath::Vec3Dot(vAccCausedVelocity, collision.m_vCollisionNormal);

	////衝突法線の逆方向になれば
	//if (fAccCausedSeparatingVelocity < 0.0f)
	//{
	//	fNewSeparatingVelocity += fAccCausedSeparatingVelocity * fBounciness;
	//	if (fNewSeparatingVelocity < 0.0f) { fNewSeparatingVelocity = 0.0f; }
	//}

	//速度差分計算
	fDeltaVelocity = fNewSeparatingVelocity - fSeparatingVelocity;

	//質量取得
	fTotalInverseMass = collision.m_pRigidBodyOne->m_fInverseMass;
	if (collision.m_pRigidBodyTwo)
	{
		fTotalInverseMass += collision.m_pRigidBodyTwo->m_fInverseMass;
	}

	//質量が無限大の場合計算しない
	if (fTotalInverseMass <= 0.0f) { return; }

	//衝突力計算
	fImpulse = fDeltaVelocity / fTotalInverseMass;

	//単位逆質量の衝突力
	vImpulsePerInverseMass = collision.m_vCollisionNormal * fImpulse;

	//速度計算
	vVelocity = vImpulsePerInverseMass * collision.m_pRigidBodyOne->m_fInverseMass;
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
	if (collision.m_fPenetration <= 0) { return; }

	//逆質量計算
	float fTotalInverseMass = collision.m_pRigidBodyOne->m_fInverseMass;
	if (collision.m_pRigidBodyTwo)
	{
		fTotalInverseMass += collision.m_pRigidBodyTwo->m_fInverseMass;
	}

	//質量が無限大の場合計算しない
	if (fTotalInverseMass <= 0) { return; }

	//質量単位戻り量計算
	CKFVec3 vMovePerInverseMass = collision.m_vCollisionNormal * collision.m_fPenetration / fTotalInverseMass;

	//各粒子戻り位置計算
	CTransformComponent* pTrans = collision.m_pRigidBodyOne->m_pGameObj->GetTransformComponent();
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
	CKFVec3 vRelativeVelocity = collision.m_pRigidBodyOne->m_vVelocity;

	//粒子と粒子の衝突
	if (collision.m_pRigidBodyTwo)
	{
		vRelativeVelocity -= collision.m_pRigidBodyTwo->m_vVelocity;
	}

	//内積計算
	float fDot = CKFMath::Vec3Dot(vRelativeVelocity, collision.m_vCollisionNormal);
	return fDot;
}

//--------------------------------------------------------------------------------
//  衝突法線をX軸として、衝突座標係の算出
//--------------------------------------------------------------------------------
void CKFPhysicsSystem::calculateCollisionBasis(CCollision& collision)
{
	CKFVec3 vAxisY, vAxisZ;

	//衝突法線が世界X軸と世界Y軸どっちとの角度が近い
	if (fabsf(collision.m_vCollisionNormal.m_fX) > fabsf(collision.m_vCollisionNormal.m_fY))
	{//Y
		float fScale = 1.0f / sqrtf(collision.m_vCollisionNormal.m_fX * collision.m_vCollisionNormal.m_fX
			+ collision.m_vCollisionNormal.m_fZ * collision.m_vCollisionNormal.m_fZ);

		vAxisZ.m_fX = collision.m_vCollisionNormal.m_fZ * fScale;
		vAxisZ.m_fY = 0.0f;
		vAxisZ.m_fZ = collision.m_vCollisionNormal.m_fX * fScale;

		vAxisY.m_fX = collision.m_vCollisionNormal.m_fY * vAxisZ.m_fX;
		vAxisY.m_fY = collision.m_vCollisionNormal.m_fZ * vAxisZ.m_fX
			- collision.m_vCollisionNormal.m_fX * vAxisZ.m_fZ;
		vAxisY.m_fZ = -collision.m_vCollisionNormal.m_fY * vAxisZ.m_fX;
	}
	else
	{//X
		float fScale = 1.0f / sqrtf(collision.m_vCollisionNormal.m_fY * collision.m_vCollisionNormal.m_fY
			+ collision.m_vCollisionNormal.m_fZ * collision.m_vCollisionNormal.m_fZ);

		vAxisZ.m_fX = 0.0f;
		vAxisZ.m_fY = -collision.m_vCollisionNormal.m_fZ * fScale;
		vAxisZ.m_fZ = collision.m_vCollisionNormal.m_fY * fScale;

		vAxisY.m_fX = collision.m_vCollisionNormal.m_fY * vAxisZ.m_fZ
			- collision.m_vCollisionNormal.m_fZ * vAxisZ.m_fY;
		vAxisY.m_fY = -collision.m_vCollisionNormal.m_fX * vAxisZ.m_fZ;
		vAxisY.m_fZ = collision.m_vCollisionNormal.m_fX * vAxisZ.m_fY;
	}

	collision.m_mtxToWorld.m_af[0][0] = collision.m_vCollisionNormal.m_fX;
	collision.m_mtxToWorld.m_af[0][1] = collision.m_vCollisionNormal.m_fY;
	collision.m_mtxToWorld.m_af[0][2] = collision.m_vCollisionNormal.m_fZ;
	collision.m_mtxToWorld.m_af[1][0] = vAxisY.m_fX;
	collision.m_mtxToWorld.m_af[1][1] = vAxisY.m_fY;
	collision.m_mtxToWorld.m_af[1][2] = vAxisY.m_fZ;
	collision.m_mtxToWorld.m_af[2][0] = vAxisZ.m_fX;
	collision.m_mtxToWorld.m_af[2][1] = vAxisZ.m_fY;
	collision.m_mtxToWorld.m_af[2][2] = vAxisZ.m_fZ;
}

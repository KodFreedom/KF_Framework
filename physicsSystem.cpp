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
PhysicsSystem* PhysicsSystem::instance = nullptr;
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
	for (auto iterator = collisions.begin(); iterator != collisions.end();)
	{
		resolve(**iterator);
		delete *iterator;
		iterator = collisions.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//	関数名：Clear
//  関数説明：衝突情報を全部破棄する
//	引数：	なし
//	戻り値：なし
//--------------------------------------------------------------------------------
void PhysicsSystem::Clear(void)
{
	for (auto iterator = collisions.begin(); iterator != collisions.end();)
	{
		delete *iterator;
		iterator = collisions.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
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
	float separatingVelocity = calculateSeparatingVelocity(collision);

	//分離速度チェック
	//衝突法線の逆方向になれば計算しない
	if (separatingVelocity > 0.0f) return;

	//跳ね返り係数の算出
	float bounciness = collision.RigidbodyOne->Bounciness;
	if (collision.RigidbodyTwo)
	{
		bounciness += collision.RigidbodyTwo->Bounciness;
		bounciness *= 0.5f;
	}

	//跳ね返り速度の算出
	float bouncinessVelocity = -separatingVelocity * bounciness;

	//衝突方向に作用力を計算する
	auto acceleration = collision.RigidbodyOne->m_vAcceleration;
	if (collision.RigidbodyTwo)
	{
		acceleration -= collision.RigidbodyTwo->m_vAcceleration;
	}
	float separatingAcceleration = acceleration.Dot(collision.Normal);

	//衝突法線の逆方向になれば
	if (separatingAcceleration < 0.0f)
	{
		bouncinessVelocity += separatingAcceleration * bounciness;
		if (bouncinessVelocity < 0.0f) bouncinessVelocity = 0.0f;
	}

	//速度差分計算
	float deltaVelocity = bouncinessVelocity - separatingVelocity;

	//逆質量取得
	float totalInverseMass = collision.RigidbodyOne->m_fInverseMass;
	if (collision.RigidbodyTwo)
	{
		totalInverseMass += collision.RigidbodyTwo->m_fInverseMass;
	}

	//質量が無限大の場合計算しない
	if (totalInverseMass <= 0.0f) return;

	//衝突力計算
	float impulse = deltaVelocity / totalInverseMass;

	//単位逆質量の衝突力
	auto impulsePerInverseMass = collision.Normal * impulse;

	//速度計算
	collision.RigidbodyOne->m_vVelocity += impulsePerInverseMass * collision.RigidbodyOne->m_fInverseMass;;
	if (collision.RigidbodyTwo)
	{
		collision.RigidbodyTwo->m_vVelocity += impulsePerInverseMass * -1.0f * collision.RigidbodyTwo->m_fInverseMass;;
	}
}

//--------------------------------------------------------------------------------
//  めり込みの解決
//--------------------------------------------------------------------------------
void PhysicsSystem::resolveInterpenetration(Collision& collision)
{
	//衝突しない
	if (collision.Penetration <= 0.0f) return;

	//逆質量計算
	float totalInverseMass = collision.RigidbodyOne->m_fInverseMass;
	if (collision.RigidbodyTwo)
	{
		totalInverseMass += collision.RigidbodyTwo->m_fInverseMass;
	}

	//質量が無限大の場合計算しない
	if (totalInverseMass <= 0.0f) return;

	//質量単位戻り量計算
	auto movementPerInverseMass = collision.Normal * collision.Penetration / totalInverseMass;

	//各Rigidbody戻り位置計算
	auto transform = collision.RigidbodyOne->GetGameObject()->GetTransformComponent();
	collision.RigidbodyOne->m_vMovement += movementPerInverseMass * collision.RigidbodyOne->m_fInverseMass;

	if (collision.RigidbodyTwo)
	{
		transform = collision.RigidbodyTwo->GetGameObject()->GetTransformComponent();
		collision.RigidbodyTwo->m_vMovement -= movementPerInverseMass * collision.RigidbodyTwo->m_fInverseMass;
	}
}

//--------------------------------------------------------------------------------
//  分離速度の算出
//--------------------------------------------------------------------------------
float PhysicsSystem::calculateSeparatingVelocity(Collision& collision)
{
	auto relativeVelocity = collision.RigidbodyOne->m_vVelocity;
	if (collision.RigidbodyTwo) relativeVelocity -= collision.RigidbodyTwo->m_vVelocity;
	return relativeVelocity.Dot(collision.Normal);
}

/*//--------------------------------------------------------------------------------
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
}*/
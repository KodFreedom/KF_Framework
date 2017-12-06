//--------------------------------------------------------------------------------
//	物理演算システム
//　physics_system.cpp
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "physics_system.h"
#include "collision_detector.h"
#include "rigidbody3d.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const Vector3 PhysicsSystem::kGravity = Vector3(0.0f, -9.8f, 0.0f);

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
	for (auto iterator = collisions_.begin(); iterator != collisions_.end();)
	{
		Resolve(**iterator);
		delete *iterator;
		iterator = collisions_.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//	関数名：Clear
//  関数説明：衝突情報を全部破棄する
//--------------------------------------------------------------------------------
void PhysicsSystem::Clear(void)
{
	for (auto iterator = collisions_.begin(); iterator != collisions_.end();)
	{
		delete *iterator;
		iterator = collisions_.erase(iterator);
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
void PhysicsSystem::Resolve(Collision& collision)
{
	ResolveVelocity(collision);
	ResolveInterpenetration(collision);
}

//--------------------------------------------------------------------------------
//  反発速度の算出
//--------------------------------------------------------------------------------
void PhysicsSystem::ResolveVelocity(Collision& collision)
{
	//分離速度計算
	float separating_velocity = CalculateSeparatingVelocity(collision);

	//分離速度チェック
	//衝突法線の逆方向になれば計算しない
	if (separating_velocity > 0.0f) return;

	//跳ね返り係数の算出
	float bounciness = collision.rigidbody_one->GetBounciness();
	if (collision.rigidbody_two)
	{
		bounciness += collision.rigidbody_two->GetBounciness();
		bounciness *= 0.5f;
	}

	//跳ね返り速度の算出
	float bounciness_velocity = -separating_velocity * bounciness;

	//衝突方向に作用力を計算する
	Vector3 acceleration = collision.rigidbody_one->GetAcceleration();
	if (collision.rigidbody_two)
	{
		acceleration -= collision.rigidbody_two->GetAcceleration();
	}
	float separating_acceleration = acceleration.Dot(collision.normal);
	
	//衝突法線の逆方向になれば
	if (separating_acceleration < 0.0f)
	{
		bounciness_velocity = max(bounciness_velocity + separating_acceleration * bounciness, 0.0f);
	}

	//速度差分計算
	float delta_velocity = bounciness_velocity - separating_velocity;

	//逆質量取得
	float total_inverse_mass = collision.rigidbody_one->GetInverseMass();
	if (collision.rigidbody_two)
	{
		total_inverse_mass += collision.rigidbody_two->GetInverseMass();
	}

	//質量が無限大の場合計算しない
	if (total_inverse_mass <= 0.0f) return;

	//衝突力計算
	float impulse = delta_velocity / total_inverse_mass;

	//単位逆質量の衝突力
	const Vector3& impulse_per_inverse_mass = collision.normal * impulse;

	//速度計算
	collision.rigidbody_one->AddVelocity(impulse_per_inverse_mass * collision.rigidbody_one->GetInverseMass());
	if (collision.rigidbody_two)
	{
		collision.rigidbody_two->AddVelocity(impulse_per_inverse_mass * -1.0f * collision.rigidbody_two->GetInverseMass());
	}
}

//--------------------------------------------------------------------------------
//  めり込みの解決
//--------------------------------------------------------------------------------
void PhysicsSystem::ResolveInterpenetration(Collision& collision)
{
	//衝突しない
	if (collision.penetration <= 0.0f) return;

	//逆質量計算
	float total_inverse_mass = collision.rigidbody_one->GetInverseMass();
	if (collision.rigidbody_two)
	{
		total_inverse_mass += collision.rigidbody_two->GetInverseMass();
	}

	//質量が無限大の場合計算しない
	if (total_inverse_mass <= 0.0f) return;

	//質量単位戻り量計算
	const Vector3& movement_per_inverse_mass = collision.normal * collision.penetration / total_inverse_mass;

	//各Rigidbody戻り位置計算
	collision.rigidbody_one->AddFixedMovement(movement_per_inverse_mass * collision.rigidbody_one->GetInverseMass());
	if (collision.rigidbody_two)
	{
		collision.rigidbody_two->AddFixedMovement(movement_per_inverse_mass * collision.rigidbody_two->GetInverseMass());
	}
}

//--------------------------------------------------------------------------------
//  分離速度の算出
//--------------------------------------------------------------------------------
float PhysicsSystem::CalculateSeparatingVelocity(Collision& collision)
{
	auto relative_velocity = collision.rigidbody_one->GetVelocity();
	if (collision.rigidbody_two) relative_velocity -= collision.rigidbody_two->GetVelocity();
	return relative_velocity.Dot(collision.normal);
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
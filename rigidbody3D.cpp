//--------------------------------------------------------------------------------
//
//　3DRigidbodyComponent.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "rigidbody3d.h"
#include "game_object.h"
#include "physics_system.h"
#include "collider.h"
#include "time.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
Rigidbody3D::Rigidbody3D(GameObject& owner)
	: Rigidbody(owner, Type::kRigidbody3D)
	, mass_(1.0f)
	, inverse_mass_(1.0f)
	, drag_(0.95f)
	, friction_(1.0f)
	, bounciness_(0.0f)
	, gravity_multiplier_(5.0f)
	, movement_(Vector3::kZero)
	, velocity_(Vector3::kZero)
	, acceleration_(Vector3::kZero)
	, force_accum_(Vector3::kZero)
	, fixed_movement_(Vector3::kZero)
{}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void Rigidbody3D::Update(void)
{
	auto transform = owner_.GetTransform();

	//重力加速度
	acceleration_ += PhysicsSystem::kGravity * gravity_multiplier_;

	//力から加速度を計算する
	acceleration_ += force_accum_ * inverse_mass_;
	
	//速度
	velocity_ += acceleration_ * Time::Instance()->DeltaTime();
	
	//位置更新
	movement_ += velocity_ * Time::Instance()->DeltaTime();
	transform->SetPosition(transform->GetPosition() + movement_);

	// TODO : 3D回転
	//回転力から回転加速度を計算する
	//Matrix44 mtxIitWorld;
	//calculateInertiaTensorWorld(mtxIitWorld);
	//Vector3 vAngularacceleration_ = CKFMath::Vec3TransformCoord(m_vTorqueAccum, mtxIitWorld);
	//m_vAngularvelocity_   += vAngularacceleration_;
	//回転更新
	//pTrans->RotByEuler(m_vAngularvelocity_);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void Rigidbody3D::LateUpdate(void)
{
	// 物理演算より位置補正
	auto transform = owner_.GetTransform();
	transform->SetPosition(transform->GetPosition() + fixed_movement_);

	//処理完了
	velocity_ *= drag_;
	movement_ = Vector3::kZero;
	fixed_movement_ = Vector3::kZero;
	force_accum_ = Vector3::kZero;
	acceleration_ = Vector3::kZero;
	//m_vTorqueAccum = Vector3(0.0f);
	//m_vAngularvelocity_ *= Angulardrag_;
}

//--------------------------------------------------------------------------------
//  質量の設定
//--------------------------------------------------------------------------------
void Rigidbody3D::SetMass(const float& value)
{
	mass_ = value;
	inverse_mass_ = value == 0.0f ? INFINITY : 1.0f / value;
}

//--------------------------------------------------------------------------------
//  慣性テンソルの算出
//--------------------------------------------------------------------------------
//void Rigidbody3D::SetInertiaTensor(Collider* pCollider)
//{
//	float fV00 = 1.0f;
//	float fV11 = 1.0f;
//	float fV22 = 1.0f;
//	ColliderType type = pCollider->GetType();
//	if (type == Sphere)
//	{
//		float fRadius = dynamic_cast<SphereCollider*>(pCollider)->GetRadius();
//		fV00 
//			= fV11
//			= fV22
//			= 0.4f * mass * fRadius * fRadius;
//	}
//	else if (type == AABB)
//	{
//		Vector3 vSize = dynamic_cast<AABBCollider*>(pCollider)->GetHalfSize() * 2.0f;
//		float fWork = mass / 12.0f;
//		fV00 = fWork * (vSize.Y * vSize.Y + vSize.Z * vSize.Z);
//		fV11 = fWork * (vSize.Y * vSize.Y + vSize.Z * vSize.Z);
//		fV22 = fWork * (vSize.Y * vSize.Y + vSize.Y * vSize.Y);
//	}
//	else if (type == OBB)
//	{
//		Vector3 vSize = dynamic_cast<OBBCollider*>(pCollider)->GetHalfSize() * 2.0f;
//		float fWork = mass / 12.0f;
//		fV00 = fWork * (vSize.Y * vSize.Y + vSize.Z * vSize.Z);
//		fV11 = fWork * (vSize.Y * vSize.Y + vSize.Z * vSize.Z);
//		fV22 = fWork * (vSize.Y * vSize.Y + vSize.Y * vSize.Y);
//	}
//
//	m_mtxInertisTensor = {
//		fV00, 0.0f, 0.0f, 0.0f,
//		0.0f, fV11, 0.0f, 0.0f,
//		0.0f, 0.0f, fV22, 0.0f,
//		0.0f, 0.0f, 0.0f, 1.0f };
//}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ワールド慣性テンソルの算出
//--------------------------------------------------------------------------------
//void Rigidbody3D::calculateInertiaTensorWorld(Matrix44& mtxIitWorld)
//{
//	auto listCollider = m_pGameObj->GetColliders();
//	if (!listCollider.empty())
//	{
//		//ワールドマトリクスの取得
//		auto itrBegin = listCollider.begin();
//		Matrix44 mtxWorld = (*itrBegin)->GetMatrixWorld();
//		
//		//移動量を無くす
//		mtxWorld.Elements[3][0] = 0.0f;
//		mtxWorld.Elements[3][1] = 0.0f;
//		mtxWorld.Elements[3][2] = 0.0f;
//
//		//ワールド慣性テンソルの算出
//		mtxIitWorld = m_mtxInertisTensor * mtxWorld;
//		
//		//逆行列の算出
//		mtxIitWorld = CKFMath::MtxTranspose(mtxIitWorld);
//	}
//
//	/*auto listCollider = m_pGameObj->GetColliders();
//	if (!listCollider.empty())
//	{
//		auto itrBegin = listCollider.begin();
//		const Matrix44& mtxWorld = (*itrBegin)->GetMatrixWorld();
//		float fT4 = mtxWorld.Elements[0][0] * m_mtxInverseInertisTensor.Elements[0][0]
//			+ mtxWorld.Elements[1][0] * m_mtxInverseInertisTensor.Elements[0][1]
//			+ mtxWorld.Elements[2][0] * m_mtxInverseInertisTensor.Elements[0][2];
//		float fT9 = mtxWorld.Elements[0][0] * m_mtxInverseInertisTensor.Elements[1][0]
//			+ mtxWorld.Elements[1][0] * m_mtxInverseInertisTensor.Elements[1][1]
//			+ mtxWorld.Elements[2][0] * m_mtxInverseInertisTensor.Elements[1][2];
//		float fT14 = mtxWorld.Elements[0][0] * m_mtxInverseInertisTensor.Elements[2][0]
//			+ mtxWorld.Elements[1][0] * m_mtxInverseInertisTensor.Elements[2][1]
//			+ mtxWorld.Elements[2][0] * m_mtxInverseInertisTensor.Elements[2][2];
//		float fT28 = mtxWorld.Elements[0][1] * m_mtxInverseInertisTensor.Elements[0][0]
//			+ mtxWorld.Elements[1][1] * m_mtxInverseInertisTensor.Elements[0][1]
//			+ mtxWorld.Elements[2][1] * m_mtxInverseInertisTensor.Elements[0][2];
//		float fT33 = mtxWorld.Elements[0][1] * m_mtxInverseInertisTensor.Elements[1][0]
//			+ mtxWorld.Elements[1][1] * m_mtxInverseInertisTensor.Elements[1][1]
//			+ mtxWorld.Elements[2][1] * m_mtxInverseInertisTensor.Elements[1][2];
//		float fT38 = mtxWorld.Elements[0][1] * m_mtxInverseInertisTensor.Elements[2][0]
//			+ mtxWorld.Elements[1][1] * m_mtxInverseInertisTensor.Elements[2][1]
//			+ mtxWorld.Elements[2][1] * m_mtxInverseInertisTensor.Elements[2][2];
//		float fT52 = mtxWorld.Elements[0][2] * m_mtxInverseInertisTensor.Elements[0][0]
//			+ mtxWorld.Elements[1][2] * m_mtxInverseInertisTensor.Elements[0][1]
//			+ mtxWorld.Elements[2][2] * m_mtxInverseInertisTensor.Elements[0][2];
//		float fT57 = mtxWorld.Elements[0][2] * m_mtxInverseInertisTensor.Elements[1][0]
//			+ mtxWorld.Elements[1][2] * m_mtxInverseInertisTensor.Elements[1][1]
//			+ mtxWorld.Elements[2][2] * m_mtxInverseInertisTensor.Elements[1][2];
//		float fT62 = mtxWorld.Elements[0][2] * m_mtxInverseInertisTensor.Elements[2][0]
//			+ mtxWorld.Elements[1][2] * m_mtxInverseInertisTensor.Elements[2][1]
//			+ mtxWorld.Elements[2][2] * m_mtxInverseInertisTensor.Elements[2][2];
//
//		mtxIitWorld.Elements[0][0] = fT4 * mtxWorld.Elements[0][0]
//			+ fT9 * mtxWorld.Elements[1][0]
//			+ fT14 * mtxWorld.Elements[2][0];
//		mtxIitWorld.Elements[1][0] = fT4 * mtxWorld.Elements[0][1]
//			+ fT9 * mtxWorld.Elements[1][1]
//			+ fT14 * mtxWorld.Elements[2][1];
//		mtxIitWorld.Elements[2][0] = fT4 * mtxWorld.Elements[0][2]
//			+ fT9 * mtxWorld.Elements[1][2]
//			+ fT14 * mtxWorld.Elements[2][2];
//
//		mtxIitWorld.Elements[0][1] = fT28 * mtxWorld.Elements[0][0]
//			+ fT33 * mtxWorld.Elements[1][0]
//			+ fT38 * mtxWorld.Elements[2][0];
//		mtxIitWorld.Elements[1][1] = fT28 * mtxWorld.Elements[0][1]
//			+ fT33 * mtxWorld.Elements[1][1]
//			+ fT38 * mtxWorld.Elements[2][1];
//		mtxIitWorld.Elements[2][1] = fT28 * mtxWorld.Elements[0][2]
//			+ fT33 * mtxWorld.Elements[1][2]
//			+ fT38 * mtxWorld.Elements[2][2];
//
//		mtxIitWorld.Elements[0][2] = fT52 * mtxWorld.Elements[0][0]
//			+ fT57 * mtxWorld.Elements[1][0]
//			+ fT62 * mtxWorld.Elements[2][0];
//		mtxIitWorld.Elements[1][2] = fT52 * mtxWorld.Elements[0][1]
//			+ fT57 * mtxWorld.Elements[1][1]
//			+ fT62 * mtxWorld.Elements[2][1];
//		mtxIitWorld.Elements[2][2] = fT52 * mtxWorld.Elements[0][2]
//			+ fT57 * mtxWorld.Elements[1][2]
//			+ fT62 * mtxWorld.Elements[2][2];
//	}*/
//}
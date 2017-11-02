//--------------------------------------------------------------------------------
//	コリジョン計算関数
//　KF_CollisionUtility.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-13
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "collisionDetector.h"
#include "physicsSystem.h"
#include "sphereCollider.h"
#include "fieldCollider.h"
#include "boxCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"
#include "gameObject.h"
#include "3DRigidbodyComponent.h"
#include "behaviorComponent.h"
#include "transformComponent.h"

#ifdef _DEBUG
#include "debugObserver.h"
#endif

//--------------------------------------------------------------------------------
//
//  衝突判定関数
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：Detect
//  関数説明：スフィアとスフィアの当たり判定関数
//			　当たったらコリジョン情報を物理演算マネージャに登録する
//	引数：	sphereL：スフィアコライダー
//			sphereR：スフィアコライダー
//	戻り値：なし
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(SphereCollider& sphereL, SphereCollider& sphereR)
{
	const auto& sphereLPosition = sphereL.GetNextWorldPosition();
	const auto& sphereLRadius = sphereL.GetRadius();
	const auto& sphereRPosition = sphereR.GetNextWorldPosition();
	const auto& sphereRRadius = sphereR.GetRadius();
	auto& midLine = sphereLPosition - sphereRPosition;
	float minDistance = sphereLRadius + sphereRRadius;
	float squareDistance = midLine.SquareMagnitude();
	if (squareDistance >= minDistance * minDistance) return;

	if (sphereL.IsTrigger() || sphereR.IsTrigger())
	{//トリガーだったら物理処理しない
		auto& leftBehaviors = sphereL.GetGameObject()->GetBehaviorComponent();
		for (auto& behavior : leftBehaviors) behavior->OnTrigger(sphereL, sphereR);

		auto& rightBehaviors = sphereR.GetGameObject()->GetBehaviorComponent();
		for (auto& behavior : rightBehaviors) behavior->OnTrigger(sphereR, sphereL);
		return;
	}

	auto collision = new Collision;

	//衝突点の算出
	collision->Point = sphereRPosition + midLine * 0.5f;

	//衝突深度の算出
	float distance = sqrtf(squareDistance);
	collision->Penetration = minDistance - distance;

	//衝突法線の算出
	collision->Normal = midLine / distance;

	//リジッドボディの取得
	auto leftRigidbody = sphereL.GetGameObject()->GetRigidbodyComponent();
	auto rightRigidbody = sphereR.GetGameObject()->GetRigidbodyComponent();
	if (leftRigidbody->GetType() == CRigidbodyComponent::RB_3D)
	{
		collision->RigidbodyOne = dynamic_cast<C3DRigidbodyComponent*>(leftRigidbody);

		if (rightRigidbody->GetType() == CRigidbodyComponent::RB_3D)
		{
			collision->RigidbodyTwo = dynamic_cast<C3DRigidbodyComponent*>(rightRigidbody);
		}
	}
	else
	{//一番が持ってないなら衝突法線を反転する
		collision->Normal *= -1.0f;
		collision->RigidbodyOne = dynamic_cast<C3DRigidbodyComponent*>(rightRigidbody);
	}

	//物理演算システムにレジストリ
	PhysicsSystem::Instance()->Register(collision);

	//OnCollision
	CollisionInfo info;
	info.This = &sphereL;
	info.Other = &sphereR;
	info.Collisions.push_back(collision);
	auto& leftBehaviors = sphereL.GetGameObject()->GetBehaviorComponent();
	for (auto& behavior : leftBehaviors) { behavior->OnCollision(info); }

	info.This = &sphereR;
	info.Other = &sphereL;
	auto& rightBehaviors = sphereR.GetGameObject()->GetBehaviorComponent();
	for (auto& behavior : rightBehaviors) { behavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	関数名：Detect
//  関数説明：スフィアとAABBの当たり判定関数
//			　当たったらコリジョン情報を物理演算マネージャに登録する
//	引数：	sphere：スフィアコライダー
//			aabb：回転なしボックスコライダー
//	戻り値：なし
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(SphereCollider& sphere, AABBCollider& aabb)
{
	const auto& spherePosition = sphere.GetNextWorldPosition();
	const auto& sphereRadius = sphere.GetRadius();
	const auto& aabbHalfSize = aabb.GetHalfSize();
	const auto& aabbPosition = aabb.GetNextWorldPosition();
	auto& realPosition = spherePosition - aabbPosition;

	//分離軸チェック
	if (fabsf(realPosition.X) - sphereRadius > aabbHalfSize.X
		|| fabsf(realPosition.Y) - sphereRadius > aabbHalfSize.Y
		|| fabsf(realPosition.Z) - sphereRadius > aabbHalfSize.Z)
	{
		return;
	}

	Vector3 closestPosition;
	float distance;

	//AABBとスフィアの最近点の算出
	distance = realPosition.X;
	if (distance > aabbHalfSize.X) { distance = aabbHalfSize.X; }
	else if (distance < -aabbHalfSize.X) { distance = -aabbHalfSize.X; }
	closestPosition.X = distance;

	distance = realPosition.Y;
	if (distance > aabbHalfSize.Y) { distance = aabbHalfSize.Y; }
	else if (distance < -aabbHalfSize.Y) { distance = -aabbHalfSize.Y; }
	closestPosition.Y = distance;

	distance = realPosition.Z;
	if (distance > aabbHalfSize.Z) { distance = aabbHalfSize.Z; }
	else if (distance < -aabbHalfSize.Z) { distance = -aabbHalfSize.Z; }
	closestPosition.Z = distance;

	//衝突検知
	float squareDistance = (closestPosition - realPosition).SquareMagnitude();
	if (squareDistance >= sphereRadius * sphereRadius) return;

	//OnTrigger
	if (sphere.IsTrigger() || aabb.IsTrigger())
	{//トリガーだったら物理処理しない
		auto& sphereBehaviors = sphere.GetGameObject()->GetBehaviorComponent();
		for (auto& behavior : sphereBehaviors) { behavior->OnTrigger(sphere, aabb); }

		auto& aabbBehaviors = aabb.GetGameObject()->GetBehaviorComponent();
		for (auto& behavior : aabbBehaviors) { behavior->OnTrigger(aabb, sphere); }
		return;
	}

	//衝突情報
	closestPosition = closestPosition + aabbPosition;
	auto collision = new Collision;
	collision->Normal = spherePosition - closestPosition;
	if (collision->Normal.SquareMagnitude() == 0.0f)
	{//中心がaabbの中にある
		auto transform = sphere.GetGameObject()->GetTransformComponent();
		auto& currentPosition = transform->GetPos();
		auto& nextPosition = transform->GetPosNext();
		collision->Normal = currentPosition - nextPosition;
	}
	collision->Normal.Normalize();
	collision->Point = closestPosition;
	collision->Penetration = sphereRadius - sqrtf(squareDistance);

	//リジッドボディの取得
	auto sphereRigidbody = sphere.GetGameObject()->GetRigidbodyComponent();
	auto aabbRigidbody = aabb.GetGameObject()->GetRigidbodyComponent();
	if (sphereRigidbody->GetType() == CRigidbodyComponent::RB_3D)
	{
		collision->RigidbodyOne = dynamic_cast<C3DRigidbodyComponent*>(sphereRigidbody);

		if (aabbRigidbody->GetType() == CRigidbodyComponent::RB_3D)
		{
			collision->RigidbodyTwo = dynamic_cast<C3DRigidbodyComponent*>(aabbRigidbody);
		}
	}
	else
	{//一番が持ってないなら衝突法線を反転する
		collision->Normal *= -1.0f;
		collision->RigidbodyOne = dynamic_cast<C3DRigidbodyComponent*>(aabbRigidbody);
	}

	//物理演算システムにレジストリ
	PhysicsSystem::Instance()->Register(collision);

	//OnCollision
	CollisionInfo info;
	info.This = &sphere;
	info.Other = &aabb;
	info.Collisions.push_back(collision);
	auto& sphereBehaviors = sphere.GetGameObject()->GetBehaviorComponent();
	for (auto& behavior : sphereBehaviors) { behavior->OnCollision(info); }

	info.This = &aabb;
	info.Other = &sphere;
	auto& aabbBehaviors = aabb.GetGameObject()->GetBehaviorComponent();
	for (auto& behavior : aabbBehaviors) { behavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	関数名：CheckSphereWithOBB
//  関数説明：スフィアとOBBの当たり判定関数
//			　当たったらコリジョン情報を物理演算マネージャに登録する
//	引数：	sphere：スフィアコライダー
//			obb：回転あるボックスコライダー
//	戻り値：なし
//--------------------------------------------------------------------------------
void CollisionDetector::CheckSphereWithOBB(SphereCollider& sphere, OBBCollider& obb)
{
	const auto& spherePosition = sphere.GetNextWorldPosition();
	const auto& sphereRadius = sphere.GetRadius();
	const auto& aabbHalfSize = obb.GetHalfSize();
	const auto& mtxBox = obb.GetNextWorldMatrix();
	auto& realPosition = CKFMath::TransformInverse(mtxBox, spherePosition);

	//分離軸チェック
	if (fabsf(realPosition.X) - sphereRadius > aabbHalfSize.X
		|| fabsf(realPosition.Y) - sphereRadius > aabbHalfSize.Y
		|| fabsf(realPosition.Z) - sphereRadius > aabbHalfSize.Z)
	{
		return;
	}

	Vector3 closestPosition;
	float distance;

	//OBBとスフィアの最近点の算出
	distance = realPosition.X;
	if (distance > aabbHalfSize.X) { distance = aabbHalfSize.X; }
	else if (distance < -aabbHalfSize.X) { distance = -aabbHalfSize.X; }
	closestPosition.X = distance;

	distance = realPosition.Y;
	if (distance > aabbHalfSize.Y) { distance = aabbHalfSize.Y; }
	else if (distance < -aabbHalfSize.Y) { distance = -aabbHalfSize.Y; }
	closestPosition.Y = distance;

	distance = realPosition.Z;
	if (distance > aabbHalfSize.Z) { distance = aabbHalfSize.Z; }
	else if (distance < -aabbHalfSize.Z) { distance = -aabbHalfSize.Z; }
	closestPosition.Z = distance;

	//衝突検知
	float squareDistance = CKFMath::VecMagnitudeSquare(closestPosition - realPosition);
	if (squareDistance >= sphereRadius * sphereRadius) { return; }

	//OnTrigger
	if (sphere.IsTrigger() || obb.IsTrigger())
	{//トリガーだったら物理処理しない
		auto& leftBehaviors = sphere.GetGameObject()->GetBehaviorComponent();
		for (auto& behavior : leftBehaviors) { behavior->OnTrigger(sphere, obb); }

		auto& rightBehaviors = obb.GetGameObject()->GetBehaviorComponent();
		for (auto& behavior : rightBehaviors) { behavior->OnTrigger(obb, sphere); }
		return;
	}

	//衝突情報
	closestPosition = CKFMath::Vec3TransformCoord(closestPosition, mtxBox);
	auto collision = new Collision;
	collision->Normal = spherePosition - closestPosition;
	if (CKFMath::VecMagnitudeSquare(collision->Normal) == 0.0f)
	{//中心がobbの中にある
		auto transform = sphere.GetGameObject()->GetTransformComponent();
		auto Position = transform->GetPos();
		auto PositionNext = transform->GetPosNext();
		collision->Normal = Position - PositionNext;
	}
	CKFMath::VecNormalize(collision->Normal);
	collision->Point = closestPosition;
	collision->Penetration = sphereRadius - sqrtf(squareDistance);

	//リジッドボディの取得
	CRigidbodyComponent* leftRigidbody = sphere.GetGameObject()->GetRigidbodyComponent();
	CRigidbodyComponent* rightRigidbody = obb.GetGameObject()->GetRigidbodyComponent();
	if (leftRigidbody->GetType() == CRigidbodyComponent::RB_3D)
	{
		collision->RigidbodyOne = dynamic_cast<C3DRigidbodyComponent*>(leftRigidbody);

		if (rightRigidbody->GetType() == CRigidbodyComponent::RB_3D)
		{
			collision->RigidbodyTwo = dynamic_cast<C3DRigidbodyComponent*>(rightRigidbody);
		}
	}
	else
	{//一番が持ってないなら衝突法線を反転する
		collision->Normal *= -1.0f;
		collision->RigidbodyOne = dynamic_cast<C3DRigidbodyComponent*>(rightRigidbody);
	}

	//物理演算システムにレジストリ
	PhysicsSystem::Instance()->Register(collision);

	//OnCollision
	CollisionInfo info;
	info.This = &sphere;
	info.Other = &obb;
	info.Collisions.push_back(collision);
	auto& leftBehaviors = sphere.GetGameObject()->GetBehaviorComponent();
	for (auto& behavior : leftBehaviors) { behavior->OnCollision(info); }

	info.This = &obb;
	info.Other = &sphere;
	auto& rightBehaviors = obb.GetGameObject()->GetBehaviorComponent();
	for (auto& behavior : rightBehaviors) { behavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	関数名：CheckAABBWithAABB
//  関数説明：AABBとAABBの当たり判定関数
//			　当たったらコリジョン情報を物理演算マネージャに登録する
//	引数：	aabbL：回転なしボックスコライダー
//			aabbR：回転なしボックスコライダー
//	戻り値：なし
//--------------------------------------------------------------------------------
void CollisionDetector::CheckAABBWithAABB(AABBCollider& aabbL, AABBCollider& aabbR)
{
	if (!checkOverlapAABB(aabbL, aabbR)) { return; }

	//OnTrigger
	if (aabbL.IsTrigger() || aabbR.IsTrigger())
	{//トリガーだったら物理処理しない
		auto& leftBehaviors = aabbL.GetGameObject()->GetBehaviorComponent();
		for (auto& behavior : leftBehaviors) { behavior->OnTrigger(aabbL, aabbR); }

		auto& rightBehaviors = aabbR.GetGameObject()->GetBehaviorComponent();
		for (auto& behavior : rightBehaviors) { behavior->OnTrigger(aabbR, aabbL); }
		return;
	}

	//XYZ軸一番深度が浅いの軸を洗い出す
	const auto& PositionL = aabbL.GetNextWorldPosition();
	const auto& vHalfSizeL = aabbL.GetHalfSize();
	const auto& PositionR = aabbR.GetNextWorldPosition();
	const auto& vHalfSizeR = aabbR.GetHalfSize();
	auto midLine = PositionL - PositionR;
	auto vDisNoCol = vHalfSizeL + vHalfSizeR;
	auto PenetrationX = vDisNoCol.X - fabsf(midLine.X);
	auto PenetrationY = vDisNoCol.Y - fabsf(midLine.Y);
	auto PenetrationZ = vDisNoCol.Z - fabsf(midLine.Z);

	PenetrationX = PenetrationX > 0.0f ? PenetrationX : vDisNoCol.X;
	PenetrationY = PenetrationY > 0.0f ? PenetrationY : vDisNoCol.Y;
	PenetrationZ = PenetrationZ > 0.0f ? PenetrationZ : vDisNoCol.Z;
	auto PenetrationMin = min(PenetrationX, min(PenetrationY, PenetrationZ));
	
	auto collision = new Collision;
	collision->Penetration = PenetrationMin;
	collision->Point = midLine * 0.5f;
	if (PenetrationX == PenetrationMin)
	{
		collision->Normal = midLine.X < 0.0f ? Vector3(-1.0f, 0.0f, 0.0f) : Vector3(1.0f, 0.0f, 0.0f);
	}
	else if (PenetrationY == PenetrationMin)
	{
		collision->Normal = midLine.Y < 0.0f ? Vector3(0.0f, -1.0f, 0.0f) : Vector3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		collision->Normal = midLine.Z < 0.0f ? Vector3(0.0f, 0.0f, -1.0f) : Vector3(0.0f, 0.0f, 1.0f);
	}

	//リジッドボディの取得
	auto leftRigidbody = aabbL.GetGameObject()->GetRigidbodyComponent();
	auto rightRigidbody = aabbR.GetGameObject()->GetRigidbodyComponent();

	if (leftRigidbody->GetType() == CRigidbodyComponent::RB_3D)
	{
		collision->RigidbodyOne = dynamic_cast<C3DRigidbodyComponent*>(leftRigidbody);

		if (rightRigidbody->GetType() == CRigidbodyComponent::RB_3D)
		{
			collision->RigidbodyTwo = dynamic_cast<C3DRigidbodyComponent*>(rightRigidbody);
		}
	}
	else
	{//一番が持ってないなら衝突法線を反転する
		collision->Normal *= -1.0f;
		collision->RigidbodyOne = dynamic_cast<C3DRigidbodyComponent*>(rightRigidbody);
	}

	//物理演算システムにレジストリ
	PhysicsSystem::Instance()->Register(collision);

	//OnCollision
	CollisionInfo info;
	info.Collisions.push_back(collision);
	info.This = &aabbL;
	info.Other = &aabbR;
	auto& leftBehaviors = aabbL.GetGameObject()->GetBehaviorComponent();
	for (auto& behavior : leftBehaviors) { behavior->OnCollision(info); }

	info.This = &aabbR;
	info.Other = &aabbL;
	auto& rightBehaviors = aabbR.GetGameObject()->GetBehaviorComponent();
	for (auto& behavior : rightBehaviors) { behavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	関数名：CheckBoxWithBox
//  関数説明：boxとboxの当たり判定関数(いずれかが必ずOBB)
//			　当たったらコリジョン情報を物理演算マネージャに登録する
//	引数：	boxL：ボックスコライダー
//			boxR：ボックスコライダー
//	戻り値：なし
//--------------------------------------------------------------------------------
void CollisionDetector::CheckBoxWithBox(BoxCollider& boxL, BoxCollider& boxR)
{
	//ボックスがxyz軸上に重ねてるかどうかをチェックする
	if (!checkOverlapOnAxis(boxL, boxR, Vector3(1.0f, 0.0f, 0.0f))
		&& !checkOverlapOnAxis(boxL, boxR, Vector3(0.0f, 1.0f, 0.0f))
		&& !checkOverlapOnAxis(boxL, boxR, Vector3(0.0f, 0.0f, 1.0f)))
	{
		return;
	}

	bool bFindL = false;
	Collision collisionDepthMaxL;

	//boxLのすべての頂点とboxRと判定し、めり込みが一番深いの頂点を洗い出す
	auto& listVtxBoxL = boxL.GetWorldVertexes();
	for (auto itr = listVtxBoxL.begin(); itr != listVtxBoxL.end(); ++itr)
	{
		Collision collision;
		if (checkPointWithBox(collision, *itr, boxR))
		{
			if (!bFindL)
			{
				bFindL = true;
				collisionDepthMaxL = collision;
			}
			else if (collision.Penetration > collisionDepthMaxL.Penetration)
			{
				collisionDepthMaxL = collision;
			}
		}
	}

	bool bFindR = false;
	Collision collisionDepthMaxR;

	//boxRのすべての頂点とboxLと判定し、めり込みが一番深いの頂点を洗い出す
	auto& listVtxBoxR = boxR.GetWorldVertexes();
	for (auto itr = listVtxBoxR.begin(); itr != listVtxBoxR.end(); ++itr)
	{
		Collision collision;
		if (checkPointWithBox(collision, *itr, boxL))
		{
			if (!bFindR)
			{
				bFindR = true;
				collisionDepthMaxR = collision;
			}
			else if (collision.Penetration > collisionDepthMaxR.Penetration)
			{
				collisionDepthMaxR = collision;
			}
		}
	}

	if (!bFindL && !bFindR) { return; }

	//OnTrigger
	if (boxL.IsTrigger() || boxR.IsTrigger())
	{//トリガーだったら物理処理しない
		auto& leftBehaviors = boxL.GetGameObject()->GetBehaviorComponent();
		for (auto& behavior : leftBehaviors) { behavior->OnTrigger(boxL, boxR); }

		auto& rightBehaviors = boxR.GetGameObject()->GetBehaviorComponent();
		for (auto& behavior : rightBehaviors) { behavior->OnTrigger(boxR, boxL); }
		return;
	}

	CollisionInfo info;

	//リジッドボディの取得
	auto leftRigidbody = boxL.GetGameObject()->GetRigidbodyComponent();
	auto rightRigidbody = boxR.GetGameObject()->GetRigidbodyComponent();

	if (bFindL)
	{
		if (leftRigidbody->GetType() == CRigidbodyComponent::RB_3D)
		{
			collisionDepthMaxL.RigidbodyOne = dynamic_cast<C3DRigidbodyComponent*>(leftRigidbody);

			if (rightRigidbody->GetType() == CRigidbodyComponent::RB_3D)
			{
				collisionDepthMaxL.RigidbodyTwo = dynamic_cast<C3DRigidbodyComponent*>(rightRigidbody);
			}
		}
		else
		{//一番が持ってないなら衝突法線を反転する
			collisionDepthMaxL.Normal *= -1.0f;
			collisionDepthMaxL.RigidbodyOne = dynamic_cast<C3DRigidbodyComponent*>(rightRigidbody);
		}

		//物理演算システムにレジストリ
		auto collision = new Collision;
		*collision = collisionDepthMaxL;
		PhysicsSystem::Instance()->Register(collision);
		info.Collisions.push_back(&collisionDepthMaxL);
	}

	if (bFindR)
	{
		if (rightRigidbody->GetType() == CRigidbodyComponent::RB_3D)
		{
			collisionDepthMaxR.RigidbodyOne = dynamic_cast<C3DRigidbodyComponent*>(rightRigidbody);

			if (leftRigidbody->GetType() == CRigidbodyComponent::RB_3D)
			{
				collisionDepthMaxR.RigidbodyTwo = dynamic_cast<C3DRigidbodyComponent*>(leftRigidbody);
			}
		}
		else
		{//一番が持ってないなら衝突法線を反転する
			collisionDepthMaxR.Normal *= -1.0f;
			collisionDepthMaxR.RigidbodyOne = dynamic_cast<C3DRigidbodyComponent*>(leftRigidbody);
		}

		//物理演算システムにレジストリ
		auto collision = new Collision;
		*collision = collisionDepthMaxR;
		PhysicsSystem::Instance()->Register(collision);
		info.Collisions.push_back(&collisionDepthMaxR);
	}

	//OnCollision
	info.This = &boxL;
	info.Other = &boxR;
	info.Collisions.push_back(&collisionDepthMaxL);
	auto& leftBehaviors = boxL.GetGameObject()->GetBehaviorComponent();
	for (auto& behavior : leftBehaviors) { behavior->OnCollision(info); }

	info.This = &boxR;
	info.Other = &boxL;
	auto& rightBehaviors = boxR.GetGameObject()->GetBehaviorComponent();
	for (auto& behavior : rightBehaviors) { behavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//
//  フィールド
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：CheckSphereWithField
//  関数説明：スフィアとフィールドの当たり判定関数
//			　当たったらコリジョン情報を物理演算マネージャに登録する
//	引数：	sphere：スフィアコライダー
//			field：フィールドコライダー
//	戻り値：なし
//--------------------------------------------------------------------------------
void CollisionDetector::CheckSphereWithField(SphereCollider& sphere, FieldCollider& field)
{
	const auto& vSpherePos = sphere.GetNextWorldPosition();
	const auto& fSphereRadius = sphere.GetRadius();
	auto info = field.DetectCollision(vSpherePos);

	if (info.IsInTheField == false) { return; }

	//スフィア中心とポリゴンの距離の算出
	float Penetration = info.Penetration + fSphereRadius;

	if (Penetration <= 0.0f) { return; }

	if (sphere.IsTrigger())
	{
		auto& leftBehaviors = sphere.GetGameObject()->GetBehaviorComponent();
		for (auto& behavior : leftBehaviors) { behavior->OnTrigger(sphere, field); }

		auto& rightBehaviors = field.GetGameObject()->GetBehaviorComponent();
		for (auto& behavior : rightBehaviors) { behavior->OnTrigger(field, sphere); }
		return;
	}
	auto collision = new Collision;

	//衝突点の算出
	collision->Point = vSpherePos + info.SurfaceNormal * Penetration;

	//衝突深度の算出
	collision->Penetration = Penetration;

	//衝突法線の算出
	collision->Normal = info.SurfaceNormal;

	//リジッドボディの取得
	auto p3DRB = dynamic_cast<C3DRigidbodyComponent*>(sphere.GetGameObject()->GetRigidbodyComponent());
	collision->RigidbodyOne = p3DRB;
	collision->RigidbodyTwo = nullptr;

	//物理演算システムにレジストリ
	PhysicsSystem::Instance()->Register(collision);

	//OnCollision
	CollisionInfo cInfo;
	cinfo.This = &sphere;
	cinfo.Other = &field;
	cInfo.Collisions.push_back(collision);
	auto& leftBehaviors = sphere.GetGameObject()->GetBehaviorComponent();
	for (auto& behavior : leftBehaviors) { behavior->OnCollision(cInfo); }

	cinfo.This = &field;
	cinfo.Other = &sphere;
	auto& rightBehaviors = field.GetGameObject()->GetBehaviorComponent();
	for (auto& behavior : rightBehaviors) { behavior->OnCollision(cInfo); }
}

//--------------------------------------------------------------------------------
//	関数名：CheckBoxWithField
//  関数説明：Boxとフィールドの当たり判定関数
//			　当たったらコリジョン情報を物理演算マネージャに登録する
//	引数：	box：ボックスコライダー
//			field：フィールドコライダー
//	戻り値：なし
//--------------------------------------------------------------------------------
void CollisionDetector::CheckBoxWithField(BoxCollider& box, FieldCollider& field)
{
	auto& listOBBVtx = box.GetWorldVertexes();

	//回転なしの場合一番深いの頂点を案出する
	bool bFind = false;
	auto collision = new Collision;
	for (auto itr = listOBBVtx.begin(); itr != listOBBVtx.end(); ++itr)
	{
		auto info = field.DetectCollision((*itr));
		if (!info.IsInTheField) { continue; }
		if (info.Penetration <= 0.0f) { continue; }

		if (!bFind)
		{
			bFind = true;

			//衝突点の算出
			collision->Point = *itr;

			//衝突深度の算出
			collision->Penetration = info.Penetration;

			//衝突法線の算出
			collision->Normal = info.SurfaceNormal;

			//リジッドボディの取得
			auto p3DRB = dynamic_cast<C3DRigidbodyComponent*>(box.GetGameObject()->GetRigidbodyComponent());
			collision->RigidbodyOne = p3DRB;
			collision->RigidbodyTwo = nullptr;
		}
		else
		{//深度を比べる
			if (info.Penetration <= collision->Penetration) { continue; }

			//衝突点の算出
			collision->Point = *itr;

			//衝突深度の算出
			collision->Penetration = info.Penetration;

			//衝突法線の算出
			collision->Normal = info.SurfaceNormal;
		}
	}

	if (!bFind) { return; }

	if (box.IsTrigger())
	{
		auto& leftBehaviors = box.GetGameObject()->GetBehaviorComponent();
		for (auto& behavior : leftBehaviors) { behavior->OnTrigger(box, field); }

		auto& rightBehaviors = field.GetGameObject()->GetBehaviorComponent();
		for (auto& behavior : rightBehaviors) { behavior->OnTrigger(field, box); }
		
		delete collision;
		return;
	}

	//物理演算システムにレジストリ
	PhysicsSystem::Instance()->Register(collision);

	//OnCollision
	CollisionInfo cInfo;
	cinfo.This = &box;
	cinfo.Other = &field;
	cInfo.Collisions.push_back(collision);
	auto& leftBehaviors = box.GetGameObject()->GetBehaviorComponent();
	for (auto& behavior : leftBehaviors) { behavior->OnCollision(cInfo); }

	cinfo.This = &field;
	cinfo.Other = &box;
	auto& rightBehaviors = field.GetGameObject()->GetBehaviorComponent();
	for (auto& behavior : rightBehaviors) { behavior->OnCollision(cInfo); }
}

//--------------------------------------------------------------------------------
//
//  レイ
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：CheckRayWithBox
//  関数説明：レイとボックスの当たり判定
//	引数：	ray：レイ
//			distancetance：レイの長さ
//			box：ボックスコライダー
//			infoOut：レイの当たり判定情報(出力用)
//	戻り値：bool
//--------------------------------------------------------------------------------
bool CollisionDetector::CheckRayWithBox(const Ray& ray, const float& distancetance, BoxCollider& box, RayHitInfo& infoOut)
{
	Collision collision;
	if (checkPointWithBox(collision, ray.Origin, box))
	{
		infoOut.Normal = collision.Normal;
		infoOut.Position = ray.Origin;
		infoOut.Collider = &box;
		infoOut.Distance = collision.Penetration;
		return true;
	}
	
	auto& vRayEnd = ray.Origin + ray.Direction * distancetance;
	if (checkPointWithBox(collision, vRayEnd, box))
	{
		infoOut.Normal = collision.Normal;
		infoOut.Position = vRayEnd;
		infoOut.Collider = &box;
		infoOut.Distance = collision.Penetration;
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------
//	関数名：CheckRayWithSphere
//  関数説明：レイとスフィアの当たり判定
//	引数：	ray：レイ
//			distancetance：レイの長さ
//			sphere：スフィアコライダー
//			infoOut：レイの当たり判定情報(出力用)
//	戻り値：bool
//--------------------------------------------------------------------------------
bool CollisionDetector::CheckRayWithSphere(const Ray& ray, const float& distancetance, SphereCollider& sphere, RayHitInfo& infoOut)
{
	Vector3 vOriginToSphere;
	const auto& vSpherePos = sphere.GetNextWorldPosition();
	const auto& fRadius = sphere.GetRadius();
	float fWorkA, fWorkB, fTimeA, fTimeB;
	float distancecriminant;

	vOriginToSphere = ray.Origin - vSpherePos;
	fWorkA = 2.0f * CKFMath::Vec3Dot(ray.Direction, vOriginToSphere);
	fWorkB = CKFMath::Vec3Dot(vOriginToSphere, vOriginToSphere) - fRadius * fRadius;

	distancecriminant = fWorkA * fWorkA - 4.0f * fWorkB;

	if (distancecriminant < 0.0f)
	{
		return false;
	}

	distancecriminant = sqrtf(distancecriminant);

	fTimeA = (-fWorkA + distancecriminant) / 2.0f;
	fTimeB = (-fWorkA - distancecriminant) / 2.0f;

	if (fTimeA < 0.0f && fTimeB < 0.0f) { return false; }
	
	//最短時間を保存
	fTimeA = fTimeA < 0.0f ? fTimeB : fTimeA;
	fTimeB = fTimeB < 0.0f ? fTimeA : fTimeB;
	auto fTimingMin = fTimeA <= fTimeB ? fTimeA : fTimeB;

	if (fTimingMin > distancetance) { return false; }

	infoOut.Distance = fTimingMin;
	infoOut.Collider = &sphere;
	infoOut.Position = ray.Origin + ray.Direction * fTimingMin;
	infoOut.Normal = infoOut.Position - vSpherePos;
	CKFMath::VecNormalize(infoOut.Normal);
	return true;
}

//--------------------------------------------------------------------------------
//	関数名：CheckRayWithField
//  関数説明：レイとフィールドの当たり判定
//	引数：	ray：レイ
//			distancetance：レイの長さ
//			field：フィールドコライダー
//			infoOut：レイの当たり判定情報(出力用)
//	戻り値：bool
//--------------------------------------------------------------------------------
bool CollisionDetector::CheckRayWithField(const Ray& ray, const float& distancetance, FieldCollider& field, RayHitInfo& infoOut)
{
	//auto vRayMax = ray.Origin + ray.Direction * distancetance;
	//int nNumVtxX = 0;
	//int nNumVtxZ = 0;
	//vector<Vector3> vecVtx;
	//if (!field.GetVtxByRange(ray.Origin, vRayMax, nNumVtxX, nNumVtxZ, vecVtx))
	//{
	//	return false;
	//}

	//一時採用
	auto info = field.DetectCollision(ray.Origin + ray.Direction * distancetance);
	if (!info.IsInTheField) { return false; }
	if (info.Penetration < 0.0f) { return false; }
	infoOut.Distance = info.Penetration;
	infoOut.Normal = info.SurfaceNormal;
	infoOut.Collider = &field;
	return true;
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：checkPointWithAABB
//  関数説明：点とAABBの当たり判定
//	引数：	collisionOut：衝突情報(出力用)
//			vPoint：点の位置
//			aabb：AABBコライダー
//	戻り値：bool
//--------------------------------------------------------------------------------
bool CollisionDetector::checkPointWithAABB(Collision& collisionOut, const Vector3 vPoint, const AABBCollider& aabb)
{
	const auto& aabbPosition = aabb.GetNextWorldPosition();
	const auto& aabbHalfSize = aabb.GetHalfSize();
	auto& vRealPoint = vPoint - aabbPosition;

	float fDepthMin = aabbHalfSize.X - fabsf(vRealPoint.X);
	if (fDepthMin <= 0.0f) { return false; }
	collisionOut.Normal = vRealPoint.X < 0.0f ? CKFMath::sc_vLeft : CKFMath::sc_vRight;

	float fDepth = aabbHalfSize.Y - fabsf(vRealPoint.Y);
	if (fDepth <= 0.0f) { return false; }
	else if (fDepth < fDepthMin)
	{
		fDepthMin = fDepth;
		collisionOut.Normal = vRealPoint.Y < 0.0f ? CKFMath::sc_vDown : CKFMath::sc_vUp;
	}

	fDepth = aabbHalfSize.Z - fabsf(vRealPoint.Z);
	if (fDepth <= 0.0f) { return false; }
	else if (fDepth < fDepthMin)
	{
		fDepthMin = fDepth;
		collisionOut.Normal = vRealPoint.Z < 0.0f ? CKFMath::sc_vBack : CKFMath::sc_vForward;
	}

	collisionOut.Penetration = fDepthMin;
	collisionOut.Point = vPoint;
	return true;
}

//--------------------------------------------------------------------------------
//	関数名：checkPointWithBox
//  関数説明：点とBoxの当たり判定
//	引数：	collisionOut：衝突情報(出力用)
//			vPoint：点の位置
//			box：boxコライダー
//	戻り値：bool
//--------------------------------------------------------------------------------
bool CollisionDetector::checkPointWithBox(Collision& collisionOut, const Vector3 vPoint, const BoxCollider& box)
{
	const auto& mtxBox = box.GetNextWorldMatrix();
	const auto& aabbHalfSize = box.GetHalfSize();
	auto& vRealPoint = CKFMath::TransformInverse(mtxBox, vPoint);

	float fDepthMin = aabbHalfSize.X - fabsf(vRealPoint.X);
	if (fDepthMin <= 0.0f) { return false; }
	collisionOut.Normal = Vector3(mtxBox.Elements[0][0], mtxBox.Elements[0][1], mtxBox.Elements[0][2])
		* (vRealPoint.X < 0.0f ? -1.0f : 1.0f);

	float fDepth = aabbHalfSize.Y - fabsf(vRealPoint.Y);
	if (fDepth <= 0.0f) { return false; }
	else if (fDepth < fDepthMin)
	{
		fDepthMin = fDepth;
		collisionOut.Normal = Vector3(mtxBox.Elements[1][0], mtxBox.Elements[1][1], mtxBox.Elements[1][2])
			* (vRealPoint.Y < 0.0f ? -1.0f : 1.0f);
	}

	fDepth = aabbHalfSize.Z - fabsf(vRealPoint.Z);
	if (fDepth <= 0.0f) { return false; }
	else if (fDepth < fDepthMin)
	{
		fDepthMin = fDepth;
		collisionOut.Normal = Vector3(mtxBox.Elements[2][0], mtxBox.Elements[2][1], mtxBox.Elements[2][2])
			* (vRealPoint.Z < 0.0f ? -1.0f : 1.0f);
	}

	collisionOut.Penetration = fDepthMin;
	collisionOut.Point = vPoint;
	return true;
}

//--------------------------------------------------------------------------------
//	関数名：checkLineWithLine
//  関数説明：直線と直線の交差判定(二次元)
//	引数：	vLA：LineLeftの始点
//			vLB：LineLeftの終点
//			vRA：LineRightの始点
//			vRB：LineRightの終点
//			vOut：交差点(出力用)
//	戻り値：bool
//--------------------------------------------------------------------------------
bool CollisionDetector::checkLineWithLine(const Vector2& vLA, const Vector2& vLB, const Vector2& vRA, const Vector2& vRB, Vector2& vOut)
{
	auto& vLineL = vLB - vLA;
	auto& vLineR = vRB - vRA;

	auto fSlopeL = (vLA.X - vLB.X) == 0.0f ? 0.0f
		: (vLA.Y - vLB.Y) / (vLA.X - vLB.X);
	auto fAddL = vLA.Y - fSlopeL * vLA.X;
	auto fSlopeR = (vRA.X - vRB.X) == 0.0f ? 0.0f
		: (vRA.Y - vRB.Y) / (vRA.X - vRB.X);
	auto fAddR = vRA.Y - fSlopeL * vRA.X;
	
	//平行
	if (fSlopeL == fSlopeR) { return false; }
	
	//交点の算出
	vOut.X = (fAddR - fAddL) / (fSlopeL - fSlopeR);
	vOut.Y = fSlopeL * vOut.X + fAddL;

	//交点がラインの範囲内にあるかをチェック
	//LineL
	auto vDirL = vOut - vLA;
	//方向チェック
	if (CKFMath::Vec2Dot(vLineL, vDirL) < 0.0f) { return false; }

	//長さチェック
	if (CKFMath::VecMagnitudeSquare(vDirL) > CKFMath::VecMagnitudeSquare(vLineL)) { return false; }

	//LineR
	auto vDirR = vOut - vRA;
	//方向チェック
	if (CKFMath::Vec2Dot(vLineR, vDirR) < 0.0f) { return false; }

	//長さチェック
	if (CKFMath::VecMagnitudeSquare(vDirR) > CKFMath::VecMagnitudeSquare(vLineR)) { return false; }

	return true;
}

//--------------------------------------------------------------------------------
//	関数名：checkLineWithLine
//  関数説明：直線と直線の交差判定(三次元)
//	引数：	vLA：LineLeftの始点
//			vLB：LineLeftの終点
//			vRA：LineRightの始点
//			vRB：LineRightの終点
//			vOut：交差点(出力用)
//	戻り値：bool
//--------------------------------------------------------------------------------
bool CollisionDetector::checkLineWithLine(const Vector3& vLA, const Vector3& vLB, const Vector3& vRA, const Vector3& vRB, Vector3& vOut)
{
	auto vLineL = vLA - vLB;
	auto vLineR = vRA - vRB;

	if ((vLineL * vLineR) == Vector3(0.0f))
	{//平行
		return false;
	}

	//XY平面の交点の算出
	Vector2 vOutXY;
	if (!checkLineWithLine(Vector2(vLA.X, vLA.Y), Vector2(vLA.X, vLA.Y), Vector2(vLA.X, vLA.Y), Vector2(vLA.X, vLA.Y), vOutXY))
	{
		return false;
	}

	CKFMath::VecNormalize(vLineL);
	CKFMath::VecNormalize(vLineR);

	//相応のZ値を算出する
	auto fRateL = vLineL.X != 0.0f ? (vOutXY.X - vLA.X) / vLineL.X
		: vLineL.Y != 0.0f ? (vOutXY.Y - vLA.Y) / vLineL.Y
		: 0.0f;
	auto fRateR = vLineR.X != 0.0f ? (vOutXY.X - vRA.X) / vLineR.X
		: vLineR.Y != 0.0f ? (vOutXY.Y - vRA.Y) / vLineR.Y
		: 0.0f;
	auto fZL = vLA.Z + fRateL * vLineL.Z;
	auto fZR = vRA.Z + fRateR * vLineR.Z;
	if (fZL != fZR) { return false; }

	vOut = Vector3(vOutXY.X, vOutXY.Y, fZL);
	return true;
}

//--------------------------------------------------------------------------------
//	関数名：transformBoxToAxis
//  関数説明：boxを与えられた軸に投影して長さを算出する関数
//	引数：	box：ボックスコライダー
//			vAxis：軸情報
//	戻り値：float
//--------------------------------------------------------------------------------
float CollisionDetector::transformBoxToAxis(const BoxCollider& box, const Vector3& vAxis)
{
	const Vector3& vHalfSize = box.GetHalfSize();
	const Matrix44& mtxBox = box.GetNextWorldMatrix();
	float fAnswer = vHalfSize.X * fabsf(CKFMath::Vec3Dot(vAxis, Vector3(mtxBox.Elements[0][0], mtxBox.Elements[0][1], mtxBox.Elements[0][2])))
		+ vHalfSize.Y * fabsf(CKFMath::Vec3Dot(vAxis, Vector3(mtxBox.Elements[1][0], mtxBox.Elements[1][1], mtxBox.Elements[1][2])))
		+ vHalfSize.Z * fabsf(CKFMath::Vec3Dot(vAxis, Vector3(mtxBox.Elements[2][0], mtxBox.Elements[2][1], mtxBox.Elements[2][2])));
	return fAnswer;
}

//--------------------------------------------------------------------------------
//	関数名：checkOverlapOnAxis
//  関数説明：ボックス同士が与えられた軸に重ねてるかどうかをチェックする
//	引数：	boxL：ボックスコライダー
//			boxR：ボックスコライダー
//			vAxis：軸情報
//	戻り値：bool
//--------------------------------------------------------------------------------
bool CollisionDetector::checkOverlapOnAxis(const BoxCollider& boxL, const BoxCollider& boxR, const Vector3& vAxis)
{
	//軸上の長さを算出する
	float fHaldistanceL = transformBoxToAxis(boxL, vAxis);
	float fHaldistanceR = transformBoxToAxis(boxR, vAxis);

	//軸上の中心間の距離を算出する
	Vector3 vLtoR = boxL.GetNextWorldPosition() - boxR.GetNextWorldPosition();
	float distance = fabsf(CKFMath::Vec3Dot(vLtoR, vAxis));

	//重ねてるかどうかをチェックする
	bool bAns = distance < (fHaldistanceL + fHaldistanceR);
	return bAns;
}

//--------------------------------------------------------------------------------
//	関数名：checkOverlapOnAxis
//  関数説明：AABB同士が与えられた軸に重ねてるかどうかをチェックする(二次元)
//	引数：	vMinL：AABB一番左上の点情報
//			vMaxL：AABB一番右下の点情報
//			vMinR：AABB一番左上の点情報
//			vMaxR：AABB一番右下の点情報
//	戻り値：bool
//--------------------------------------------------------------------------------
bool CollisionDetector::checkOverlapOnAxis(const Vector2& vMinL, const Vector2& vMaxL, const Vector2& vMinR, const Vector2& vMaxR)
{
	bool bAnswer = vMinL.X < vMaxR.X && vMinR.X < vMaxL.X
		&& vMinL.Y < vMaxR.Y && vMinR.Y < vMaxL.Y;
	return bAnswer;
}

//--------------------------------------------------------------------------------
//	関数名：checkOverlapOnAxis
//  関数説明：AABB同士がXYZ軸に重ねてるかどうかをチェックする(三次元)
//	引数：	aabbL：AABBコライダー
//			aabbR：AABBコライダー
//	戻り値：bool
//--------------------------------------------------------------------------------
bool CollisionDetector::checkOverlapAABB(const AABBCollider& aabbL, const AABBCollider& aabbR)
{
	const auto& PositionL = aabbL.GetNextWorldPosition();
	const auto& vHalfSizeL = aabbL.GetHalfSize();
	const auto& PositionR = aabbR.GetNextWorldPosition();
	const auto& vHalfSizeR = aabbR.GetHalfSize();
	auto& vMinL = PositionL - vHalfSizeL;
	auto& vMaxL = PositionL + vHalfSizeL;
	auto& vMinR = PositionR - vHalfSizeR;
	auto& vMaxR = PositionR + vHalfSizeR;

	//AABB同士がxyz軸上に重ねてるかどうかをチェックする
	bool bAnswer = checkOverlapOnAxis(Vector2(vMinL.Y, vMinL.Z), Vector2(vMaxL.Y, vMaxL.Z), Vector2(vMinR.Y, vMinR.Z), Vector2(vMaxR.Y, vMaxR.Z))	//X軸
		&& checkOverlapOnAxis(Vector2(vMinL.Z, vMinL.X), Vector2(vMaxL.Z, vMaxL.X), Vector2(vMinR.Z, vMinR.X), Vector2(vMaxR.Z, vMaxR.X))			//Y軸
		&& checkOverlapOnAxis(Vector2(vMinL.X, vMinL.Y), Vector2(vMaxL.X, vMaxL.Y), Vector2(vMinR.X, vMinR.Y), Vector2(vMaxR.X, vMaxR.Y));			//Z軸

	return bAnswer;
}
//--------------------------------------------------------------------------------
//	�R���W�����v�Z�֐�
//�@KF_CollisionUtility.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-13
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
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
#include "rigidbody3D.h"
#include "behavior.h"
#include "transform.h"

#ifdef _DEBUG
#include "debugObserver.h"
#endif

//--------------------------------------------------------------------------------
//
//  �Փ˔���֐�
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FDetect
//  �֐������F�X�t�B�A�ƃX�t�B�A�̓����蔻��֐�
//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
//	�����F	sphereL�F�X�t�B�A�R���C�_�[
//			sphereR�F�X�t�B�A�R���C�_�[
//	�߂�l�F�Ȃ�
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
	{//�g���K�[�������畨���������Ȃ�
		auto& leftBehaviors = sphereL.GetGameObject()->GetBehaviors();
		for (auto& behavior : leftBehaviors) behavior->OnTrigger(sphereL, sphereR);

		auto& rightBehaviors = sphereR.GetGameObject()->GetBehaviors();
		for (auto& behavior : rightBehaviors) behavior->OnTrigger(sphereR, sphereL);
		return;
	}

	auto collision = new Collision;

	//�Փ˓_�̎Z�o
	collision->Point = sphereRPosition + midLine * 0.5f;

	//�Փː[�x�̎Z�o
	float distance = sqrtf(squareDistance);
	collision->Penetration = minDistance - distance;

	//�Փ˖@���̎Z�o
	collision->Normal = midLine / distance;

	//���W�b�h�{�f�B�̎擾
	auto leftRigidbody = sphereL.GetGameObject()->GetRigidbodyComponent();
	auto rightRigidbody = sphereR.GetGameObject()->GetRigidbodyComponent();
	if (leftRigidbody->GetType() == Rigidbody::RB_3D)
	{
		collision->RigidbodyOne = dynamic_cast<Rigidbody3D*>(leftRigidbody);

		if (rightRigidbody->GetType() == Rigidbody::RB_3D)
		{
			collision->RigidbodyTwo = dynamic_cast<Rigidbody3D*>(rightRigidbody);
		}
	}
	else
	{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
		collision->Normal *= -1.0f;
		collision->RigidbodyOne = dynamic_cast<Rigidbody3D*>(rightRigidbody);
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	PhysicsSystem::Instance()->Register(collision);

	//OnCollision
	CollisionInfo info;
	info.This = &sphereL;
	info.Other = &sphereR;
	info.Collisions.push_back(collision);
	auto& leftBehaviors = sphereL.GetGameObject()->GetBehaviors();
	for (auto& behavior : leftBehaviors) { behavior->OnCollision(info); }

	info.This = &sphereR;
	info.Other = &sphereL;
	auto& rightBehaviors = sphereR.GetGameObject()->GetBehaviors();
	for (auto& behavior : rightBehaviors) { behavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	�֐����FDetect
//  �֐������F�X�t�B�A��AABB�̓����蔻��֐�
//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
//	�����F	sphere�F�X�t�B�A�R���C�_�[
//			aabb�F��]�Ȃ��{�b�N�X�R���C�_�[
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(SphereCollider& sphere, AABBCollider& aabb)
{
	const auto& spherePosition = sphere.GetNextWorldPosition();
	const auto& sphereRadius = sphere.GetRadius();
	const auto& aabbHalfSize = aabb.GetHalfSize();
	const auto& aabbPosition = aabb.GetNextWorldPosition();
	auto& realPosition = spherePosition - aabbPosition;

	//�������`�F�b�N
	if (fabsf(realPosition.X) - sphereRadius > aabbHalfSize.X
		|| fabsf(realPosition.Y) - sphereRadius > aabbHalfSize.Y
		|| fabsf(realPosition.Z) - sphereRadius > aabbHalfSize.Z)
	{
		return;
	}

	Vector3 closestPosition;
	float distance;

	//AABB�ƃX�t�B�A�̍ŋߓ_�̎Z�o
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

	//�Փˌ��m
	float squareDistance = (closestPosition - realPosition).SquareMagnitude();
	if (squareDistance >= sphereRadius * sphereRadius) return;

	//OnTrigger
	if (sphere.IsTrigger() || aabb.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		auto& sphereBehaviors = sphere.GetGameObject()->GetBehaviors();
		for (auto& behavior : sphereBehaviors) { behavior->OnTrigger(sphere, aabb); }

		auto& aabbBehaviors = aabb.GetGameObject()->GetBehaviors();
		for (auto& behavior : aabbBehaviors) { behavior->OnTrigger(aabb, sphere); }
		return;
	}

	//�Փˏ��
	closestPosition = closestPosition + aabbPosition;
	auto collision = new Collision;
	collision->Normal = spherePosition - closestPosition;
	if (collision->Normal.SquareMagnitude() == 0.0f)
	{//���S��aabb�̒��ɂ���
		auto transform = sphere.GetGameObject()->GetTransform();
		auto& currentPosition = transform->GetPos();
		auto& nextPosition = transform->GetPosNext();
		collision->Normal = currentPosition - nextPosition;
	}
	collision->Normal.Normalize();
	collision->Point = closestPosition;
	collision->Penetration = sphereRadius - sqrtf(squareDistance);

	//���W�b�h�{�f�B�̎擾
	auto sphereRigidbody = sphere.GetGameObject()->GetRigidbodyComponent();
	auto aabbRigidbody = aabb.GetGameObject()->GetRigidbodyComponent();
	if (sphereRigidbody->GetType() == Rigidbody::RB_3D)
	{
		collision->RigidbodyOne = dynamic_cast<Rigidbody3D*>(sphereRigidbody);

		if (aabbRigidbody->GetType() == Rigidbody::RB_3D)
		{
			collision->RigidbodyTwo = dynamic_cast<Rigidbody3D*>(aabbRigidbody);
		}
	}
	else
	{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
		collision->Normal *= -1.0f;
		collision->RigidbodyOne = dynamic_cast<Rigidbody3D*>(aabbRigidbody);
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	PhysicsSystem::Instance()->Register(collision);

	//OnCollision
	CollisionInfo info;
	info.This = &sphere;
	info.Other = &aabb;
	info.Collisions.push_back(collision);
	auto& sphereBehaviors = sphere.GetGameObject()->GetBehaviors();
	for (auto& behavior : sphereBehaviors) { behavior->OnCollision(info); }

	info.This = &aabb;
	info.Other = &sphere;
	auto& aabbBehaviors = aabb.GetGameObject()->GetBehaviors();
	for (auto& behavior : aabbBehaviors) { behavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	�֐����FDetect
//  �֐������F�X�t�B�A��OBB�̓����蔻��֐�
//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
//	�����F	sphere�F�X�t�B�A�R���C�_�[
//			obb�F��]����{�b�N�X�R���C�_�[
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(SphereCollider& sphere, OBBCollider& obb)
{
	const auto& spherePosition = sphere.GetNextWorldPosition();
	const auto& sphereRadius = sphere.GetRadius();
	const auto& obbHalfSize = obb.GetHalfSize();
	const auto& obbMatrix = obb.GetNextWorldMatrix();
	auto& realPosition = Vector3::TransformInverse(spherePosition, obbMatrix);

	//�������`�F�b�N
	if (fabsf(realPosition.X) - sphereRadius > obbHalfSize.X
		|| fabsf(realPosition.Y) - sphereRadius > obbHalfSize.Y
		|| fabsf(realPosition.Z) - sphereRadius > obbHalfSize.Z)
	{
		return;
	}

	Vector3 closestPosition;
	float distance;

	//OBB�ƃX�t�B�A�̍ŋߓ_�̎Z�o
	distance = realPosition.X;
	if (distance > obbHalfSize.X) { distance = obbHalfSize.X; }
	else if (distance < -obbHalfSize.X) { distance = -obbHalfSize.X; }
	closestPosition.X = distance;

	distance = realPosition.Y;
	if (distance > obbHalfSize.Y) { distance = obbHalfSize.Y; }
	else if (distance < -obbHalfSize.Y) { distance = -obbHalfSize.Y; }
	closestPosition.Y = distance;

	distance = realPosition.Z;
	if (distance > obbHalfSize.Z) { distance = obbHalfSize.Z; }
	else if (distance < -obbHalfSize.Z) { distance = -obbHalfSize.Z; }
	closestPosition.Z = distance;

	//�Փˌ��m
	float squareDistance = (closestPosition - realPosition).SquareMagnitude();
	if (squareDistance >= sphereRadius * sphereRadius) return;

	//OnTrigger
	if (sphere.IsTrigger() || obb.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		auto& sphereBehaviors = sphere.GetGameObject()->GetBehaviors();
		for (auto& behavior : sphereBehaviors) { behavior->OnTrigger(sphere, obb); }

		auto& obbBehaviors = obb.GetGameObject()->GetBehaviors();
		for (auto& behavior : obbBehaviors) { behavior->OnTrigger(obb, sphere); }
		return;
	}

	//�Փˏ��
	closestPosition = Vector3::TransformCoord(closestPosition, obbMatrix);
	auto collision = new Collision;
	collision->Normal = spherePosition - closestPosition;
	if (collision->Normal.SquareMagnitude() == 0.0f)
	{//���S��obb�̒��ɂ���
		auto transform = sphere.GetGameObject()->GetTransform();
		auto currentPosition = transform->GetPos();
		auto nextPosition = transform->GetPosNext();
		collision->Normal = currentPosition - nextPosition;
	}
	collision->Normal.Normalize();
	collision->Point = closestPosition;
	collision->Penetration = sphereRadius - sqrtf(squareDistance);

	//���W�b�h�{�f�B�̎擾
	auto sphereRigidbody = sphere.GetGameObject()->GetRigidbodyComponent();
	auto obbRigidbody = obb.GetGameObject()->GetRigidbodyComponent();
	if (sphereRigidbody->GetType() == Rigidbody::RB_3D)
	{
		collision->RigidbodyOne = dynamic_cast<Rigidbody3D*>(sphereRigidbody);

		if (obbRigidbody->GetType() == Rigidbody::RB_3D)
		{
			collision->RigidbodyTwo = dynamic_cast<Rigidbody3D*>(obbRigidbody);
		}
	}
	else
	{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
		collision->Normal *= -1.0f;
		collision->RigidbodyOne = dynamic_cast<Rigidbody3D*>(obbRigidbody);
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	PhysicsSystem::Instance()->Register(collision);

	//OnCollision
	CollisionInfo info;
	info.This = &sphere;
	info.Other = &obb;
	info.Collisions.push_back(collision);
	auto& sphereBehaviors = sphere.GetGameObject()->GetBehaviors();
	for (auto& behavior : sphereBehaviors) { behavior->OnCollision(info); }

	info.This = &obb;
	info.Other = &sphere;
	auto& obbBehaviors = obb.GetGameObject()->GetBehaviors();
	for (auto& behavior : obbBehaviors) { behavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	�֐����FDetect
//  �֐������FAABB��AABB�̓����蔻��֐�
//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
//	�����F	aabbL�F��]�Ȃ��{�b�N�X�R���C�_�[
//			aabbR�F��]�Ȃ��{�b�N�X�R���C�_�[
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(AABBCollider& aabbL, AABBCollider& aabbR)
{
	if (!isOverlap(aabbL, aabbR)) return;

	//OnTrigger
	if (aabbL.IsTrigger() || aabbR.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		auto& leftBehaviors = aabbL.GetGameObject()->GetBehaviors();
		for (auto& behavior : leftBehaviors) { behavior->OnTrigger(aabbL, aabbR); }

		auto& rightBehaviors = aabbR.GetGameObject()->GetBehaviors();
		for (auto& behavior : rightBehaviors) { behavior->OnTrigger(aabbR, aabbL); }
		return;
	}

	//XYZ����Ԑ[�x���󂢂̎���􂢏o��
	const auto& leftPosition = aabbL.GetNextWorldPosition();
	const auto& leftHalfSize = aabbL.GetHalfSize();
	const auto& rightPosition = aabbR.GetNextWorldPosition();
	const auto& rightHalfSize = aabbR.GetHalfSize();
	auto midLine = leftPosition - rightPosition;
	auto noCollisionDistance = leftHalfSize + rightHalfSize;
	auto penetrationX = noCollisionDistance.X - fabsf(midLine.X);
	auto penetrationY = noCollisionDistance.Y - fabsf(midLine.Y);
	auto penetrationZ = noCollisionDistance.Z - fabsf(midLine.Z);

	penetrationX = penetrationX > 0.0f ? penetrationX : noCollisionDistance.X;
	penetrationY = penetrationY > 0.0f ? penetrationY : noCollisionDistance.Y;
	penetrationZ = penetrationZ > 0.0f ? penetrationZ : noCollisionDistance.Z;
	auto penetrationMin = min(penetrationX, min(penetrationY, penetrationZ));
	
	auto collision = new Collision;
	collision->Penetration = penetrationMin;
	collision->Point = midLine * 0.5f;
	if (penetrationX == penetrationMin)
	{
		collision->Normal = midLine.X < 0.0f ? Vector3::Left : Vector3::Right;
	}
	else if (penetrationY == penetrationMin)
	{
		collision->Normal = midLine.Y < 0.0f ? Vector3::Down : Vector3::Up;
	}
	else
	{
		collision->Normal = midLine.Z < 0.0f ? Vector3::Back : Vector3::Forward;
	}

	//���W�b�h�{�f�B�̎擾
	auto leftRigidbody = aabbL.GetGameObject()->GetRigidbodyComponent();
	auto rightRigidbody = aabbR.GetGameObject()->GetRigidbodyComponent();

	if (leftRigidbody->GetType() == Rigidbody::RB_3D)
	{
		collision->RigidbodyOne = dynamic_cast<Rigidbody3D*>(leftRigidbody);

		if (rightRigidbody->GetType() == Rigidbody::RB_3D)
		{
			collision->RigidbodyTwo = dynamic_cast<Rigidbody3D*>(rightRigidbody);
		}
	}
	else
	{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
		collision->Normal *= -1.0f;
		collision->RigidbodyOne = dynamic_cast<Rigidbody3D*>(rightRigidbody);
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	PhysicsSystem::Instance()->Register(collision);

	//OnCollision
	CollisionInfo info;
	info.Collisions.push_back(collision);
	info.This = &aabbL;
	info.Other = &aabbR;
	auto& leftBehaviors = aabbL.GetGameObject()->GetBehaviors();
	for (auto& behavior : leftBehaviors) { behavior->OnCollision(info); }

	info.This = &aabbR;
	info.Other = &aabbL;
	auto& rightBehaviors = aabbR.GetGameObject()->GetBehaviors();
	for (auto& behavior : rightBehaviors) { behavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	�֐����FDetect
//  �֐������Fbox��box�̓����蔻��֐�(�����ꂩ���K��OBB)
//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
//	�����F	boxL�F�{�b�N�X�R���C�_�[
//			boxR�F�{�b�N�X�R���C�_�[
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(BoxCollider& boxL, BoxCollider& boxR)
{
	//�{�b�N�X��xyz����ɏd�˂Ă邩�ǂ������`�F�b�N����
	if (!isOverlapOnAxis(boxL, boxR, Vector3::AxisX)
		&& !isOverlapOnAxis(boxL, boxR, Vector3::AxisY)
		&& !isOverlapOnAxis(boxL, boxR, Vector3::AxisZ))
	{
		return;
	}

	//boxL�̂��ׂĂ̒��_��boxR�Ɣ��肵�A�߂荞�݂���Ԑ[���̒��_��􂢏o��
	Collision* boxLMaxPenetrationCollision = nullptr;
	auto& leftVertexes = boxL.GetNextWorldVertexes();
	for (auto& vertex : leftVertexes)
	{
		auto collision = detect(vertex, boxR);
		boxLMaxPenetrationCollision = maxPenetration(boxLMaxPenetrationCollision, collision);
	}

	//boxR�̂��ׂĂ̒��_��boxL�Ɣ��肵�A�߂荞�݂���Ԑ[���̒��_��􂢏o��
	Collision* boxRMaxPenetrationCollision = nullptr;
	auto& rightVertexes = boxR.GetNextWorldVertexes();
	for (auto& vertex : rightVertexes)
	{
		auto collision = detect(vertex, boxR);
		boxRMaxPenetrationCollision = maxPenetration(boxRMaxPenetrationCollision, collision);
	}

	if (!boxLMaxPenetrationCollision && !boxRMaxPenetrationCollision) return;

	//OnTrigger
	if (boxL.IsTrigger() || boxR.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		auto& leftBehaviors = boxL.GetGameObject()->GetBehaviors();
		for (auto& behavior : leftBehaviors) behavior->OnTrigger(boxL, boxR);
		auto& rightBehaviors = boxR.GetGameObject()->GetBehaviors();
		for (auto& behavior : rightBehaviors) behavior->OnTrigger(boxR, boxL);
		if (boxLMaxPenetrationCollision) delete boxLMaxPenetrationCollision;
		if (boxRMaxPenetrationCollision) delete boxRMaxPenetrationCollision;
		return;
	}

	CollisionInfo info;

	//���W�b�h�{�f�B�̎擾
	auto leftRigidbody = boxL.GetGameObject()->GetRigidbodyComponent();
	auto rightRigidbody = boxR.GetGameObject()->GetRigidbodyComponent();

	if (boxLMaxPenetrationCollision)
	{
		if (leftRigidbody->GetType() == Rigidbody::RB_3D)
		{
			boxLMaxPenetrationCollision->RigidbodyOne = dynamic_cast<Rigidbody3D*>(leftRigidbody);

			if (rightRigidbody->GetType() == Rigidbody::RB_3D)
			{
				boxLMaxPenetrationCollision->RigidbodyTwo = dynamic_cast<Rigidbody3D*>(rightRigidbody);
			}
		}
		else
		{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
			boxLMaxPenetrationCollision->Normal *= -1.0f;
			boxLMaxPenetrationCollision->RigidbodyOne = dynamic_cast<Rigidbody3D*>(rightRigidbody);
		}

		//�������Z�V�X�e���Ƀ��W�X�g��
		PhysicsSystem::Instance()->Register(boxLMaxPenetrationCollision);
		info.Collisions.push_back(boxLMaxPenetrationCollision);
	}

	if (boxRMaxPenetrationCollision)
	{
		if (rightRigidbody->GetType() == Rigidbody::RB_3D)
		{
			boxRMaxPenetrationCollision->RigidbodyOne = dynamic_cast<Rigidbody3D*>(rightRigidbody);

			if (leftRigidbody->GetType() == Rigidbody::RB_3D)
			{
				boxRMaxPenetrationCollision->RigidbodyTwo = dynamic_cast<Rigidbody3D*>(leftRigidbody);
			}
		}
		else
		{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
			boxRMaxPenetrationCollision->Normal *= -1.0f;
			boxRMaxPenetrationCollision->RigidbodyOne = dynamic_cast<Rigidbody3D*>(leftRigidbody);
		}

		//�������Z�V�X�e���Ƀ��W�X�g��
		PhysicsSystem::Instance()->Register(boxRMaxPenetrationCollision);
		info.Collisions.push_back(boxRMaxPenetrationCollision);
	}

	//OnCollision
	info.This = &boxL;
	info.Other = &boxR;
	auto& leftBehaviors = boxL.GetGameObject()->GetBehaviors();
	for (auto& behavior : leftBehaviors) behavior->OnCollision(info);

	info.This = &boxR;
	info.Other = &boxL;
	auto& rightBehaviors = boxR.GetGameObject()->GetBehaviors();
	for (auto& behavior : rightBehaviors) behavior->OnCollision(info);
}

//--------------------------------------------------------------------------------
//
//  �t�B�[���h
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FDetect
//  �֐������F�X�t�B�A�ƃt�B�[���h�̓����蔻��֐�
//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
//	�����F	sphere�F�X�t�B�A�R���C�_�[
//			field�F�t�B�[���h�R���C�_�[
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(SphereCollider& sphere, FieldCollider& field)
{
	const auto& spherePosition = sphere.GetNextWorldPosition();
	const auto& radius = sphere.GetRadius();
	auto collision = detect(spherePosition, field);
	if (!collision) return;

	float penetration = collision->Penetration + radius;
	if (penetration <= 0.0f)
	{
		delete collision;
		return;
	}

	if (sphere.IsTrigger())
	{
		auto& sphereBehaviors = sphere.GetGameObject()->GetBehaviors();
		for (auto& behavior : sphereBehaviors) behavior->OnTrigger(sphere, field);
		auto& fieldBehaviors = field.GetGameObject()->GetBehaviors();
		for (auto& behavior : fieldBehaviors) behavior->OnTrigger(field, sphere);
		delete collision;
		return;
	}

	collision->Point = spherePosition + collision->Normal * penetration;
	collision->Penetration = penetration;
	collision->RigidbodyOne = dynamic_cast<Rigidbody3D*>(sphere.GetGameObject()->GetRigidbodyComponent());
	collision->RigidbodyTwo = nullptr;

	//�������Z�V�X�e���Ƀ��W�X�g��
	PhysicsSystem::Instance()->Register(collision);

	//OnCollision
	CollisionInfo info;
	info.This = &sphere;
	info.Other = &field;
	info.Collisions.push_back(collision);
	auto& sphereBehaviors = sphere.GetGameObject()->GetBehaviors();
	for (auto& behavior : sphereBehaviors) behavior->OnCollision(info);

	info.This = &field;
	info.Other = &sphere;
	auto& fieldBehaviors = field.GetGameObject()->GetBehaviors();
	for (auto& behavior : fieldBehaviors) behavior->OnCollision(info);
}

//--------------------------------------------------------------------------------
//	�֐����FDetect
//  �֐������FBox�ƃt�B�[���h�̓����蔻��֐�
//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
//	�����F	box�F�{�b�N�X�R���C�_�[
//			field�F�t�B�[���h�R���C�_�[
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(BoxCollider& box, FieldCollider& field)
{
	//��Ԑ[���̒��_���ďo����
	auto& vertexes = box.GetNextWorldVertexes();
	Collision* maxPenetrationCollision = nullptr;
	for (auto& vertex : vertexes)
	{
		auto collision = detect(vertex, field);
		maxPenetrationCollision = maxPenetration(maxPenetrationCollision, collision);
	}

	if (!maxPenetrationCollision) return;

	if (box.IsTrigger())
	{
		auto& boxBehaviors = box.GetGameObject()->GetBehaviors();
		for (auto& behavior : boxBehaviors) { behavior->OnTrigger(box, field); }
		auto& fieldBehaviors = field.GetGameObject()->GetBehaviors();
		for (auto& behavior : fieldBehaviors) { behavior->OnTrigger(field, box); }
		delete maxPenetrationCollision;
		return;
	}

	//���W�b�h�{�f�B�̎擾
	maxPenetrationCollision->RigidbodyOne = dynamic_cast<Rigidbody3D*>(box.GetGameObject()->GetRigidbodyComponent());
	maxPenetrationCollision->RigidbodyTwo = nullptr;

	//�������Z�V�X�e���Ƀ��W�X�g��
	PhysicsSystem::Instance()->Register(maxPenetrationCollision);

	//OnCollision
	CollisionInfo info;
	info.This = &box;
	info.Other = &field;
	info.Collisions.push_back(maxPenetrationCollision);
	auto& boxBehaviors = box.GetGameObject()->GetBehaviors();
	for (auto& behavior : boxBehaviors) { behavior->OnCollision(info); }

	info.This = &field;
	info.Other = &box;
	auto& fieldBehaviors = field.GetGameObject()->GetBehaviors();
	for (auto& behavior : fieldBehaviors) { behavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//
//  ���C
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FDetect
//  �֐������F���C�ƃ{�b�N�X�̓����蔻��
//	�����F	ray�F���C
//			distancetance�F���C�̒���
//			box�F�{�b�N�X�R���C�_�[
//	�߂�l�FRayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionDetector::Detect(const Ray& ray, const float& distance, BoxCollider& box)
{
	auto collision = detect(ray.Origin, box);
	if (collision)
	{
		auto result = new RayHitInfo;
		result->Normal = collision->Normal;
		result->Position = ray.Origin;
		result->Collider = &box;
		result->Distance = collision->Penetration;
		delete collision;
		return result;
	}
	
	auto& rayEnd = ray.Origin + ray.Direction * distance;
	collision = detect(rayEnd, box);
	if (collision)
	{
		auto result = new RayHitInfo;
		result->Normal = collision->Normal;
		result->Position = rayEnd;
		result->Collider = &box;
		result->Distance = collision->Penetration;
		delete collision;
		return result;
	}

	return nullptr;
}

//--------------------------------------------------------------------------------
//	�֐����FDetect
//  �֐������F���C�ƃX�t�B�A�̓����蔻��
//	�����F	ray�F���C
//			distancetance�F���C�̒���
//			sphere�F�X�t�B�A�R���C�_�[
//	�߂�l�FRayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionDetector::Detect(const Ray& ray, const float& distance, SphereCollider& sphere)
{
	const auto& spherePosition = sphere.GetNextWorldPosition();
	const auto& radius = sphere.GetRadius();

	auto& sphereToOrigin = ray.Origin - spherePosition;
	float workA = 2.0f * ray.Direction.Dot(sphereToOrigin);
	float workB = sphereToOrigin.Dot(sphereToOrigin) - radius * radius;
	
	float dicriminant = workA * workA - 4.0f * workB;
	if (dicriminant < 0.0f) return nullptr;
	dicriminant = sqrtf(dicriminant);

	float timeA = (-workA + dicriminant) / 2.0f;
	float timeB = (-workA - dicriminant) / 2.0f;
	if (timeA < 0.0f && timeB < 0.0f) return nullptr;
	
	//�ŒZ���Ԃ�ۑ�
	timeA = timeA < 0.0f ? timeB : timeA;
	timeB = timeB < 0.0f ? timeA : timeB;
	auto minTime = timeA <= timeB ? timeA : timeB;

	if (minTime > distance) { return nullptr; }

	auto result = new RayHitInfo;
	result->Distance = minTime;
	result->Collider = &sphere;
	result->Position = ray.Origin + ray.Direction * minTime;
	result->Normal = (result->Position - spherePosition).Normalized();
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FDetect
//  �֐������F���C�ƃt�B�[���h�̓����蔻��
//	�����F	ray�F���C
//			distancetance�F���C�̒���
//			field�F�t�B�[���h�R���C�_�[
//	�߂�l�FRayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionDetector::Detect(const Ray& ray, const float& distance, FieldCollider& field)
{
	//auto beginRyMax = ray.Origin + ray.Direction * distancetance;
	//int nNumVtxX = 0;
	//int nNumVtxZ = 0;
	//vector<Vector3> vecVtx;
	//if (!field.GetVtxByRange(ray.Origin, beginRyMax, nNumVtxX, nNumVtxZ, vecVtx))
	//{
	//	return false;
	//}

	//�ꎞ�̗p
	auto collision = detect(ray.Origin + ray.Direction * distance, field);
	if (!collision) return nullptr;
	if (collision->Penetration < 0.0f) 
	{
		delete collision;
		return nullptr;
	}
	auto result = new RayHitInfo;
	result->Distance = collision->Penetration;
	result->Normal = collision->Normal;
	result->Collider = &field;
	result->Position = ray.Origin + ray.Direction * distance;
	delete collision;
	return result;
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����Fdetect
//  �֐������F�_��AABB�̓����蔻��
//	�����F	point�F�_�̈ʒu
//			aabb�FAABB�R���C�_�[
//	�߂�l�FCollision*
//--------------------------------------------------------------------------------
Collision* CollisionDetector::detect(const Vector3& point, const AABBCollider& aabb)
{
	const auto& aabbPosition = aabb.GetNextWorldPosition();
	const auto& halfSize = aabb.GetHalfSize();
	auto& realPoint = point - aabbPosition;

	float minDepth = halfSize.X - fabsf(realPoint.X);
	if (minDepth <= 0.0f) return nullptr;
	auto normal = realPoint.X < 0.0f ? Vector3::Left : Vector3::Right;

	float depth = halfSize.Y - fabsf(realPoint.Y);
	if (depth <= 0.0f) return nullptr;
	else if (depth < minDepth)
	{
		minDepth = depth;
		normal = realPoint.Y < 0.0f ? Vector3::Down : Vector3::Up;
	}

	depth = halfSize.Z - fabsf(realPoint.Z);
	if (depth <= 0.0f) return nullptr;
	else if (depth < minDepth)
	{
		minDepth = depth;
		normal = realPoint.Z < 0.0f ? Vector3::Back : Vector3::Forward;
	}

	auto result = new Collision;
	result->Normal = normal;
	result->Penetration = minDepth;
	result->Point = point;
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����Fdetect
//  �֐������F�_��Box�̓����蔻��
//	�����F	collisionOut�F�Փˏ��(�o�͗p)
//			vPoint�F�_�̈ʒu
//			box�Fbox�R���C�_�[
//	�߂�l�FCollision*
//--------------------------------------------------------------------------------
Collision* CollisionDetector::detect(const Vector3& point, const BoxCollider& box)
{
	const auto& boxMatrix = box.GetNextWorldMatrix();
	const auto& halfSize = box.GetHalfSize();
	auto& realPoint = Vector3::TransformInverse(point, boxMatrix);

	float minDepth = halfSize.X - fabsf(realPoint.X);
	if (minDepth <= 0.0f) return nullptr;
	auto normal = Vector3(boxMatrix.Elements[0][0], boxMatrix.Elements[0][1], boxMatrix.Elements[0][2])
		* (realPoint.X < 0.0f ? -1.0f : 1.0f);

	float depth = halfSize.Y - fabsf(realPoint.Y);
	if (depth <= 0.0f) return nullptr;
	else if (depth < minDepth)
	{
		minDepth = depth;
		normal = Vector3(boxMatrix.Elements[1][0], boxMatrix.Elements[1][1], boxMatrix.Elements[1][2])
			* (realPoint.Y < 0.0f ? -1.0f : 1.0f);
	}

	depth = halfSize.Z - fabsf(realPoint.Z);
	if (depth <= 0.0f) return nullptr;
	else if (depth < minDepth)
	{
		minDepth = depth;
		normal = Vector3(boxMatrix.Elements[2][0], boxMatrix.Elements[2][1], boxMatrix.Elements[2][2])
			* (realPoint.Z < 0.0f ? -1.0f : 1.0f);
	}

	auto result = new Collision;
	result->Normal = normal;
	result->Penetration = minDepth;
	result->Point = point;
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����Fdetect
//  �֐������F�_��field�̓����蔻��
//	�����F	point�F�_�̈ʒu
//			field�Ffield�R���C�_�[
//	�߂�l�FCollision*
//--------------------------------------------------------------------------------
Collision* CollisionDetector::detect(const Vector3& point, const FieldCollider& field)
{
	auto polygonInfo = field.GetPolygonAt(point);
	if (!polygonInfo) return nullptr;
	Collision* result = nullptr;

	// �n�ʖ@���̊p�x��60�x�ȏ�Ȃ�n�ʖ@����Ԃ�
	// ��������Ȃ��Ȃ�������Ԃ�
	float dot = Vector3::Up.Dot(polygonInfo->SurfaceNormal);
	if (dot > 0.5f)
	{
		float pointYOnField = polygonInfo->Side.Y - ((point.X - polygonInfo->Side.X) * polygonInfo->SurfaceNormal.X + (point.Z - polygonInfo->Side.Z) * polygonInfo->SurfaceNormal.Z) / polygonInfo->SurfaceNormal.Y;
		float penetration = pointYOnField - point.Y;
		if (penetration > 0.0f)
		{
			result = new Collision;
			result->Point = point;
			result->Penetration = penetration;
			result->Normal = Vector3::Up;
		}
		delete polygonInfo;
		return result;
	}

	// �n�ʂ̓��e�ʒu�̎Z�o
	auto& center = (polygonInfo->LeftUp + polygonInfo->RightDown) * 0.5f;
	auto& right = (polygonInfo->RightDown - polygonInfo->LeftUp).Normalized();
	auto& forward = (right * polygonInfo->SurfaceNormal).Normalized();
	auto& transform = Matrix44::Transform(right, polygonInfo->SurfaceNormal, forward, center);
	auto& realPosition = Vector3::TransformInverse(point, transform);
	
	if (realPosition.Y < 0.0f)
	{// �o���Ȃ����ߖ@����������Ɛ��������ɂ���
		result = new Collision;
		result->Point = point;
		result->Normal = (Vector3::Up * polygonInfo->SurfaceNormal * Vector3::Up).Normalized();
		result->Penetration = -realPosition.Y;
	}
	delete polygonInfo;
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����Fdetect
//  �֐������F�����ƒ����̌�������(�񎟌�)
//	�����F	beginL�FLineLeft�̎n�_
//			endL�FLineLeft�̏I�_
//			beginR�FLineRight�̎n�_
//			endR�FLineRight�̏I�_
//	�߂�l�FVector2*
//--------------------------------------------------------------------------------
Vector2* CollisionDetector::detect(const Vector2& beginL, const Vector2& endL, const Vector2& beginR, const Vector2& endR)
{
	auto& lineL = endL - beginL;
	auto& lineR = endR - beginR;

	//���F Y = slope * X + add
	auto slopeL = (beginL.X - endL.X) == 0.0f ? 0.0f
		: (beginL.Y - endL.Y) / (beginL.X - endL.X);
	auto addL = beginL.Y - slopeL * beginL.X;
	auto slopeR = (beginR.X - endR.X) == 0.0f ? 0.0f
		: (beginR.Y - endR.Y) / (beginR.X - endR.X);
	auto addR = beginR.Y - slopeL * beginR.X;

	//���s
	if (slopeL == slopeR) return nullptr;

	//��_�̎Z�o
	auto result = new Vector2;
	result->X = (addR - addL) / (slopeL - slopeR);
	result->Y = slopeL * result->X + addL;

	//��_�����C���͈͓̔��ɂ��邩���`�F�b�N
	//LineL
	auto beginLToResult = *result - beginL;
	if (lineL.Dot(beginLToResult) < 0.0f // �����`�F�b�N
		|| beginLToResult.SquareMagnitude() > lineL.SquareMagnitude()) //�����`�F�b�N
	{
		delete result;
		return nullptr;
	}

	//LineR
	auto beginRToResult = *result - beginR;
	if (lineR.Dot(beginRToResult) < 0.0f // �����`�F�b�N
		|| beginRToResult.SquareMagnitude() > lineR.SquareMagnitude()) //�����`�F�b�N
	{
		delete result;
		return nullptr;
	}
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����Fdetect
//  �֐������F�����ƒ����̌�������(�O����)
//	�����F	beginL�FLineLeft�̎n�_
//			endL�FLineLeft�̏I�_
//			beginR�FLineRight�̎n�_
//			endR�FLineRight�̏I�_
//	�߂�l�FVector3*
//--------------------------------------------------------------------------------
Vector3* CollisionDetector::detect(const Vector3& beginL, const Vector3& endL, const Vector3& beginR, const Vector3& endR)
{
	auto& lineL = beginL - endL;
	auto& lineR = beginR - endR;

	//���s�`�F�b�N
	if ((lineL * lineR) == Vector3::Zero) return nullptr;

	//XY���ʂ̌�_�̎Z�o
	auto pointXY = detect(Vector2(beginL.X, beginL.Y), Vector2(beginL.X, beginL.Y), Vector2(beginL.X, beginL.Y), Vector2(beginL.X, beginL.Y));
	if (!pointXY) return nullptr;

	//������Z�l���Z�o����
	lineL.Normalize();
	lineR.Normalize();
	auto rateL = lineL.X != 0.0f ? (pointXY->X - beginL.X) / lineL.X
		: lineL.Y != 0.0f ? (pointXY->Y - beginL.Y) / lineL.Y
		: 0.0f;
	auto rateR = lineR.X != 0.0f ? (pointXY->X - beginR.X) / lineR.X
		: lineR.Y != 0.0f ? (pointXY->Y - beginR.Y) / lineR.Y
		: 0.0f;
	auto zL = beginL.Z + rateL * lineL.Z;
	auto zR = beginR.Z + rateR * lineR.Z;

	if (zL != zR)
	{
		delete pointXY;
		return nullptr;
	}

	auto result = new Vector3(pointXY->X, pointXY->Y, zL);
	delete pointXY;
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FprojectBoxToAxis
//  �֐������Fbox��^����ꂽ���ɓ��e���Ē������Z�o����֐�
//	�����F	box�F�{�b�N�X�R���C�_�[
//			axis�F�����
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float CollisionDetector::projectBoxToAxis(const BoxCollider& box, const Vector3& axis)
{
	const Vector3& halfSize = box.GetHalfSize();
	const Matrix44& boxMatrix = box.GetNextWorldMatrix();
	return halfSize.X * fabsf(axis.Dot(Vector3(boxMatrix.Elements[0][0], boxMatrix.Elements[0][1], boxMatrix.Elements[0][2])))
		+ halfSize.Y * fabsf(axis.Dot(Vector3(boxMatrix.Elements[1][0], boxMatrix.Elements[1][1], boxMatrix.Elements[1][2])))
		+ halfSize.Z * fabsf(axis.Dot(Vector3(boxMatrix.Elements[2][0], boxMatrix.Elements[2][1], boxMatrix.Elements[2][2])));
}

//--------------------------------------------------------------------------------
//	�֐����FisOverlapOnAxis
//  �֐������F�{�b�N�X���m���^����ꂽ���ɏd�˂Ă邩�ǂ������`�F�b�N����
//	�����F	boxL�F�{�b�N�X�R���C�_�[
//			boxR�F�{�b�N�X�R���C�_�[
//			axis�F�����
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool CollisionDetector::isOverlapOnAxis(const BoxCollider& boxL, const BoxCollider& boxR, const Vector3& axis)
{
	//����̒������Z�o����
	float lengthOnAxisL = projectBoxToAxis(boxL, axis);
	float lengthOnAxisR = projectBoxToAxis(boxR, axis);

	//����̒��S�Ԃ̋������Z�o����
	auto& boxRToBoxL = boxL.GetNextWorldPosition() - boxR.GetNextWorldPosition();
	float distance = fabsf(boxRToBoxL.Dot(axis));

	//�d�˂Ă邩�ǂ������`�F�b�N����
	return distance < (lengthOnAxisL + lengthOnAxisR);
}

//--------------------------------------------------------------------------------
//	�֐����FcheckOverlapOnAxis
//  �֐������FAABB���m��XYZ���ɏd�˂Ă邩�ǂ������`�F�b�N����(�O����)
//	�����F	aabbL�FAABB�R���C�_�[
//			aabbR�FAABB�R���C�_�[
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool CollisionDetector::isOverlap(const AABBCollider& aabbL, const AABBCollider& aabbR)
{
	const auto& aabbLPosition = aabbL.GetNextWorldPosition();
	const auto& aabbLHalfSize = aabbL.GetHalfSize();
	const auto& aabbRPosition = aabbR.GetNextWorldPosition();
	const auto& aabbRHalfSize = aabbR.GetHalfSize();
	auto& minL = aabbLPosition - aabbLHalfSize;
	auto& maxL = aabbLPosition + aabbLHalfSize;
	auto& minR = aabbRPosition - aabbRHalfSize;
	auto& maxR = aabbRPosition + aabbRHalfSize;

	//AABB���m��xyz����ɏd�˂Ă邩�ǂ������`�F�b�N����
	return isOverlap(Vector2(minL.Y, minL.Z), Vector2(maxL.Y, maxL.Z), Vector2(minR.Y, minR.Z), Vector2(maxR.Y, maxR.Z))	//X��
		&& isOverlap(Vector2(minL.Z, minL.X), Vector2(maxL.Z, maxL.X), Vector2(minR.Z, minR.X), Vector2(maxR.Z, maxR.X))	//Y��
		&& isOverlap(Vector2(minL.X, minL.Y), Vector2(maxL.X, maxL.Y), Vector2(minR.X, minR.Y), Vector2(maxR.X, maxR.Y));	//Z��;
}

//--------------------------------------------------------------------------------
//	�֐����FisOverlap
//  �֐������FAABB���m���^����ꂽ���ɏd�˂Ă邩�ǂ������`�F�b�N����(�񎟌�)
//	�����F	minL�FaabbL��ԍ���̓_���
//			maxL�FaabbL��ԉE���̓_���
//			minR�FaabbR��ԍ���̓_���
//			maxR�FaabbR��ԉE���̓_���
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool CollisionDetector::isOverlap(const Vector2& minL, const Vector2& maxL, const Vector2& minR, const Vector2& maxR)
{
	return minL.X < maxR.X && minR.X < maxL.X
		&& minL.Y < maxR.Y && minR.Y < maxL.Y;
}

//--------------------------------------------------------------------------------
//	�֐����FmaxPenetration
//  �֐������F�Փː[�x����Ԑ[���̃R���W������Ԃ��A�c��͔̂j������
//	�����F	current�F����Ԑ[���R���W����
//			next�F�V�����R���W����
//	�߂�l�FCollision*
//--------------------------------------------------------------------------------
Collision* CollisionDetector::maxPenetration(Collision* current, Collision* next)
{
	if (!next) return current;
	if (!current) return next;
	if (next->Penetration > current->Penetration)
	{
		delete current;
		return next;
	}
	delete next;
	return current;
}
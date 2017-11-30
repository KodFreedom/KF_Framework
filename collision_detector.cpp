//--------------------------------------------------------------------------------
//	�R���W�����T�m��
//�@collisionDetector.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "collision_detector.h"
#include "main_system.h"
#include "physics_system.h"
#include "sphere_collider.h"
#include "field_collider.h"
#include "aabb_collider.h"
#include "obb_collider.h"
#include "game_object.h"
#include "rigidbody3d.h"
#include "behavior.h"
#include "transform.h"

#ifdef _DEBUG
#include "debug_observer.h"
#endif

//--------------------------------------------------------------------------------
//
//  �Փ˔���֐�
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�X�t�B�A�ƃX�t�B�A�̓����蔻��֐�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(SphereCollider& sphere_left, SphereCollider& sphere_right)
{
	const auto& sphere_left_position = sphere_left.GetWorldPosition();
	const auto& sphere_left_radius = sphere_left.GetRadius();
	const auto& sphere_right_position = sphere_right.GetWorldPosition();
	const auto& sphere_right_radius = sphere_right.GetRadius();
	auto& midline = sphere_left_position - sphere_right_position;
	float min_distance = sphere_left_radius + sphere_right_radius;
	float square_distance = midline.SquareMagnitude();
	if (square_distance >= min_distance * min_distance) return;

	if (sphere_left.IsTrigger() || sphere_right.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		for (auto& pair : sphere_left.GetGameObject().GetBehaviors()) pair.second->OnTrigger(sphere_left, sphere_right);
		for (auto& pair : sphere_right.GetGameObject().GetBehaviors()) pair.second->OnTrigger(sphere_right, sphere_left);
		return;
	}

	auto collision = new Collision;

	//�Փ˓_�̎Z�o
	collision->point = sphere_right_position + midline * 0.5f;

	//�Փː[�x�̎Z�o
	float distance = sqrtf(square_distance);
	collision->penetration = min_distance - distance;

	//�Փ˖@���̎Z�o
	collision->normal = midline / distance;

	//���W�b�h�{�f�B�̎擾
	auto left_rigidbody = sphere_left.GetGameObject().GetRigidbody();
	auto right_rigidbody = sphere_right.GetGameObject().GetRigidbody();
	if (left_rigidbody->GetType() == Rigidbody::kRigidbody3D)
	{
		collision->rigidbody_one = static_cast<Rigidbody3D*>(left_rigidbody);

		if (right_rigidbody->GetType() == Rigidbody::kRigidbody3D)
		{
			collision->rigidbody_two = static_cast<Rigidbody3D*>(right_rigidbody);
		}
	}
	else
	{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
		collision->normal *= -1.0f;
		collision->rigidbody_one = static_cast<Rigidbody3D*>(right_rigidbody);
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	MainSystem::Instance()->GetPhysicsSystem()->Register(collision);

	//OnCollision
	CollisionInfo info;
	info.self = &sphere_left;
	info.other = &sphere_right;
	info.collisions.push_back(collision);
	for (auto& pair : sphere_left.GetGameObject().GetBehaviors()) { pair.second->OnCollision(info); }

	info.self = &sphere_right;
	info.other = &sphere_left;
	for (auto& pair : sphere_right.GetGameObject().GetBehaviors()) { pair.second->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	�X�t�B�A��AABB�̓����蔻��֐�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(SphereCollider& sphere, AabbCollider& aabb)
{
	const auto& sphere_position = sphere.GetWorldPosition();
	const auto& sphere_radius = sphere.GetRadius();
	const auto& aabb_half_size = aabb.GetHalfSize();
	const auto& aabb_position = aabb.GetWorldPosition();
	auto& real_position = sphere_position - aabb_position;

	//�������`�F�b�N
	if (fabsf(real_position.x_) - sphere_radius > aabb_half_size.x_
		|| fabsf(real_position.y_) - sphere_radius > aabb_half_size.y_
		|| fabsf(real_position.z_) - sphere_radius > aabb_half_size.z_)
	{
		return;
	}

	Vector3 closest_position;
	float distance;

	//AABB�ƃX�t�B�A�̍ŋߓ_�̎Z�o
	distance = real_position.x_;
	if (distance > aabb_half_size.x_) { distance = aabb_half_size.x_; }
	else if (distance < -aabb_half_size.x_) { distance = -aabb_half_size.x_; }
	closest_position.x_ = distance;

	distance = real_position.y_;
	if (distance > aabb_half_size.y_) { distance = aabb_half_size.y_; }
	else if (distance < -aabb_half_size.y_) { distance = -aabb_half_size.y_; }
	closest_position.y_ = distance;

	distance = real_position.z_;
	if (distance > aabb_half_size.z_) { distance = aabb_half_size.z_; }
	else if (distance < -aabb_half_size.z_) { distance = -aabb_half_size.z_; }
	closest_position.z_ = distance;

	//�Փˌ��m
	float square_distance = (closest_position - real_position).SquareMagnitude();
	if (square_distance >= sphere_radius * sphere_radius) return;

	//OnTrigger
	if (sphere.IsTrigger() || aabb.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		for (auto& pair : sphere.GetGameObject().GetBehaviors()) { pair.second->OnTrigger(sphere, aabb); }
		for (auto& pair : aabb.GetGameObject().GetBehaviors()) { pair.second->OnTrigger(aabb, sphere); }
		return;
	}

	//�Փˏ��
	closest_position = closest_position + aabb_position;
	auto collision = new Collision;
	collision->normal = sphere_position - closest_position;
	if (collision->normal.SquareMagnitude() == 0.0f)
	{//���S��aabb�̒��ɂ���
		auto sphere_rigidbody = sphere.GetGameObject().GetRigidbody();
		if (sphere_rigidbody->GetType() == Rigidbody::kRigidbody3D)
		{
			auto rigidbody = static_cast<Rigidbody3D*>(sphere_rigidbody);
			collision->normal = rigidbody->GetAcceleration() * -1.0f;
		}
	}
	collision->normal.Normalize();
	collision->point = closest_position;
	collision->penetration = sphere_radius - sqrtf(square_distance);

	//���W�b�h�{�f�B�̎擾
	auto sphere_rigidbody = sphere.GetGameObject().GetRigidbody();
	auto aabb_rigidbody = aabb.GetGameObject().GetRigidbody();
	if (sphere_rigidbody->GetType() == Rigidbody::kRigidbody3D)
	{
		collision->rigidbody_one = static_cast<Rigidbody3D*>(sphere_rigidbody);

		if (aabb_rigidbody->GetType() == Rigidbody::kRigidbody3D)
		{
			collision->rigidbody_two = static_cast<Rigidbody3D*>(aabb_rigidbody);
		}
	}
	else
	{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
		collision->normal *= -1.0f;
		collision->rigidbody_one = static_cast<Rigidbody3D*>(aabb_rigidbody);
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	MainSystem::Instance()->GetPhysicsSystem()->Register(collision);

	//OnCollision
	CollisionInfo info;
	info.self = &sphere;
	info.other = &aabb;
	info.collisions.push_back(collision);
	for (auto& pair : sphere.GetGameObject().GetBehaviors()) { pair.second->OnCollision(info); }

	info.self = &aabb;
	info.other = &sphere;
	for (auto& pair : aabb.GetGameObject().GetBehaviors()) { pair.second->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	�X�t�B�A��OBB�̓����蔻��֐�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(SphereCollider& sphere, ObbCollider& obb)
{
	const auto& sphere_position = sphere.GetWorldPosition();
	const auto& sphere_radius = sphere.GetRadius();
	const auto& obb_half_size = obb.GetHalfSize();
	const auto& obb_matrix = obb.GetWorldMatrix();
	auto& real_position = Vector3::TransformInverse(sphere_position, obb_matrix);

	//�������`�F�b�N
	if (fabsf(real_position.x_) - sphere_radius > obb_half_size.x_
		|| fabsf(real_position.y_) - sphere_radius > obb_half_size.y_
		|| fabsf(real_position.z_) - sphere_radius > obb_half_size.z_)
	{
		return;
	}

	Vector3 closest_position;
	float distance;

	//OBB�ƃX�t�B�A�̍ŋߓ_�̎Z�o
	distance = real_position.x_;
	if (distance > obb_half_size.x_) { distance = obb_half_size.x_; }
	else if (distance < -obb_half_size.x_) { distance = -obb_half_size.x_; }
	closest_position.x_ = distance;

	distance = real_position.y_;
	if (distance > obb_half_size.y_) { distance = obb_half_size.y_; }
	else if (distance < -obb_half_size.y_) { distance = -obb_half_size.y_; }
	closest_position.y_ = distance;

	distance = real_position.z_;
	if (distance > obb_half_size.z_) { distance = obb_half_size.z_; }
	else if (distance < -obb_half_size.z_) { distance = -obb_half_size.z_; }
	closest_position.z_ = distance;

	//�Փˌ��m
	float square_distance = (closest_position - real_position).SquareMagnitude();
	if (square_distance >= sphere_radius * sphere_radius) return;

	//OnTrigger
	if (sphere.IsTrigger() || obb.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		for (auto& pair : sphere.GetGameObject().GetBehaviors()) { pair.second->OnTrigger(sphere, obb); }
		for (auto& pair : obb.GetGameObject().GetBehaviors()) { pair.second->OnTrigger(obb, sphere); }
		return;
	}

	//�Փˏ��
	closest_position = Vector3::TransformCoord(closest_position, obb_matrix);
	auto collision = new Collision;
	collision->normal = sphere_position - closest_position;
	if (collision->normal.SquareMagnitude() == 0.0f)
	{//���S��obb�̒��ɂ���
		auto sphere_rigidbody = sphere.GetGameObject().GetRigidbody();
		if (sphere_rigidbody->GetType() == Rigidbody::kRigidbody3D)
		{
			auto rigidbody = static_cast<Rigidbody3D*>(sphere_rigidbody);
			collision->normal = rigidbody->GetAcceleration() * -1.0f;
		}
	}
	collision->normal.Normalize();
	collision->point = closest_position;
	collision->penetration = sphere_radius - sqrtf(square_distance);

	//���W�b�h�{�f�B�̎擾
	auto sphere_rigidbody = sphere.GetGameObject().GetRigidbody();
	auto obbRigidbody = obb.GetGameObject().GetRigidbody();
	if (sphere_rigidbody->GetType() == Rigidbody::kRigidbody3D)
	{
		collision->rigidbody_one = static_cast<Rigidbody3D*>(sphere_rigidbody);

		if (obbRigidbody->GetType() == Rigidbody::kRigidbody3D)
		{
			collision->rigidbody_two = static_cast<Rigidbody3D*>(obbRigidbody);
		}
	}
	else
	{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
		collision->normal *= -1.0f;
		collision->rigidbody_one = static_cast<Rigidbody3D*>(obbRigidbody);
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	MainSystem::Instance()->GetPhysicsSystem()->Register(collision);

	//OnCollision
	CollisionInfo info;
	info.self = &sphere;
	info.other = &obb;
	info.collisions.push_back(collision);
	for (auto& pair : sphere.GetGameObject().GetBehaviors()) { pair.second->OnCollision(info); }

	info.self = &obb;
	info.other = &sphere;
	for (auto& pair : obb.GetGameObject().GetBehaviors()) { pair.second->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	AABB��AABB�̓����蔻��֐�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(AabbCollider& aabb_left, AabbCollider& aabb_right)
{
	if (!IsOverlap(aabb_left, aabb_right)) return;

	//OnTrigger
	if (aabb_left.IsTrigger() || aabb_right.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		for (auto& pair : aabb_left.GetGameObject().GetBehaviors()) { pair.second->OnTrigger(aabb_left, aabb_right); }
		for (auto& pair : aabb_right.GetGameObject().GetBehaviors()) { pair.second->OnTrigger(aabb_right, aabb_left); }
		return;
	}

	//XYZ����Ԑ[�x���󂢂̎���􂢏o��
	const auto& left_position = aabb_left.GetWorldPosition();
	const auto& left_half_size = aabb_left.GetHalfSize();
	const auto& right_position = aabb_right.GetWorldPosition();
	const auto& right_half_size = aabb_right.GetHalfSize();
	auto midline = left_position - right_position;
	auto no_collision_distance = left_half_size + right_half_size;
	auto penetration_x = no_collision_distance.x_ - fabsf(midline.x_);
	auto penetration_y = no_collision_distance.y_ - fabsf(midline.y_);
	auto penetration_z = no_collision_distance.z_ - fabsf(midline.z_);

	penetration_x = penetration_x > 0.0f ? penetration_x : no_collision_distance.x_;
	penetration_y = penetration_y > 0.0f ? penetration_y : no_collision_distance.y_;
	penetration_z = penetration_z > 0.0f ? penetration_z : no_collision_distance.z_;
	auto penetrationMin = min(penetration_x, min(penetration_y, penetration_z));
	
	auto collision = new Collision;
	collision->penetration = penetrationMin;
	collision->point = midline * 0.5f;
	if (penetration_x == penetrationMin)
	{
		collision->normal = midline.x_ < 0.0f ? Vector3::kLeft : Vector3::kRight;
	}
	else if (penetration_y == penetrationMin)
	{
		collision->normal = midline.y_ < 0.0f ? Vector3::kDown : Vector3::kUp;
	}
	else
	{
		collision->normal = midline.z_ < 0.0f ? Vector3::kBack : Vector3::kForward;
	}

	//���W�b�h�{�f�B�̎擾
	auto left_rigidbody = aabb_left.GetGameObject().GetRigidbody();
	auto right_rigidbody = aabb_right.GetGameObject().GetRigidbody();

	if (left_rigidbody->GetType() == Rigidbody::kRigidbody3D)
	{
		collision->rigidbody_one = static_cast<Rigidbody3D*>(left_rigidbody);

		if (right_rigidbody->GetType() == Rigidbody::kRigidbody3D)
		{
			collision->rigidbody_two = static_cast<Rigidbody3D*>(right_rigidbody);
		}
	}
	else
	{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
		collision->normal *= -1.0f;
		collision->rigidbody_one = static_cast<Rigidbody3D*>(right_rigidbody);
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	MainSystem::Instance()->GetPhysicsSystem()->Register(collision);

	//OnCollision
	CollisionInfo info;
	info.collisions.push_back(collision);
	info.self = &aabb_left;
	info.other = &aabb_right;
	for (auto& pair : aabb_left.GetGameObject().GetBehaviors()) { pair.second->OnCollision(info); }

	info.self = &aabb_right;
	info.other = &aabb_left;
	for (auto& pair : aabb_right.GetGameObject().GetBehaviors()) { pair.second->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	box��box�̓����蔻��֐�(�����ꂩ���K��OBB)
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(BoxCollider& box_left, BoxCollider& box_right)
{
	//�{�b�N�X��xyz����ɏd�˂Ă邩�ǂ������`�F�b�N����
	if (!IsOverlapOnAxis(box_left, box_right, Vector3::kAxisX)
		&& !IsOverlapOnAxis(box_left, box_right, Vector3::kAxisY)
		&& !IsOverlapOnAxis(box_left, box_right, Vector3::kAxisZ))
	{
		return;
	}

	//box_left�̂��ׂĂ̒��_��box_right�Ɣ��肵�A�߂荞�݂���Ԑ[���̒��_��􂢏o��
	Collision* left_max_penetration_collision = nullptr;
	for (auto& vertex : box_left.GetWorldVertexes())
	{
		auto collision = Detect(vertex, box_right);
		left_max_penetration_collision = MaxPenetration(left_max_penetration_collision, collision);
	}

	//box_right�̂��ׂĂ̒��_��box_left�Ɣ��肵�A�߂荞�݂���Ԑ[���̒��_��􂢏o��
	Collision* right_max_penetration_collision = nullptr;
	for (auto& vertex : box_right.GetWorldVertexes())
	{
		auto collision = Detect(vertex, box_right);
		right_max_penetration_collision = MaxPenetration(right_max_penetration_collision, collision);
	}

	if (!left_max_penetration_collision && !right_max_penetration_collision) return;

	//OnTrigger
	if (box_left.IsTrigger() || box_right.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		for (auto& pair : box_left.GetGameObject().GetBehaviors()) pair.second->OnTrigger(box_left, box_right);
		for (auto& pair : box_right.GetGameObject().GetBehaviors()) pair.second->OnTrigger(box_right, box_left);
		if (left_max_penetration_collision) delete left_max_penetration_collision;
		if (right_max_penetration_collision) delete right_max_penetration_collision;
		return;
	}

	CollisionInfo info;

	//���W�b�h�{�f�B�̎擾
	auto left_rigidbody = box_left.GetGameObject().GetRigidbody();
	auto right_rigidbody = box_right.GetGameObject().GetRigidbody();

	if (left_max_penetration_collision)
	{
		if (left_rigidbody->GetType() == Rigidbody::kRigidbody3D)
		{
			left_max_penetration_collision->rigidbody_one = static_cast<Rigidbody3D*>(left_rigidbody);

			if (right_rigidbody->GetType() == Rigidbody::kRigidbody3D)
			{
				left_max_penetration_collision->rigidbody_two = static_cast<Rigidbody3D*>(right_rigidbody);
			}
		}
		else
		{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
			left_max_penetration_collision->normal *= -1.0f;
			left_max_penetration_collision->rigidbody_one = static_cast<Rigidbody3D*>(right_rigidbody);
		}

		//�������Z�V�X�e���Ƀ��W�X�g��
		MainSystem::Instance()->GetPhysicsSystem()->Register(left_max_penetration_collision);
		info.collisions.push_back(left_max_penetration_collision);
	}

	if (right_max_penetration_collision)
	{
		if (right_rigidbody->GetType() == Rigidbody::kRigidbody3D)
		{
			right_max_penetration_collision->rigidbody_one = static_cast<Rigidbody3D*>(right_rigidbody);

			if (left_rigidbody->GetType() == Rigidbody::kRigidbody3D)
			{
				right_max_penetration_collision->rigidbody_two = static_cast<Rigidbody3D*>(left_rigidbody);
			}
		}
		else
		{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
			right_max_penetration_collision->normal *= -1.0f;
			right_max_penetration_collision->rigidbody_one = static_cast<Rigidbody3D*>(left_rigidbody);
		}

		//�������Z�V�X�e���Ƀ��W�X�g��
		MainSystem::Instance()->GetPhysicsSystem()->Register(right_max_penetration_collision);
		info.collisions.push_back(right_max_penetration_collision);
	}

	//OnCollision
	info.self = &box_left;
	info.other = &box_right;
	for (auto& pair : box_left.GetGameObject().GetBehaviors()) pair.second->OnCollision(info);

	info.self = &box_right;
	info.other = &box_left;
	for (auto& pair : box_right.GetGameObject().GetBehaviors()) pair.second->OnCollision(info);
}

//--------------------------------------------------------------------------------
//
//  �t�B�[���h
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�X�t�B�A�ƃt�B�[���h�̓����蔻��֐�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(SphereCollider& sphere, FieldCollider& field)
{
	const auto& sphere_position = sphere.GetWorldPosition();
	const auto& radius = sphere.GetRadius();
	auto collision = Detect(sphere_position, field);
	if (!collision) return;

	float penetration = collision->penetration + radius;
	if (penetration <= 0.0f)
	{
		delete collision;
		return;
	}

	if (sphere.IsTrigger())
	{
		for (auto& pair : sphere.GetGameObject().GetBehaviors()) pair.second->OnTrigger(sphere, field);
		for (auto& pair : field.GetGameObject().GetBehaviors()) pair.second->OnTrigger(field, sphere);
		delete collision;
		return;
	}

	collision->point = sphere_position + collision->normal * penetration;
	collision->penetration = penetration;
	collision->rigidbody_one = static_cast<Rigidbody3D*>(sphere.GetGameObject().GetRigidbody());
	collision->rigidbody_two = nullptr;

	//�������Z�V�X�e���Ƀ��W�X�g��
	MainSystem::Instance()->GetPhysicsSystem()->Register(collision);

	//OnCollision
	CollisionInfo info;
	info.self = &sphere;
	info.other = &field;
	info.collisions.push_back(collision);
	for (auto& pair : sphere.GetGameObject().GetBehaviors()) pair.second->OnCollision(info);

	info.self = &field;
	info.other = &sphere;
	for (auto& pair : field.GetGameObject().GetBehaviors()) pair.second->OnCollision(info);
}

//--------------------------------------------------------------------------------
//	Box�ƃt�B�[���h�̓����蔻��֐�
//--------------------------------------------------------------------------------
void CollisionDetector::Detect(BoxCollider& box, FieldCollider& field)
{
	//��Ԑ[���̒��_���ďo����
	Collision* max_penetration_collision = nullptr;
	for (auto& vertex : box.GetWorldVertexes())
	{
		auto collision = Detect(vertex, field);
		max_penetration_collision = MaxPenetration(max_penetration_collision, collision);
	}

	if (!max_penetration_collision) return;

	if (box.IsTrigger())
	{
		for (auto& pair : box.GetGameObject().GetBehaviors()) { pair.second->OnTrigger(box, field); }
		for (auto& pair : field.GetGameObject().GetBehaviors()) { pair.second->OnTrigger(field, box); }
		delete max_penetration_collision;
		return;
	}

	//���W�b�h�{�f�B�̎擾
	max_penetration_collision->rigidbody_one = static_cast<Rigidbody3D*>(box.GetGameObject().GetRigidbody());
	max_penetration_collision->rigidbody_two = nullptr;

	//�������Z�V�X�e���Ƀ��W�X�g��
	MainSystem::Instance()->GetPhysicsSystem()->Register(max_penetration_collision);

	//OnCollision
	CollisionInfo info;
	info.self = &box;
	info.other = &field;
	info.collisions.push_back(max_penetration_collision);
	for (auto& pair : box.GetGameObject().GetBehaviors()) { pair.second->OnCollision(info); }

	info.self = &field;
	info.other = &box;
	for (auto& pair : field.GetGameObject().GetBehaviors()) { pair.second->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//
//  ���C
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	���C�ƃ{�b�N�X�̓����蔻��
//--------------------------------------------------------------------------------
RayHitInfo* CollisionDetector::Detect(const Ray& ray, const float& distance, BoxCollider& box)
{
	auto collision = Detect(ray.origin_, box);
	if (collision)
	{
		auto result = new RayHitInfo;
		result->normal = collision->normal;
		result->position = ray.origin_;
		result->other = &box;
		result->distance = collision->penetration;
		delete collision;
		return result;
	}
	
	auto& ray_end = ray.origin_ + ray.direction_ * distance;
	collision = Detect(ray_end, box);
	if (collision)
	{
		auto result = new RayHitInfo;
		result->normal = collision->normal;
		result->position = ray_end;
		result->other = &box;
		result->distance = collision->penetration;
		delete collision;
		return result;
	}

	return nullptr;
}

//--------------------------------------------------------------------------------
//	���C�ƃX�t�B�A�̓����蔻��
//--------------------------------------------------------------------------------
RayHitInfo* CollisionDetector::Detect(const Ray& ray, const float& distance, SphereCollider& sphere)
{
	const auto& sphere_position = sphere.GetWorldPosition();
	const auto& radius = sphere.GetRadius();

	auto& sphereToorigin_ = ray.origin_ - sphere_position;
	float work_a = 2.0f * ray.direction_.Dot(sphereToorigin_);
	float work_b = sphereToorigin_.Dot(sphereToorigin_) - radius * radius;
	
	float dicriminant = work_a * work_a - 4.0f * work_b;
	if (dicriminant < 0.0f) return nullptr;
	dicriminant = sqrtf(dicriminant);

	float time_a = (-work_a + dicriminant) / 2.0f;
	float time_b = (-work_a - dicriminant) / 2.0f;
	if (time_a < 0.0f && time_b < 0.0f) return nullptr;
	
	//�ŒZ���Ԃ�ۑ�
	time_a = time_a < 0.0f ? time_b : time_a;
	time_b = time_b < 0.0f ? time_a : time_b;
	auto min_time = time_a <= time_b ? time_a : time_b;

	if (min_time > distance) { return nullptr; }

	auto result = new RayHitInfo;
	result->distance = min_time;
	result->other = &sphere;
	result->position = ray.origin_ + ray.direction_ * min_time;
	result->normal = (result->position - sphere_position).Normalized();
	return result;
}

//--------------------------------------------------------------------------------
//	���C�ƃt�B�[���h�̓����蔻��
//--------------------------------------------------------------------------------
RayHitInfo* CollisionDetector::Detect(const Ray& ray, const float& distance, FieldCollider& field)
{
	//auto begin_rightyMax = ray.origin_ + ray.direction_ * distancetance;
	//int nNumVtxX = 0;
	//int nNumVtxZ = 0;
	//vector<Vector3> vecVtx;
	//if (!field.GetVtxByRange(ray.origin_, begin_rightyMax, nNumVtxX, nNumVtxZ, vecVtx))
	//{
	//	return false;
	//}

	//�ꎞ�̗p
	auto collision = Detect(ray.origin_ + ray.direction_ * distance, field);
	if (!collision) return nullptr;
	if (collision->penetration < 0.0f) 
	{
		delete collision;
		return nullptr;
	}
	auto result = new RayHitInfo;
	result->distance = collision->penetration;
	result->normal = collision->normal;
	result->other = &field;
	result->position = ray.origin_ + ray.direction_ * distance;
	delete collision;
	return result;
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�_��AABB�̓����蔻��
//--------------------------------------------------------------------------------
Collision* CollisionDetector::Detect(const Vector3& point, const AabbCollider& aabb)
{
	const auto& aabb_position = aabb.GetWorldPosition();
	const auto& half_size = aabb.GetHalfSize();
	auto& real_point = point - aabb_position;

	float min_depth = half_size.x_ - fabsf(real_point.x_);
	if (min_depth <= 0.0f) return nullptr;
	auto normal = real_point.x_ < 0.0f ? Vector3::kLeft : Vector3::kRight;

	float depth = half_size.y_ - fabsf(real_point.y_);
	if (depth <= 0.0f) return nullptr;
	else if (depth < min_depth)
	{
		min_depth = depth;
		normal = real_point.y_ < 0.0f ? Vector3::kDown : Vector3::kUp;
	}

	depth = half_size.z_ - fabsf(real_point.z_);
	if (depth <= 0.0f) return nullptr;
	else if (depth < min_depth)
	{
		min_depth = depth;
		normal = real_point.z_ < 0.0f ? Vector3::kBack : Vector3::kForward;
	}

	auto result = new Collision;
	result->normal = normal;
	result->penetration = min_depth;
	result->point = point;
	return result;
}

//--------------------------------------------------------------------------------
//	�_��Box�̓����蔻��
//--------------------------------------------------------------------------------
Collision* CollisionDetector::Detect(const Vector3& point, const BoxCollider& box)
{
	const auto& boxMatrix = box.GetWorldMatrix();
	const auto& half_size = box.GetHalfSize();
	auto& real_point = Vector3::TransformInverse(point, boxMatrix);

	float min_depth = half_size.x_ - fabsf(real_point.x_);
	if (min_depth <= 0.0f) return nullptr;
	auto normal = Vector3(boxMatrix.m00_, boxMatrix.m01_, boxMatrix.m02_)
		* (real_point.x_ < 0.0f ? -1.0f : 1.0f);

	float depth = half_size.y_ - fabsf(real_point.y_);
	if (depth <= 0.0f) return nullptr;
	else if (depth < min_depth)
	{
		min_depth = depth;
		normal = Vector3(boxMatrix.m10_, boxMatrix.m11_, boxMatrix.m12_)
			* (real_point.y_ < 0.0f ? -1.0f : 1.0f);
	}

	depth = half_size.z_ - fabsf(real_point.z_);
	if (depth <= 0.0f) return nullptr;
	else if (depth < min_depth)
	{
		min_depth = depth;
		normal = Vector3(boxMatrix.m20_, boxMatrix.m21_, boxMatrix.m22_)
			* (real_point.z_ < 0.0f ? -1.0f : 1.0f);
	}

	auto result = new Collision;
	result->normal = normal;
	result->penetration = min_depth;
	result->point = point;
	return result;
}

//--------------------------------------------------------------------------------
//	�_��field�̓����蔻��
//--------------------------------------------------------------------------------
Collision* CollisionDetector::Detect(const Vector3& point, const FieldCollider& field)
{
	auto polygon_info = field.GetPolygonAt(point);
	if (!polygon_info) return nullptr;
	Collision* result = nullptr;

	// �n�ʖ@���̊p�x��60�x�ȏ�Ȃ�n�ʖ@����Ԃ�
	// ��������Ȃ��Ȃ�������Ԃ�
	float dot = Vector3::kUp.Dot(polygon_info->normal);
	if (dot > 0.5f)
	{
		float point_y_on_field = polygon_info->side.y_ - ((point.x_ - polygon_info->side.x_) * polygon_info->normal.x_ + (point.z_ - polygon_info->side.z_) * polygon_info->normal.z_) / polygon_info->normal.y_;
		float penetration = point_y_on_field - point.y_;
		if (penetration > 0.0f)
		{
			result = new Collision;
			result->point = point;
			result->penetration = penetration;
			result->normal = Vector3::kUp;
		}
		delete polygon_info;
		return result;
	}

	// �n�ʂ̓��e�ʒu�̎Z�o
	auto& center = (polygon_info->left_up + polygon_info->right_down) * 0.5f;
	auto& right = (polygon_info->right_down - polygon_info->left_up).Normalized();
	auto& forward = (right * polygon_info->normal).Normalized();
	auto& transform = Matrix44::Transform(right, polygon_info->normal, forward, center);
	auto& real_position = Vector3::TransformInverse(point, transform);
	
	if (real_position.y_ < 0.0f)
	{// �o���Ȃ����ߖ@����������Ɛ��������ɂ���
		result = new Collision;
		result->point = point;
		result->normal = (Vector3::kUp * polygon_info->normal * Vector3::kUp).Normalized();
		result->penetration = -real_position.y_;
	}
	delete polygon_info;
	return result;
}

//--------------------------------------------------------------------------------
//	�����ƒ����̌�������(�񎟌�)
//--------------------------------------------------------------------------------
Vector2* CollisionDetector::Detect(const Vector2& begin_left, const Vector2& end_left, const Vector2& begin_right, const Vector2& end_right)
{
	auto& line_left = end_left - begin_left;
	auto& line_right = end_right - begin_right;

	//���F Y = slope * X + add
	auto slope_left = (begin_left.x_ - end_left.x_) == 0.0f ? 0.0f
		: (begin_left.y_ - end_left.y_) / (begin_left.x_ - end_left.x_);
	auto add_left = begin_left.y_ - slope_left * begin_left.x_;
	auto slope_right = (begin_right.x_ - end_right.x_) == 0.0f ? 0.0f
		: (begin_right.y_ - end_right.y_) / (begin_right.x_ - end_right.x_);
	auto add_right = begin_right.y_ - slope_left * begin_right.x_;

	//���s
	if (slope_left == slope_right) return nullptr;

	//��_�̎Z�o
	auto result = new Vector2;
	result->x_ = (add_right - add_left) / (slope_left - slope_right);
	result->y_ = slope_left * result->x_ + add_left;

	//��_�����C���͈͓̔��ɂ��邩���`�F�b�N
	//line_left
	auto begin_left_to_result = *result - begin_left;
	if (line_left.Dot(begin_left_to_result) < 0.0f // �����`�F�b�N
		|| begin_left_to_result.SquareMagnitude() > line_left.SquareMagnitude()) //�����`�F�b�N
	{
		delete result;
		return nullptr;
	}

	//line_right
	auto begin_right_to_result = *result - begin_right;
	if (line_right.Dot(begin_right_to_result) < 0.0f // �����`�F�b�N
		|| begin_right_to_result.SquareMagnitude() > line_right.SquareMagnitude()) //�����`�F�b�N
	{
		delete result;
		return nullptr;
	}
	return result;
}

//--------------------------------------------------------------------------------
//	�����ƒ����̌�������(�O����)
//--------------------------------------------------------------------------------
Vector3* CollisionDetector::Detect(const Vector3& begin_left, const Vector3& end_left, const Vector3& begin_right, const Vector3& end_right)
{
	auto& line_left = begin_left - end_left;
	auto& line_right = begin_right - end_right;

	//���s�`�F�b�N
	if ((line_left * line_right) == Vector3::kZero) return nullptr;

	//XY���ʂ̌�_�̎Z�o
	auto point_on_xy = Detect(Vector2(begin_left.x_, begin_left.y_), Vector2(begin_left.x_, begin_left.y_), Vector2(begin_left.x_, begin_left.y_), Vector2(begin_left.x_, begin_left.y_));
	if (!point_on_xy) return nullptr;

	//������Z�l���Z�o����
	line_left.Normalize();
	line_right.Normalize();
	auto rate_left = line_left.x_ != 0.0f ? (point_on_xy->x_ - begin_left.x_) / line_left.x_
		: line_left.y_ != 0.0f ? (point_on_xy->y_ - begin_left.y_) / line_left.y_
		: 0.0f;
	auto rate_right = line_right.x_ != 0.0f ? (point_on_xy->x_ - begin_right.x_) / line_right.x_
		: line_right.y_ != 0.0f ? (point_on_xy->y_ - begin_right.y_) / line_right.y_
		: 0.0f;
	auto z_left = begin_left.z_ + rate_left * line_left.z_;
	auto z_right = begin_right.z_ + rate_right * line_right.z_;

	if (z_left != z_right)
	{
		delete point_on_xy;
		return nullptr;
	}

	auto result = new Vector3(point_on_xy->x_, point_on_xy->y_, z_left);
	delete point_on_xy;
	return result;
}

//--------------------------------------------------------------------------------
//	box��^����ꂽ���ɓ��e���Ē������Z�o����֐�
//--------------------------------------------------------------------------------
float CollisionDetector::ProjectBoxToAxis(const BoxCollider& box, const Vector3& axis)
{
	const Vector3& half_size = box.GetHalfSize();
	const Matrix44& box_matrix = box.GetWorldMatrix();
	return half_size.x_ * fabsf(axis.Dot(Vector3(box_matrix.m00_, box_matrix.m01_, box_matrix.m02_)))
		+ half_size.y_ * fabsf(axis.Dot(Vector3(box_matrix.m10_, box_matrix.m11_, box_matrix.m12_)))
		+ half_size.z_ * fabsf(axis.Dot(Vector3(box_matrix.m20_, box_matrix.m21_, box_matrix.m22_)));
}

//--------------------------------------------------------------------------------
//	�{�b�N�X���m���^����ꂽ���ɏd�˂Ă邩�ǂ������`�F�b�N����
//--------------------------------------------------------------------------------
bool CollisionDetector::IsOverlapOnAxis(const BoxCollider& box_left, const BoxCollider& box_right, const Vector3& axis)
{
	//����̒������Z�o����
	float length_on_axis_left = ProjectBoxToAxis(box_left, axis);
	float length_on_axis_right = ProjectBoxToAxis(box_right, axis);

	//����̒��S�Ԃ̋������Z�o����
	auto& box_right_to_box_left = box_left.GetWorldPosition() - box_right.GetWorldPosition();
	float distance = fabsf(box_right_to_box_left.Dot(axis));

	//�d�˂Ă邩�ǂ������`�F�b�N����
	return distance < (length_on_axis_left + length_on_axis_right);
}

//--------------------------------------------------------------------------------
//	AABB���m��XYZ���ɏd�˂Ă邩�ǂ������`�F�b�N����(�O����)
//--------------------------------------------------------------------------------
bool CollisionDetector::IsOverlap(const AabbCollider& aabb_left, const AabbCollider& aabb_right)
{
	const auto& aabb_left_position = aabb_left.GetWorldPosition();
	const auto& aabb_left_half_size = aabb_left.GetHalfSize();
	const auto& aabb_right_position = aabb_right.GetWorldPosition();
	const auto& aabb_right_half_size = aabb_right.GetHalfSize();
	auto& min_left = aabb_left_position - aabb_left_half_size;
	auto& max_left = aabb_left_position + aabb_left_half_size;
	auto& min_right = aabb_right_position - aabb_right_half_size;
	auto& max_right = aabb_right_position + aabb_right_half_size;

	//AABB���m��xyz����ɏd�˂Ă邩�ǂ������`�F�b�N����
	return IsOverlap(Vector2(min_left.y_, min_left.z_), Vector2(max_left.y_, max_left.z_), Vector2(min_right.y_, min_right.z_), Vector2(max_right.y_, max_right.z_))	//X��
		&& IsOverlap(Vector2(min_left.z_, min_left.x_), Vector2(max_left.z_, max_left.x_), Vector2(min_right.z_, min_right.x_), Vector2(max_right.z_, max_right.x_))	//Y��
		&& IsOverlap(Vector2(min_left.x_, min_left.y_), Vector2(max_left.x_, max_left.y_), Vector2(min_right.x_, min_right.y_), Vector2(max_right.x_, max_right.y_));	//Z��;
}

//--------------------------------------------------------------------------------
//	AABB���m���^����ꂽ���ɏd�˂Ă邩�ǂ������`�F�b�N����(�񎟌�)
//--------------------------------------------------------------------------------
bool CollisionDetector::IsOverlap(const Vector2& min_left, const Vector2& max_left, const Vector2& min_right, const Vector2& max_right)
{
	return min_left.x_ < max_right.x_ && min_right.x_ < max_left.x_
		&& min_left.y_ < max_right.y_ && min_right.y_ < max_left.y_;
}

//--------------------------------------------------------------------------------
//	�Փː[�x����Ԑ[���̃R���W������Ԃ��A�c��͔̂j������
//--------------------------------------------------------------------------------
Collision* CollisionDetector::MaxPenetration(Collision* current, Collision* next)
{
	if (!next) return current;
	if (!current) return next;
	if (next->penetration > current->penetration)
	{
		delete current;
		return next;
	}
	delete next;
	return current;
}
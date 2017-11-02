//--------------------------------------------------------------------------------
//	�R���W�����T�m��
//�@collisionDetector.h
//	Author : Xu Wenjie
//	Date   : 2017-08-13
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class C3DRigidbodyComponent;
class Collider;
class BoxCollider;
class SphereCollider;
class AABBCollider;
class OBBCollider;
class FieldCollider;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���W����
//--------------------------------------------------------------------------------
class Collision
{
public:
	Collision()
		: RigidbodyOne(nullptr)
		, RigidbodyTwo(nullptr)
		, Normal(Vector3::Zero)
		, Point(Vector3::Zero)
		, Penetration(0.0f)
	{}
	~Collision() {}

	C3DRigidbodyComponent*	RigidbodyOne;
	C3DRigidbodyComponent*	RigidbodyTwo;
	Vector3					Normal;
	Vector3					Point;
	float					Penetration;
};

//--------------------------------------------------------------------------------
//  �R���W�������
//--------------------------------------------------------------------------------
class CollisionInfo
{
public:
	CollisionInfo()
		: This(nullptr)
		, Other(nullptr)
	{
		Collisions.clear();
	}
	~CollisionInfo()
	{
		Collisions.clear();
	}

	Collider*			This;
	Collider*			Other;
	list<Collision*>	Collisions;
};

//--------------------------------------------------------------------------------
//  ���C�Փˏ��(�ŋߓ_)
//--------------------------------------------------------------------------------
class RayHitInfo
{
public:
	RayHitInfo()
		: Collider(nullptr)
		, Distance(0.0f)
		, Normal(Vector3::Zero)
		, Position(Vector3::Zero)
	{}
	~RayHitInfo() {}

	Collider*		Collider;
	float			Distance;
	Vector3			Normal;
	Vector3			Position;
};

//--------------------------------------------------------------------------------
//  �R���W�����v�Z�N���X
//--------------------------------------------------------------------------------
class CollisionDetector
{
public:
	//�Փ˔���֐�
	static void Detect(SphereCollider& sphereL, SphereCollider& sphereR);
	static void Detect(SphereCollider& sphere, AABBCollider& aabb);
	static void Detect(SphereCollider& sphere, OBBCollider& obb);
	static void Detect(AABBCollider& aabbL, AABBCollider& aabbR);
	static void Detect(BoxCollider& boxL, BoxCollider& boxR);  
	static void Detect(BoxCollider& box, FieldCollider& field);
	static void Detect(SphereCollider& sphere, FieldCollider& field);
	
	//Ray
	static RayHitInfo* Detect(const Ray& ray, const float& distance, BoxCollider& box);
	static RayHitInfo* Detect(const Ray& ray, const float& distance, SphereCollider& sphere);
	static RayHitInfo* Detect(const Ray& ray, const float& distance, FieldCollider& field);

private:
	CollisionDetector() {}
	~CollisionDetector() {}

	//�v�Z�p�֐�
	static bool		checkPointWithAABB(Collision& collisionOut, const Vector3 vPoint, const AABBCollider& aabb);
	static bool		checkPointWithBox(Collision& collisionOut, const Vector3 vPoint, const BoxCollider& box);
	static bool		checkLineWithLine(const Vector2& vLA, const Vector2& vLB, const Vector2& vRA, const Vector2& vRB, Vector2& vOut);
	static bool		checkLineWithLine(const Vector3& vLA, const Vector3& vLB, const Vector3& vRA, const Vector3& vRB, Vector3& vOut);

	static float	transformBoxToAxis(const BoxCollider& box, const Vector3& vAxis);
	static bool		checkOverlapOnAxis(const BoxCollider& boxL, const BoxCollider& boxR, const Vector3& vAxis);
	static bool		checkOverlapOnAxis(const Vector2& vMinL, const Vector2& vMaxL, const Vector2& vMinR, const Vector2& vMaxR);
	static bool		checkOverlapAABB(const AABBCollider& aabbL, const AABBCollider& aabbR);

};
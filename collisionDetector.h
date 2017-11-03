//--------------------------------------------------------------------------------
//	コリジョン探知器
//　collisionDetector.h
//	Author : Xu Wenjie
//	Date   : 2017-08-13
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class C3DRigidbodyComponent;
class Collider;
class BoxCollider;
class SphereCollider;
class AABBCollider;
class OBBCollider;
class FieldCollider;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コリジョン
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
//  コリジョン情報
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

	Collider*		 This;
	Collider*		 Other;
	list<Collision*> Collisions;
};

//--------------------------------------------------------------------------------
//  レイ衝突情報(最近点)
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

	Collider* Collider;
	float	  Distance;
	Vector3	  Normal;
	Vector3	  Position;
};

//--------------------------------------------------------------------------------
//  コリジョン計算クラス
//--------------------------------------------------------------------------------
class CollisionDetector
{
public:
	//衝突判定関数
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
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CollisionDetector() {}
	~CollisionDetector() {}

	static Collision*	detect(const Vector3& point, const AABBCollider& aabb);
	static Collision*	detect(const Vector3& point, const BoxCollider& box);
	static Collision*	detect(const Vector3& point, const FieldCollider& field);
	static Vector2*		detect(const Vector2& beginL, const Vector2& endL, const Vector2& beginR, const Vector2& endR);
	static Vector3*		detect(const Vector3& beginL, const Vector3& endL, const Vector3& beginR, const Vector3& endR);

	static float		projectBoxToAxis(const BoxCollider& box, const Vector3& axis);
	static bool			isOverlapOnAxis(const BoxCollider& boxL, const BoxCollider& boxR, const Vector3& axis);
	static bool			isOverlap(const AABBCollider& aabbL, const AABBCollider& aabbR);
	static bool			isOverlap(const Vector2& minL, const Vector2& maxL, const Vector2& minR, const Vector2& maxR);
	static Collision*	maxPenetration(Collision* current, Collision* next);
};
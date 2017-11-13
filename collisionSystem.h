//--------------------------------------------------------------------------------
//	コリジョンシステム
//　KF_CollisionSystem.h
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class Collider;
class BoxCollider;
class SphereCollider;
class AABBCollider;
class OBBCollider;
class FieldCollider;
class Collision;
class RayHitInfo;
class GameObject;

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum ColliderMode
{
	CM_Static = 0,
	CM_Dynamic,
	CM_Max
};

enum ColliderType
{
	CT_Sphere = 0,
	CT_AABB,
	CT_OBB,
	CT_Plane,
	CT_Cylinder,
	CT_Max,		//以下は特別なCollider(Static又はDynamicのみ使う)
	CT_Field
};

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CollisionSystem
{
#ifdef _DEBUG
	friend class DebugObserver;
#endif // _DEBUG

public:
	//--------------------------------------------------------------------------------
	//  静的メンバ変数
	//--------------------------------------------------------------------------------
	static const float maxCollisionRange;

	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	//struct COL_INFO
	//{//まだ使わない、当たり判定タイプが指定できる仕様
	//	Collider* pCollider;		//Colliderのポインタ
	//	list<ColliderType> listTargetType;	//Colliderがどのタイプと当たり判定するか
	//};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	static auto	Create(void)
	{
		if (instance) return instance;
		instance = new CollisionSystem;
		instance->init();
		return instance;
	}
	static void Release(void) { SAFE_UNINIT(instance); }
	static auto Instance(void) { return instance; }

	void		Update(void);
	void		LateUpdate(void);
	void		Clear(void);

#ifdef _DEBUG
	void		DrawCollider(void);
#endif

	void		Register(Collider* collider);
	void		Deregister(Collider* collider);
	RayHitInfo*	RayCast(const Ray& ray, const float& distance, const GameObject* const rayOwner = nullptr);

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	typedef list<Collider*>::iterator Iterator;

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CollisionSystem();
	~CollisionSystem() {}
	void init(void);
	void uninit(void);
	void checkWithDynamicSphere(const Iterator& begin, SphereCollider& sphere);
	void checkWithDynamicAABB(SphereCollider& sphere);
	void checkWithDynamicAABB(const Iterator& begin, AABBCollider& aabb);
	void checkWithDynamicOBB(SphereCollider& sphere);
	void checkWithDynamicOBB(AABBCollider& aabb);
	void checkWithDynamicOBB(const Iterator& begin, OBBCollider& obb);
	void checkWithStaticSphere(SphereCollider& sphere);
	void checkWithStaticSphere(AABBCollider& aabb);
	void checkWithStaticSphere(OBBCollider& obb);
	void checkWithStaticAABB(SphereCollider& sphere);
	void checkWithStaticAABB(AABBCollider& aabb);
	void checkWithStaticAABB(OBBCollider& obb);
	void checkWithStaticOBB(SphereCollider& sphere);
	void checkWithStaticOBB(BoxCollider& box);
	void checkWithField(SphereCollider& sphere);
	void checkWithField(BoxCollider& box);
	
#ifdef _DEBUG
	string modeToString(const ColliderMode& mode);
	string typeToString(const ColliderType& type);
#endif

	// RayCast
	RayHitInfo*	raycastDynamicSphere(const Ray& ray, const float& distance, const GameObject* const rayOwner);
	RayHitInfo*	raycastDynamicAABB(const Ray& ray, const float& distance, const GameObject* const rayOwner);
	RayHitInfo*	raycastDynamicOBB(const Ray& ray, const float& distance, const GameObject* const rayOwner);
	RayHitInfo*	raycastStaticSphere(const Ray& ray, const float& distance);
	RayHitInfo*	raycastStaticAABB(const Ray& ray, const float& distance);
	RayHitInfo*	raycastStaticOBB(const Ray& ray, const float& distance);
	RayHitInfo*	raycastField(const Ray& ray, const float& distance);
	RayHitInfo* getRealResult(RayHitInfo* current, RayHitInfo* next);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	list<Collider*>			collidersArrays[CM_Max][CT_Max];
	list<Collider*>			fields;
	static CollisionSystem*	instance;

#ifdef _DEBUG
	//LPD3DXMESH	meshSphere;
	//LPD3DXMESH	meshCube;
	//bool		isDrawCollider;
#endif
};

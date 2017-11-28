//--------------------------------------------------------------------------------
//　collision_system.h
//	衝突判定システム
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class Collider;
class BoxCollider;
class SphereCollider;
class AabbCollider;
class ObbCollider;
class FieldCollider;
class Collision;
class RayHitInfo;
class GameObject;

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum ColliderMode
{
	kStatic = 0,
	kDynamic,
	kColliderModeMax
};

enum ColliderType
{
	kSphere = 0,
	kAabb,
	kObb,
	kPlane,
	kCylinder,
	kColliderTypeMax, //以下は特別なCollider(Static又はDynamicのみ使う)
	kField
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
	//  定数定義
	//--------------------------------------------------------------------------------
	static constexpr float kMaxCollisionCheckRange = 250.0f;

	//--------------------------------------------------------------------------------
	//  生成処理
	//  return : CollisionSystem*
	//--------------------------------------------------------------------------------
	static CollisionSystem* Create(void)
	{
		auto instance = MY_NEW CollisionSystem;
		instance->Init();
		return instance;
	}

	//--------------------------------------------------------------------------------
	//  破棄処理
	//--------------------------------------------------------------------------------
	void Release(void) { Uninit(); }

	//--------------------------------------------------------------------------------
	//  更新処理
	//--------------------------------------------------------------------------------
	void Update(void);

	//--------------------------------------------------------------------------------
	//  後更新処理
	//--------------------------------------------------------------------------------
	void LateUpdate(void);

	//--------------------------------------------------------------------------------
	//  クリア処理
	//--------------------------------------------------------------------------------
	void Clear(void);

#ifdef _DEBUG
	//--------------------------------------------------------------------------------
	//  コライダー表示処理
	//--------------------------------------------------------------------------------
	void Render(void);
#endif

	//--------------------------------------------------------------------------------
	//  登録処理
	//--------------------------------------------------------------------------------
	void Register(Collider* collider);

	//--------------------------------------------------------------------------------
	//  削除処理
	//--------------------------------------------------------------------------------
	void Deregister(Collider* collider);

	//--------------------------------------------------------------------------------
	//  レイキャスト処理
	//--------------------------------------------------------------------------------
	RayHitInfo*	RayCast(const Ray& ray, const float& distance, const GameObject* const rayOwner = nullptr);

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	typedef list<Collider*>::iterator Iterator;

	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	CollisionSystem() {}
	CollisionSystem(const CollisionSystem& value) {}
	CollisionSystem& operator=(const CollisionSystem& value) {}
	~CollisionSystem() {}

	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	void Init(void);

	//--------------------------------------------------------------------------------
	//  終了処理
	//--------------------------------------------------------------------------------
	void Uninit(void);

#ifdef _DEBUG
	// ディバゲ表示
	String ModeToString(const ColliderMode& mode);
	String TypeToString(const ColliderType& type);
#endif

	// 衝突判定
	void CheckWithDynamicSphere(const Iterator& begin, SphereCollider& sphere);
	void CheckWithDynamicAABB(SphereCollider& sphere);
	void CheckWithDynamicAABB(const Iterator& begin, AabbCollider& aabb);
	void CheckWithDynamicOBB(SphereCollider& sphere);
	void CheckWithDynamicOBB(AabbCollider& aabb);
	void CheckWithDynamicOBB(const Iterator& begin, ObbCollider& obb);
	void CheckWithStaticSphere(SphereCollider& sphere);
	void CheckWithStaticSphere(AabbCollider& aabb);
	void CheckWithStaticSphere(ObbCollider& obb);
	void CheckWithStaticAABB(SphereCollider& sphere);
	void CheckWithStaticAABB(AabbCollider& aabb);
	void CheckWithStaticAABB(ObbCollider& obb);
	void CheckWithStaticOBB(SphereCollider& sphere);
	void CheckWithStaticOBB(BoxCollider& box);
	void CheckWithField(SphereCollider& sphere);
	void CheckWithField(BoxCollider& box);

	// RayCast
	RayHitInfo*	RaycastDynamicSphere(const Ray& ray, const float& distance, const GameObject* const rayOwner);
	RayHitInfo*	RaycastDynamicAABB(const Ray& ray, const float& distance, const GameObject* const rayOwner);
	RayHitInfo*	RaycastDynamicOBB(const Ray& ray, const float& distance, const GameObject* const rayOwner);
	RayHitInfo*	RaycastStaticSphere(const Ray& ray, const float& distance);
	RayHitInfo*	RaycastStaticAABB(const Ray& ray, const float& distance);
	RayHitInfo*	RaycastStaticOBB(const Ray& ray, const float& distance);
	RayHitInfo*	RaycastField(const Ray& ray, const float& distance);
	RayHitInfo* GetRealResult(RayHitInfo* current, RayHitInfo* next);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	list<Collider*> colliders_arrays_[kColliderModeMax][kColliderTypeMax];
	list<Collider*> fields_;
};

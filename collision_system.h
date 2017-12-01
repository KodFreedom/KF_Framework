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
	//  レイキャスト関数、レイと衝突したの最近点を算出する
	//	ray：レイ
	//	distance：レイの長さ
	//	rayOwner：自分のゲームオブジェクト
	//	return：RayHitInfo*
	//--------------------------------------------------------------------------------
	RayHitInfo*	RayCast(const Ray& ray, const float& distance, const GameObject* const ray_owner = nullptr);

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	typedef list<Collider*>::iterator Iterator;

	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	CollisionSystem();
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
	//--------------------------------------------------------------------------------
	//	モードのenumをstringに変換
	//--------------------------------------------------------------------------------
	String ModeToString(const ColliderMode& mode);

	//--------------------------------------------------------------------------------
	//  タイプのenumをstringに変換
	//--------------------------------------------------------------------------------
	String TypeToString(const ColliderType& type);
#endif

	//--------------------------------------------------------------------------------
	//  Dynamicスフィアとスフィアの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithDynamicSphere(const Iterator& begin, SphereCollider& sphere);
	
	//--------------------------------------------------------------------------------
	//  DynamicAABBとスフィアの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithDynamicAabb(SphereCollider& sphere);

	//--------------------------------------------------------------------------------
	//  DynamicAABBとAABBの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithDynamicAabb(const Iterator& begin, AabbCollider& aabb);

	//--------------------------------------------------------------------------------
	//  DynamicOBBとスフィアの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithDynamicObb(SphereCollider& sphere);

	//--------------------------------------------------------------------------------
	//  DynamicOBBとAABBの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithDynamicObb(AabbCollider& aabb);

	//--------------------------------------------------------------------------------
	//  DynamicOBBとOBBの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithDynamicObb(const Iterator& begin, ObbCollider& obb);

	//--------------------------------------------------------------------------------
	//  Staticスフィアとスフィアの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithStaticSphere(SphereCollider& sphere);

	//--------------------------------------------------------------------------------
	//  StaticスフィアとAABBの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithStaticSphere(AabbCollider& aabb);

	//--------------------------------------------------------------------------------
	//  StaticスフィアとOBBの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithStaticSphere(ObbCollider& obb);

	//--------------------------------------------------------------------------------
	//  StaticAABBとスフィアの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithStaticAabb(SphereCollider& sphere);

	//--------------------------------------------------------------------------------
	//  StaticAABBとAABBの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithStaticAabb(AabbCollider& aabb);

	//--------------------------------------------------------------------------------
	//  StaticAABBとOBBの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithStaticAabb(ObbCollider& obb);

	//--------------------------------------------------------------------------------
	//  StaticOBBとスフィアの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithStaticObb(SphereCollider& sphere);

	//--------------------------------------------------------------------------------
	//  Staticobbとaabb, obbの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithStaticObb(BoxCollider& box);

	//--------------------------------------------------------------------------------
	//  スフィアとフィールドの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithField(SphereCollider& sphere);

	//--------------------------------------------------------------------------------
	//  aabb, obbとフィールドの当たり判定
	//--------------------------------------------------------------------------------
	void CheckWithField(BoxCollider& box);

	//--------------------------------------------------------------------------------
	//	関数名：RaycastDynamicSphere
	//  関数説明：DynamicSphereとのraycast
	//	引数：	ray：レイ
	//			distance：レイの距離
	//			rayOwner：レイの所有者
	//	戻り値：RayHitInfo*
	//--------------------------------------------------------------------------------
	RayHitInfo*	RaycastDynamicSphere(const Ray& ray, const float& distance, const GameObject& ray_owner);

	//--------------------------------------------------------------------------------
	//	関数名：RaycastDynamicAabb
	//  関数説明：DynamicAABBとのraycast
	//	引数：	ray：レイ
	//			distance：レイの距離
	//			ray_owner：レイの所有者
	//	戻り値：RayHitInfo*
	//--------------------------------------------------------------------------------
	RayHitInfo*	RaycastDynamicAabb(const Ray& ray, const float& distance, const GameObject& ray_owner);
	
	//--------------------------------------------------------------------------------
	//	関数名：RaycastDynamicObb
	//  関数説明：DynamicOBBとのraycast
	//	引数：	ray：レイ
	//			distance：レイの距離
	//			ray_owner：レイの所有者
	//	戻り値：RayHitInfo*
	//--------------------------------------------------------------------------------
	RayHitInfo*	RaycastDynamicObb(const Ray& ray, const float& distance, const GameObject& ray_owner);
	
	//--------------------------------------------------------------------------------
	//	関数名：RaycastStaticSphere
	//  関数説明：StaticSphereとのraycast
	//	引数：	ray：レイ
	//			distance：レイの距離
	//	戻り値：RayHitInfo*
	//--------------------------------------------------------------------------------
	RayHitInfo*	RaycastStaticSphere(const Ray& ray, const float& distance);
	
	//--------------------------------------------------------------------------------
	//	関数名：RaycastStaticAabb
	//  関数説明：StaticAABBとのraycast
	//	引数：	ray：レイ
	//			distance：レイの距離
	//	戻り値：RayHitInfo*
	//--------------------------------------------------------------------------------
	RayHitInfo*	RaycastStaticAabb(const Ray& ray, const float& distance);
	
	//--------------------------------------------------------------------------------
	//	関数名：RaycastStaticObb
	//  関数説明：StaticOBBとのraycast
	//	引数：	ray：レイ
	//			distance：レイの距離
	//	戻り値：RayHitInfo*
	//--------------------------------------------------------------------------------
	RayHitInfo*	RaycastStaticObb(const Ray& ray, const float& distance);
	
	//--------------------------------------------------------------------------------
	//	関数名：RaycastField
	//  関数説明：StaticOBBとのraycast
	//	引数：	ray：レイ
	//			distance：レイの距離
	//	戻り値：RayHitInfo*
	//--------------------------------------------------------------------------------
	RayHitInfo*	RaycastField(const Ray& ray, const float& distance);
	
	//--------------------------------------------------------------------------------
	//	関数名：GetRealResult
	//  関数説明：レイとの距離が近いの情報を返す、残りのは破棄する
	//	引数：	current：今の一番近い情報
	//			next：新しい情報
	//	戻り値：RayHitInfo*
	//--------------------------------------------------------------------------------
	RayHitInfo* GetRealResult(RayHitInfo* current, RayHitInfo* next);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	list<Collider*> colliders_arrays_[kColliderModeMax][kColliderTypeMax];
	list<Collider*> fields_;
};

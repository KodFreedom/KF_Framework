//--------------------------------------------------------------------------------
//	コリジョン探知器
//　collisionDetector.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class Rigidbody3D;
class Collider;
class BoxCollider;
class SphereCollider;
class AabbCollider;
class ObbCollider;
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
		: rigidbody_one(nullptr)
		, rigidbody_two(nullptr)
		, normal(Vector3::kZero)
		, point(Vector3::kZero)
		, penetration(0.0f) {}
	~Collision() {}

	Rigidbody3D* rigidbody_one;
	Rigidbody3D* rigidbody_two;
	Vector3      normal; // 衝突法線
	Vector3	     point; // 衝突点
	float        penetration; // 衝突深度
};

//--------------------------------------------------------------------------------
//  コリジョン情報
//--------------------------------------------------------------------------------
class CollisionInfo
{
public:
	CollisionInfo()
		: self(nullptr)
		, other(nullptr)
	{}
	~CollisionInfo()
	{
		collisions.clear();
	}
	Collider* self;
	Collider* other;
	list<Collision*> collisions;
};

//--------------------------------------------------------------------------------
//  レイ衝突情報(最近点)
//--------------------------------------------------------------------------------
class RayHitInfo
{
public:
	RayHitInfo()
		: other(nullptr)
		, distance(0.0f)
		, normal(Vector3::kZero)
		, position(Vector3::kZero)
	{}
	~RayHitInfo() {}

	Collider* other;
	float     distance;
	Vector3   normal;
	Vector3   position;
};

//--------------------------------------------------------------------------------
//  コリジョン計算クラス
//--------------------------------------------------------------------------------
class CollisionDetector
{
public:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static constexpr float kMaxFieldSlope = 45.0f * kPi / 180.0f; // フィールド登れる最大角度
	static const float kMaxFieldSlopeCos; // フィールド登れる最大角度のcos値
	static constexpr float kMinWallSlope = 60.0f * kPi / 180.0f; // フィールドが壁と認める最小角度
	static const float kMinWallSlopeCos; // フィールドが壁と認める最小角度のcos値
	static constexpr float kMaxObbSlope = 60.0f * kPi / 180.0f; // Obb登れる最大角度
	static const float kMaxObbSlopeCos; // Obb登れる最大角度のcos値

	//--------------------------------------------------------------------------------
	//	関数名：Detect
	//  関数説明：スフィアとスフィアの当たり判定関数
	//			　当たったらコリジョン情報を物理演算マネージャに登録する
	//	引数：	sphereL：スフィアコライダー
	//			sphereR：スフィアコライダー
	//	戻り値：なし
	//--------------------------------------------------------------------------------
	static void Detect(SphereCollider& sphere_left, SphereCollider& sphere_right);
	
	//--------------------------------------------------------------------------------
	//	関数名：Detect
	//  関数説明：スフィアとAABBの当たり判定関数
	//			　当たったらコリジョン情報を物理演算マネージャに登録する
	//	引数：	sphere：スフィアコライダー
	//			aabb：回転なしボックスコライダー
	//	戻り値：なし
	//--------------------------------------------------------------------------------
	static void Detect(SphereCollider& sphere, AabbCollider& aabb);
	
	//--------------------------------------------------------------------------------
	//	関数名：Detect
	//  関数説明：スフィアとOBBの当たり判定関数
	//			　当たったらコリジョン情報を物理演算マネージャに登録する
	//	引数：	sphere：スフィアコライダー
	//			obb：回転あるボックスコライダー
	//	戻り値：なし
	//--------------------------------------------------------------------------------
	static void Detect(SphereCollider& sphere, ObbCollider& obb);
	
	//--------------------------------------------------------------------------------
	//	関数名：Detect
	//  関数説明：AABBとAABBの当たり判定関数
	//			　当たったらコリジョン情報を物理演算マネージャに登録する
	//	引数：	aabbL：回転なしボックスコライダー
	//			aabbR：回転なしボックスコライダー
	//	戻り値：なし
	//--------------------------------------------------------------------------------
	static void Detect(AabbCollider& aabb_left, AabbCollider& aabb_right);
	
	//--------------------------------------------------------------------------------
	//	関数名：Detect
	//  関数説明：boxとboxの当たり判定関数(いずれかが必ずOBB)
	//			　当たったらコリジョン情報を物理演算マネージャに登録する
	//	引数：	boxL：ボックスコライダー
	//			boxR：ボックスコライダー
	//	戻り値：なし
	//--------------------------------------------------------------------------------
	static void Detect(BoxCollider& box_left, BoxCollider& box_right);
	
	//--------------------------------------------------------------------------------
	//	関数名：Detect
	//  関数説明：スフィアとフィールドの当たり判定関数
	//			　当たったらコリジョン情報を物理演算マネージャに登録する
	//	引数：	sphere：スフィアコライダー
	//			field：フィールドコライダー
	//	戻り値：なし
	//--------------------------------------------------------------------------------
	static void Detect(BoxCollider& box, FieldCollider& field);
	
	//--------------------------------------------------------------------------------
	//	関数名：Detect
	//  関数説明：Boxとフィールドの当たり判定関数
	//			　当たったらコリジョン情報を物理演算マネージャに登録する
	//	引数：	box：ボックスコライダー
	//			field：フィールドコライダー
	//	戻り値：なし
	//--------------------------------------------------------------------------------
	static void Detect(SphereCollider& sphere, FieldCollider& field);
	
	//--------------------------------------------------------------------------------
	//	関数名：Detect
	//  関数説明：レイとボックスの当たり判定
	//	引数：	ray：レイ
	//			distancetance：レイの長さ
	//			box：ボックスコライダー
	//	戻り値：RayHitInfo*
	//--------------------------------------------------------------------------------
	static RayHitInfo* Detect(const Ray& ray, const float& distance, BoxCollider& box);

	//--------------------------------------------------------------------------------
	//	関数名：Detect
	//  関数説明：レイとスフィアの当たり判定
	//	引数：	ray：レイ
	//			distancetance：レイの長さ
	//			sphere：スフィアコライダー
	//	戻り値：RayHitInfo*
	//--------------------------------------------------------------------------------
	static RayHitInfo* Detect(const Ray& ray, const float& distance, SphereCollider& sphere);
	
	//--------------------------------------------------------------------------------
	//	関数名：Detect
	//  関数説明：レイとフィールドの当たり判定
	//	引数：	ray：レイ
	//			distancetance：レイの長さ
	//			field：フィールドコライダー
	//	戻り値：RayHitInfo*
	//--------------------------------------------------------------------------------
	static RayHitInfo* Detect(const Ray& ray, const float& distance, FieldCollider& field);

private:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	CollisionDetector() {}
	CollisionDetector(const CollisionDetector& value) {}
	CollisionDetector& operator=(const CollisionDetector& value) {}
	~CollisionDetector() {}

	//--------------------------------------------------------------------------------
	//	関数名：detect
	//  関数説明：点とAABBの当たり判定
	//	引数：	point：点の位置
	//			aabb：AABBコライダー
	//	戻り値：Collision*
	//--------------------------------------------------------------------------------
	static Collision* Detect(const Vector3& point, const AabbCollider& aabb);
	
	//--------------------------------------------------------------------------------
	//	関数名：detect
	//  関数説明：点とBoxの当たり判定
	//	引数：	collisionOut：衝突情報(出力用)
	//			vPoint：点の位置
	//			box：boxコライダー
	//	戻り値：Collision*
	//--------------------------------------------------------------------------------
	static Collision* Detect(const Vector3& point, const BoxCollider& box);

	//--------------------------------------------------------------------------------
	//	関数名：detect
	//  関数説明：点とfieldの当たり判定
	//	引数：	point：点の位置
	//			field：fieldコライダー
	//	戻り値：Collision*
	//--------------------------------------------------------------------------------
	static Collision* Detect(const Vector3& point, const FieldCollider& field);
	
	//--------------------------------------------------------------------------------
	//	関数名：detect
	//  関数説明：直線と直線の交差判定(二次元)
	//	引数：	beginL：LineLeftの始点
	//			endL：LineLeftの終点
	//			beginR：LineRightの始点
	//			endR：LineRightの終点
	//	戻り値：Vector2*
	//--------------------------------------------------------------------------------
	static Vector2* Detect(const Vector2& begin_left, const Vector2& end_left, const Vector2& begin_right, const Vector2& end_right);
	
	//--------------------------------------------------------------------------------
	//	関数名：detect
	//  関数説明：直線と直線の交差判定(三次元)
	//	引数：	begin_left：line_lefteftの始点
	//			end_left：line_lefteftの終点
	//			begin_right：line_rightightの始点
	//			end_right：line_rightightの終点
	//	戻り値：Vector3*
	//--------------------------------------------------------------------------------
	static Vector3* Detect(const Vector3& begin_left, const Vector3& end_left, const Vector3& begin_right, const Vector3& end_right);

	//--------------------------------------------------------------------------------
	//	関数名：projectBoxToAxis
	//  関数説明：boxを与えられた軸に投影して長さを算出する関数
	//	引数：	box：ボックスコライダー
	//			axis：軸情報
	//	戻り値：float
	//--------------------------------------------------------------------------------
	static float ProjectBoxToAxis(const BoxCollider& box, const Vector3& axis);

	//--------------------------------------------------------------------------------
	//	関数名：IsOverlapOnAxis
	//  関数説明：ボックス同士が与えられた軸に重ねてるかどうかをチェックする
	//	引数：	box_left：ボックスコライダー
	//			box_right：ボックスコライダー
	//			axis：軸情報
	//	戻り値：bool
	//--------------------------------------------------------------------------------
	static bool	IsOverlapOnAxis(const BoxCollider& box_left, const BoxCollider& box_right, const Vector3& axis);

	//--------------------------------------------------------------------------------
	//	関数名：checkOverlapOnAxis
	//  関数説明：AABB同士がXYZ軸に重ねてるかどうかをチェックする(三次元)
	//	引数：	aabb_left：AABBコライダー
	//			aabb_right：AABBコライダー
	//	戻り値：bool
	//--------------------------------------------------------------------------------
	static bool IsOverlap(const AabbCollider& aabb_left, const AabbCollider& aabb_right);

	//--------------------------------------------------------------------------------
	//	関数名：isOverlap
	//  関数説明：AABB同士が与えられた軸に重ねてるかどうかをチェックする(二次元)
	//	引数：	min_left：aabb_left一番左上の点情報
	//			max_left：aabb_left一番右下の点情報
	//			min_right：aabb_right一番左上の点情報
	//			max_right：aabb_right一番右下の点情報
	//	戻り値：bool
	//--------------------------------------------------------------------------------
	static bool IsOverlap(const Vector2& min_left, const Vector2& max_left, const Vector2& min_right, const Vector2& max_right);
	
	//--------------------------------------------------------------------------------
	//	関数名：MaxPenetration
	//  関数説明：衝突深度が一番深いのコリジョンを返す、残りのは破棄する
	//	引数：	current：今一番深いコリジョン
	//			next：新しいコリジョン
	//	戻り値：Collision*
	//--------------------------------------------------------------------------------
	static Collision*	MaxPenetration(Collision* current, Collision* next);
};
//--------------------------------------------------------------------------------
//	コリジョン計算関数
//　KF_CollisionUtility.h
//	Author : Xu Wenjie
//	Date   : 2017-08-13
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class C3DRigidbodyComponent;
class CColliderComponent;
class CBoxColliderComponent;
class CSphereColliderComponent;
class CAABBColliderComponent;
class COBBColliderComponent;
class CFieldColliderComponent;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コリジョン
//--------------------------------------------------------------------------------
class CCollision
{
public:
	CCollision()
		: m_pRigidBodyOne(nullptr)
		, m_pRigidBodyTwo(nullptr)
		, m_vCollisionNormal(Vector3(0.0f))
		, m_vCollisionPos(Vector3(0.0f))
		, m_fPenetration(0.0f)
	{}
	~CCollision() {}

	C3DRigidbodyComponent*	m_pRigidBodyOne;
	C3DRigidbodyComponent*	m_pRigidBodyTwo;
	Vector3					m_vCollisionNormal;
	Vector3					m_vCollisionPos;
	float					m_fPenetration;
	Matrix44				m_mtxToWorld;
};

//--------------------------------------------------------------------------------
//  コリジョン情報
//--------------------------------------------------------------------------------
class CCollisionInfo
{
public:
	CCollisionInfo()
		: m_pColliderThis(nullptr)
		, m_pCollider(nullptr)
	{
		m_listCollision.clear();
	}
	~CCollisionInfo()
	{
		m_listCollision.clear();
	}

	CColliderComponent*		m_pColliderThis;
	CColliderComponent*		m_pCollider;
	list<CCollision*>		m_listCollision;
};

//--------------------------------------------------------------------------------
//  レイ衝突情報(最近点)
//--------------------------------------------------------------------------------
class CRaycastHitInfo
{
public:
	CRaycastHitInfo()
		: m_pCollider(nullptr)
		, m_fDistance(0.0f)
		, m_Normal(Vector3(0.0f))
		, m_Position(Vector3(0.0f))
	{}
	~CRaycastHitInfo() {}

	CColliderComponent*		m_pCollider;	//衝突相手のコライダー
	float					m_fDistance;	//レイの始点と衝突点との距離
	Vector3					m_Normal;		//衝突点の面法線
	Vector3					m_Position;			//衝突点の位置
};

//--------------------------------------------------------------------------------
//  コリジョン計算クラス
//--------------------------------------------------------------------------------
class CCollisionDetector
{
public:
	//衝突判定関数
	static void		CheckSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR);
	static void		CheckSphereWithAABB(CSphereColliderComponent& sphere, CAABBColliderComponent& aabb);
	static void		CheckSphereWithOBB(CSphereColliderComponent& sphere, COBBColliderComponent& obb);
	static void		CheckAABBWithAABB(CAABBColliderComponent& aabbL, CAABBColliderComponent& aabbR);
	static void		CheckBoxWithBox(CBoxColliderComponent& boxL, CBoxColliderComponent& boxR);
	
	//Field
	static void		CheckBoxWithField(CBoxColliderComponent& box, CFieldColliderComponent& field);
	static void		CheckSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field);
	
	//Ray
	static bool		CheckRayWithBox(const Ray& ray, const float& fDistance, CBoxColliderComponent& box, CRaycastHitInfo& infoOut);
	static bool		CheckRayWithSphere(const Ray& ray, const float& fDistance, CSphereColliderComponent& sphere, CRaycastHitInfo& infoOut);
	static bool		CheckRayWithField(const Ray& ray, const float& fDistance, CFieldColliderComponent& field, CRaycastHitInfo& infoOut);

private:
	CCollisionDetector() {}
	~CCollisionDetector() {}

	//計算用関数
	static bool		checkPointWithAABB(CCollision& collisionOut, const Vector3 vPoint, const CAABBColliderComponent& aabb);
	static bool		checkPointWithBox(CCollision& collisionOut, const Vector3 vPoint, const CBoxColliderComponent& box);
	static bool		checkLineWithLine(const Vector2& vLA, const Vector2& vLB, const Vector2& vRA, const Vector2& vRB, Vector2& vOut);
	static bool		checkLineWithLine(const Vector3& vLA, const Vector3& vLB, const Vector3& vRA, const Vector3& vRB, Vector3& vOut);

	static float	transformBoxToAxis(const CBoxColliderComponent& box, const Vector3& vAxis);
	static bool		checkOverlapOnAxis(const CBoxColliderComponent& boxL, const CBoxColliderComponent& boxR, const Vector3& vAxis);
	static bool		checkOverlapOnAxis(const Vector2& vMinL, const Vector2& vMaxL, const Vector2& vMinR, const Vector2& vMaxR);
	static bool		checkOverlapAABB(const CAABBColliderComponent& aabbL, const CAABBColliderComponent& aabbR);

};
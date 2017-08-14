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
		, m_vCollisionNormal(CKFVec3(0.0f))
		, m_vCollisionPos(CKFVec3(0.0f))
		, m_fPenetration(0.0f)
	{}
	~CCollision() {}

	C3DRigidbodyComponent*	m_pRigidBodyOne;
	C3DRigidbodyComponent*	m_pRigidBodyTwo;
	CKFVec3					m_vCollisionNormal;
	CKFVec3					m_vCollisionPos;
	float					m_fPenetration;
	CKFMtx44				m_mtxToWorld;
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
		, m_vNormal(CKFVec3(0.0f))
		, m_vPos(CKFVec3(0.0f))
	{}
	~CRaycastHitInfo() {}

	CColliderComponent*		m_pCollider;	//衝突相手のコライダー
	float					m_fDistance;	//レイの始点と衝突点との距離
	CKFVec3					m_vNormal;		//衝突点の面法線
	CKFVec3					m_vPos;			//衝突点の位置
};

//--------------------------------------------------------------------------------
//  コリジョンテスト
//--------------------------------------------------------------------------------
class CCollisionTest
{
public:


private:
	CCollisionTest() {}
	~CCollisionTest() {}
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
	static bool		CheckRayWithBox(const CKFRay& ray, const float& fDistance, CBoxColliderComponent& box, CRaycastHitInfo& infoOut);
	static bool		CheckRayWithSphere(const CKFRay& ray, const float& fDistance, CSphereColliderComponent& sphere, CRaycastHitInfo& infoOut);
	static bool		CheckRayWithField(const CKFRay& ray, const float& fDistance, CFieldColliderComponent& field, CRaycastHitInfo& infoOut);

private:
	CCollisionDetector() {}
	~CCollisionDetector() {}

	//計算用関数
	static bool		checkPointWithAABB(CCollision& collisionOut, const CKFVec3 vPoint, const CAABBColliderComponent& aabb);
	static bool		checkPointWithBox(CCollision& collisionOut, const CKFVec3 vPoint, const CBoxColliderComponent& box);
	static bool		checkLineWithLine(const CKFVec2& vLA, const CKFVec2& vLB, const CKFVec2& vRA, const CKFVec2& vRB, CKFVec2& vOut);
	static bool		checkLineWithLine(const CKFVec3& vLA, const CKFVec3& vLB, const CKFVec3& vRA, const CKFVec3& vRB, CKFVec3& vOut);

	static float	transformBoxToAxis(const CBoxColliderComponent& box, const CKFVec3& vAxis);
	static bool		checkOverlapOnAxis(const CBoxColliderComponent& boxL, const CBoxColliderComponent& boxR, const CKFVec3& vAxis);
	static bool		checkOverlapOnAxis(const CKFVec2& vMinL, const CKFVec2& vMaxL, const CKFVec2& vMinR, const CKFVec2& vMaxR);
	static bool		checkOverlapAABB(const CAABBColliderComponent& aabbL, const CAABBColliderComponent& aabbR);

};
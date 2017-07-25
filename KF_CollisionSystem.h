//--------------------------------------------------------------------------------
//	コリジョン判定クラス
//　KF_CollisionSystem.h
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CColliderComponent;
class CSphereColliderComponent;
class CFieldColliderComponent;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コリジョン情報
//--------------------------------------------------------------------------------
//class CCollisionInfo
//{
//public:
//	CCollisionInfo(const list<CKFVec3>& listPoints, CColliderComponent& collider)
//		: m_listPoints(listPoints), m_collider(collider) {}
//	~CCollisionInfo() {}
//
//	list<CKFVec3>		m_listPoints;	//当たったの頂点
//	CColliderComponent&	m_collider;		//相手のコライダー
//};

//--------------------------------------------------------------------------------
//  コリジョン判定システム
//--------------------------------------------------------------------------------
class CKFCollisionSystem
{
public:
	static void CheckCollisionSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR);
	static void CheckCollisionSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field);

private:
	CKFCollisionSystem() {}
	~CKFCollisionSystem() {}
};
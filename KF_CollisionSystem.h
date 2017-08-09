//--------------------------------------------------------------------------------
//	コリジョンシステム
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
class CBoxColliderComponent;
class CSphereColliderComponent;
class CAABBColliderComponent;
class COBBColliderComponent;
class CFieldColliderComponent;
class CCollision;

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define CS CKFCollisionSystem	//CKFCollisionSystemの略称

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コリジョンシステム
//--------------------------------------------------------------------------------
class CKFCollisionSystem
{
public:
	//--------------------------------------------------------------------------------
	//  列挙型定義
	//--------------------------------------------------------------------------------
	enum COL_MODE
	{
		STATIC = 0,
		DYNAMIC,
		MODE_MAX
	};

	enum COL_TYPE
	{
		COL_SPHERE = 0,
		COL_AABB,
		COL_OBB,
		COL_PLANE,
		COL_CYLINDER,
		COL_MAX,		//以下は特別なCollider(Static又はDynamicのみ使う)
		COL_FIELD
	};

	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	//typedef list<CColliderComponent*>::iterator COL_ITR;

	//struct COL_INFO
	//{//まだ使わない、当たり判定タイプが指定できる仕様
	//	CColliderComponent* pCollider;		//Colliderのポインタ
	//	list<COL_TYPE> listTargetType;	//Colliderがどのタイプと当たり判定するか
	//};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CKFCollisionSystem();
	~CKFCollisionSystem() {}

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	LateUpdate(void);
	void	Release(void)
	{
		Uninit();
		delete this;
	}

#ifdef _DEBUG
	void	DrawCollider(void);
#endif

	void	RegisterCollider(const COL_MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider);
	
private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void	checkWithDynamicSphere(const list<CColliderComponent*>::iterator& itrBegin, CSphereColliderComponent& sphere);
	void	checkWithDynamicAABB(CSphereColliderComponent& sphere);
	void	checkWithDynamicAABB(const list<CColliderComponent*>::iterator& itrBegin, CAABBColliderComponent& aabb);
	void	checkWithDynamicOBB(CSphereColliderComponent& sphere);
	void	checkWithDynamicOBB(CAABBColliderComponent& aabb);
	void	checkWithDynamicOBB(const list<CColliderComponent*>::iterator& itrBegin, COBBColliderComponent& obb);
	void	checkWithStaticSphere(CSphereColliderComponent& sphere);
	void	checkWithStaticSphere(CAABBColliderComponent& aabb);
	void	checkWithStaticSphere(COBBColliderComponent& obb);
	void	checkWithStaticAABB(CSphereColliderComponent& sphere);
	void	checkWithStaticAABB(CAABBColliderComponent& aabb);
	void	checkWithStaticAABB(COBBColliderComponent& obb);
	void	checkWithStaticOBB(CSphereColliderComponent& sphere);
	void	checkWithStaticOBB(CBoxColliderComponent& box);
	void	checkWithField(CSphereColliderComponent& sphere);
	void	checkWithField(CBoxColliderComponent& box);

	//衝突判定関数
	void	checkCollisionSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR);
	void	checkCollisionSphereWithAABB(CSphereColliderComponent& sphere, CAABBColliderComponent& aabb);
	void	checkCollisionSphereWithOBB(CSphereColliderComponent& sphere, COBBColliderComponent& obb);
	void	checkCollisionAABBWithAABB(CAABBColliderComponent& aabbL, CAABBColliderComponent& aabbR);
	void	checkCollisionBoxWithBox(CBoxColliderComponent& boxL, CBoxColliderComponent& boxR);
	bool	checkCollisionPointWithAABB(CCollision& collisionOut, const CKFVec3 vPoint, const CAABBColliderComponent& aabb);
	bool	checkCollisionPointWithBox(CCollision& collisionOut, const CKFVec3 vPoint, const CBoxColliderComponent& box);
	
	//Field
	void	checkCollisionBoxWithField(CBoxColliderComponent& box, CFieldColliderComponent& field);
	void	checkCollisionSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field);
	
	//計算用関数
	float	transformBoxToAxis(const CBoxColliderComponent& box, const CKFVec3& vAxis);
	bool	checkOverlapOnAxis(const CBoxColliderComponent& boxL, const CBoxColliderComponent& boxR, const CKFVec3& vAxis);
	bool	checkOverlapOnAxis(const CKFVec2& vMinL, const CKFVec2& vMaxL, const CKFVec2& vMinR, const CKFVec2& vMaxR);
	bool	checkOverlapAABB(const CAABBColliderComponent& aabbL, const CAABBColliderComponent& aabbR);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	list<CColliderComponent*> m_alistCollider[MODE_MAX][COL_MAX];
	list<CColliderComponent*> m_listField;

#ifdef _DEBUG
	LPD3DXMESH m_pMeshSphere;
	LPD3DXMESH m_pMeshCube;
#endif
};

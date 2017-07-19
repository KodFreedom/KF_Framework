//--------------------------------------------------------------------------------
//	コリジョンマネージャ
//　colliderManager.h
//	Author : Xu Wenjie
//	Date   : 2017-06-04
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CColliderComponent;
class CSphereColliderComponent;
class CFieldColliderComponent;

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define CM CColliderManager	//CColliderManagerの略称

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CColliderManager
{
public:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	enum MODE
	{
		STATIC = 0,
		DYNAMIC,
		MODE_MAX
	};

	enum COL_TYPE
	{
		COL_SPHERE = 0,
		COL_CYLINDER,
		COL_MAX,		//以下は特別なCollider(Static又はDynamicのみ使う)
		COL_FIELD
	};

	typedef list<CColliderComponent*>::iterator COL_ITR;

	struct COL_INFO
	{//まだ使わない、当たり判定タイプが指定できる仕様
		CColliderComponent* pCollider;		//Colliderのポインタ
		list<COL_TYPE> listTargetType;	//Colliderがどのタイプと当たり判定するか
	};

	struct HIT_INFO
	{
		CKFVec3 vPos;
		CKFVec3	vNormal;
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CColliderManager();
	~CColliderManager() {}

	void		Init(void);
	void		Uninit(void);
	void		Update(void);

#ifdef _DEBUG
	void		DrawCollider(void);
#endif

	COL_ITR		SaveCollider(const MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider);
	void		ReleaseCollider(const MODE& mode, const COL_TYPE& type, const COL_ITR& itr);
	COL_ITR		SaveField(CColliderComponent* pCollider);
	void		ReleaseField(const COL_ITR& itr);

	//臨時採用
	bool		SphereCast(const CKFVec3& vCenter, const float& fRadius, HIT_INFO& hitInfoOut);
	bool		RayCast(const CKFRay& ray, const float& fLength, HIT_INFO& hitInfoOut);

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void		CheckWithDynamicSphere(const COL_ITR& itrBegin, CSphereColliderComponent& sphere);
	void		CheckWithStaticSphere(CSphereColliderComponent& sphere);
	void		CheckWithField(CSphereColliderComponent& sphere);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	list<CColliderComponent*> m_alistCollider[MODE_MAX][COL_MAX];
	list<CColliderComponent*> m_listField;
#ifdef _DEBUG
	LPD3DXMESH m_pMeshSphere;
#endif
};
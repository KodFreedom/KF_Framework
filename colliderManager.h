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
		COL_MAX,		//以下は特別なCollider(Static又はDynamicのみ使う)
		COL_FIELD
	};

	typedef std::list<CColliderComponent*>::iterator COL_ITR;

	struct COL_INFO
	{//まだ使わない、当たり判定タイプが指定できる仕様
		CColliderComponent* pCollider;		//Colliderのポインタ
		std::list<COL_TYPE> listTargetType;	//Colliderがどのタイプと当たり判定するか
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CColliderManager();
	~CColliderManager() {}

	void		Init(void);
	void		Uninit(void);
	void		Update(void);

	COL_ITR		SaveCollider(const MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider);
	void		ReleaseCollider(const MODE& mode, const COL_TYPE& type, const COL_ITR& itr);
	COL_ITR		SaveField(CColliderComponent* pCollider);
	void		ReleaseField(const COL_ITR& itr);

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void		CheckWithField(CSphereColliderComponent& sphere);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	std::list<CColliderComponent*> m_alistCollider[MODE_MAX][COL_MAX];
	std::list<CColliderComponent*> m_listField;
};
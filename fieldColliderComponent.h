//--------------------------------------------------------------------------------
//	fieldColliderコンポネント
//　fieldColliderComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "colliderComponent.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  fieldColliderポネントクラス
//--------------------------------------------------------------------------------
class CFieldColliderComponent : public CColliderComponent
{
public:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct INFO
	{
		bool	bInTheField;
		float	fHeight;
		CKFVec3	vFaceNormal;
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CFieldColliderComponent(CGameObject* const pGameObj, const int& nNumBlockX, const int& nNumBlockZ, const CKFVec2& vBlockSize)
		: CColliderComponent(pGameObj, CM::COL_FIELD, CM::STATIC)
		, m_nNumBlockX(nNumBlockX)
		, m_nNumBlockZ(nNumBlockZ)
		, m_vBlockSize(vBlockSize)
	{
		m_vectorVtx.clear();
	}

	~CFieldColliderComponent() {}

	KFRESULT	Init(void) override;
	void		Uninit(void) override;
	void		Update(void) override;

	//Get関数
	INFO			GetPointInfo(const CKFVec3& vPos);
	std::list<INFO> GetRangeInfo(const CKFVec3& vBegin, const CKFVec3& vEnd);
	//float		GetHeight(const CKFVec3& vPos);

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void		MakeVertex(void);	//将来はエディタで作る

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	int						m_nNumBlockX;
	int						m_nNumBlockZ;
	CKFVec2					m_vBlockSize;
	std::vector<CKFVec3>	m_vectorVtx;
};
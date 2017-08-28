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

	struct FINFO
	{
		bool	bInFieldRange;
		float   fPenetration;
		CKFVec3 vFaceNormal;
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CFieldColliderComponent(CGameObject* const pGameObj, const string& strFieldName)
		: CColliderComponent(pGameObj, CS::COL_FIELD, CS::STATIC)
		, m_nNumBlockX(0)
		, m_nNumBlockZ(0)
		, m_vBlockSize(CKFVec2(0.0f))
	{
		m_vectorVtx.clear();
		load(strFieldName);
	}

	~CFieldColliderComponent() {}

	bool	Init(void) override { return true; }
	void	Uninit(void) override;

	//Get関数
	//INFO	GetPointInfo(const CKFVec3& vPos);
	FINFO	GetProjectionInfo(const CKFVec3& vPos);
	bool	GetVtxByRange(const CKFVec3& vBegin, const CKFVec3& vEnd, int& nNumVtxXOut, int& nNumVtxZOut, vector<CKFVec3>& vecOut);

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void	load(const string& strFieldName);
	bool	getPointInfo(const CKFVec3& vPos, CKFVec3& vPLeftUp, CKFVec3& vPRightDown, CKFVec3& vPSide, CKFVec3& vFaceNormal);
	
	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	int				m_nNumBlockX;
	int				m_nNumBlockZ;
	CKFVec2			m_vBlockSize;
	vector<CKFVec3>	m_vectorVtx;
};
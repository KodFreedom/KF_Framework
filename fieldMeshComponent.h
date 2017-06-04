//--------------------------------------------------------------------------------
//	フィールドメッシュコンポネント
//　fieldMeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#ifndef _FIELD_MESH_COMPONENT_H_
#define _FIELD_MESH_COMPONENT_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "3DMeshComponent.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  フィールドメッシュコンポネントクラス
//--------------------------------------------------------------------------------
class CFieldMeshComponent : public C3DMeshComponent
{
public:
	CFieldMeshComponent(CGameObject* const pGameObj, const int& nNumBlockX, const int& nNumBlockZ, const CKFVec2& vBlockSize) : C3DMeshComponent(pGameObj)
		, m_nNumBlockX(nNumBlockX)
		, m_nNumBlockZ(nNumBlockZ)
		, m_vBlockSize(vBlockSize)
	{}

	~CFieldMeshComponent() {}

	KFRESULT	Init(void) override;
	float		GetHeight(const CKFVec3& vPos);

private:
	void		MakeVertex(void);

	int			m_nNumBlockX;
	int			m_nNumBlockZ;
	CKFVec2		m_vBlockSize;
};

#endif
//--------------------------------------------------------------------------------
//
//　meshField.h
//	Author : Xu Wenjie
//	Date   : 2016-12-13
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "gameObject3D.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------
class CMeshField : public CGameObject3D
{
public:
	CMeshField();
	~CMeshField();

	KFRESULT	Init(const int &nNumBlockX, const int &nNumBlockZ, const CKFVec2 &vBlockSize, const CKFVec3 &vPos, const CKFVec3 &vRot);
	void		Update(void) override;

	static CMeshField*	Create(const int &nNumBlockX, const int &nNumBlockZ, const CKFVec2 &vBlockSize, const CKFVec3 &vPos, const CKFVec3 &vRot);
private:
	void		MakeVertex(void);

	int			m_nNumBlockX;
	int			m_nNumBlockZ;
	CKFVec2		m_vBlockSize;
};

#endif
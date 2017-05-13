//--------------------------------------------------------------------------------
//
//　gameObject3D.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_3D_H_
#define _GAMEOBJECT_3D_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "gameObject.h"
#include "textureManager.h"
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CGameObject3D : public CGameObject
{
public:
	CGameObject3D();
	~CGameObject3D();

	virtual KFRESULT	Init(const int &nVtxNum, const int &nIdxNum, const int &nPolygonNum);
	virtual void		Uninit(void);
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Draw(void);

protected:
	virtual void			SetMatrix(void);
	virtual void			SetRenderState(void);
	virtual void			ResetRenderState(void);

	CKFVec3					m_vRot;			// 回転
	CKFVec3					m_vScale;		// 拡大率
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuffer;	// インデックスへのポインタ
	CTM::TEX_NAME			m_texName;		// テクスチャ
	CMM::MATERIAL			m_matType;		// マテリアル

private:
	KFRESULT	CreateBuffer(void);
	
	int			m_nVtxNum;		// 頂点数
	int			m_nIdxNum;		// インデックス数
	int			m_nPolygonNum;	// ポリゴン数
};

#endif
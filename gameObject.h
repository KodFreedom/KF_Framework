//--------------------------------------------------------------------------------
//
//　gameObject.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "gameObjectManager.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CGameObject
{
public:
	CGameObject() : m_vPos(CKFVec3(0.0f)), m_pVtxBuffer(NULL), m_nID(-1), m_pri(GOM::PRI_MAX) {}
	~CGameObject() {}
	
	virtual KFRESULT	Init(void) { return KF_SUCCEEDED; }
	virtual void		Uninit(void) = 0;
	virtual void		Update(void) = 0;
	virtual void		LateUpdate(void) = 0;
	virtual void		Draw(void) = 0;
	void				Release(void);

	//Get関数
	CKFVec3				GetPos(void);

	//Set関数
	void				SetPos(const CKFVec3 &vPos);

protected:
	CKFVec3						m_vPos;			//オブジェクト位置
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffer;	//頂点バッファ管理インターフェースポインタ
	int							m_nID;
	GOM::PRIORITY				m_pri;
};

#endif
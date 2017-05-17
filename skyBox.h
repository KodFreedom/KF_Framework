//--------------------------------------------------------------------------------
//
//　skyBox.h
//	Author : Xu Wenjie
//	Date   : 2017-05-10
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _SKT_BOX_H_
#define _SKT_BOX_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "gameObject3D.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CSkyBox : public CGameObject3D
{
public:
	CSkyBox();
	~CSkyBox() {}

	KFRESULT	Init(const CKFVec3 &vPos, const CKFVec3 &vSize);

	static CSkyBox*	Create(void);
	static CSkyBox*	Create(const CKFVec3 &vPos, const CKFVec3 &vSize);
private:
	void		MakeVertex(void);
	void		SetRenderState(void) override;
	void		ResetRenderState(void) override;

	CKFVec3		m_vSize;
};

#endif
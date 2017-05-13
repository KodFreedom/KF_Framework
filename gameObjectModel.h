//--------------------------------------------------------------------------------
//
//　gameObjectModel.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_MODEL_H_
#define _GAMEOBJECT_MODEL_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "gameObject.h"
#include "modelManager.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CGameObjectModel : public CGameObject
{
public:
	CGameObjectModel();
	~CGameObjectModel();

	void	Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName);
	void	Uninit(void);
	void	Update(void);
	void	LateUpdate(void);
	void	Draw(void);

	static CGameObjectModel*	Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName);

protected:
	void				SetMatrix(void);
	void				SetRenderState(void);
	void				ResetRenderState(void);

	CKFVec3				m_vRot;			// 回転
	CMOM::MODEL_NAME	m_modelName;	// モデル
	CMM::MATERIAL		m_matType;		// マテリアル
};

#endif
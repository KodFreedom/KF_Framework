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
#include "modelManager.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CGameObject3D : public CGameObject
{
public:
	CGameObject3D();
	CGameObject3D(const GOM::PRIORITY &pri);
	~CGameObject3D() {}

	static CGameObject3D* Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale, const CTM::TEX_NAME &texName, const CMM::MATERIAL &matType, const CMOM::MODEL_NAME &modelName);
protected:

	CGameObject3D*	m_pParent;	//親パーツ

	//ヌルゲームオブジェクト
	static CNullGameObject	m_nullGameObj;
};

#endif
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

	//Get関数
	//const CGameObject3D& GetParents(void) const { return *m_pParents; }

	//生成関数
	static CGameObject3D* CreateSkyBox(const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale);
	static CGameObject3D* CreateField(const int &nNumBlockX, const int &nNumBlockZ, const CKFVec2 &vBlockSize, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale);
	static CGameObject3D* CreateCube(const CKFVec3 &vSize, const CKFColor &cColor, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale);

protected:

	//CGameObject3D*	m_pParents;	//親パーツ
};

#endif
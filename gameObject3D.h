//--------------------------------------------------------------------------------
//
//　gameObject3D.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
#pragma once

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
	CGameObject3D(const GOM::PRIORITY& pri = GOM::PRI_3D, const OBJ_TYPE& type = OT_DEFAULT);
	~CGameObject3D() {}

	//生成関数
	static CGameObject3D* CreateSkyBox(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject3D* CreateField(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject3D* CreateCube(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject3D* CreateXModel(const string& strPath, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject3D* CreateModel(const CMOM::MODEL_NAME& modelName, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);

private:
};
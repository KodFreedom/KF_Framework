//--------------------------------------------------------------------------------
//	アクター
//　gameObjectActor.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_ACTOR_H_
#define _GAMEOBJECT_ACTOR_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "gameObject3D.h"
#include "modelManager.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CGameObjectActor : public CGameObject3D
{
public:
	CGameObjectActor(const GOM::PRIORITY &pri, const OBJ_TYPE& type);
	~CGameObjectActor() {}

	//Get関数
	
	//Set関数

	//生成関数
	static CGameObjectActor* CreatePlayer(const CMOM::MODEL_NAME &modelName, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale);
	static CGameObjectActor* CreateEnemy(const CMOM::MODEL_NAME &modelName, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale);
private:
};

#endif
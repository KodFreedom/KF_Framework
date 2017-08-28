//--------------------------------------------------------------------------------
//	アクター
//　gameObjectActor.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "gameObject.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CAnimatorComponent;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CGameObjectActor : public CGameObject
{
public:
	CGameObjectActor(const GOM::PRIORITY &pri, const OBJ_TYPE& type);
	~CGameObjectActor() {}

	bool Init(void) override;
	void Uninit(void) override;
	void LateUpdate(void) override;

	//Get関数
	CAnimatorComponent* GetAnimatorComponent(void) { return m_pAnimator; }
	
	//Set関数

	//生成関数
	static CGameObjectActor* CreatePlayer(const string &modelPath, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale);
	static CGameObjectActor* CreateEnemy(const string &modelPath, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale);
private:
	CAnimatorComponent* m_pAnimator;
};

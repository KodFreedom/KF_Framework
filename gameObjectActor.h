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
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	CGameObjectActor(const GOMLAYER& layer = L_DEFAULT);
	~CGameObjectActor() {}

	bool Init(void) override;
	void LateUpdate(void) override;

	//Get関数
	auto GetAnimatorComponent(void) const { return m_pAnimator; }
	
	//Set関数

	//生成関数
	static CGameObjectActor* CreatePlayer(const string &modelPath, const Vector3 &Position, const Vector3 &vRot, const Vector3 &vScale);
	static CGameObjectActor* CreateEnemy(const string &modelPath, const Vector3 &Position, const Vector3 &vRot, const Vector3 &vScale);

private:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	void uninit(void) override;

	//--------------------------------------------------------------------------------
	//  変数宣言
	//--------------------------------------------------------------------------------
	CAnimatorComponent* m_pAnimator;
};

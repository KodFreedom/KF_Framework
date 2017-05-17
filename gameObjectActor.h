//--------------------------------------------------------------------------------
//
//　gameObjectActor.h
//	Author : Xu Wenjie
//	Date   : 2017-05-15
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_ACTOR_H_
#define _GAMEOBJECT_ACTOR_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "gameObjectModel.h"
#include "modelManager.h"
#include "modelActorX.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CGameObjectActor : public CGameObjectModel
{
public:
	CGameObjectActor();
	~CGameObjectActor() {};

	void	Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName);
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override;
	void	Draw(void) override;

	static CGameObjectActor*	Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName);

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	enum MOTION
	{
		MOTION_NEUTAL = 0,
		MOTION_MOVE,
		MOTION_ATTACK,
		MOTION_JUMP,
		MOTION_LANDING,
		MOTION_MAX
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void	UpdateMotion(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	MOTION	m_motionNow;	//今のモーション
	int		m_nKeyNow;		//今のキーフレーム
	int		m_nCntFrame;	//今のフレーム
	std::vector<CModelActorX::PARTS_INFO>	m_vectorPartsInfo;	// パーツ情報
};

#endif
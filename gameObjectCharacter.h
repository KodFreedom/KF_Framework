//--------------------------------------------------------------------------------
//
//　gameObjectCharacter.h
//	Author : Xu Wenjie
//	Date   : 2017-05-15
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_CHARACTER_H_
#define _GAMEOBJECT_CHARACTER_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "gameObjectModel.h"
#include "modelManager.h"
#include "modelCharacterX.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CGameObjectCharacter : public CGameObjectModel
{
public:
	CGameObjectCharacter();
	~CGameObjectCharacter() {};

	void	Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName);
	void	Uninit(void);
	void	Update(void);
	void	LateUpdate(void);
	void	Draw(void);

	static CGameObjectCharacter*	Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName);

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

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	MOTION	m_motionNow;	//今のモーション
	int		m_nKeyNow;		//今のキーフレーム
	int		m_nCntFrame;	//今のフレーム
	std::vector<CModelCharacterX::PARTS_INFO>	m_vectorPartsInfo;	// パーツ情報
};

#endif
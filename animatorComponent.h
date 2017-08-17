//--------------------------------------------------------------------------------
//
//　animatorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "component.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CMotion;
class CMotionInfo;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  アニメーターコンポネントクラス
//--------------------------------------------------------------------------------
class CAnimatorComponent : public CComponent
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CAnimatorComponent(CGameObject* const pGameObj, const string& strPath);
	~CAnimatorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void);

	//Set関数
	void	SetAttack(const bool& bAttack);
	void	SetGrounded(const bool& bGrounded);
	void	SetJump(const bool& bJump);
	void	SetMove(const float& fMovement);

	//Get関数

private:
	//--------------------------------------------------------------------------------
	//  列挙型定義
	//--------------------------------------------------------------------------------
	enum MOTION_PATTERN
	{
		MP_NEUTAL,
		MP_RUN,
		MP_ATTACK,
		MP_JUMP,
		MP_LAND,
		MP_MAX
	};
	
	enum MOTION_STATUS
	{
		MS_NORMAL,
		MS_CHANGE
	};

	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static const int	sc_nChangeFrame = 10;

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void analyzeFile(const string& strPath);
	void changeMotion(const MOTION_PATTERN& motion);
	//void changeMotionImmediately(const MOTION_PATTERN& motion);
	bool checkCanChange(const MOTION_PATTERN& motion);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	CMotionInfo*						m_apMotionData[MP_MAX];
	list<CGameObject*>					m_listNodes;
	MOTION_PATTERN						m_motionNow;
	MOTION_PATTERN						m_motionNext;
	MOTION_STATUS						m_status;
	int									m_nKeyNow;	
	int									m_nCntFrame;
	int									m_nCntChangeFrame;
};

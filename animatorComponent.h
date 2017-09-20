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
#include "motionStatus.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CMotion;
class CMotionInfo;
class CMotionKey;
class CColliderComponent;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  アニメーターコンポネントクラス
//--------------------------------------------------------------------------------
class CAnimatorComponent : public CComponent
{
	//--------------------------------------------------------------------------------
	//  フレンドクラス
	//--------------------------------------------------------------------------------
	friend class CNormalMotionStatus;
	friend class CAwaitMotionStatus;
	friend class CBlendMotionStatus;

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
	bool	CanAct(void);

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void			analyzeFile(const string& strPath);
	//void			updateAttack(void);
	void			changeMotionStatus(CMotionStatus* pMotionStatus);
	MOTION_PATTERN	getMotionNext(const MOTION_PATTERN& motion);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	CMotionInfo*				m_apMotionData[MP_MAX];
	vector<CGameObject*>		m_vecBorns;
	MOTION_PATTERN				m_motionNow;
	MOTION_PATTERN				m_motionNext;
	CMotionStatus*				m_pMotionStatus;
	bool						m_bIsGrounded;
};

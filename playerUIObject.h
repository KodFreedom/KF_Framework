//--------------------------------------------------------------------------------
//
//　PlayerUIObject.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "2DUIObject.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CPlayerBehaviorComponent;

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class CPlayerUIObject : public C2DUIObject
{
public:
	CPlayerUIObject(CPlayerBehaviorComponent* const pPlayerBehavior)
		: C2DUIObject(0)
		, m_pPlayerBehavior(pPlayerBehavior)
		, m_vFaceUVBegin(CKFVec2(0.0f)) {}
	~CPlayerUIObject() {}

	bool Init(void) override;
	void Update(void) override;

	static CPlayerUIObject* Create(CPlayerBehaviorComponent* const pPlayerBehavior);
	
private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static const float		sc_fScreenRate;
	static const CKFVec2	sc_vLifeGaugeSize;
	static const CKFVec2	sc_vLifeGaugePosLeftTop;
	static const CKFColor	sc_cLifeGaugeColorMax;
	static const CKFColor	sc_cLifeGaugeColorMin;
	static const CKFVec2	sc_vCoverSize;
	static const CKFVec2	sc_vCoverPosCenter;
	static const CKFVec2	sc_vFaceSize;
	static const CKFVec2	sc_vFacePosCenter;
	static const CKFVec2	sc_vFaceUVSize;

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	static bool					s_bCreated;
	CKFVec2						m_vFaceUVBegin;
	CPlayerBehaviorComponent*	m_pPlayerBehavior;
};
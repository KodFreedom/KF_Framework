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
	CPlayerUIObject(CPlayerBehaviorComponent* const pPlayerBehavior) : m_pPlayerBehavior(pPlayerBehavior), m_vFaceUVBegin(CKFVec2(0.0f)) {}
	~CPlayerUIObject() {}

	bool Init(void) override;
	void Update(void) override;

	static CPlayerUIObject* Create(CPlayerBehaviorComponent* const pPlayerBehavior);
	
private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static const float		c_fScreenRate;
	static const CKFVec2	c_vLifeGaugeSize;
	static const CKFVec2	c_vLifeGaugePosLeftTop;
	static const CKFColor	c_cLifeGaugeColorMax;
	static const CKFColor	c_cLifeGaugeColorMin;
	static const CKFVec2	c_vCoverSize;
	static const CKFVec2	c_vCoverPosCenter;
	static const CKFVec2	c_vFaceSize;
	static const CKFVec2	c_vFacePosCenter;
	static const CKFVec2	c_vFaceUVSize;

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	CKFVec2						m_vFaceUVBegin;
	CPlayerBehaviorComponent*	m_pPlayerBehavior;
};
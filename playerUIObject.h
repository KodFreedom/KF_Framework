//--------------------------------------------------------------------------------
//
//�@PlayerUIObject.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "2DUIObject.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CPlayerBehaviorComponent;

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class CPlayerUIObject : public C2DUIObject
{
public:
	CPlayerUIObject(CPlayerBehaviorComponent* const pPlayerBehavior)
		: C2DUIObject(0)
		, m_pPlayerBehavior(pPlayerBehavior)
		, m_vFaceUVBegin(Vector2(0.0f)) {}
	~CPlayerUIObject() {}

	bool Init(void) override;
	void Update(void) override;

	static CPlayerUIObject* Create(CPlayerBehaviorComponent* const pPlayerBehavior);
	
private:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const float		sc_fScreenRate;
	static const Vector2	sc_vLifeGaugeSize;
	static const Vector2	sc_vLifeGaugePosLeftTop;
	static const Color	sc_cLifeGaugeColorMax;
	static const Color	sc_cLifeGaugeColorMin;
	static const Vector2	sc_vCoverSize;
	static const Vector2	sc_vCoverPosCenter;
	static const Vector2	sc_vFaceSize;
	static const Vector2	sc_vFacePosCenter;
	static const Vector2	sc_vFaceUVSize;

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	Vector2						m_vFaceUVBegin;
	CPlayerBehaviorComponent*	m_pPlayerBehavior;
};
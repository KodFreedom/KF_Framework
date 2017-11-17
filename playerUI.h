//--------------------------------------------------------------------------------
//
//�@playerUI.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "UIObject2D.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class PlayerController;

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class PlayerUI : public UIObject2D
{
public:
	PlayerUI(PlayerController* const playerBehavior)
		: UIObject2D(0)
		, playerBehavior(playerBehavior)
		, faceUVBegin(Vector2::Zero) {}
	~PlayerUI() {}

	bool Init(void) override;
	void Update(void) override;

	static PlayerUI* Create(PlayerController* const playerBehavior);
	
private:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const float	 screenRate;
	static const Vector2 lifeGaugeSize;
	static const Vector2 lifeGaugePosLeftTop;
	static const Color	 lifeGaugeColorMax;
	static const Color	 lifeGaugeColorMin;
	static const Vector2 coverSize;
	static const Vector2 coverCenter;
	static const Vector2 faceSize;
	static const Vector2 faceCenter;
	static const Vector2 faceUVSize;

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	Vector2						faceUVBegin;
	PlayerController*	playerBehavior;
};
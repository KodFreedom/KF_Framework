//--------------------------------------------------------------------------------
//
//　playerUI.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "UIObject2D.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class PlayerController;

//--------------------------------------------------------------------------------
//  クラス
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
	//  定数定義
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
	//  変数定義
	//--------------------------------------------------------------------------------
	Vector2						faceUVBegin;
	PlayerController*	playerBehavior;
};
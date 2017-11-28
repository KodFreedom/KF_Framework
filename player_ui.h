//--------------------------------------------------------------------------------
//　player_ui.h
//	player uiオブジェクト
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "ui_object_2d.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class PlayerController;

//--------------------------------------------------------------------------------
//  player uiオブジェクトクラス
//--------------------------------------------------------------------------------
class PlayerUI : public UIObject2D
{
public:
	//--------------------------------------------------------------------------------
	//  初期化処理
	//--------------------------------------------------------------------------------
	bool Init(void) override;

	//--------------------------------------------------------------------------------
	//  更新処理
	//--------------------------------------------------------------------------------
	void Update(void) override;

	//--------------------------------------------------------------------------------
	//  作成処理
	//--------------------------------------------------------------------------------
	static PlayerUI* Create(PlayerController* const player);
	
private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static constexpr float kScreenRate = static_cast<float>(SCREEN_WIDTH) / 1920.0f;
	static const Vector2 kLifeGaugeSize;
	static const Vector2 kLifeGaugePositionLeftTop;
	static const Color	 kLifeGaugeColorMax;
	static const Color	 kLifeGaugeColorMin;
	static const Vector2 kCoverSize;
	static const Vector2 kCoverCenter;
	static const Vector2 kFaceSize;
	static const Vector2 kFaceCenter;
	static const Vector2 kFaceUVSize;

	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	PlayerUI(PlayerController* const playerBehavior)
		: UIObject2D(0)
		, player_(playerBehavior)
		, face_uv_begin_(Vector2::kZero) {}
	PlayerUI(const PlayerUI& value) {}
	PlayerUI& operator=(const PlayerUI& value) {}
	~PlayerUI() {}

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	Vector2 face_uv_begin_;
	PlayerController* player_;
};
//--------------------------------------------------------------------------------
//�@player_ui.h
//	player ui�I�u�W�F�N�g
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "ui_object_2d.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class PlayerController;

//--------------------------------------------------------------------------------
//  player ui�I�u�W�F�N�g�N���X
//--------------------------------------------------------------------------------
class PlayerUI : public UIObject2D
{
public:
	//--------------------------------------------------------------------------------
	//  ����������
	//--------------------------------------------------------------------------------
	bool Init(void) override;

	//--------------------------------------------------------------------------------
	//  �X�V����
	//--------------------------------------------------------------------------------
	void Update(void) override;

	//--------------------------------------------------------------------------------
	//  �쐬����
	//--------------------------------------------------------------------------------
	static PlayerUI* Create(PlayerController* const player);
	
private:
	//--------------------------------------------------------------------------------
	//  �萔��`
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
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	Vector2 face_uv_begin_;
	PlayerController* player_;
};
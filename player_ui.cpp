//--------------------------------------------------------------------------------
//　player_ui.cpp
//	player uiオブジェクト
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "player_ui.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const Vector2 PlayerUI::kLifeGaugeSize = Vector2(540.0f, 33.0f) * kScreenRate;
const Vector2 PlayerUI::kLifeGaugePositionLeftTop = Vector2(287.0f, 960.0f) * kScreenRate;
const Color   PlayerUI::kLifeGaugeColorMax = Color::kGreen;
const Color   PlayerUI::kLifeGaugeColorMin = Color::kRed;
const Vector2 PlayerUI::kCoverSize = Vector2(804.0f, 179.0f) * kScreenRate;
const Vector2 PlayerUI::kCoverCenter = Vector2(35.0f, 883.0f) * kScreenRate + kCoverSize * 0.5f;
const Vector2 PlayerUI::kFaceSize = Vector2(153.0f, 169.0f) * kScreenRate;
const Vector2 PlayerUI::kFaceCenter = Vector2(63.0f, 840.0f) * kScreenRate + kFaceSize * 0.5f;
const Vector2 PlayerUI::kFaceUVSize = Vector2(0.25f, 1.0f);

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool PlayerUI::Init(void)
{
//	//HPGauge
//	SPRITE lifeSprite;
//	lifeSprite.polygonNumber = 2;
//	lifeSprite.texture = "playerUILifeGauge.png";
//	TextureManager::Instance()->Use(lifeSprite.texture);
//#ifdef USING_DIRECTX
//	UtilityDX::MakeVertexGauge(lifeSprite.pVtxBuffer, kLifeGaugePositionLeftTop, kLifeGaugeSize, kLifeGaugeColorMax);
//#endif
//	sprites.push_back(lifeSprite);
//
//	//EPGauge
//
//	//Skill Icon
//
//	//UltraGauge
//
//	//Covor
//	SPRITE coverSprite;
//	coverSprite.polygonNumber = 2;
//	coverSprite.texture = "playerUICover.png";
//	TextureManager::Instance()->Use(coverSprite.texture);
//#ifdef USING_DIRECTX
//	UtilityDX::MakeVertex(coverSprite.pVtxBuffer, kCoverCenter, kCoverSize);
//#endif
//	sprites.push_back(coverSprite);
//
//	//Face顔
//	SPRITE faceSprite;
//	faceSprite.polygonNumber = 2;
//	faceSprite.texture = "playerUIFace.png";
//	TextureManager::Instance()->Use(faceSprite.texture);
//#ifdef USING_DIRECTX
//	UtilityDX::MakeVertex(faceSprite.pVtxBuffer, kFaceCenter, kFaceSize, faceUVBegin, kFaceUVSize);
//#endif
//	sprites.push_back(faceSprite);
	return true;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void PlayerUI::Update(void)
{
//	auto iterator = sprites.begin();
//	auto& paramater = playerBehavior->GetParamater();
//
//	//To do
//	//HPゲージ更新
//	float lifeRate = paramater.GetCurrentLife() / paramater.GetMaxLife();
//	Color color = Math::Lerp(kLifeGaugeColorMin, kLifeGaugeColorMax, lifeRate);
//#ifdef USING_DIRECTX
//	UtilityDX::UpdateVertexGauge(iterator->pVtxBuffer, kLifeGaugePositionLeftTop, kLifeGaugeSize, lifeRate, color);
//#endif
//	++iterator;
//
//	//EPゲージ更新
//	//++iterator;
//
//	//ウルトラゲージ更新
//	//++iterator;
//
//	//スキルIcon更新
//	//++iterator;
//
//	//スタイルによってCoverのいろを変える
//	++iterator;
//
//	//HPによって表情を変える
//	float faceU = 0.25f * (int)((0.75f - lifeRate) * 4.0f);
//	faceU = faceU < 0.0f ? 0.0f : faceU;
//#ifdef USING_DIRECTX
//	UtilityDX::UpdateUV(iterator->pVtxBuffer, Vector2(faceU, 0.0f), kFaceUVSize);
//#endif
}

//--------------------------------------------------------------------------------
//  作成
//--------------------------------------------------------------------------------
PlayerUI* PlayerUI::Create(PlayerController* const playerBehavior)
{
	PlayerUI* ui = MY_NEW PlayerUI(playerBehavior);
	ui->Init();
	return ui;
}
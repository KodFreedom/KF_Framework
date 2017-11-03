//--------------------------------------------------------------------------------
//
//�@PlayerUIObject.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "textureManager.h"
#include "playerUI.h"
#include "playerBehaviorComponent.h"
#include "actorBehaviorComponent.h"

#ifdef USING_DIRECTX
#include "KF_UtilityDX.h"
#endif

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
const float		PlayerUI::screenRate = (float)SCREEN_WIDTH / 1920.0f;
const Vector2	PlayerUI::lifeGaugeSize = Vector2(540.0f, 33.0f) * screenRate;
const Vector2	PlayerUI::lifeGaugePosLeftTop = Vector2(287.0f, 960.0f) * screenRate;
const Color		PlayerUI::lifeGaugeColorMax = Color::Green;
const Color		PlayerUI::lifeGaugeColorMin = Color::Red;
const Vector2	PlayerUI::coverSize = Vector2(804.0f, 179.0f) * screenRate;
const Vector2	PlayerUI::coverCenter = Vector2(35.0f, 883.0f) * screenRate + coverSize * 0.5f;
const Vector2	PlayerUI::faceSize = Vector2(153.0f, 169.0f) * screenRate;
const Vector2	PlayerUI::faceCenter = Vector2(63.0f, 840.0f) * screenRate + faceSize * 0.5f;
const Vector2	PlayerUI::faceUVSize = Vector2(0.25f, 1.0f);

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool PlayerUI::Init(void)
{
	//HPGauge
	SPRITE lifeSprite;
	lifeSprite.polygonNumber = 2;
	lifeSprite.texture = "playerUILifeGauge.png";
	TextureManager::Instance()->Use(lifeSprite.texture);
#ifdef USING_DIRECTX
	UtilityDX::MakeVertexGauge(lifeSprite.pVtxBuffer, lifeGaugePosLeftTop, lifeGaugeSize, lifeGaugeColorMax);
#endif
	sprites.push_back(lifeSprite);

	//EPGauge

	//Skill Icon

	//UltraGauge

	//Covor
	SPRITE coverSprite;
	coverSprite.polygonNumber = 2;
	coverSprite.texture = "playerUICover.png";
	TextureManager::Instance()->Use(coverSprite.texture);
#ifdef USING_DIRECTX
	UtilityDX::MakeVertex(coverSprite.pVtxBuffer, coverCenter, coverSize);
#endif
	sprites.push_back(coverSprite);

	//Face��
	SPRITE faceSprite;
	faceSprite.polygonNumber = 2;
	faceSprite.texture = "playerUIFace.png";
	TextureManager::Instance()->Use(faceSprite.texture);
#ifdef USING_DIRECTX
	UtilityDX::MakeVertex(faceSprite.pVtxBuffer, faceCenter, faceSize, faceUVBegin, faceUVSize);
#endif
	sprites.push_back(faceSprite);
	return true;
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerUI::Update(void)
{
	auto iterator = sprites.begin();
	auto& actorBehavior = playerBehavior->GetActorBehavior();

	//To do
	//HP�Q�[�W�X�V
	float lifeRate = actorBehavior.GetLifeNow() / actorBehavior.GetLifeMax();
	Color color = Math::Lerp(lifeGaugeColorMin, lifeGaugeColorMax, lifeRate);
#ifdef USING_DIRECTX
	UtilityDX::UpdateVertexGauge(iterator->pVtxBuffer, lifeGaugePosLeftTop, lifeGaugeSize, lifeRate, color);
#endif
	++iterator;

	//EP�Q�[�W�X�V
	//++iterator;

	//�E���g���Q�[�W�X�V
	//++iterator;

	//�X�L��Icon�X�V
	//++iterator;

	//�X�^�C���ɂ����Cover�̂����ς���
	++iterator;

	//HP�ɂ���ĕ\���ς���
	float faceU = 0.25f * (int)((0.75f - lifeRate) * 4.0f);
	faceU = faceU < 0.0f ? 0.0f : faceU;
#ifdef USING_DIRECTX
	UtilityDX::UpdateUV(iterator->pVtxBuffer, Vector2(faceU, 0.0f), faceUVSize);
#endif
}

//--------------------------------------------------------------------------------
//  �쐬
//--------------------------------------------------------------------------------
PlayerUI* PlayerUI::Create(CPlayerBehaviorComponent* const playerBehavior)
{
	PlayerUI* ui = new PlayerUI(playerBehavior);
	ui->Init();
	return ui;
}
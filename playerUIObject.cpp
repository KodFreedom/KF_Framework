//--------------------------------------------------------------------------------
//
//　PlayerUIObject.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "playerUIObject.h"
#include "playerBehaviorComponent.h"

#ifdef USING_DIRECTX
#include "KF_UtilityDX.h"
#endif

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const float		CPlayerUIObject::sc_fScreenRate = (float)SCREEN_WIDTH / 1920.0f;
const CKFVec2	CPlayerUIObject::sc_vLifeGaugeSize = CKFVec2(540.0f, 33.0f) * sc_fScreenRate;
const CKFVec2	CPlayerUIObject::sc_vLifeGaugePosLeftTop = CKFVec2(287.0f, 960.0f) * sc_fScreenRate;
const CKFColor	CPlayerUIObject::sc_cLifeGaugeColorMax = CKFColor(0.0f, 1.0f, 0.0f, 1.0f);
const CKFColor	CPlayerUIObject::sc_cLifeGaugeColorMin = CKFColor(1.0f, 0.0f, 0.0f, 1.0f);
const CKFVec2	CPlayerUIObject::sc_vCoverSize = CKFVec2(804.0f, 179.0f) * sc_fScreenRate;
const CKFVec2	CPlayerUIObject::sc_vCoverPosCenter = CKFVec2(35.0f, 883.0f) * sc_fScreenRate + sc_vCoverSize * 0.5f;
const CKFVec2	CPlayerUIObject::sc_vFaceSize = CKFVec2(153.0f, 169.0f) * sc_fScreenRate;
const CKFVec2	CPlayerUIObject::sc_vFacePosCenter = CKFVec2(63.0f, 840.0f) * sc_fScreenRate + sc_vFaceSize * 0.5f;
const CKFVec2	CPlayerUIObject::sc_vFaceUVSize = CKFVec2(0.25f, 1.0f);

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool CPlayerUIObject::Init(void)
{
	CTextureManager* pTexManager = GetManager()->GetTextureManager();
	
	//HPGauge
	SPRITE sHP;
	sHP.usNumPolygon = 2;
	sHP.strTexName = "playerUILifeGauge.png";
	pTexManager->UseTexture(sHP.strTexName);
#ifdef USING_DIRECTX
	CKFUtilityDX::MakeVertexGauge(sHP.pVtxBuffer, sc_vLifeGaugePosLeftTop, sc_vLifeGaugeSize, sc_cLifeGaugeColorMax);
#endif
	m_listSprite.push_back(sHP);

	//EPGauge

	//Skill Icon

	//UltraGauge

	//Covor
	SPRITE sCover;
	sCover.usNumPolygon = 2;
	sCover.strTexName = "playerUICover.png";
	pTexManager->UseTexture(sCover.strTexName);
#ifdef USING_DIRECTX
	CKFUtilityDX::MakeVertex(sCover.pVtxBuffer, sc_vCoverPosCenter, sc_vCoverSize);
#endif
	m_listSprite.push_back(sCover);

	//Face顔
	SPRITE sFace;
	sFace.usNumPolygon = 2;
	sFace.strTexName = "playerUIFace.png";
	pTexManager->UseTexture(sFace.strTexName);
#ifdef USING_DIRECTX
	CKFUtilityDX::MakeVertex(sFace.pVtxBuffer, sc_vFacePosCenter, sc_vFaceSize, m_vFaceUVBegin, sc_vFaceUVSize);
#endif
	m_listSprite.push_back(sFace);

	return true;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CPlayerUIObject::Update(void)
{
	auto itr = m_listSprite.begin();

	//To do
	//HPゲージ更新
	float fLifeRate = m_pPlayerBehavior->GetLifeNow() / m_pPlayerBehavior->GetLifeMax();
	CKFColor cColor = CKFMath::LerpColor(sc_cLifeGaugeColorMin, sc_cLifeGaugeColorMax, fLifeRate);
#ifdef USING_DIRECTX
	CKFUtilityDX::UpdateVertexGauge(itr->pVtxBuffer, sc_vLifeGaugePosLeftTop, sc_vLifeGaugeSize, fLifeRate, cColor);
#endif
	++itr;

	//EPゲージ更新
	//++itr;

	//ウルトラゲージ更新
	//++itr;

	//スキルIcon更新
	//++itr;

	//スタイルによってCoverのいろを変える
	++itr;

	//HPによって表情を変える
	float fFaceU = 0.25f * (int)((0.75f - fLifeRate) * 4.0f);
	fFaceU = fFaceU < 0.0f ? 0.0f : fFaceU;
#ifdef USING_DIRECTX
	CKFUtilityDX::UpdateUV(itr->pVtxBuffer, CKFVec2(fFaceU, 0.0f), sc_vFaceUVSize);
#endif
}

//--------------------------------------------------------------------------------
//  作成
//--------------------------------------------------------------------------------
CPlayerUIObject* CPlayerUIObject::Create(CPlayerBehaviorComponent* const pPlayerBehavior)
{
	CPlayerUIObject* pUI = new CPlayerUIObject(pPlayerBehavior);
	pUI->Init();
	return pUI;
}
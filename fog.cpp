//--------------------------------------------------------------------------------
//
//　fog.cpp
//	Author : Xu Wenjie
//	Date   : 2017-10-09
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "fog.h"
#include "manager.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif // USING_DIRECTX


//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Public
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CFog::CFog()
	: m_bEnable(true)
	, m_bEnableRangeFog(true)
	, m_fStart(1.0f)
	, m_fEnd(1000.0f)
	, m_fDensity(0.01f)
	, m_cColor(CKFMath::sc_cWhite)
{

}

//--------------------------------------------------------------------------------
//  フォグの使用の設定
//--------------------------------------------------------------------------------
void CFog::SetEnable(const bool& bEnable)
{
	m_bEnable = bEnable;
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_FOGENABLE, m_bEnable);
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  範囲ベースのフォグの使用の設定
//--------------------------------------------------------------------------------
void CFog::SetEnableRangeFog(const bool& bEnable)
{
	m_bEnableRangeFog = bEnable;
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, m_bEnableRangeFog);
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  フォグ開始点設定
//--------------------------------------------------------------------------------
void CFog::SetStart(const float& fStart)
{
	m_fStart = fStart;
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_FOGSTART, *((LPDWORD)(&m_fStart)));
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  フォグ終了点設定
//--------------------------------------------------------------------------------
void CFog::SetEnd(const float& fEnd)
{
	m_fEnd = fEnd;
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_FOGEND, *((LPDWORD)(&m_fEnd)));
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  フォグ密度設定
//--------------------------------------------------------------------------------
void CFog::SetDensity(const float& fDensity)
{
	m_fDensity = fDensity;
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *((LPDWORD)(&m_fDensity)));
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  フォグ色設定
//--------------------------------------------------------------------------------
void CFog::SetColor(const Color& cColor)
{
	m_cColor = cColor;
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_FOGCOLOR, m_cColor);
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  Private
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CFog::init(void)
{
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	
	// フォグを有効にする
	pDevice->SetRenderState(D3DRS_FOGENABLE, m_bEnable);

	// フォグカラー設定
	pDevice->SetRenderState(D3DRS_FOGCOLOR, m_cColor);

	// バーテックスフォグ(線形公式)を使用
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);

	// フォグ範囲設定
	pDevice->SetRenderState(D3DRS_FOGSTART, *((LPDWORD)(&m_fStart)));
	pDevice->SetRenderState(D3DRS_FOGEND, *((LPDWORD)(&m_fEnd)));

	// ピクセルフォグ(指数の２ )を使用
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP2);

	// フォグ密度設定
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *((LPDWORD)(&m_fDensity)));

	// 範囲ベースのフォグを使用
	pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, m_bEnableRangeFog);
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CFog::uninit(void)
{
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();

	// フォグを無効にする
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// 範囲ベースのフォグを禁止
	pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
#endif // USING_DIRECTX
}
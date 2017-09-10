//--------------------------------------------------------------------------------
//	debug表示管理処理
//　debugManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-09-07
//--------------------------------------------------------------------------------
#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "debugManager.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif //USING_DIRECTX

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CDebugManager::CDebugManager()
	: m_usCntScroll(0)
#ifdef USING_DIRECTX
	, m_pFont(nullptr)
#endif //USING_DIRECTX
{
	m_strDebugInfo.clear();
	m_listStrDebugScroll.clear();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CDebugManager::Update(void)
{
	if (!m_listStrDebugScroll.empty())
	{
		++m_usCntScroll;
		if (m_usCntScroll >= sc_usScrollTime)
		{//一番目のstringを削除する
			m_usCntScroll = 0;
			m_listStrDebugScroll.erase(m_listStrDebugScroll.begin());
		}
		
		for (auto& str : m_listStrDebugScroll)
		{//Debug表示に追加する
			m_strDebugInfo += str;
		}
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CDebugManager::Draw(void)
{
#ifdef USING_DIRECTX
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// テキスト描画
	m_pFont->DrawText(NULL, m_strDebugInfo.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
#endif

	//前フレームの文字列を削除
	m_strDebugInfo.clear();
}

//--------------------------------------------------------------------------------
//	関数名：DisplayAlways
//  関数説明：Debug表示に常に表示したい文字列を追加する
//	引数：	strInfo：表示したい文字列
//	戻り値：なし
//--------------------------------------------------------------------------------
void CDebugManager::DisplayAlways(const string& strInfo)
{
	m_strDebugInfo.append(strInfo);
}

//--------------------------------------------------------------------------------
//	関数名：DisplayAlways
//  関数説明：Debug表示に常に表示したい文字を追加する
//	引数：	cInfo：表示したい文字
//	戻り値：なし
//--------------------------------------------------------------------------------
void CDebugManager::DisplayAlways(const char& cInfo)
{
	m_strDebugInfo.push_back(cInfo);
}

//--------------------------------------------------------------------------------
//	関数名：DisplayScroll
//  関数説明：Debugスクロール表示に表示したい文字列を追加する
//			  一定時間経ったら消えます
//	引数：	strInfo：表示したい文字列
//	戻り値：なし
//--------------------------------------------------------------------------------
void CDebugManager::DisplayScroll(const string& strInfo)
{
	m_listStrDebugScroll.push_back(strInfo);
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
void CDebugManager::init(void)
{
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();

	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif //USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CDebugManager::uninit(void)
{
#ifdef USING_DIRECTX
	// デバッグ情報表示用フォントの破棄
	SAFE_RELEASE(m_pFont);
#endif //USING_DIRECTX

	m_strDebugInfo.clear();
}

#endif//_DEBUG
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
#include "ImGui\imgui.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#include "ImGui\imgui_impl_dx9.h"
#endif // USING_DIRECTX

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
{
	m_strDebugInfo.clear();
	m_listStrDebugScroll.clear();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CDebugManager::Update(void)
{
#ifdef USING_DIRECTX
	ImGui_ImplDX9_NewFrame();
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CDebugManager::LateUpdate(void)
{
	showMainWindow();

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
	//描画
	ImGui::Render();

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
void CDebugManager::init(HWND hWnd)
{
#ifdef USING_DIRECTX
	ImGui_ImplDX9_Init(hWnd, CMain::GetManager()->GetRenderer()->GetDevice());
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CDebugManager::uninit(void)
{
#ifdef USING_DIRECTX
	ImGui_ImplDX9_Shutdown();
#endif // USING_DIRECTX

	m_strDebugInfo.clear();
}

//--------------------------------------------------------------------------------
//  showMainWindow
//--------------------------------------------------------------------------------
void CDebugManager::showMainWindow(void)
{
	auto pRenderer = CMain::GetManager()->GetRenderer();
	auto cBGColor = pRenderer->GetBGColor();
	auto bWireFrame = pRenderer->GetWireFrameFlag();

	// Begin
	if (!ImGui::Begin("Main Debug Window"))
	{
		ImGui::End();
		return;
	}

	// FPS
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	// BG Color
	ImGui::ColorEdit3("BG Color", (float*)&cBGColor);
	pRenderer->SetBGColor(cBGColor);

	// WireFrame
	if (ImGui::Checkbox("WireFrame", &bWireFrame)){ pRenderer->SetWireFrameFlag(bWireFrame); }
	
	// Camera Window
	//if (ImGui::Button("Camera")) m_bCameraWindow ^= 1;

	// End
	ImGui::End();
}
#endif//_DEBUG
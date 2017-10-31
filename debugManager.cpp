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
#include "KF_CollisionSystem.h"
#include "textureManager.h"
#include "meshManager.h"
#include "mode.h"
#include "camera.h"
#include "gameObjectActor.h"
#include "transformComponent.h"
#include "actorBehaviorComponent.h"
#include "fog.h"

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
	, m_bCollisionSystemWindow(false)
	, m_bCameraWindow(false)
	, m_bPlayerWindow(false)
	, m_bFogWindow(false)
	, m_pPlayer(nullptr)
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
	showCollisionSystemWindow();
	showCameraWindow();
	showPlayerWindow();
	showFogWindow();

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
	ImGui_ImplDX9_Init(hWnd, Main::GetManager()->GetRenderer()->GetDevice());
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
	auto pRenderer = Main::GetManager()->GetRenderer();
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
	if (ImGui::ColorEdit3("BG Color", (float*)&cBGColor))
	{
		pRenderer->SetBGColor(cBGColor);
	}
	

	// WireFrame
	if (ImGui::Checkbox("WireFrame", &bWireFrame)){ pRenderer->SetWireFrameFlag(bWireFrame); }
	
	// Collision System
	if (ImGui::Button("Collision System")) { m_bCollisionSystemWindow ^= 1; }

	// Camera Window
	if (ImGui::Button("Camera")) { m_bCameraWindow ^= 1; }

	// Player Window
	if (ImGui::Button("Player")) { m_bPlayerWindow ^= 1; }

	// Fog Window
	if (ImGui::Button("Fog")) { m_bFogWindow ^= 1; }

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  showCollisionSystemWindow
//--------------------------------------------------------------------------------
void CDebugManager::showCollisionSystemWindow(void)
{
	if (!m_bCollisionSystemWindow) { return; }
	auto pCS = Main::GetManager()->GetCollisionSystem();
	
	// Begin
	if (!ImGui::Begin("Collision System Debug Window", &m_bCollisionSystemWindow))
	{
		ImGui::End();
		return;
	}

	// Draw Collider
	ImGui::Checkbox("Show Collider", &pCS->m_bDrawCollider);

	// Num Collider
	if (ImGui::CollapsingHeader("Static"))
	{
		ImGui::Text("Sphere : %d", (int)pCS->m_alistCollider[CS::STATIC][CS::COL_SPHERE].size());
		ImGui::Text("AABB : %d", (int)pCS->m_alistCollider[CS::STATIC][CS::COL_AABB].size());
		ImGui::Text("OBB : %d", (int)pCS->m_alistCollider[CS::STATIC][CS::COL_OBB].size());
	}
	if (ImGui::CollapsingHeader("Dynamic"))
	{
		ImGui::Text("Sphere : %d", (int)pCS->m_alistCollider[CS::DYNAMIC][CS::COL_SPHERE].size());
		ImGui::Text("AABB : %d", (int)pCS->m_alistCollider[CS::DYNAMIC][CS::COL_AABB].size());
		ImGui::Text("OBB : %d", (int)pCS->m_alistCollider[CS::DYNAMIC][CS::COL_OBB].size());
	}

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  showCameraWindow
//--------------------------------------------------------------------------------
void CDebugManager::showCameraWindow(void)
{
	if (!m_bCameraWindow) { return; }
	auto pCamera = Main::GetManager()->GetMode()->GetCamera();

	// Begin
	if (!ImGui::Begin("Camera Window", &m_bCameraWindow))
	{
		ImGui::End();
		return;
	}

	// Fov
	ImGui::InputFloat("Fov", &pCamera->m_fFovY);

	// PosEye
	ImGui::Text("PosEye : %f %f %f", pCamera->m_PositionEye.X, pCamera->m_PositionEye.Y, pCamera->m_PositionEye.Z);

	// PosAt
	ImGui::Text("PosEye : %f %f %f", pCamera->m_PositionAt.X, pCamera->m_PositionAt.Y, pCamera->m_PositionAt.Z);

	// Forward
	ImGui::Text("Forward : %f %f %f", pCamera->m_vVecLook.X, pCamera->m_vVecLook.Y, pCamera->m_vVecLook.Z);

	// Up
	ImGui::Text("Up : %f %f %f", pCamera->m_vVecUp.X, pCamera->m_vVecUp.Y, pCamera->m_vVecUp.Z);

	// Right
	ImGui::Text("Right : %f %f %f", pCamera->m_vVecRight.X, pCamera->m_vVecRight.Y, pCamera->m_vVecRight.Z);

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  showPlayerWindow
//--------------------------------------------------------------------------------
void CDebugManager::showPlayerWindow(void)
{
	if (!m_bPlayerWindow || !m_pPlayer) { return; }

	// Begin
	if (!ImGui::Begin("Player Window", &m_bCameraWindow))
	{
		ImGui::End();
		return;
	}

	// Trans
	auto pTrans = m_pPlayer->GetTransformComponent();
	ImGui::InputFloat3("Trans", &pTrans->m_PositionNext.X);

	// Actor Behavior
	auto pActor = static_cast<CActorBehaviorComponent*>(m_pPlayer->GetBehaviorComponent().front());
	ImGui::InputFloat("Move Speed", &pActor->m_fMoveSpeed);
	ImGui::InputFloat("Jump Speed", &pActor->m_fJumpSpeed);
	ImGui::Text("IsGrounded : %d", (int)pActor->m_bIsGrounded);

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  showFogWindow
//--------------------------------------------------------------------------------
void CDebugManager::showFogWindow(void)
{
	if (!m_bFogWindow) { return; }
	auto pFog = Main::GetManager()->GetFog();

	// Begin
	if (!ImGui::Begin("Fog Window", &m_bFogWindow))
	{
		ImGui::End();
		return;
	}

	// Enable
	auto bEnable = pFog->GetEnable();
	if (ImGui::Checkbox("Enable", &bEnable))
	{
		pFog->SetEnable(bEnable);
	}

	// Enable RangeFog
	auto bRangeFog = pFog->GetEnableRangeFog();
	if (ImGui::Checkbox("RangeFog", &bRangeFog))
	{
		pFog->SetEnableRangeFog(bRangeFog);
	}

	// Start
	auto fStart = pFog->GetStart();
	if (ImGui::InputFloat("Start", &fStart))
	{
		pFog->SetStart(fStart);
	}

	// End
	auto fEnd = pFog->GetEnd();
	if (ImGui::InputFloat("End", &fEnd))
	{
		pFog->SetEnd(fEnd);
	}

	// Density
	auto fDensity = pFog->GetDensity();
	if (ImGui::InputFloat("Density", &fDensity))
	{
		pFog->SetDensity(fDensity);
	}

	// Color
	auto cColor = pFog->GetColor();
	if (ImGui::ColorEdit4("Color", (float*)&cColor))
	{
		pFog->SetColor(cColor);
	}

	// End
	ImGui::End();
}

#endif//_DEBUG
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
	
	// Collision System
	if (ImGui::Button("Collision System")) { m_bCollisionSystemWindow ^= 1; }

	// Camera Window
	if (ImGui::Button("Camera")) { m_bCameraWindow ^= 1; }

	// Player Window
	if (ImGui::Button("Player")) { m_bPlayerWindow ^= 1; }

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  showCollisionSystemWindow
//--------------------------------------------------------------------------------
void CDebugManager::showCollisionSystemWindow(void)
{
	if (!m_bCollisionSystemWindow) { return; }
	auto pCS = CMain::GetManager()->GetCollisionSystem();
	
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
	auto pCamera = CMain::GetManager()->GetMode()->GetCamera();

	// Begin
	if (!ImGui::Begin("Camera Window", &m_bCameraWindow))
	{
		ImGui::End();
		return;
	}

	// Fov
	ImGui::InputFloat("Fov", &pCamera->m_fFovY);

	// PosEye
	ImGui::Text("PosEye : %f %f %f", pCamera->m_vPosEye.m_fX, pCamera->m_vPosEye.m_fY, pCamera->m_vPosEye.m_fZ);

	// PosAt
	ImGui::Text("PosEye : %f %f %f", pCamera->m_vPosAt.m_fX, pCamera->m_vPosAt.m_fY, pCamera->m_vPosAt.m_fZ);

	// Forward
	ImGui::Text("Forward : %f %f %f", pCamera->m_vVecLook.m_fX, pCamera->m_vVecLook.m_fY, pCamera->m_vVecLook.m_fZ);

	// Up
	ImGui::Text("Up : %f %f %f", pCamera->m_vVecUp.m_fX, pCamera->m_vVecUp.m_fY, pCamera->m_vVecUp.m_fZ);

	// Right
	ImGui::Text("Right : %f %f %f", pCamera->m_vVecRight.m_fX, pCamera->m_vVecRight.m_fY, pCamera->m_vVecRight.m_fZ);

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
	ImGui::InputFloat3("Trans", &pTrans->m_vPosNext.m_fX);

	// Actor Behavior
	auto pActor = static_cast<CActorBehaviorComponent*>(m_pPlayer->GetBehaviorComponent().front());
	ImGui::InputFloat("Move Speed", &pActor->m_fMoveSpeed);
	ImGui::InputFloat("Jump Speed", &pActor->m_fJumpSpeed);
	ImGui::Text("IsGrounded : %d", (int)pActor->m_bIsGrounded);

	// End
	ImGui::End();
}

#endif//_DEBUG
//--------------------------------------------------------------------------------
//	debug�ώ@��
//�@debugObserver.cpp
//	Author : Xu Wenjie
//	Date   : 2017-09-07
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "versionSetting.h"
#if defined(_DEBUG) || defined(EDITOR)
#include "main.h"
#include "manager.h"
#include "debugObserver.h"
#include "renderer.h"
#include "ImGui\imgui.h"
#include "collisionSystem.h"
#include "textureManager.h"
#include "meshManager.h"
#include "mode.h"
#include "camera.h"
#include "gameObjectActor.h"
#include "transform.h"
#include "actorBehaviorComponent.h"
#include "fog.h"

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include "ImGui\imgui_impl_dx9.h"
#endif

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FCreate
//  �֐������F��������
//	�����F	hInstance�F�l
//			hWnd�F
//			isWindowMode�F
//	�߂�l�FDebugObserver*
//--------------------------------------------------------------------------------
DebugObserver* DebugObserver::Create(HWND hWnd)
{
	if (instance) return instance;
	instance = new DebugObserver;
	instance->init(hWnd);
	return instance;
}

//--------------------------------------------------------------------------------
//	�֐����FRelease
//  �֐������F�j������
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void DebugObserver::Release(void)
{
	SAFE_UNINIT(instance);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void DebugObserver::Update(void)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	ImGui_ImplDX9_NewFrame();
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void DebugObserver::LateUpdate(void)
{
	showMainWindow();
	showCollisionSystemWindow();
	showCameraWindow();
	showPlayerWindow();
	showFogWindow();

	if (!debugLog.empty())
	{
		++crollcounter;
		if (crollcounter >= scrollTime)
		{//��Ԗڂ�string���폜����
			crollcounter = 0;
			debugLog.erase(debugLog.begin());
		}

		for (auto& str : debugLog)
		{//Debug�\���ɒǉ�����
			debugInfo += str;
		}
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void DebugObserver::Draw(void)
{
	//�`��
	ImGui::Render();

	//�O�t���[���̕�������폜
	debugInfo.clear();
}

//--------------------------------------------------------------------------------
//	�֐����FDisplayAlways
//  �֐������FDebug�\���ɏ�ɕ\���������������ǉ�����
//	�����F	strInfo�F�\��������������
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void DebugObserver::DisplayAlways(const string& strInfo)
{
	debugInfo.append(strInfo);
}

//--------------------------------------------------------------------------------
//	�֐����FDisplayAlways
//  �֐������FDebug�\���ɏ�ɕ\��������������ǉ�����
//	�����F	cInfo�F�\������������
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void DebugObserver::DisplayAlways(const char& cInfo)
{
	debugInfo.push_back(cInfo);
}

//--------------------------------------------------------------------------------
//	�֐����FDisplayScroll
//  �֐������FDebug�X�N���[���\���ɕ\���������������ǉ�����
//			  ��莞�Ԍo����������܂�
//	�����F	strInfo�F�\��������������
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void DebugObserver::DisplayScroll(const string& strInfo)
{
	debugLog.push_back(strInfo);
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
DebugObserver::DebugObserver()
	: crollcounter(0)
	, enableCollisionSystemWindow(false)
	, enableCameraWindow(false)
	, enablePlayerWindow(false)
	, enableFogWindow(false)
	, player(nullptr)
{
	debugInfo.clear();
	debugLog.clear();
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
void DebugObserver::init(HWND hWnd)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	ImGui_ImplDX9_Init(hWnd, Main::GetManager()->GetRenderer()->GetDevice());
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void DebugObserver::uninit(void)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	ImGui_ImplDX9_Shutdown();
#endif // USING_DIRECTX

	debugInfo.clear();
}

//--------------------------------------------------------------------------------
//  showMainWindow
//--------------------------------------------------------------------------------
void DebugObserver::showMainWindow(void)
{
	auto renderer = Renderer::Instance();
	auto backgroundColor = renderer->GetBackgroundColor();
	auto enableWireFrame = renderer->GetWireFrameFlag();

	// Begin
	if (!ImGui::Begin("Main Debug Window"))
	{
		ImGui::End();
		return;
	}

	// FPS
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
	// BG Color
	if (ImGui::ColorEdit3("BG Color", (float*)&backgroundColor))
	{
		renderer->SetBackgroundColor(backgroundColor);
	}

	// WireFrame
	if (ImGui::Checkbox("WireFrame", &enableWireFrame)) { renderer->SetWireFrameFlag(enableWireFrame); }
	
	// Collision System
	if (ImGui::Button("Collision System")) { enableCollisionSystemWindow ^= 1; }

	// Camera Window
	if (ImGui::Button("Camera")) { enableCameraWindow ^= 1; }

	// Player Window
	if (ImGui::Button("Player")) { enablePlayerWindow ^= 1; }

	// Fog Window
	if (ImGui::Button("Fog")) { enableFogWindow ^= 1; }

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  showCollisionSystemWindow
//--------------------------------------------------------------------------------
void DebugObserver::showCollisionSystemWindow(void)
{
	if (!enableCollisionSystemWindow) { return; }
	auto pCS = CollisionSystem::Instance();
	
	// Begin
	if (!ImGui::Begin("Collision System Debug Window", &enableCollisionSystemWindow))
	{
		ImGui::End();
		return;
	}

	// Draw Collider
	ImGui::Checkbox("Show Collider", &pCS->m_bDrawCollider);

	// Num Collider
	if (ImGui::CollapsingHeader("Static"))
	{
		ImGui::Text("Sphere : %d", (int)pCS->m_alistCollider[STATIC][Sphere].size());
		ImGui::Text("AABB : %d", (int)pCS->m_alistCollider[STATIC][AABB].size());
		ImGui::Text("OBB : %d", (int)pCS->m_alistCollider[STATIC][OBB].size());
	}
	if (ImGui::CollapsingHeader("Dynamic"))
	{
		ImGui::Text("Sphere : %d", (int)pCS->m_alistCollider[DYNAMIC][Sphere].size());
		ImGui::Text("AABB : %d", (int)pCS->m_alistCollider[DYNAMIC][AABB].size());
		ImGui::Text("OBB : %d", (int)pCS->m_alistCollider[DYNAMIC][OBB].size());
	}

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  showCameraWindow
//--------------------------------------------------------------------------------
void DebugObserver::showCameraWindow(void)
{
	if (!enableCameraWindow) { return; }
	auto pCamera = Main::GetManager()->GetMode()->GetCamera();

	// Begin
	if (!ImGui::Begin("Camera Window", &enableCameraWindow))
	{
		ImGui::End();
		return;
	}

	// Fov
	ImGui::InputFloat("Fov", &pCamera->m_fFovY);

	// PosEye
	ImGui::Text("PosEye : %f %f %f", pCamera->PositionEye.X, pCamera->PositionEye.Y, pCamera->PositionEye.Z);

	// PosAt
	ImGui::Text("PosEye : %f %f %f", pCamera->PositionAt.X, pCamera->PositionAt.Y, pCamera->PositionAt.Z);

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
void DebugObserver::showPlayerWindow(void)
{
	if (!enablePlayerWindow || !player) { return; }

	// Begin
	if (!ImGui::Begin("Player Window", &enableCameraWindow))
	{
		ImGui::End();
		return;
	}

	// Trans
	auto pTrans = player->GetTransformComponent();
	ImGui::InputFloat3("Trans", &pTrans->PositionNext.X);

	// Actor Behavior
	auto pActor = static_cast<CActorBehaviorComponent*>(player->GetBehaviorComponent().front());
	ImGui::InputFloat("Move Speed", &pActor->m_fMoveSpeed);
	ImGui::InputFloat("Jump Speed", &pActor->m_fJumpSpeed);
	ImGui::Text("IsGrounded : %d", (int)pActor->m_bIsGrounded);

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  showFogWindow
//--------------------------------------------------------------------------------
void DebugObserver::showFogWindow(void)
{
	if (!enableFogWindow) { return; }
	auto pFog = Main::GetManager()->GetFog();

	// Begin
	if (!ImGui::Begin("Fog Window", &enableFogWindow))
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
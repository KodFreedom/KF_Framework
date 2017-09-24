//--------------------------------------------------------------------------------
//	エディタビヘイビアコンポネント
//　editorControllerBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "inputManager.h"
#include "mode.h"
#include "camera.h"
#include "gameObject.h"
#include "editorControllerBehaviorComponent.h"
#include "fieldEditorBehaviorComponent.h"
#include "modelEditorBehaviorComponent.h"
#include "transformComponent.h"
#include "ImGui\imgui.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//	Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CEditorControllerBehaviorComponent::CEditorControllerBehaviorComponent(CGameObject* const pGameObj)
	: CBehaviorComponent(pGameObj)
	, m_pFieldEditor(nullptr)
	, m_pModelEditor(nullptr)
	, m_bAutoHeight(true)
{

}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
bool CEditorControllerBehaviorComponent::Init(void)
{
	return true;
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void CEditorControllerBehaviorComponent::Uninit(void)
{

}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void CEditorControllerBehaviorComponent::Update(void)
{
	showMainWindow();
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void CEditorControllerBehaviorComponent::SetFieldEditor(CGameObject* pFieldEditor)
{
	auto listBehavior = pFieldEditor->GetBehaviorComponent();
	if (listBehavior.empty()) { return; }
	auto pBehavior = listBehavior.front();
	m_pFieldEditor = static_cast<CFieldEditorBehaviorComponent*>(pBehavior);
}

//--------------------------------------------------------------------------------
//
//	Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void CEditorControllerBehaviorComponent::save(void)
{
	m_pFieldEditor->SaveAs("demoField");
	m_pModelEditor->SaveAs("demoStage");
}

//--------------------------------------------------------------------------------
//  showMainWindow
//--------------------------------------------------------------------------------
void CEditorControllerBehaviorComponent::showMainWindow(void)
{
	// Begin
	if (!ImGui::Begin("Editor Window"))
	{
		ImGui::End();
		return;
	}

	// Pos
	showPosWindow();

	// Auto Height
	ImGui::Checkbox("Auto Height", &m_bAutoHeight);

	// Mode
	bool bModelEditor = m_pModelEditor->GetActive();
	bool bFieldEditor = m_pFieldEditor->GetActive();
	if (ImGui::Button("Model Editor")) { bModelEditor ^= 1; }
	if (ImGui::Button("Field Editor")) { bFieldEditor ^= 1; }
	m_pModelEditor->SetActive(bModelEditor);
	m_pFieldEditor->SetActive(bFieldEditor);

	// Save
	if (ImGui::Button("Save")) { save(); }

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  showPosWindow
//--------------------------------------------------------------------------------
void CEditorControllerBehaviorComponent::showPosWindow(void)
{
	//標的操作
	auto pInput = CMain::GetManager()->GetInputManager();
	auto pTrans = m_pGameObj->GetTransformComponent();
	auto vPos = pTrans->GetPos();
	auto vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
	auto pCamera = CMain::GetManager()->GetMode()->GetCamera();
	auto vCamForward = CKFMath::Vec3Scale(pCamera->GetVecLook(), CKFMath::VecNormalize(CKFVec3(1.0f, 0.0f, 1.0f)));
	auto vMove = pCamera->GetVecRight() * vAxis.m_fX + vCamForward * vAxis.m_fY;
	auto fHeight = (float)(pInput->GetKeyPress(CInputManager::K_LEFT) - pInput->GetKeyPress(CInputManager::K_RIGHT));
	vPos += vMove;
	vPos.m_fY += fHeight;

	//Adjust Pos
	m_pFieldEditor->AdjustPosInField(vPos, m_bAutoHeight);

	//ImGui
	ImGui::Text("Move : W A S D");
	ImGui::InputFloat3("Pos", &vPos.m_fX);

	//操作位置の更新
	m_pFieldEditor->SetPos(vPos);
	m_pModelEditor->SetPos(vPos);

	//カメラの移動
	auto vMovement = vPos - pTrans->GetPos();
	pCamera->MoveCamera(vMovement);

	//Pos設定
	pTrans->SetPos(vPos);
	pTrans->SetPosNext(vPos);
}

#endif // _DEBUG
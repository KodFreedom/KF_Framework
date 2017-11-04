//--------------------------------------------------------------------------------
//	�G�f�B�^�r�w�C�r�A�R���|�l���g
//�@editorControllerBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "versionSetting.h"
#if defined(_DEBUG) || defined(EDITOR)
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
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//	Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CEditorControllerBehaviorComponent::CEditorControllerBehaviorComponent(GameObject* const pGameObj)
	: CBehaviorComponent(pGameObj)
	, m_pFieldEditor(nullptr)
	, m_pModelEditor(nullptr)
	, m_bAutoHeight(true)
	, m_fMoveSpeed(1.0f)
{

}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
bool CEditorControllerBehaviorComponent::Init(void)
{
	return true;
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
void CEditorControllerBehaviorComponent::Uninit(void)
{

}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
void CEditorControllerBehaviorComponent::Update(void)
{
	showMainWindow();
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
void CEditorControllerBehaviorComponent::SetFieldEditor(GameObject* pFieldEditor)
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
//  �R���X�g���N�^
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
	//�W�I����
	auto pInput = Main::GetManager()->GetInputManager();
	auto pTrans = m_pGameObj->GetTransformComponent();
	auto Position = pTrans->GetPos();
	auto vAxis = Vector2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
	auto pCamera = Main::GetManager()->GetMode()->GetCamera();
	auto vCamForward = CKFMath::Vec3Scale(pCamera->GetVecLook(), CKFMath::VecNormalize(Vector3(1.0f, 0.0f, 1.0f)));
	auto vMove = pCamera->GetVecRight() * vAxis.X * m_fMoveSpeed + vCamForward * vAxis.Y * m_fMoveSpeed;
	auto fHeight = (float)(pInput->GetKeyPress(Input::Left) - pInput->GetKeyPress(Input::Right));
	Position += vMove;
	Position.Y += fHeight * m_fMoveSpeed;;

	//Adjust Pos
	m_pFieldEditor->AdjustPosInField(Position, m_bAutoHeight);

	//ImGui
	ImGui::Text("Move : W A S D");
	ImGui::Text("Raise / Reduce : <- / ->");
	ImGui::Text("CameraRot : RightClick + MouseMove");
	ImGui::Text("CameraZoom : RightClick + MouseWheel");
	ImGui::InputFloat("Move / Raise Speed", &m_fMoveSpeed);
	ImGui::InputFloat3("Pos", &Position.X);

	//����ʒu�̍X�V
	m_pFieldEditor->SetPos(Position);
	m_pModelEditor->SetPos(Position);

	//�J�����̈ړ�
	auto vMovement = Position - pTrans->GetPos();
	pCamera->MoveCamera(vMovement);

	//Pos�ݒ�
	pTrans->SetPos(Position);
	pTrans->SetPosNext(Position);
}

#endif // _DEBUG
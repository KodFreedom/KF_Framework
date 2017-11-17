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
#include "input.h"
#include "mode.h"
#include "camera.h"
#include "cameraManager.h"
#include "gameObject.h"
#include "editorController.h"
#include "fieldEditor.h"
#include "ModelEditor.h"
#include "transform.h"
#include "ImGui\imgui.h"

//--------------------------------------------------------------------------------
//
//	Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
EditorController::EditorController(GameObject* const owner)
	: Behavior(owner, "EditorController")
	, fieldEditor(nullptr)
	, modelEditor(nullptr)
	, isAutoHeight(true)
	, moveSpeed(1.0f)
{

}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
bool EditorController::Init(void)
{
	return true;
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
void EditorController::Uninit(void)
{

}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
void EditorController::Update(void)
{
	showMainWindow();
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
void EditorController::SetFieldEditor(GameObject* fieldEditorObject)
{
	auto& behaviors = fieldEditorObject->GetBehaviors();
	for (auto behavior : behaviors)
	{
		if (behavior->GetName()._Equal("FieldEditor"))
		{
			fieldEditor = static_cast<FieldEditor*>(behavior);
			break;
		}
	}	
}

//--------------------------------------------------------------------------------
//
//	Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  save
//--------------------------------------------------------------------------------
void EditorController::save(void)
{
	fieldEditor->SaveAs("demoField");
	modelEditor->SaveAs("demoStage");
}

//--------------------------------------------------------------------------------
//  showMainWindow
//--------------------------------------------------------------------------------
void EditorController::showMainWindow(void)
{
	// Begin
	if (!ImGui::Begin("Editor Window"))
	{
		ImGui::End();
		return;
	}

	// Pos
	showPositonWindow();

	// Auto Height
	ImGui::Checkbox("Auto Height", &isAutoHeight);

	// Mode
	bool isModelEditor = modelEditor->GetActive();
	bool isFieldEditor = fieldEditor->GetActive();
	if (ImGui::Button("Model Editor")) { isModelEditor ^= 1; }
	if (ImGui::Button("Field Editor")) { isFieldEditor ^= 1; }
	modelEditor->SetActive(isModelEditor);
	fieldEditor->SetActive(isFieldEditor);

	// Save
	if (ImGui::Button("Save")) { save(); }

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  showPositonWindow
//--------------------------------------------------------------------------------
void EditorController::showPositonWindow(void)
{
	//�W�I����
	auto input = Input::Instance();
	auto transform = owner->GetTransform();
	auto& position = transform->GetCurrentPosition();
	auto& axis = Vector2(input->MoveHorizontal(), input->MoveVertical());
	auto camera = CameraManager::Instance()->GetMainCamera();
	auto& cameraForward = Vector3::Scale(camera->GetForward(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();
	auto& movement = camera->GetRight() * axis.X * moveSpeed + cameraForward * axis.Y * moveSpeed;
	auto height = (float)(input->GetKeyPress(Key::Left) - input->GetKeyPress(Key::Right));
	position += movement;
	position.Y += height * moveSpeed;;

	//Adjust Pos
	position = fieldEditor->AdjustPosInField(position, isAutoHeight);

	//ImGui
	ImGui::Text("Move : W A S D");
	ImGui::Text("Raise / Reduce : <- / ->");
	ImGui::Text("CameraRot : RightClick + MouseMove");
	ImGui::Text("CameraZoom : RightClick + MouseWheel");
	ImGui::InputFloat("Move / Raise Speed", &moveSpeed);
	ImGui::InputFloat3("Pos", &position.X);

	//����ʒu�̍X�V
	fieldEditor->SetPosition(position);
	modelEditor->SetPosition(position);

	//�J�����̈ړ�
	movement = position - transform->GetCurrentPosition();
	camera->Move(movement);

	//Pos�ݒ�
	transform->SetNextPosition(position);
}

#endif // _DEBUG
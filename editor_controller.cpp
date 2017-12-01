//--------------------------------------------------------------------------------
//	エディタビヘイビアコンポネント
//　EditorController.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "editor_controller.h"
#if defined(EDITOR)
#include "main_system.h"
#include "input.h"
#include "mode.h"
#include "camera.h"
#include "camera_manager.h"
#include "game_object.h"
#include "field_editor.h"
#include "Model_editor.h"
#include "transform.h"
#include "ImGui\imgui.h"

//--------------------------------------------------------------------------------
//
//	Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
EditorController::EditorController(GameObject& owner)
	: Behavior(owner, L"EditorController")
	, field_editor_(nullptr)
	, model_editor_(nullptr)
	, is_auto_height_(true)
	, move_speed_(1.0f)
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool EditorController::Init(void)
{
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void EditorController::Uninit(void)
{

}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EditorController::Update(void)
{
	ShowMainWindow();
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void EditorController::SetFieldEditor(GameObject* object)
{
	for (auto& pair : object->GetBehaviors())
	{
		if (pair.second->GetName()._Equal(L"FieldEditor"))
		{
			field_editor_ = static_cast<FieldEditor*>(pair.second);
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
void EditorController::Save(void)
{
	field_editor_->SaveAsBinary(L"demoField");
	model_editor_->SaveAsBinary(L"demoStage");
}

//--------------------------------------------------------------------------------
//  ShowMainWindow
//--------------------------------------------------------------------------------
void EditorController::ShowMainWindow(void)
{
	// Begin
	if (!ImGui::Begin("Editor Window"))
	{
		ImGui::End();
		return;
	}

	// Pos
	ShowPositonWindow();

	// Auto Height
	ImGui::Checkbox("Auto Height", &is_auto_height_);

	// Mode
	bool is_model_editor_ = model_editor_->IsActive();
	bool is_field_editor_ = field_editor_->IsActive();
	if (ImGui::Button("Model Editor")) { is_model_editor_ ^= 1; }
	if (ImGui::Button("Field Editor")) { is_field_editor_ ^= 1; }
	model_editor_->SetActive(is_model_editor_);
	field_editor_->SetActive(is_field_editor_);

	// Save
	if (ImGui::Button("Save")) { Save(); }

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  ShowPositonWindow
//--------------------------------------------------------------------------------
void EditorController::ShowPositonWindow(void)
{
	//標的操作
	auto input = MainSystem::Instance()->GetInput();
	auto transform = owner_.GetTransform();
	Vector3 position = transform->GetPosition();
	auto& axis = Vector2(input->MoveHorizontal(), input->MoveVertical());
	auto camera = MainSystem::Instance()->GetCameraManager()->GetMainCamera();
	auto& camera_forward = Vector3::Scale(camera->GetWorldForward(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();
	auto& movement = camera->GetWorldRight() * axis.x_ * move_speed_ + camera_forward * axis.y_ * move_speed_;
	auto height = static_cast<float>(input->GetKeyPress(Key::kLeft) - input->GetKeyPress(Key::kRight));
	position += movement;
	position.y_ += height * move_speed_;

	//Adjust Pos
	position = field_editor_->AdjustPositionInField(position, is_auto_height_);

	//ImGui
	ImGui::Text("Move : W A S D");
	ImGui::Text("Raise / Reduce : <- / ->");
	ImGui::Text("CameraRot : RightClick + MouseMove");
	ImGui::Text("CameraZoom : RightClick + MouseWheel");
	ImGui::InputFloat("Move / Raise Speed", &move_speed_);
	ImGui::InputFloat3("Pos", &position.x_);

	//操作位置の更新
	field_editor_->SetPosition(position);
	model_editor_->SetPosition(position);

	//カメラの移動
	movement = position - transform->GetPosition();
	camera->Move(movement);

	//Pos設定
	transform->SetPosition(position);
}
#endif // EDITOR
//--------------------------------------------------------------------------------
//  エディタビヘイビアコンポネント
//　EditorController.h
//  Author : Xu Wenjie
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
#include "labels.h"
#include "debug_observer.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
EditorController::EditorController(GameObject& owner)
    : Behavior(owner, L"EditorController")
    , field_editor_(nullptr)
    , model_editor_(nullptr)
    , enable_auto_adjust_height_(true)
    , move_speed_(1.0f)
    , stage_name_("demo")
{}

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
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  save
//--------------------------------------------------------------------------------
void EditorController::Save(void)
{
    String stage_name(stage_name_.begin(), stage_name_.end());
    field_editor_->SaveAsBinary(stage_name + L"Field");
    model_editor_->SaveAsBinary(stage_name + L"Stage");
}

//--------------------------------------------------------------------------------
//  ShowMainWindow
//--------------------------------------------------------------------------------
void EditorController::ShowMainWindow(void)
{
    // Begin
    if (!ImGui::Begin("Editor window"))
    {
        ImGui::End();
        return;
    }

    auto& current_language = MainSystem::Instance()->GetDebugObserver()->GetCurrentLanguage();

    // State name
    ImGui::InputText(kStageName[current_language], &stage_name_[0], _MAX_PATH);

    // Mode
    ShowModeWindow();

    // Position
    ShowPositonWindow();

    // Save
    if (ImGui::Button(kSaveStage[current_language])) { Save(); }

    // End
    ImGui::End();
}

//--------------------------------------------------------------------------------
//  モードウィンドウの表示
//--------------------------------------------------------------------------------
void EditorController::ShowModeWindow(void)
{
    auto& current_language = MainSystem::Instance()->GetDebugObserver()->GetCurrentLanguage();

    // model
    bool enable_model_editor = model_editor_->IsActive();
    if (ImGui::Button(enable_model_editor ? kCloseModelEditor[current_language]
        : kOpenModelEditor[current_language]))
    {
        enable_model_editor ^= 1;
    }
    model_editor_->SetActive(enable_model_editor);

    // field
    bool enable_field_editor = field_editor_->IsActive();
    if (ImGui::Button(enable_field_editor ? kCloseFieldEditor[current_language]
        : kOpenFieldEditor[current_language]))
    {
        enable_field_editor ^= 1;
    }
    field_editor_->SetActive(enable_field_editor);
}

//--------------------------------------------------------------------------------
//  ShowPositonWindow
//--------------------------------------------------------------------------------
void EditorController::ShowPositonWindow(void)
{
    auto& current_language = MainSystem::Instance()->GetDebugObserver()->GetCurrentLanguage();

    // 位置の取得
    auto transform = owner_.GetTransform();
    Vector3 previous_position = transform->GetPosition();
    Vector3 current_position = previous_position;

    // 操作更新
    auto input = MainSystem::Instance()->GetInput();
    Vector2 axis(input->MoveHorizontal(), input->MoveVertical());
    auto camera = MainSystem::Instance()->GetCameraManager()->GetMainCamera();
    Vector3& camera_forward = Vector3::Scale(camera->GetWorldForward(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();
    Vector3& movement = camera->GetWorldRight() * axis.x_ * move_speed_ + camera_forward * axis.y_ * move_speed_;
    float height = static_cast<float>(input->GetKeyPress(Key::kLeft) - input->GetKeyPress(Key::kRight));
    current_position += movement;
    current_position.y_ += height * move_speed_;

    // 高さの自動調節モード
    ImGui::Checkbox(kAutoAdjustHeight[current_language], &enable_auto_adjust_height_);

    // 位置の調節
    current_position = field_editor_->AdjustPositionInField(current_position, enable_auto_adjust_height_);

    // 操作方法
    ImGui::Text(kExplainMove[current_language]);
    ImGui::Text(kExplainRaiseReduce[current_language]);
    ImGui::Text(kExplainCameraRotation[current_language]);
    ImGui::Text(kExplainCameraZoom[current_language]);
    ImGui::InputFloat(kMoveRaiseSpeed[current_language], &move_speed_);
    ImGui::InputFloat3(kTargetPosition[current_language], &current_position.x_);

    // 操作位置の更新
    field_editor_->SetPosition(current_position);
    model_editor_->SetPosition(current_position);
    transform->SetPosition(current_position);

    // カメラの移動
    movement = current_position - previous_position;
    camera->Move(movement);
}

#endif // EDITOR
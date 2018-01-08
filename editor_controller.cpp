//--------------------------------------------------------------------------------
//  �G�f�B�^�r�w�C�r�A�R���|�l���g
//�@EditorController.h
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
#include "game_object_spawner.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
EditorController::EditorController(GameObject& owner)
    : Behavior(owner, L"EditorController")
    , field_editor_(nullptr)
    , model_editor_(nullptr)
    , player_(nullptr)
    , enable_auto_adjust_height_(true)
    , move_speed_(1.0f)
    , stage_name_("demo")
{}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool EditorController::Init(void)
{
    // �W�I������������
    owner_.GetTransform()->SetScale(Vector3(0.25f));

    // Player�̐���
    player_ = GameObjectSpawner::CreateXModel(L"data/model/player.x", Vector3::kZero, Vector3::kZero, Vector3::kOne);
    return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void EditorController::Uninit(void)
{

}

//--------------------------------------------------------------------------------
//  �X�V����
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
//  �ۑ�����
//--------------------------------------------------------------------------------
void EditorController::Save(void)
{
    String stage_name(stage_name_.begin(), stage_name_.end());
    field_editor_->SaveAsBinary(stage_name + L"Field");
    model_editor_->SaveAsBinary(stage_name + L"Stage");
    SaveAsBinary(stage_name + L"Player");
}

//--------------------------------------------------------------------------------
//  �Ǎ�����
//--------------------------------------------------------------------------------
void EditorController::Load(void)
{
    UINT id = MessageBox(NULL, L"���̃X�e�[�W��j�����ĐV�����X�e�[�W��ǂݍ��݂܂����H", L"�m�F", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);
    if (id != IDYES) { return; }
    String stage_name(stage_name_.begin(), stage_name_.end());
    field_editor_->LoadFrom(stage_name + L"Field");
    model_editor_->LoadFrom(stage_name + L"Stage");
    LoadFrom(stage_name + L"Player");
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

    // Load
    if (ImGui::Button(kLoadStage[current_language])) { Load(); }

    // End
    ImGui::End();
}

//--------------------------------------------------------------------------------
//  ���[�h�E�B���h�E�̕\��
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

    // �ʒu�̎擾
    auto transform = owner_.GetTransform();
    Vector3 previous_position = transform->GetPosition();
    Vector3 current_position = previous_position;

    // ����X�V
    auto input = MainSystem::Instance()->GetInput();
    Vector2 axis(input->MoveHorizontal(), input->MoveVertical());
    auto camera = MainSystem::Instance()->GetCameraManager()->GetMainCamera();
    Vector3& camera_forward = Vector3::Scale(camera->GetWorldForward(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();
    Vector3& movement = camera->GetWorldRight() * axis.x_ * move_speed_ + camera_forward * axis.y_ * move_speed_;
    float height = static_cast<float>(input->GetKeyPress(Key::kRaise) - input->GetKeyPress(Key::kReduce));
    current_position += movement;
    current_position.y_ += height * move_speed_;

    // �����̎������߃��[�h
    ImGui::Checkbox(kAutoAdjustHeight[current_language], &enable_auto_adjust_height_);

    // �ʒu�̒���
    current_position = field_editor_->AdjustPositionInField(current_position, enable_auto_adjust_height_);

    // ������@
    ImGui::Text(kExplainMove[current_language]);
    ImGui::Text(kExplainRaiseReduce[current_language]);
    ImGui::Text(kExplainCameraRotation[current_language]);
    ImGui::Text(kExplainCameraZoom[current_language]);
    ImGui::InputFloat(kMoveRaiseSpeed[current_language], &move_speed_);
    ImGui::InputFloat3(kTargetPosition[current_language], &current_position.x_);

    // ����ʒu�̍X�V
    field_editor_->SetPosition(current_position);
    model_editor_->SetPosition(current_position);
    transform->SetPosition(current_position);

    // �J�����̈ړ�
    movement = current_position - previous_position;
    camera->Move(movement);

    // �v���C���[�ʒu�̐ݒ�
    if (ImGui::Button(kSetPlayer[current_language]))
    {
        player_->GetTransform()->SetPosition(
            owner_.GetTransform()->GetPosition());
    }
}

//--------------------------------------------------------------------------------
//	�t�B�[���h����ۑ�����֐�
//--------------------------------------------------------------------------------
void EditorController::SaveAsBinary(const String& name)
{
    //�t�B�[���h�̕ۑ�
    String path = L"data/stage/" + name + L".player";
    ofstream file(path, ios::binary);
    if (!file.is_open())
    {
        MessageBox(NULL, L"�J���܂���ł���", path.c_str(), MB_OK | MB_ICONWARNING);
        return;
    }
    BinaryOutputArchive archive(file);

    //�ʒu�̕ۑ�
    Vector3 position = player_->GetTransform()->GetPosition();
    archive.saveBinary(&position, sizeof(position));

    file.close();
    MessageBox(NULL, L"�Z�[�u���܂���", path.c_str(), MB_OK | MB_ICONWARNING);
}

//--------------------------------------------------------------------------------
// player����Ǎ��֐�
//--------------------------------------------------------------------------------
void EditorController::LoadFrom(const String& name)
{
    //�t�B�[���h�̓Ǎ�
    String path = L"data/stage/" + name + L".player";
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        MessageBox(NULL, L"�J���܂���ł���", path.c_str(), MB_OK | MB_ICONWARNING);
        return;
    }
    BinaryInputArchive archive(file);

    //�ʒu�̓Ǎ�
    Vector3 position;
    archive.loadBinary(&position, sizeof(position));
    player_->GetTransform()->SetPosition(position);
    
    file.close();
    MessageBox(NULL, L"���[�h���܂���", path.c_str(), MB_OK | MB_ICONWARNING);
}

#endif // EDITOR
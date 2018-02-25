//--------------------------------------------------------------------------------
//  エネミーエディタ
//　enemy_editor.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "enemy_editor.h"
#if defined(EDITOR)
#include "../../../../library/ImGui/imgui.h"
#include "../../../game_object/game_object_spawner.h"
#include "../../../game_object/game_object.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
EnemyEditor::EnemyEditor(GameObject& owner)
    : Editor(owner, L"EnemyEditor")
{}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool EnemyEditor::Init(void)
{
    // 表示用enemyの生成
    auto warning = GameObjectSpawner::CreateXModel(L"data/model/warning.x", Vector3::kZero, Vector3::kZero, Vector3::kOne, kUseDepthSort);
    auto patrol = GameObjectSpawner::CreateXModel(L"data/model/patrol.x", Vector3::kZero, Vector3::kZero, Vector3::kOne, kUseDepthSort);
    warning->SetActive(false);
    patrol->SetActive(false);
    current_info_.warning_transform = warning->GetTransform();
    current_info_.warning_transform->SetScale(Vector3(10.0f, 1.0f, 10.0f));
    current_info_.patrol_transform = patrol->GetTransform();
    current_info_.patrol_transform->SetScale(Vector3(20.0f, 1.0f, 20.0f));
    return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void EnemyEditor::Uninit(void)
{
    created_infos_.clear();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void EnemyEditor::Update(void)
{
    if (!is_active_) return;
    ShowMainWindow();
}

//--------------------------------------------------------------------------------
//  アクティブフラグ関数
//--------------------------------------------------------------------------------
void EnemyEditor::SetActive(const bool& value)
{
    Editor::SetActive(value);
    current_info_.warning_transform->GetGameObject().SetActive(value);
    current_info_.patrol_transform->GetGameObject().SetActive(value);
}

//--------------------------------------------------------------------------------
//  位置の設定
//--------------------------------------------------------------------------------
void EnemyEditor::SetPosition(const Vector3& position)
{
    current_info_.warning_transform->SetPosition(position);
    current_info_.patrol_transform->SetPosition(position);
}

//--------------------------------------------------------------------------------
//  ファイルに保存
//--------------------------------------------------------------------------------
void EnemyEditor::SaveAsBinary(const String& name)
{
    //フィールドの保存
    String path = L"data/stage/" + name + L".enemy";
    ofstream file(path, ios::binary);
    if (!file.is_open())
    {
        MessageBox(NULL, L"開けませんでした", path.c_str(), MB_OK | MB_ICONWARNING);
        return;
    }
    BinaryOutputArchive archive(file);

    // enemy数の保存
    size_t enemy_number = created_infos_.size();
    archive.saveBinary(&enemy_number, sizeof enemy_number);

    // 情報保存
    for (auto& info : created_infos_)
    {
        // 位置
        auto& position = info.warning_transform->GetPosition();
        archive.saveBinary(&position, sizeof(position));
        
        // 警戒範囲
        float warning_range = info.warning_transform->GetScale().x_;
        archive.saveBinary(&warning_range, sizeof(warning_range));

        // 巡回範囲
        float patrol_range = info.patrol_transform->GetScale().x_;
        archive.saveBinary(&patrol_range, sizeof(patrol_range));

        // パラメーター
        archive.saveBinary(&info.parameter, sizeof(info.parameter));
    }

    file.close();
    MessageBox(NULL, L"セーブしました", path.c_str(), MB_OK | MB_ICONWARNING);
}

//--------------------------------------------------------------------------------
//  情報を読込関数
//--------------------------------------------------------------------------------
void EnemyEditor::LoadFrom(const String& name)
{
    String path = L"data/stage/" + name + L".enemy";
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        MessageBox(NULL, L"開けませんでした", path.c_str(), MB_OK | MB_ICONWARNING);
        return;
    }
    BinaryInputArchive archive(file);

    // 今生成したエネミーの破棄
    ClearList();

    // enemy数の読込
    size_t enemy_number;
    archive.loadBinary(&enemy_number, sizeof enemy_number);

    // 情報の読込
    for (size_t count = 0; count < enemy_number; ++count)
    {
        EnemyInfo info;
       
        // 位置
        Vector3 position;
        archive.loadBinary(&position, sizeof(position));

        // 警戒範囲
        float warning_range;
        archive.loadBinary(&warning_range, sizeof(warning_range));
        auto warning = GameObjectSpawner::CreateXModel(L"data/model/warning.x"
            , position, Vector3::kZero, Vector3(warning_range, 1.0f, warning_range), kUseDepthSort);
        info.warning_transform = warning->GetTransform();

        // 巡回範囲
        float patrol_range;
        archive.loadBinary(&patrol_range, sizeof(patrol_range));
        auto patrol = GameObjectSpawner::CreateXModel(L"data/model/patrol.x"
            , position, Vector3::kZero, Vector3(patrol_range, 1.0f, patrol_range), kUseDepthSort);
        info.patrol_transform = patrol->GetTransform();

        // パラメーター
        archive.loadBinary(&info.parameter, sizeof(info.parameter));

        created_infos_.push_back(info);
    }

    file.close();
    MessageBox(NULL, L"ロードしました", path.c_str(), MB_OK | MB_ICONWARNING);
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  関数定義
//--------------------------------------------------------------------------------
void EnemyEditor::ShowMainWindow(void)
{
    // Begin
    if (!ImGui::Begin("Enemy editor Window"))
    {
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader(kDemo[current_language_]))
    {// デモエネミーの調節
        EditEnemyInfo(current_info_);

        // エネミーの作成
        if (ImGui::Button(kCreate[current_language_])) { Create(); }
    }

    if (ImGui::CollapsingHeader(kShowCreatedList[current_language_]))
    {// 作成したエネミーの調節
        int count_enemy = 0;
        for (auto iterator = created_infos_.begin(); iterator != created_infos_.end();)
        {
            bool is_delete = false;
            if (ImGui::TreeNode(to_string(count_enemy).c_str()))
            {
                // Position
                Vector3 position = iterator->warning_transform->GetPosition();
                if (ImGui::InputFloat3(kPosition[current_language_], &position.x_))
                {
                   iterator->warning_transform->SetPosition(position);
                   iterator->patrol_transform->SetPosition(position);
                }

                // Info
                EditEnemyInfo(*iterator);

                // Delete
                is_delete = ImGui::Button(kDelete[current_language_]);
                ImGui::TreePop();
            }

            if (is_delete)
            {
                iterator->warning_transform->GetGameObject().SetAlive(false);
                iterator->patrol_transform->GetGameObject().SetAlive(false);
                iterator = created_infos_.erase(iterator);
            }
            else
            {
                ++iterator;
                ++count_enemy;
            }
        }
    }

    // End
    ImGui::End();
}

//--------------------------------------------------------------------------------
//  作成処理
//--------------------------------------------------------------------------------
void EnemyEditor::Create(void)
{
    auto warning = GameObjectSpawner::CreateXModel(L"data/model/warning.x"
        , current_info_.warning_transform->GetPosition(), Vector3::kZero, current_info_.warning_transform->GetScale(), kUseDepthSort);
    auto patrol = GameObjectSpawner::CreateXModel(L"data/model/patrol.x"
        , current_info_.patrol_transform->GetPosition(), Vector3::kZero, current_info_.patrol_transform->GetScale(), kUseDepthSort);
    
    EnemyInfo info;
    info.warning_transform = warning->GetTransform();
    info.patrol_transform = patrol->GetTransform();
    info.parameter = current_info_.parameter;

    created_infos_.push_back(info);
}

//--------------------------------------------------------------------------------
//  情報の調節
//--------------------------------------------------------------------------------
void EnemyEditor::EditEnemyInfo(EnemyInfo& info)
{
    // エネミーの範囲調節
    float warning_range = info.warning_transform->GetScale().x_;
    if (ImGui::SliderFloat(kWarningRange[current_language_], &warning_range, 0.0f, 100.0f))
    {
        info.warning_transform->SetScale(Vector3(warning_range, 1.0f, warning_range));
    }

    float patrol_range = info.patrol_transform->GetScale().x_;
    if (ImGui::SliderFloat(kPatrolRange[current_language_], &patrol_range, 0.0f, 100.0f))
    {
        info.patrol_transform->SetScale(Vector3(patrol_range, 1.0f, patrol_range));
    }

    // パラメーターの調節
    if (ImGui::InputFloat(kLife[current_language_], &info.parameter.max_life_))
    {
        info.parameter.max_life_ = Math::Clamp(info.parameter.max_life_, 0.0f, info.parameter.max_life_);
        info.parameter.current_life_ = info.parameter.max_life_;
    }

    if (ImGui::InputFloat(kAttack[current_language_], &info.parameter.attack_))
    {
        info.parameter.attack_ = Math::Clamp(info.parameter.attack_, 0.0f, info.parameter.attack_);
    }

    if (ImGui::InputFloat(kDefence[current_language_], &info.parameter.defence_))
    {
        info.parameter.defence_ = Math::Clamp(info.parameter.defence_, 0.0f, info.parameter.defence_);
    }

    if (ImGui::InputFloat(kMoveSpeed[current_language_], &info.parameter.move_speed_))
    {
        info.parameter.move_speed_ = Math::Clamp(info.parameter.move_speed_, 0.0f, info.parameter.move_speed_);
    }

    if (ImGui::InputFloat(kMinTurnSpeed[current_language_], &info.parameter.min_turn_speed_))
    {
        info.parameter.min_turn_speed_ = Math::Clamp(info.parameter.min_turn_speed_, 0.0f, info.parameter.min_turn_speed_);
    }

    if (ImGui::InputFloat(kMaxTurnSpeed[current_language_], &info.parameter.max_turn_speed_))
    {
        info.parameter.max_turn_speed_ = Math::Clamp(info.parameter.max_turn_speed_, 0.0f, info.parameter.max_turn_speed_);
    }
}

//--------------------------------------------------------------------------------
//  生成リストの破棄
//--------------------------------------------------------------------------------
void EnemyEditor::ClearList(void)
{
    for (auto& info : created_infos_)
    {
        info.warning_transform->GetGameObject().SetAlive(false);
        info.patrol_transform->GetGameObject().SetAlive(false);
    }
    created_infos_.clear();
}
#endif

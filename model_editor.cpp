//--------------------------------------------------------------------------------
//    ���f���G�f�B�^�r�w�C�r�A�R���|�l���g
//�@ModelEditor.h
//    Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "model_editor.h"
#if defined(EDITOR)
#include "game_object.h"
#include "game_object_spawner.h"
#include "transform.h"
#include "ImGui\imgui.h"
#include "main_system.h"
#include "labels.h"
#include "debug_observer.h"

namespace ImGui
{
    static auto vector_getter = [](void* vec, int idx, const char** out_text)
    {
        auto& vector = *static_cast<std::vector<std::string>*>(vec);
        if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
        *out_text = vector.at(idx).c_str();
        return true;
    };

    bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return Combo(label, currIndex, vector_getter,
            static_cast<void*>(&values), values.size());
    }

    bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
    {
        if (values.empty()) { return false; }
        return ListBox(label, currIndex, vector_getter,
            static_cast<void*>(&values), values.size());
    }

}

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
ModelEditor::ModelEditor(GameObject& owner)
    : Behavior(owner, L"ModelEditor")
    , current_model_no_(0)
    , is_active_(false)
    , is_show_created_list_(false)
{}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool ModelEditor::Init(void)
{
    // �t�H���_����S�Ẵt�@�C����ǂݍ���
    String& folder_path = Utility::GetProjectPath() + L"data\\model";
    model_names_ = Utility::GetFilesFromFolder(folder_path, L"model");

    // Demo Object�̍쐬
    size_t model_number = model_names_.size();
    demo_model_infos_.resize(model_number);
    for (size_t count = 0; count < model_number; ++count)
    {
        auto game_object = GameObjectSpawner::CreateModel(model_names_[count], Vector3::kZero, Quaternion::kIdentity, Vector3::kOne);
        game_object->SetActive(false);
        demo_model_infos_[count].my_transform = game_object->GetTransform();
    }
    created_model_infos_.resize(model_number);
    return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void ModelEditor::Uninit(void)
{
    ClearList();
    model_names_.clear();
    for (auto& info : demo_model_infos_)
    {
        info.my_transform->GetGameObject().SetActive(false);
    }
    demo_model_infos_.clear();
    current_model_no_ = 0;
    is_show_created_list_ = false;
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void ModelEditor::Update(void)
{
    if (!is_active_)
    {
        for (auto& info : demo_model_infos_) 
        {
            info.my_transform->GetGameObject().SetActive(false); 
        }
        return;
    }
    ShowMainWindow();
    ShowCreatedList();
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void ModelEditor::LateUpdate(void)
{
    if (!is_active_)
    {
        return;
    }
}

//--------------------------------------------------------------------------------
//  SetPosition
//--------------------------------------------------------------------------------
void ModelEditor::SetPosition(const Vector3& position)
{
    if (demo_model_infos_.empty()) return;
    demo_model_infos_[current_model_no_].my_transform->SetPosition(position);
}

//--------------------------------------------------------------------------------
//  SaveAs
//--------------------------------------------------------------------------------
void ModelEditor::SaveAsBinary(const String& name)
{
    //�t�B�[���h�̕ۑ�
    String path = L"data/stage/" + name + L".stage";
    ofstream file(path, ios::binary);
    if (!file.is_open())
    {
        MessageBox(NULL, L"�J���܂���ł���", path.c_str(), MB_OK | MB_ICONWARNING);
        return;
    }
    BinaryOutputArchive archive(file);

    //Model���̕ۑ�
    size_t model_number = model_names_.size();
    archive.saveBinary(&model_number, sizeof model_number);

    for (size_t count = 0; count < model_number; ++count)
    {
        //�t�@�C�����ۑ�
        string name = string(model_names_[count].begin(), model_names_[count].end());
        size_t name_size = name.size();
        archive.saveBinary(&name_size, sizeof(name_size));
        archive.saveBinary(&name[0], name_size);
        
        //��������f�����̕ۑ�
        size_t created_model_number = created_model_infos_[count].size();
        archive.saveBinary(&created_model_number, sizeof(created_model_number));

        //�ʒu��]�X�P�[���̕ۑ�
        for (auto& info : created_model_infos_[count])
        {
            auto& position = info.my_transform->GetPosition();
            archive.saveBinary(&position, sizeof(position));
            auto& rotation = info.my_transform->GetRotation();
            archive.saveBinary(&rotation, sizeof(rotation));
            auto& scale = info.my_transform->GetScale();
            archive.saveBinary(&scale, sizeof(scale));
        }
    }
    file.close();
    MessageBox(NULL, L"�Z�[�u���܂���", path.c_str(), MB_OK | MB_ICONWARNING);
}

//--------------------------------------------------------------------------------
//  ����Ǎ��֐�
//--------------------------------------------------------------------------------
void ModelEditor::LoadFrom(const String& name)
{
    String path = L"data/stage/" + name + L".stage";
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        MessageBox(NULL, L"�J���܂���ł���", path.c_str(), MB_OK | MB_ICONWARNING);
        return;
    }
    BinaryInputArchive archive(file);

    // �������������f���̔j��
    ClearList();

    //Model���̓Ǎ�
    size_t model_number;
    archive.loadBinary(&model_number, sizeof model_number);

    for (size_t count_model = 0; count_model < model_number; ++count_model)
    {
        //�t�@�C�����Ǎ�
        string name;
        size_t name_size;
        archive.loadBinary(&name_size, sizeof(name_size));
        name.resize(name_size);
        archive.loadBinary(&name[0], name_size);

        //��������f�����̓Ǎ�
        size_t created_model_number;
        archive.loadBinary(&created_model_number, sizeof(created_model_number));

        //�ʒu��]�X�P�[���̓Ǎ�
        for (size_t count = 0; count < created_model_number; ++count)
        {
            Vector3 position;
            archive.loadBinary(&position, sizeof(position));
            Quaternion rotation;
            archive.loadBinary(&rotation, sizeof(rotation));
            Vector3 scale;
            archive.loadBinary(&scale, sizeof(scale));
            auto game_object = GameObjectSpawner::CreateModel(model_names_[count_model], position, rotation, scale);
            Info info;
            info.my_transform = game_object->GetTransform();
            info.rotation = Vector3::kZero;
            created_model_infos_[count_model].push_back(info);
        }
    }

    file.close();
    MessageBox(NULL, L"���[�h���܂���", path.c_str(), MB_OK | MB_ICONWARNING);
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  create
//--------------------------------------------------------------------------------
void ModelEditor::Create(void)
{
    auto game_object = GameObjectSpawner::CreateModel(model_names_[current_model_no_], Vector3::kZero, Quaternion::kIdentity, Vector3::kOne);
    auto transform = game_object->GetTransform();
    auto& current_info = demo_model_infos_[current_model_no_];
    auto& position = current_info.my_transform->GetPosition();
    auto& rotation = current_info.my_transform->GetRotation();
    auto& scale = current_info.my_transform->GetScale();
    transform->SetPosition(position);
    transform->SetRotation(rotation);
    transform->SetScale(scale);
    Info info;
    info.my_transform = transform;
    info.rotation = current_info.rotation;
    created_model_infos_[current_model_no_].push_back(info);
}

//--------------------------------------------------------------------------------
//  ShowMainWindow
//--------------------------------------------------------------------------------
void ModelEditor::ShowMainWindow(void)
{
    // Begin
    if (!ImGui::Begin("Model Editor Window"))
    {
        ImGui::End();
        return;
    }

    auto& current_language = MainSystem::Instance()->GetDebugObserver()->GetCurrentLanguage();

    // Model Type
    ShowTypeListBox();

    if (!demo_model_infos_.empty())
    {
        // Model Trans
        auto& current_info = demo_model_infos_[current_model_no_];

        //���f����]
        if (ImGui::SliderFloat3(kRotation[current_language], &current_info.rotation.x_, 0.0f, kPi * 2.0f))
        {
            current_info.my_transform->SetRotation(current_info.rotation);
        }

        //���f���X�P�[��
        Vector3 scale = current_info.my_transform->GetScale();
        if (ImGui::InputFloat3(kScale[current_language], &scale.x_))
        {
            current_info.my_transform->SetScale(scale);
        }

        //���f���̍쐬
        if (ImGui::Button(kCreate[current_language])) { Create(); }
    }

    //���f�����X�g
    size_t model_number = model_names_.size();
    for (size_t count = 0; count < model_number; ++count)
    {
        ImGui::Text("%s : %d", string(model_names_[count].begin(), model_names_[count].end()).c_str()
            , static_cast<int>(created_model_infos_[count].size()));
    }
    if (ImGui::Button(kShowCreatedList[current_language])) { is_show_created_list_ ^= 1; }

    // End
    ImGui::End();
}

//--------------------------------------------------------------------------------
//  ShowTypeListBox
//--------------------------------------------------------------------------------
void ModelEditor::ShowTypeListBox(void)
{
    auto& current_language = MainSystem::Instance()->GetDebugObserver()->GetCurrentLanguage();

    // copy name
    size_t model_number = model_names_.size();
    vector<string> labels;
    labels.reserve(model_number);
    for (auto& model_name : model_names_)
    {
        labels.push_back(string(model_name.begin(), model_name.end()));
    }

    //Type
    if (ImGui::Combo(kModel[current_language], (int*)&current_model_no_, labels))
    {
        //���f���A�N�e�B�u�̐ݒ�
        for (size_t count = 0; count < model_number; ++count)
        {
            demo_model_infos_[count].my_transform->GetGameObject().
                SetActive(current_model_no_ == static_cast<int>(count));
        }
    }
}

//--------------------------------------------------------------------------------
//  ShowCreatedList
//--------------------------------------------------------------------------------
void ModelEditor::ShowCreatedList(void)
{
    if (!is_show_created_list_) return;

    // Begin
    if (!ImGui::Begin("Created List Window", &is_show_created_list_))
    {
        ImGui::End();
        return;
    }

    auto& current_language = MainSystem::Instance()->GetDebugObserver()->GetCurrentLanguage();

    size_t model_number = model_names_.size();
    for (size_t count = 0; count < model_number; ++count)
    {
        string model_name(model_names_[count].begin(), model_names_[count].end());
        if (ImGui::TreeNode(model_name.c_str()))
        {
            int count_object = 0;
            for (auto iterator = created_model_infos_[count].begin(); iterator != created_model_infos_[count].end();)
            {
                bool is_delete = false;
                string buffer = model_name + to_string(count_object);
                if (ImGui::TreeNode(buffer.c_str()))
                {
                    Vector3 position = iterator->my_transform->GetPosition();
                    Vector3 scale = iterator->my_transform->GetScale();

                    //Offset
                    if (ImGui::InputFloat3(kPosition[current_language], &position.x_))
                    {
                        iterator->my_transform->SetPosition(position);
                    }
                    if (ImGui::InputFloat3(kRotation[current_language], &iterator->rotation.x_))
                    {
                        iterator->my_transform->SetRotation(iterator->rotation);
                    }
                    if (ImGui::InputFloat3(kScale[current_language], &scale.x_))
                    {
                        iterator->my_transform->SetScale(scale);
                    }

                    //Delete
                    is_delete = ImGui::Button(kDelete[current_language]);
                    ImGui::TreePop();
                }

                if (is_delete)
                {
                    iterator->my_transform->GetGameObject().SetAlive(false);
                    iterator = created_model_infos_[count].erase(iterator);
                }
                else
                {
                    ++iterator;
                    ++count_object;
                }
            }
            ImGui::TreePop();
        }
    }

    // End
    ImGui::End();
}

//--------------------------------------------------------------------------------
//  ClearList
//--------------------------------------------------------------------------------
void ModelEditor::ClearList(void)
{
    for (auto& list : created_model_infos_)
    {
        for (auto& info : list)
        {
            info.my_transform->GetGameObject().SetAlive(false);
        }
        list.clear();
    }
}
#endif // EDITOR
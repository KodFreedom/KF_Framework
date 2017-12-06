//--------------------------------------------------------------------------------
//	���f���G�f�B�^�r�w�C�r�A�R���|�l���g
//�@ModelEditor.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "model_editor.h"
#if defined(EDITOR)
#include "main.h"
#include "game_object.h"
#include "game_object_spawner.h"
#include "transform.h"
#include "ImGui\imgui.h"

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
		auto gameObject = GameObjectSpawner::CreateModel(model_names_[count], Vector3::kZero, Quaternion::kIdentity, Vector3::kOne);
		gameObject->SetActive(false);
		demo_model_infos_[count].my_transform = gameObject->GetTransform();
	}
	created_model_infos_.resize(model_number);

	// �W�I������������
	owner_.GetTransform()->SetScale(Vector3(0.5f));
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void ModelEditor::Uninit(void)
{
	for (auto& modelName : model_names_) { modelName.clear(); }
	for (auto& list : created_model_infos_) { list.clear(); }
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
		assert(file.is_open());
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
	transform->SetPosition(position);
	transform->SetRotation(rotation);
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

	// Model Type
	ShowTypeListBox();

	if (!demo_model_infos_.empty())
	{
		// Model Trans
		auto& current_info = demo_model_infos_[current_model_no_];

		//���f����]
		if (ImGui::SliderFloat3("rotation", &current_info.rotation.x_, 0.0f, kPi * 2.0f))
		{
			current_info.my_transform->SetRotation(current_info.rotation);
		}

		//���f���X�P�[��
		Vector3 scale = current_info.my_transform->GetScale();
		if (ImGui::InputFloat3("scale", &scale.x_))
		{
			current_info.my_transform->SetScale(scale);
		}

		//���f���̍쐬
		if (ImGui::Button("Create")) { Create(); }
	}

	//���f�����X�g
	size_t model_number = model_names_.size();
	for (size_t count = 0; count < model_number; ++count)
	{
		ImGui::Text("%s : %d", string(model_names_[count].begin(), model_names_[count].end()).c_str()
			, static_cast<int>(created_model_infos_[count].size()));
	}
	if (ImGui::Button("Show Created List")) { is_show_created_list_ ^= 1; }

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  ShowTypeListBox
//--------------------------------------------------------------------------------
void ModelEditor::ShowTypeListBox(void)
{
	// copy name
	size_t model_number = model_names_.size();
	vector<string> labels;
	labels.reserve(model_number);
	for (auto& model_name : model_names_)
	{
		labels.push_back(string(model_name.begin(), model_name.end()));
	}

	//Type
	if (ImGui::Combo("Model", (int*)&current_model_no_, labels))
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

	size_t model_number = model_names_.size();
	for (size_t count = 0; count < model_number; ++count)
	{
		string model_name = string(model_names_[count].begin(), model_names_[count].end());
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
					if (ImGui::InputFloat3("Trans", &position.x_))
					{
						iterator->my_transform->SetPosition(position);
					}
					if (ImGui::InputFloat3("Rot", &iterator->rotation.x_, 0.0f, kPi * 2.0f))
					{
						iterator->my_transform->SetRotation(iterator->rotation);
					}
					if (ImGui::InputFloat3("Scale", &scale.x_))
					{
						iterator->my_transform->SetScale(scale);
					}

					//Delete
					is_delete = ImGui::Button("Delete");
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
#endif // EDITOR
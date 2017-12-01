//--------------------------------------------------------------------------------
//	モデルエディタビヘイビアコンポネント
//　ModelEditor.h
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
//  コンストラクタ
//--------------------------------------------------------------------------------
ModelEditor::ModelEditor(GameObject& owner)
	: Behavior(owner, L"ModelEditor")
	, current_model_no_(0)
	, is_active_(false)
	, is_show_created_list_(false)
{}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool ModelEditor::Init(void)
{
	//model_names_[Box] = "cube";
	//model_names_[Tree] = "tree_1";
	//model_names_[PineTree] = "Pine_tree";
	//model_names_[Windmill] = "Medieval_Windmill";
	//model_names_[Rock1] = "rock_1";
	//model_names_[Rock2] = "rock_2";
	//model_names_[Rock3] = "rock_3";
	//model_names_[BakerHouse] = "Baker_house";
	//model_names_[Barrel] = "Barrayel";
	//model_names_[Bridge] = "Bridge";
	//model_names_[Fence] = "Fence";
	//model_names_[FancyTavern] = "Fancy_Tavern";
	//model_names_[MedHouse1] = "Medieval house";
	//model_names_[MedHouse2] = "Medieval_house_1";
	//model_names_[MedHouseFantasy] = "Medieva_fantasy_house";
	//model_names_[House] = "House";
	//model_names_[Well] = "cartoon_well";
	//model_names_[MedBridge] = "Medieval Bridge";

	// フォルダから全てのファイルを読み込む
	String& folder_path = Utility::GetProjectPath() + L"data\\model";
	model_names_ = Utility::GetFilesFromFolder(folder_path, L"model");

	//Demo Objectの作成
	size_t model_number = model_names_.size();
	demo_model_infos_.resize(model_number);
	for (size_t count = 0; count < model_number; ++count)
	{
		auto gameObject = GameObjectSpawner::CreateModel(model_names_[count], Vector3::kZero, Quaternion::kIdentity, Vector3::kOne);
		gameObject->SetActive(false);
		demo_model_infos_[count].my_transform = gameObject->GetTransform();
	}
	created_model_infos_.resize(model_number);
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void ModelEditor::Uninit(void)
{
	for (auto& modelName : model_names_) { modelName.clear(); }
	for (auto& list : created_model_infos_) { list.clear(); }
}

//--------------------------------------------------------------------------------
//  更新処理
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
//  更新処理
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
	demo_model_infos_[current_model_no_].my_transform->SetPosition(position);
}

//--------------------------------------------------------------------------------
//  SaveAs
//--------------------------------------------------------------------------------
void ModelEditor::SaveAsBinary(const String& name)
{
	//フィールドの保存
	String path = L"data/stage/" + name + L".stage";
	ofstream file(path);
	if (!file.is_open())
	{
		assert("failed to open file");
		return;
	}
	BinaryOutputArchive archive(file);

	//Model数の保存
	size_t model_number = model_names_.size();
	archive.saveBinary(&model_number, sizeof model_number);

	for (size_t count = 0; count < model_number; ++count)
	{
		//ファイル名保存
		size_t name_size = model_names_[count].size();
		archive.saveBinary(&name_size, sizeof(name_size));
		archive.saveBinary(&model_names_[count][0], name_size);
		
		//作ったモデル数の保存
		size_t created_model_number = created_model_infos_[count].size();
		archive.saveBinary(&created_model_number, sizeof(created_model_number));

		//位置回転スケールの保存
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

	// Model Trans
	auto& current_info = demo_model_infos_[current_model_no_];

	//モデル回転
	if(ImGui::SliderFloat3("rotation", &current_info.rotation.x_, 0.0f, kPi * 2.0f))
	{ 
		current_info.my_transform->SetRotation(current_info.rotation);
	}

	//モデルの作成
	if (ImGui::Button("Create")) { Create(); }

	//モデルリスト
	size_t model_number = model_names_.size();
	for (size_t count = 0; count < model_number; ++count)
	{
		ImGui::Text("%s : %d", model_names_[count].c_str()
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
		//モデルアクティブの設定
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
//--------------------------------------------------------------------------------
//	モデルエディタビヘイビアコンポネント
//　ModelEditor.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-23
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "versionSetting.h"
#if defined(_DEBUG) || defined(EDITOR)
#include "main.h"
#include "gameObject.h"
#include "gameObjectSpawner.h"
#include "transform.h"
#include "ModelEditor.h"
#include "ImGui\imgui.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
ModelEditor::ModelEditor(GameObject* const owner)
	: Behavior(owner, "ModelEditor")
	, currentModelType(ModelType::Box)
	, isActive(false)
	, isShowCreatedList(false)
{
	for (auto& modelName : modelNames) { modelName.clear(); }
	for (auto& list : createdModelInfos) { list.clear(); }
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool ModelEditor::Init(void)
{
	modelNames[Box] = "cube";
	modelNames[Tree] = "tree_1";
	modelNames[PineTree] = "Pine_tree";
	modelNames[Windmill] = "Medieval_Windmill";
	modelNames[Rock1] = "rock_1";
	modelNames[Rock2] = "rock_2";
	modelNames[Rock3] = "rock_3";
	modelNames[BakerHouse] = "Baker_house";
	modelNames[Barrel] = "Barrayel";
	modelNames[Bridge] = "Bridge";
	modelNames[Fence] = "Fence";
	modelNames[FancyTavern] = "Fancy_Tavern";
	modelNames[MedHouse1] = "Medieval house";
	modelNames[MedHouse2] = "Medieval_house_1";
	modelNames[MedHouseFantasy] = "Medieva_fantasy_house";
	modelNames[House] = "House";
	modelNames[Well] = "cartoon_well";
	modelNames[MedBridge] = "Medieval Bridge";

	//Demo Objectの作成
	for (int count = 0; count < (int)ModelType::Max; ++count)
	{
		auto gameObject = GameObjectSpawner::CreateModel(modelNames[count] + ".model", Vector3::Zero, Quaternion::Identity, Vector3::One);
		gameObject->SetActive(false);
		demoModelInfos[count].MyTransform = gameObject->GetTransform();
	}
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void ModelEditor::Uninit(void)
{
	for (auto& modelName : modelNames) { modelName.clear(); }
	for (auto& list : createdModelInfos) { list.clear(); }
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void ModelEditor::Update(void)
{
	if (!isActive)
	{
		for (auto info : demoModelInfos) { info.MyTransform->GetGameObject()->SetActive(false); }
		return;
	}

	showMainWindow();
	showCreatedList();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void ModelEditor::LateUpdate(void)
{
	if (!isActive)
	{
		return;
	}
}

//--------------------------------------------------------------------------------
//  SetPosition
//--------------------------------------------------------------------------------
void ModelEditor::SetPosition(const Vector3& position)
{
	demoModelInfos[(int)currentModelType].MyTransform->SetNextPosition(position);
}

//--------------------------------------------------------------------------------
//  SaveAs
//--------------------------------------------------------------------------------
void ModelEditor::SaveAs(const string& fileName)
{
	//フィールドの保存
	string strName = "data/STAGE/" + fileName + ".stage";
	FILE *filePointer;

	//file open
	fopen_s(&filePointer, strName.c_str(), "wb");

	//ModelType数の保存
	int nTypeSize = (int)ModelType::Max;
	fwrite(&nTypeSize, sizeof(int), 1, filePointer);

	for (int count = 0; count < (int)ModelType::Max; ++count)
	{
		//ファイル名保存
		int nameSize = (int)modelNames[count].size();
		fwrite(&nameSize, sizeof(int), 1, filePointer);
		fwrite(&modelNames[count][0], sizeof(char), nameSize, filePointer);

		//モデル数の保存
		int number = createdModelInfos[count].size();
		fwrite(&number, sizeof(int), 1, filePointer);

		//位置回転の保存
		for (auto& info : createdModelInfos[count])
		{
			auto& position = info.MyTransform->GetCurrentPosition();
			fwrite(&position, sizeof(Vector3), 1, filePointer);
			auto& rotation = info.MyTransform->GetCurrentRotation();
			fwrite(&rotation, sizeof(Quaternion), 1, filePointer);
		}
	}

	fclose(filePointer);
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  create
//--------------------------------------------------------------------------------
void ModelEditor::create(void)
{
	auto gameObject = GameObjectSpawner::CreateModel(modelNames[(int)currentModelType] + ".model", Vector3::Zero, Quaternion::Identity, Vector3::One);
	auto transform = gameObject->GetTransform();
	auto currentInfo = demoModelInfos[(int)currentModelType];
	auto& position = currentInfo.MyTransform->GetCurrentPosition();
	auto& rotation = currentInfo.MyTransform->GetCurrentRotation();
	transform->SetNextPosition(position);
	transform->SetNextRotation(rotation);
	Info info;
	info.MyTransform = transform;
	info.Rotation = currentInfo.Rotation;
	createdModelInfos[(int)currentModelType].push_back(info);
}

//--------------------------------------------------------------------------------
//  showMainWindow
//--------------------------------------------------------------------------------
void ModelEditor::showMainWindow(void)
{
	// Begin
	if (!ImGui::Begin("Model Editor Window"))
	{
		ImGui::End();
		return;
	}

	// Model Type
	showTypeListBox();

	// Model Trans
	auto& infoNow = demoModelInfos[(int)currentModelType];

	//モデル回転
	if(ImGui::SliderFloat3("Rot", &infoNow.Rotation.X, 0.0f, Pi * 2.0f))
	{ 
		infoNow.MyTransform->SetNextRotation(infoNow.Rotation);
	}

	//モデルの作成
	if (ImGui::Button("Create")) { create(); }

	//モデルリスト
	for (int count = 0; count < (int)ModelType::Max; ++count)
	{
		ImGui::Text("%s : %d", modelNames[count].c_str(), (int)createdModelInfos[count].size());
	}
	if (ImGui::Button("Show Created List")) { isShowCreatedList ^= 1; }

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  showTypeListBox
//--------------------------------------------------------------------------------
void ModelEditor::showTypeListBox(void)
{
	//new
	char **array = new char*[ModelType::Max];
	for (int count = 0; count < (int)ModelType::Max; ++count)
	{
		auto& strName = modelNames[count];
		int nNumChar = (int)strName.size();
		array[count] = new char[nNumChar + 1];
		for (int countChar = 0; countChar < nNumChar; ++countChar)
		{
			array[count][countChar] = strName[countChar];
		}
		array[count][nNumChar] = '\0';
	}

	//Type
	if (ImGui::ListBox("Model Type\n", (int*)&currentModelType, array, ModelType::Max))
	{
		//モデルアクティブの設定
		for (int count = 0; count < (int)ModelType::Max; ++count)
		{
			demoModelInfos[count].MyTransform->GetGameObject()->SetActive(currentModelType == (ModelType)count);
		}
	}

	//delete
	for (int count = 0; count < (int)ModelType::Max; ++count)
	{
		delete[] array[count];
		array[count] = nullptr;
	}
	delete[] array;
	array = nullptr;
}

//--------------------------------------------------------------------------------
//  showCreatedList
//--------------------------------------------------------------------------------
void ModelEditor::showCreatedList(void)
{
	if (!isShowCreatedList) return;

	for (int count = 0; count < (int)ModelType::Max; ++count)
	{
		if (ImGui::TreeNode(modelNames[count].c_str()))
		{
			int countObject = 0;
			for (auto itr = createdModelInfos[count].begin(); itr != createdModelInfos[count].end();)
			{
				bool isDelete = false;
				char buffer[128];
				wsprintf(buffer, "%s_%d", modelNames[count].c_str(), countObject);
				if (ImGui::TreeNode(buffer))
				{
					auto& position = itr->MyTransform->GetCurrentPosition();
					auto& scale = itr->MyTransform->GetCurrentScale();

					//Offset
					if (ImGui::InputFloat3("Trans", &position.X)) itr->MyTransform->SetNextPosition(position);
					if (ImGui::InputFloat3("Rot", &itr->Rotation.X, 0.0f, Pi * 2.0f)) itr->MyTransform->SetNextRotation(itr->Rotation);
					if (ImGui::InputFloat3("Scale", &scale.X)) itr->MyTransform->SetNextScale(scale);

					//Delete
					isDelete = ImGui::Button("Delete");
					ImGui::TreePop();
				}

				if (isDelete)
				{
					itr->MyTransform->GetGameObject()->SetAlive(false);
					itr = createdModelInfos[count].erase(itr);
				}
				else
				{
					++itr;
					++countObject;
				}
			}
			ImGui::TreePop();
		}
	}
}

#endif // _DEBUG
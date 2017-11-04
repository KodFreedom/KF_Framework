//--------------------------------------------------------------------------------
//	モデルエディタビヘイビアコンポネント
//　modelEditorBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-23
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "versionSetting.h"
#if defined(_DEBUG) || defined(EDITOR)
#include "main.h"
#include "manager.h"
#include "inputManager.h"
#include "gameObject.h"
#include "gameObjectSpawner.h"
#include "transformComponent.h"
#include "modelEditorBehaviorComponent.h"
#include "ImGui\imgui.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
ModelEditorBehaviorComponent::ModelEditorBehaviorComponent(GameObject* const pGameObj)
	: CBehaviorComponent(pGameObj)
	, modelType(MT_BOX)
	, m_bActive(false)
	, m_bShowCreatedList(false)
{
	for (auto& str : m_aStrName) { str.clear(); }
	for (auto& list : m_alistCreated) { list.clear(); }
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool ModelEditorBehaviorComponent::Init(void)
{
	m_aStrName[MT_BOX] = "cube";
	m_aStrName[MT_TREE] = "tree_1";
	m_aStrName[MT_PINE_TREE] = "Pine_tree";
	m_aStrName[MT_WINDMILL] = "Medieval_Windmill";
	m_aStrName[MT_ROCK_1] = "rock_1";
	m_aStrName[MT_ROCK_2] = "rock_2";
	m_aStrName[MT_ROCK_3] = "rock_3";
	m_aStrName[MT_BAKER_HOUSE] = "Baker_house";
	m_aStrName[MT_BARREL] = "Barrel";
	m_aStrName[MT_BRIDGE] = "Bridge";
	m_aStrName[MT_FENCE] = "Fence";
	m_aStrName[MT_FANCY_TAVERN] = "Fancy_Tavern";
	m_aStrName[MT_MED_HOUSE] = "Medieval house";
	m_aStrName[MT_MED_HOUSE_1] = "Medieval_house_1";
	m_aStrName[MT_MED_HOUSE_FANTASY] = "Medieva_fantasy_house";
	m_aStrName[MT_HOUSE] = "House";
	m_aStrName[MT_WELL] = "cartoon_well";
	m_aStrName[MT_MED_BRIDGE] = "Medieval Bridge";

	//Demo Objectの作成
	for (int count = 0; count < (int)MT_MAX; ++count)
	{
		auto pObj = GameObjectSpawner::CreateModel(m_aStrName[count] + ".model", CKFMath::sc_vZero, CKFMath::sc_qRotZero, CKFMath::sc_vOne);
		pObj->SetActive(false);
		m_aObjInfoDemo[count].pTransform = pObj->GetTransformComponent();
	}
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void ModelEditorBehaviorComponent::Uninit(void)
{
	for (auto& str : m_aStrName) { str.clear(); }
	for (auto& list : m_alistCreated) { list.clear(); }
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void ModelEditorBehaviorComponent::Update(void)
{
	if (!m_bActive)
	{
		for (auto info : m_aObjInfoDemo) { info.pTransform->GetGameObject()->SetActive(false); }
		return;
	}

	showMainWindow();
	showCreatedList();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void ModelEditorBehaviorComponent::LateUpdate(void)
{
	if (!m_bActive)
	{
		return;
	}
}

//--------------------------------------------------------------------------------
//  SetPos
//--------------------------------------------------------------------------------
void ModelEditorBehaviorComponent::SetPos(const Vector3& Position)
{
	m_aObjInfoDemo[(int)modelType].pTransform->SetPosNext(Position);
}

//--------------------------------------------------------------------------------
//  SetPos
//--------------------------------------------------------------------------------
void ModelEditorBehaviorComponent::SaveAs(const string& strFileName)
{
	//フィールドの保存
	string strName = "data/STAGE/" + strFileName + ".stage";
	FILE *filePointer;

	//file open
	fopen_s(&filePointer, strName.c_str(), "wb");

	//ModelType数の保存
	int nTypeSize = (int)MT_MAX;
	fwrite(&nTypeSize, sizeof(int), 1, filePointer);

	for (int count = 0; count < (int)MT_MAX; ++count)
	{
		//ファイル名保存
		int nSize = (int)m_aStrName[count].size();
		fwrite(&nSize, sizeof(int), 1, filePointer);
		fwrite(&m_aStrName[count][0], sizeof(char), nSize, filePointer);

		//モデル数の保存
		int nNum = m_alistCreated[count].size();
		fwrite(&nNum, sizeof(int), 1, filePointer);

		//位置回転の保存
		for (auto& info : m_alistCreated[count])
		{
			auto Position = info.pTransform->GetPos();
			fwrite(&Position, sizeof(Vector3), 1, filePointer);
			auto qRot = info.pTransform->GetRot();
			fwrite(&qRot, sizeof(Quaternion), 1, filePointer);
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
void ModelEditorBehaviorComponent::create(void)
{
	auto pObj = GameObjectSpawner::CreateModel(m_aStrName[(int)modelType] + ".model", CKFMath::sc_vZero, CKFMath::sc_qRotZero, CKFMath::sc_vOne);;
	auto pTrans = pObj->GetTransformComponent();
	auto infoNow = m_aObjInfoDemo[(int)modelType];
	auto& Position = infoNow.pTransform->GetPos();
	auto& qRot = infoNow.pTransform->GetRot();
	pTrans->SetPosNext(Position);
	pTrans->SetRotNext(qRot);
	INFO info;
	info.pTransform = pTrans;
	info.vRot = infoNow.vRot;
	m_alistCreated[(int)modelType].push_back(info);
}

//--------------------------------------------------------------------------------
//  showMainWindow
//--------------------------------------------------------------------------------
void ModelEditorBehaviorComponent::showMainWindow(void)
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
	auto& infoNow = m_aObjInfoDemo[(int)modelType];

	//モデル回転
	if(ImGui::SliderFloat3("Rot", &infoNow.vRot.X, 0.0f, KF_PI * 2.0f))
	{ 
		infoNow.pTransform->SetRotNext(infoNow.vRot);
	}

	//モデルの作成
	if (ImGui::Button("Create")) { create(); }

	//モデルリスト
	for (int count = 0; count < (int)MT_MAX; ++count)
	{
		ImGui::Text("%s : %d", m_aStrName[count].c_str(), (int)m_alistCreated[count].size());
	}
	if (ImGui::Button("Show Created List")) { m_bShowCreatedList ^= 1; }

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  showTypeListBox
//--------------------------------------------------------------------------------
void ModelEditorBehaviorComponent::showTypeListBox(void)
{
	//new
	char **arr = new char*[MT_MAX];
	for (int count = 0; count < (int)MT_MAX; ++count)
	{
		auto& strName = m_aStrName[count];
		int nNumChar = (int)strName.size();
		arr[count] = new char[nNumChar + 1];
		for (int countChar = 0; countChar < nNumChar; ++countChar)
		{
			arr[count][countChar] = strName[countChar];
		}
		arr[count][nNumChar] = '\0';
	}

	//Type
	if (ImGui::ListBox("Model Type\n", (int*)&modelType, arr, MT_MAX))
	{
		//モデルアクティブの設定
		for (int count = 0; count < (int)MT_MAX; ++count)
		{
			m_aObjInfoDemo[count].pTransform->GetGameObject()->SetActive(modelType == (MODEL_TYPE)count);
		}
	}

	//delete
	for (int count = 0; count < (int)MT_MAX; ++count)
	{
		delete[] arr[count];
		arr[count] = nullptr;
	}
	delete[] arr;
	arr = nullptr;
}

//--------------------------------------------------------------------------------
//  showCreatedList
//--------------------------------------------------------------------------------
void ModelEditorBehaviorComponent::showCreatedList(void)
{
	if (!m_bShowCreatedList) { return; }

	for (int count = 0; count < (int)MT_MAX; ++count)
	{
		if (ImGui::TreeNode(m_aStrName[count].c_str()))
		{
			int countObj = 0;
			for (auto itr = m_alistCreated[count].begin(); itr != m_alistCreated[count].end();)
			{
				bool bDelete = false;
				char aBuf[128];
				wsprintf(aBuf, "%s_%d", m_aStrName[count].c_str(), countObj);
				if (ImGui::TreeNode(aBuf))
				{
					auto Position = itr->pTransform->GetPos();
					auto vScale = itr->pTransform->GetScale();

					//Offset
					if (ImGui::InputFloat3("Trans", &Position.X)) { itr->pTransform->SetPosNext(Position); }
					if (ImGui::InputFloat3("Rot", &itr->vRot.X, 0.0f, KF_PI * 2.0f)) { itr->pTransform->SetRotNext(itr->vRot); }
					if (ImGui::InputFloat3("Scale", &vScale.X)) { itr->pTransform->SetScaleNext(vScale); }

					//Delete
					bDelete = ImGui::Button("Delete");
					ImGui::TreePop();
				}

				if (bDelete)
				{
					itr->pTransform->GetGameObject()->SetAlive(false);
					itr = m_alistCreated[count].erase(itr);
				}
				else
				{
					++itr;
					++countObj;
				}
			}
			ImGui::TreePop();
		}
	}
}

#endif // _DEBUG
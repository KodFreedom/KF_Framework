//--------------------------------------------------------------------------------
//	モデルエディタビヘイビアコンポネント
//　modelEditorBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-23
//--------------------------------------------------------------------------------
#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
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
CModelEditorBehaviorComponent::CModelEditorBehaviorComponent(CGameObject* const pGameObj)
	: CBehaviorComponent(pGameObj)
	, m_modelType(MT_BOX)
	, m_bActive(false)
	, m_bShowCreatedList(false)
{
	for (auto& str : m_aStrName) { str.clear(); }
	for (auto& list : m_alistCreated) { list.clear(); }
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool CModelEditorBehaviorComponent::Init(void)
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
	for (int nCnt = 0; nCnt < (int)MT_MAX; ++nCnt)
	{
		auto pObj = CGameObjectSpawner::CreateModel(m_aStrName[nCnt] + ".model", CKFMath::sc_vZero, CKFMath::sc_qRotZero, CKFMath::sc_vOne);
		pObj->SetActive(false);
		m_aObjInfoDemo[nCnt].pTransform = pObj->GetTransformComponent();
	}
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::Uninit(void)
{
	for (auto& str : m_aStrName) { str.clear(); }
	for (auto& list : m_alistCreated) { list.clear(); }
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::Update(void)
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
void CModelEditorBehaviorComponent::LateUpdate(void)
{
	if (!m_bActive)
	{
		return;
	}
}

//--------------------------------------------------------------------------------
//  SetPos
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::SetPos(const CKFVec3& vPos)
{
	m_aObjInfoDemo[(int)m_modelType].pTransform->SetPosNext(vPos);
}

//--------------------------------------------------------------------------------
//  SetPos
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::SaveAs(const string& strFileName)
{
	//フィールドの保存
	string strName = "data/STAGE/" + strFileName + ".stage";
	FILE *pFile;

	//file open
	fopen_s(&pFile, strName.c_str(), "wb");

	//ModelType数の保存
	int nTypeSize = (int)MT_MAX;
	fwrite(&nTypeSize, sizeof(int), 1, pFile);

	for (int nCnt = 0; nCnt < (int)MT_MAX; ++nCnt)
	{
		//ファイル名保存
		int nSize = (int)m_aStrName[nCnt].size();
		fwrite(&nSize, sizeof(int), 1, pFile);
		fwrite(&m_aStrName[nCnt][0], sizeof(char), nSize, pFile);

		//モデル数の保存
		int nNum = m_alistCreated[nCnt].size();
		fwrite(&nNum, sizeof(int), 1, pFile);

		//位置回転の保存
		for (auto& info : m_alistCreated[nCnt])
		{
			auto vPos = info.pTransform->GetPos();
			fwrite(&vPos, sizeof(CKFVec3), 1, pFile);
			auto qRot = info.pTransform->GetRot();
			fwrite(&qRot, sizeof(CKFQuaternion), 1, pFile);
		}
	}

	fclose(pFile);
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  create
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::create(void)
{
	auto pObj = CGameObjectSpawner::CreateModel(m_aStrName[(int)m_modelType] + ".model", CKFMath::sc_vZero, CKFMath::sc_qRotZero, CKFMath::sc_vOne);;
	auto pTrans = pObj->GetTransformComponent();
	auto infoNow = m_aObjInfoDemo[(int)m_modelType];
	auto& vPos = infoNow.pTransform->GetPos();
	auto& qRot = infoNow.pTransform->GetRot();
	pTrans->SetPosNext(vPos);
	pTrans->SetRotNext(qRot);
	INFO info;
	info.pTransform = pTrans;
	info.vRot = infoNow.vRot;
	m_alistCreated[(int)m_modelType].push_back(info);
}

//--------------------------------------------------------------------------------
//  showMainWindow
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::showMainWindow(void)
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
	auto& infoNow = m_aObjInfoDemo[(int)m_modelType];

	//モデル回転
	if(ImGui::SliderFloat3("Rot", &infoNow.vRot.m_fX, 0.0f, KF_PI * 2.0f))
	{ 
		infoNow.pTransform->SetRotNext(infoNow.vRot);
	}

	//モデルの作成
	if (ImGui::Button("Create")) { create(); }

	//モデルリスト
	for (int nCnt = 0; nCnt < (int)MT_MAX; ++nCnt)
	{
		ImGui::Text("%s : %d", m_aStrName[nCnt].c_str(), (int)m_alistCreated[nCnt].size());
	}
	if (ImGui::Button("Show Created List")) { m_bShowCreatedList ^= 1; }

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  showTypeListBox
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::showTypeListBox(void)
{
	//new
	char **arr = new char*[MT_MAX];
	for (int nCnt = 0; nCnt < (int)MT_MAX; ++nCnt)
	{
		auto& strName = m_aStrName[nCnt];
		int nNumChar = (int)strName.size();
		arr[nCnt] = new char[nNumChar + 1];
		for (int nCntChar = 0; nCntChar < nNumChar; ++nCntChar)
		{
			arr[nCnt][nCntChar] = strName[nCntChar];
		}
		arr[nCnt][nNumChar] = '\0';
	}

	//Type
	if (ImGui::ListBox("Model Type\n", (int*)&m_modelType, arr, MT_MAX))
	{
		//モデルアクティブの設定
		for (int nCnt = 0; nCnt < (int)MT_MAX; ++nCnt)
		{
			m_aObjInfoDemo[nCnt].pTransform->GetGameObject()->SetActive(m_modelType == (MODEL_TYPE)nCnt);
		}
	}

	//delete
	for (int nCnt = 0; nCnt < (int)MT_MAX; ++nCnt)
	{
		delete[] arr[nCnt];
		arr[nCnt] = nullptr;
	}
	delete[] arr;
	arr = nullptr;
}

//--------------------------------------------------------------------------------
//  showCreatedList
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::showCreatedList(void)
{
	if (!m_bShowCreatedList) { return; }

	for (int nCnt = 0; nCnt < (int)MT_MAX; ++nCnt)
	{
		if (ImGui::TreeNode(m_aStrName[nCnt].c_str()))
		{
			int nCntObj = 0;
			for (auto itr = m_alistCreated[nCnt].begin(); itr != m_alistCreated[nCnt].end();)
			{
				bool bDelete = false;
				char aBuf[128];
				wsprintf(aBuf, "%s_%d", m_aStrName[nCnt].c_str(), nCntObj);
				if (ImGui::TreeNode(aBuf))
				{
					auto vPos = itr->pTransform->GetPos();
					auto vScale = itr->pTransform->GetScale();

					//Offset
					if (ImGui::InputFloat3("Trans", &vPos.m_fX)) { itr->pTransform->SetPosNext(vPos); }
					if (ImGui::InputFloat3("Rot", &itr->vRot.m_fX, 0.0f, KF_PI * 2.0f)) { itr->pTransform->SetRotNext(itr->vRot); }
					if (ImGui::InputFloat3("Scale", &vScale.m_fX)) { itr->pTransform->SetScaleNext(vScale); }

					//Delete
					bDelete = ImGui::Button("Delete");
					ImGui::TreePop();
				}

				if (bDelete)
				{
					itr->pTransform->GetGameObject()->SetAlive(false);
					itr = m_alistCreated[nCnt].erase(itr);
				}
				else
				{
					++itr;
					++nCntObj;
				}
			}
			ImGui::TreePop();
		}
	}
}

#endif // _DEBUG
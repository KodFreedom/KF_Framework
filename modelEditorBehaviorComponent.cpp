//--------------------------------------------------------------------------------
//	���f���G�f�B�^�r�w�C�r�A�R���|�l���g
//�@modelEditorBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-23
//--------------------------------------------------------------------------------
#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
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
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CModelEditorBehaviorComponent::CModelEditorBehaviorComponent(CGameObject* const pGameObj)
	: CBehaviorComponent(pGameObj)
	, m_modelType(MT_BOX)
	, m_bActive(false)
	, m_bShowCreatedList(false)
	, m_fMoveSpeed(1.0f)
	, m_fRotSpeed(0.01f)
{
	for (auto& str : m_aStrName) { str.clear(); }
	for (auto& pObj : m_apObjDemo) { pObj = nullptr; }
	for (auto& list : m_alistCreated) { list.clear(); }
}

//--------------------------------------------------------------------------------
//  ����������
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

	//Demo Object�̍쐬
	for (int nCnt = 0; nCnt < (int)MT_MAX; ++nCnt)
	{
		m_apObjDemo[nCnt] = CGameObjectSpawner::CreateModel(m_aStrName[nCnt] + ".model", CKFMath::sc_vZero, CKFMath::sc_qRotZero, CKFMath::sc_vOne);
		m_apObjDemo[nCnt]->SetActive(false);
	}
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::Uninit(void)
{
	for (auto& str : m_aStrName) { str.clear(); }
	for (auto& list : m_alistCreated) { list.clear(); }
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::Update(void)
{
	if (!m_bActive)
	{
		for (auto pObj : m_apObjDemo) { pObj->SetActive(false); }
		return;
	}

	showMainWindow();
	showCreatedList();
}

//--------------------------------------------------------------------------------
//  �X�V����
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
	auto pTrans = m_apObjDemo[(int)m_modelType]->GetTransformComponent();
	pTrans->SetPosNext(vPos);
}

//--------------------------------------------------------------------------------
//  SetPos
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::SaveAs(const string& strFileName)
{
	//�t�B�[���h�̕ۑ�
	string strName = "data/STAGE/" + strFileName + ".stage";
	FILE *pFile;

	//file open
	fopen_s(&pFile, strName.c_str(), "wb");

	//ModelType���̕ۑ�
	int nTypeSize = (int)MT_MAX;
	fwrite(&nTypeSize, sizeof(int), 1, pFile);

	for (int nCnt = 0; nCnt < (int)MT_MAX; ++nCnt)
	{
		//�t�@�C�����ۑ�
		int nSize = (int)m_aStrName[nCnt].size();
		fwrite(&nSize, sizeof(int), 1, pFile);
		fwrite(&m_aStrName[nCnt][0], sizeof(char), nSize, pFile);

		//���f�����̕ۑ�
		int nNum = m_alistCreated[nCnt].size();
		fwrite(&nNum, sizeof(int), 1, pFile);

		//�ʒu��]�̕ۑ�
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
	auto pTransNow = m_apObjDemo[(int)m_modelType]->GetTransformComponent();
	auto& vPos = pTransNow->GetPos();
	auto& qRot = pTransNow->GetRot();
	pTrans->SetPosNext(vPos);
	pTrans->SetRotNext(qRot);
	INFO info;
	info.pTransform = pTrans;
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
	auto pTransNow = m_apObjDemo[(int)m_modelType]->GetTransformComponent();

	//���f����]
	auto vRot = pTransNow->GetEulerRot();
	if(ImGui::SliderFloat3("Rot", &vRot.m_fX, 0.0f, KF_PI * 2.0f)){ pTransNow->SetRotNext(vRot); }

	//���f���̍쐬
	if (ImGui::Button("Create")) { create(); }

	//���f�����X�g
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
		arr[nCnt] = new char[nNumChar];
		for (int nCntChar = 0; nCntChar < nNumChar; ++nCntChar)
		{
			arr[nCnt][nCntChar] = strName[nCntChar];
		}
	}

	//Type
	if (ImGui::ListBox("Model Type\n", (int*)&m_modelType, arr, MT_MAX, MT_MAX))
	{
		//���f���A�N�e�B�u�̐ݒ�
		for (int nCnt = 0; nCnt < (int)MT_MAX; ++nCnt)
		{
			m_apObjDemo[nCnt]->SetActive(m_modelType == (MODEL_TYPE)nCnt);
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
					auto vRot = itr->pTransform->GetEulerRot();
					auto vScale = itr->pTransform->GetScale();

					//Offset
					if (ImGui::InputFloat3("Trans", &vPos.m_fX)) { itr->pTransform->SetPosNext(vPos); }
					if (ImGui::SliderFloat3("Rot", &vRot.m_fX, 0.0f, KF_PI * 2.0f)) { itr->pTransform->SetRotNext(vRot); }
					if (ImGui::InputFloat3("Scale", &vScale.m_fX)) { itr->pTransform->SetScaleNext(vScale); }

					//Delete
					bDelete = ImGui::Button("Delete");
					ImGui::TreePop();
				}

				if (bDelete)
				{
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
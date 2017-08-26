//--------------------------------------------------------------------------------
//	モデルエディタビヘイビアコンポネント
//　modelEditorBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-23
//--------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const float CModelEditorBehaviorComponent::sc_fRotSpeed = 0.01f;
const float CModelEditorBehaviorComponent::sc_fMoveSpeed = 0.01f;

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CModelEditorBehaviorComponent::CModelEditorBehaviorComponent(CGameObject* const pGameObj)
	: CBehaviorComponent(pGameObj)
	, m_modelType(MT_BOX)
	, m_bActive(false)
{
	for (auto& pObj : m_apObjDemo)
	{
		pObj = nullptr;
	}

	for (auto& list : m_alistCreated)
	{
		list.clear();
	}
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool CModelEditorBehaviorComponent::Init(void)
{
	//Demo Objectの作成
	m_apObjDemo[MT_BOX] = CGameObjectSpawner::CreateModel("cube.model", CKFVec3(0.0f), CKFQuaternion(0.0f), CKFVec3(1.0f));
	m_apObjDemo[MT_TREE] = CGameObjectSpawner::CreateModel("tree_1.model", CKFVec3(0.0f), CKFQuaternion(0.0f), CKFVec3(1.0f));
	m_apObjDemo[MT_PINE_TREE] = CGameObjectSpawner::CreateModel("Pine_tree.model", CKFVec3(0.0f), CKFQuaternion(0.0f), CKFVec3(1.0f));
	//m_vecpObjDemo[MT_ROCK_1] = CGameObjectSpawner::CreateModel("rock_1.model", CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	//m_vecpObjDemo[MT_ROCK_2] = CGameObjectSpawner::CreateModel("rock_2.model", CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	//m_vecpObjDemo[MT_ROCK_3] = CGameObjectSpawner::CreateModel("rock_3.model", CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	//m_vecpObjDemo[MT_ROCK_4] = CGameObjectSpawner::CreateModel("rock_4.model", CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	//m_vecpObjDemo[MT_BAKER_HOUSE] = CGameObjectSpawner::CreateModel("Baker_house.model", CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	m_apObjDemo[MT_WINDMILL] = CGameObjectSpawner::CreateModel("Medieval_Windmill.model", CKFVec3(0.0f), CKFQuaternion(0.0f), CKFVec3(1.0f));
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::Uninit(void)
{

}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::Update(void)
{
	if (!m_bActive)
	{
		for (int nCnt = 0; nCnt < (int)MT_MAX; ++nCnt)
		{
			m_apObjDemo[nCnt]->SetActive(false);
		}
		return;
	}

	auto pInput = CMain::GetManager()->GetInputManager();

	//モデルの切り替え
	m_modelType = (MODEL_TYPE)((int)m_modelType + (int)pInput->GetKeyTrigger(CInputManager::K_RRACKET) - (int)pInput->GetKeyTrigger(CInputManager::K_LRACKET));
	m_modelType = (MODEL_TYPE)((int)m_modelType < 0 ? (int)MT_MAX - 1 : m_modelType >= MT_MAX ? 0 : m_modelType);

	//モデルアクティブの設定
	for (int nCnt = 0; nCnt < (int)MT_MAX; ++nCnt)
	{
		m_apObjDemo[nCnt]->SetActive(m_modelType == (MODEL_TYPE)nCnt);
	}

	auto pTransNow = m_apObjDemo[(int)m_modelType]->GetTransformComponent();

	//モデル升降
	float fHeight = sc_fMoveSpeed * (float)(pInput->GetKeyPress(CInputManager::K_LEFT) - pInput->GetKeyPress(CInputManager::K_RIGHT));
	pTransNow->MovePosNext(CKFVec3(0.0f, fHeight, 0.0f));

	//モデル回転
	CKFVec3 vRot = CKFVec3(0.0f);
	vRot.m_fX = sc_fRotSpeed * (float)(pInput->GetKeyPress(CInputManager::K_ROTXP) - pInput->GetKeyPress(CInputManager::K_ROTXM));
	vRot.m_fY = sc_fRotSpeed * (float)(pInput->GetKeyPress(CInputManager::K_ROTYP) - pInput->GetKeyPress(CInputManager::K_ROTYM));
	vRot.m_fZ = sc_fRotSpeed * (float)(pInput->GetKeyPress(CInputManager::K_ROTZP) - pInput->GetKeyPress(CInputManager::K_ROTZM));
	pTransNow->RotByEuler(vRot);
	if (pInput->GetKeyTrigger(CInputManager::K_RESET))
	{//回転を0に戻す
		pTransNow->SetForwardNext(CKFVec3(0.0f, 0.0f, 1.0f));
		pTransNow->SetUpNext(CKFVec3(0.0f, 1.0f, 0.0f));
		pTransNow->SetRightNext(CKFVec3(1.0f, 0.0f, 0.0f));
	}

	//モデルの作成
	if (pInput->GetKeyTrigger(CInputManager::K_SUBMIT))
	{
		create();
	}
}

//--------------------------------------------------------------------------------
//  SetPos
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::SetPos(const CKFVec3& vPos)
{
	auto pTrans = m_apObjDemo[(int)m_modelType]->GetTransformComponent();
	pTrans->SetPosNext(CKFVec3(vPos.m_fX, pTrans->GetPosNext().m_fY, vPos.m_fZ));
}

//--------------------------------------------------------------------------------
//  SetPos
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::SaveAs(const string& strFileName)
{
	string aStrMeshName[MT_MAX];
	aStrMeshName[MT_BOX] = "cube";
	aStrMeshName[MT_TREE] = "tree_1";
	aStrMeshName[MT_PINE_TREE] = "Pine_tree";
	aStrMeshName[MT_WINDMILL] = "Medieval_Windmill";

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
		int nSize = aStrMeshName[nCnt].size();
		fwrite(&nSize, sizeof(int), 1, pFile);
		fwrite(&aStrMeshName[nCnt][0], sizeof(char), nSize, pFile);

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
//  SetPos
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::create(void)
{
	CTransformComponent* pTrans = nullptr;
	CGameObject* pObj = nullptr;

	switch (m_modelType)
	{
	case MT_BOX:
		pObj = CGameObjectSpawner::CreateModel("cube.model", CKFVec3(0.0f), CKFQuaternion(0.0f), CKFVec3(1.0f));
		break;
	case MT_TREE:
		pObj = CGameObjectSpawner::CreateModel("tree_1.model", CKFVec3(0.0f), CKFQuaternion(0.0f), CKFVec3(1.0f));
		break;
	case MT_PINE_TREE:
		pObj = CGameObjectSpawner::CreateModel("Pine_tree.model", CKFVec3(0.0f), CKFQuaternion(0.0f), CKFVec3(1.0f));
		break;
	case MT_WINDMILL:
		pObj = CGameObjectSpawner::CreateModel("Medieval_Windmill.model", CKFVec3(0.0f), CKFQuaternion(0.0f), CKFVec3(1.0f));
		break;
	default:
		break;
	}

	if (!pObj) { return; }
	pTrans = pObj->GetTransformComponent();
	auto pTransNow = m_apObjDemo[m_modelType]->GetTransformComponent();
	auto& vPos = pTransNow->GetPos();
	auto& qRot = pTransNow->GetRot();
	pTrans->SetPosNext(vPos);
	pTrans->SetRotNext(qRot);
	INFO info;
	info.pTransform = pTrans;
	m_alistCreated[m_modelType].push_back(info);
}
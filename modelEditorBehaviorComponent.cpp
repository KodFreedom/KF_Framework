//--------------------------------------------------------------------------------
//	���f���G�f�B�^�r�w�C�r�A�R���|�l���g
//�@modelEditorBehaviorComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-23
//--------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
const float CModelEditorBehaviorComponent::sc_fRotSpeed = 0.01f;

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CModelEditorBehaviorComponent::CModelEditorBehaviorComponent(CGameObject* const pGameObj)
	: CBehaviorComponent(pGameObj)
	, m_modelType(MT_BOX)
	, m_bActive(false)
{
	m_vecpObjDemo.clear();
	m_listObjCreated.clear();
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool CModelEditorBehaviorComponent::Init(void)
{
	//Demo Object�̍쐬
	m_vecpObjDemo.resize((int)MT_MAX);
	m_vecpObjDemo[MT_BOX] = CGameObjectSpawner::CreateCube(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	m_vecpObjDemo[MT_TREE] = CGameObjectSpawner::CreateModel("tree_1.model", CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	m_vecpObjDemo[MT_PINE_TREE] = CGameObjectSpawner::CreateModel("Pine_tree.model", CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	m_vecpObjDemo[MT_ROCK_1] = CGameObjectSpawner::CreateModel("rock_1.model", CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	m_vecpObjDemo[MT_ROCK_2] = CGameObjectSpawner::CreateModel("rock_2.model", CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	m_vecpObjDemo[MT_ROCK_3] = CGameObjectSpawner::CreateModel("rock_3.model", CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	m_vecpObjDemo[MT_ROCK_4] = CGameObjectSpawner::CreateModel("rock_4.model", CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	m_vecpObjDemo[MT_BAKER_HOUSE] = CGameObjectSpawner::CreateModel("Baker_house.model", CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::Uninit(void)
{

}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::Update(void)
{
	if (!m_bActive)
	{
		for (int nCnt = 0; nCnt < (int)MT_MAX; ++nCnt)
		{
			m_vecpObjDemo[nCnt]->SetActive(false);
		}
		return;
	}

	auto pInput = CMain::GetManager()->GetInputManager();

	//���f���̐؂�ւ�
	m_modelType = (MODEL_TYPE)((int)m_modelType + (int)pInput->GetKeyTrigger(CInputManager::K_RRACKET) - (int)pInput->GetKeyTrigger(CInputManager::K_LRACKET));
	m_modelType = (MODEL_TYPE)((int)m_modelType < 0 ? (int)MT_MAX - 1 : m_modelType >= MT_MAX ? 0 : m_modelType);

	//���f���A�N�e�B�u�̐ݒ�
	for (int nCnt = 0; nCnt < (int)MT_MAX; ++nCnt)
	{
		m_vecpObjDemo[nCnt]->SetActive(m_modelType == (MODEL_TYPE)nCnt);
	}

	auto pTransNow = m_vecpObjDemo[(int)m_modelType]->GetTransformComponent();

	//���f����]
	CKFVec3 vRot = CKFVec3(0.0f);
	vRot.m_fX = sc_fRotSpeed * (float)(pInput->GetKeyPress(CInputManager::K_ROTXP) - pInput->GetKeyPress(CInputManager::K_ROTXM));
	vRot.m_fY = sc_fRotSpeed * (float)(pInput->GetKeyPress(CInputManager::K_ROTYP) - pInput->GetKeyPress(CInputManager::K_ROTYM));
	vRot.m_fZ = sc_fRotSpeed * (float)(pInput->GetKeyPress(CInputManager::K_ROTZP) - pInput->GetKeyPress(CInputManager::K_ROTZM));
	pTransNow->RotByEuler(vRot);
	if (pInput->GetKeyTrigger(CInputManager::K_RESET))
	{//��]��0�ɖ߂�
		pTransNow->SetForwardNext(CKFVec3(0.0f, 0.0f, 1.0f));
		pTransNow->SetUpNext(CKFVec3(0.0f, 1.0f, 0.0f));
		pTransNow->SetRightNext(CKFVec3(1.0f, 0.0f, 0.0f));
	}

	//���f���̍쐬
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
	auto pTrans = m_vecpObjDemo[(int)m_modelType]->GetTransformComponent();
	pTrans->SetPosNext(vPos);
}

//--------------------------------------------------------------------------------
//  SetPos
//--------------------------------------------------------------------------------
void CModelEditorBehaviorComponent::create(void)
{
	switch (m_modelType)
	{
	case MT_BOX:
		break;
	case MT_TREE:
		break;
	default:
		break;
	}
}
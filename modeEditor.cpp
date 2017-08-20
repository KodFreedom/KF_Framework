//--------------------------------------------------------------------------------
//
//�@modeEditor.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "lightManager.h"
#include "inputManager.h"
#include "modeEditor.h"
#include "editorCamera.h"

//gameobject
#include "gameObjectSpawner.h"
#include "gameObjectActor.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CModeEditor::CModeEditor() : CMode()
{

}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
CModeEditor::~CModeEditor()
{

}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CModeEditor::Init(void)
{
	//���C�g�̏�����
	CMain::GetManager()->GetLightManager()->CreateDirectionalLight(CKFVec3(0.5f, -0.5f, 0.5f));

	//�J�����̏�����
	m_pCamera = new CEditorCamera;
	m_pCamera->Init();

	CMain::GetManager()->GetInputManager()->SetEditorMode(true);

	//�Q�[���I�u�W�F�N�g�̏�����
	auto pEditorField = CGameObjectSpawner::CreateEditorField();
	CGameObjectSpawner::CreateEditorController(pEditorField);
	//CGameObject3D::CreateSkyBox(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CModeEditor::Uninit(void)
{
	//�J�����ƃQ�[���I�u�W�F�N�g�̔j��
	CMode::Uninit();

	//���C�g�̔j��
	CMain::GetManager()->GetLightManager()->ReleaseAll();

	CMain::GetManager()->GetInputManager()->SetEditorMode(false);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CModeEditor::Update(void)
{
	CMode::Update();
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CModeEditor::LateUpdate(void)
{
	CMode::LateUpdate();

	//if (m_bEndMode || CMain::GetManager()->GetInputManager()->GetKeyTrigger(CInputManager::KEY::K_SUBMIT))
	//{
	//	CMain::GetManager()->GetFade()->FadeToMode(new CModeResult);
	//}
}
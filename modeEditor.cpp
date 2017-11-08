//--------------------------------------------------------------------------------
//
//�@modeEditor.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-19
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "versionSetting.h"
#if defined(_DEBUG) || defined(EDITOR)
#include "main.h"
#include "manager.h"
#include "lightManager.h"
#include "input.h"
#include "modeEditor.h"
#include "editorCamera.h"

//gameobject
#include "gameObjectSpawner.h"
#include "gameObjectActor.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void ModeEditor::Init(void)
{
	LightManager::Instance()->CreateLight(LightType::Directional, Vector3::Zero, Vector3(0.5f, -0.5f, 0.5f));

	//�J�����̏�����
	auto camera = new EditorCamera;
	camera->Init();

	Input::Instance()->SetEditorMode(true);

	//�Q�[���I�u�W�F�N�g�̏�����
	auto fieldEditor = GameObjectSpawner::CreateFieldEditor();
	GameObjectSpawner::CreateStageEditor(fieldEditor);
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void ModeEditor::LateUpdate(void)
{
	Mode::LateUpdate();
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void ModeEditor::uninit(void)
{
	Mode::uninit();
	Input::Instance()->SetEditorMode(false);
}
#endif // _DEBUG


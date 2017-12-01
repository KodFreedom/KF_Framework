//--------------------------------------------------------------------------------
//
//�@modeEditor.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode_editor.h"
#if defined(EDITOR)
#include "main_system.h"
#include "light_manager.h"
#include "input.h"
#include "editor_camera.h"

//gameobject
#include "game_object_spawner.h"

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
	auto main_system = MainSystem::Instance();
	main_system->GetLightManager()->CreateDirectionLight(Vector3(-1.0f, -1.0f, 1.0f).Normalized());

	//�J�����̏�����
	auto camera = MY_NEW EditorCamera;
	camera->Init();

	main_system->GetInput()->SetEditorMode(true);

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
void ModeEditor::Uninit(void)
{
	Mode::Uninit();
	MainSystem::Instance()->GetInput()->SetEditorMode(false);
}
#endif // _DEBUG

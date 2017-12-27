//--------------------------------------------------------------------------------
//
//�@modeEditor.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode_editor.h"
#if defined(EDITOR)
#include "main_system.h"
#include "light.h"
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

	//�J�����̏�����
	auto camera = MY_NEW EditorCamera;
	camera->Init();

    //���C�g�̏�����
    auto directional_light = MY_NEW DirectionalLight(Vector3(-1.0f, -4.0f, 1.0f).Normalized());

	main_system->GetInput()->SetEditorMode(true);

	//�Q�[���I�u�W�F�N�g�̏�����
	GameObjectSpawner::CreateEditor();
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


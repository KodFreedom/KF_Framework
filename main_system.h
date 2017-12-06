//--------------------------------------------------------------------------------
//�@main_system.h
//	���C���V�X�e���A�x�[�X�V�X�e�����Ǘ�����
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  forward declaration / �O���錾
//--------------------------------------------------------------------------------
class Mode;
class RenderSystem;
class RendererManager;
class Input;
class MaterialManager;
class CollisionSystem;
class PhysicsSystem;
class GameObjectManager;
class UISystem;
class SoundManager;
class FadeSystem;
class CameraManager;
class MotionManager;
class TextureManager;
class MeshManager;
class LightManager;
class ShaderManager;
class ShadowMapSystem;

#if defined(_DEBUG)
class DebugObserver;
#endif

//--------------------------------------------------------------------------------
//  ���C���V�X�e���N���X
//--------------------------------------------------------------------------------
class MainSystem
{
public:
	//--------------------------------------------------------------------------------
	//  create the instance / ��������
	//	hinstance
	//	hwnd
	//	is_window_mode
	//	return�FMainSystem*
	//--------------------------------------------------------------------------------
	static MainSystem* Create(HINSTANCE hinstance, HWND hwnd, BOOL is_window_mode);
	
	//--------------------------------------------------------------------------------
	//  �j������
	//--------------------------------------------------------------------------------
	static void Release(void);

	//--------------------------------------------------------------------------------
	//  return the instance of main system / �C���X�^���X��Ԃ�
	//--------------------------------------------------------------------------------
	static auto Instance(void) { return instance_; }

	//--------------------------------------------------------------------------------
	//  �X�V����
	//--------------------------------------------------------------------------------
	void Update(void);

	//--------------------------------------------------------------------------------
	//  ��X�V����(�`��̑O)
	//--------------------------------------------------------------------------------
	void LateUpdate(void);

	//--------------------------------------------------------------------------------
	//  �`�揈��
	//--------------------------------------------------------------------------------
	void Render(void);

	//--------------------------------------------------------------------------------
	//  change the mode / ���[�h�؂�ւ�����
	//--------------------------------------------------------------------------------
	void Change(Mode* next_mode);

	//--------------------------------------------------------------------------------
	//  �����_�[�V�X�e����Ԃ�
	//  return : RenderSystem* const
	//--------------------------------------------------------------------------------
	const auto GetRenderSystem(void) const { return render_system_; }

	//--------------------------------------------------------------------------------
	//  �e�N�X�`���Ǘ��҂�Ԃ�
	//  return : TextureManager* const
	//--------------------------------------------------------------------------------
	const auto GetTextureManager(void) const { return texture_manager_; }

	//--------------------------------------------------------------------------------
	//  ���b�V���Ǘ��҂�Ԃ�
	//  return : MeshManager* const
	//--------------------------------------------------------------------------------
	const auto GetMeshManager(void) const { return mesh_manager_; }

	//--------------------------------------------------------------------------------
	//  �}�e���A���Ǘ��҂�Ԃ�
	//  return : MaterialManager* const
	//--------------------------------------------------------------------------------
	const auto GetMaterialManager(void) const { return material_manager_; }

	//--------------------------------------------------------------------------------
	//  ���[�V�����Ǘ��҂�Ԃ�
	//  return : MotionManager* const
	//--------------------------------------------------------------------------------
	const auto GetMotionManager(void) const { return motion_manager_; }

	//--------------------------------------------------------------------------------
	//  ���C�g�Ǘ��҂�Ԃ�
	//  return : LightManager* const
	//--------------------------------------------------------------------------------
	const auto GetLightManager(void) const { return light_manager_; }

	//--------------------------------------------------------------------------------
	//  �T�E���h�Ǘ��҂�Ԃ�
	//  return : SoundManager* const
	//--------------------------------------------------------------------------------
	const auto GetSoundManager(void) const { return sound_manager_; }

	//--------------------------------------------------------------------------------
	//  �����_���[�Ǘ��҂�Ԃ�
	//  return : RendererManager* const
	//--------------------------------------------------------------------------------
	const auto GetRendererManager(void) const { return renderer_manager_; }

	//--------------------------------------------------------------------------------
	//  �V�F�[�_�Ǘ��҂�Ԃ�
	//  return : GetShaderManager* const
	//--------------------------------------------------------------------------------
	const auto GetShaderManager(void) const { return shader_manager_; }

#ifdef _DEBUG
	//--------------------------------------------------------------------------------
	//  �f�o�b�O�ώ@�҂�Ԃ�
	//  return : DebugObserver* const
	//--------------------------------------------------------------------------------
	const auto GetDebugObserver(void) const { return debug_observer_; }
#endif

	//--------------------------------------------------------------------------------
	//  �C���v�b�g��Ԃ�
	//  return : Input* const
	//--------------------------------------------------------------------------------
	const auto GetInput(void) const { return input_; }

	//--------------------------------------------------------------------------------
	//  �t�F�C�h�V�X�e����Ԃ�
	//  return : FadeSystem* const
	//--------------------------------------------------------------------------------
	const auto GetFadeSystem(void) const { return fade_system_; }

	//--------------------------------------------------------------------------------
	//  �J�����Ǘ��҂�Ԃ�
	//  return : CameraManager* const
	//--------------------------------------------------------------------------------
	const auto GetCameraManager(void) const { return camera_manager_; }

	//--------------------------------------------------------------------------------
	//  UI�V�X�e����Ԃ�
	//  return : UISystem* const
	//--------------------------------------------------------------------------------
	const auto GetUISystem(void) const { return ui_system_; }

	//--------------------------------------------------------------------------------
	//  �Փˏ����V�X�e����Ԃ�
	//  return : CollisionSystem* const
	//--------------------------------------------------------------------------------
	const auto GetCollisionSystem(void) const { return collision_system_; }

	//--------------------------------------------------------------------------------
	//  �����V�X�e����Ԃ�
	//  return : PhysicsSystem* const
	//--------------------------------------------------------------------------------
	const auto GetPhysicsSystem(void) const { return physics_system_; }

	//--------------------------------------------------------------------------------
	//  �Q�[���I�u�W�F�N�g�Ǘ��҂�Ԃ�
	//  return : GameObjectManager* const
	//--------------------------------------------------------------------------------
	const auto GetGameObjectManager(void) const { return game_object_manager_; }

	//--------------------------------------------------------------------------------
	//  �V���h�E�}�b�v�V�X�e����Ԃ�
	//  return : ShadowMapSystem* const
	//--------------------------------------------------------------------------------
	const auto GetShadowMapSystem(void) const { return shadow_map_system_; }

private:
	//--------------------------------------------------------------------------------
	//  constructors for singleton / �V���O���g���̃R���X�g���N�^
	//--------------------------------------------------------------------------------
	MainSystem() : current_mode_(nullptr), render_system_(nullptr)
		, texture_manager_(nullptr), mesh_manager_(nullptr)
		, material_manager_(nullptr), motion_manager_(nullptr)
		, light_manager_(nullptr), sound_manager_(nullptr)
		, renderer_manager_(nullptr), input_(nullptr)
#ifdef _DEBUG
		, debug_observer_(nullptr)
#endif
		, fade_system_(nullptr), camera_manager_(nullptr)
		, ui_system_(nullptr), collision_system_(nullptr)
	    , physics_system_(nullptr), game_object_manager_(nullptr)
		, shader_manager_(nullptr), shadow_map_system_(nullptr) {}
	MainSystem(const MainSystem& value) {}
	MainSystem& operator=(const MainSystem& value) {}
	~MainSystem() {}

	//--------------------------------------------------------------------------------
	//  init the main system, return ture if is successed
	//  ����������
	//	hinstance
	//	hwnd
	//	is_window_mode
	//	return�Fbool
	//--------------------------------------------------------------------------------
	bool Init(HINSTANCE hinstance, HWND hwnd, BOOL is_window_mode);
	
	//--------------------------------------------------------------------------------
	//  �I������
	//--------------------------------------------------------------------------------
	void Uninit(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	Mode*              current_mode_; // ���̃��[�h
	RenderSystem*      render_system_; // �����_�[�V�X�e��
	TextureManager*    texture_manager_; // �e�N�X�`���Ǘ���
	MeshManager*       mesh_manager_; // ���b�V���Ǘ���
	MaterialManager*   material_manager_; // �}�e���A���Ǘ���
	MotionManager*     motion_manager_; // ���[�V�����Ǘ���
	LightManager*      light_manager_; // ���C�g�̊Ǘ���
	SoundManager*      sound_manager_; // �T�E���h�Ǘ���
	RendererManager*   renderer_manager_; // �����_���[�Ǘ���
	ShaderManager*     shader_manager_; // �V�F�[�_�Ǘ���
#ifdef _DEBUG
	DebugObserver*     debug_observer_; // �f�o�b�O�ώ@��
#endif
	Input*             input_; // ���͊Ǘ���
	FadeSystem*        fade_system_; // �t�F�C�h�V�X�e��
	CameraManager*     camera_manager_; // �J�����Ǘ���
	UISystem*          ui_system_; // UI�V�X�e��
	CollisionSystem*   collision_system_; // �Փˏ����V�X�e��
	PhysicsSystem*     physics_system_; // �����V�X�e��
	GameObjectManager* game_object_manager_; // �Q�[���I�u�W�F�N�g�Ǘ���
	ShadowMapSystem*   shadow_map_system_; // �V���h�E�}�b�v�V�X�e��

	static MainSystem* instance_; // �C���X�^���X
};
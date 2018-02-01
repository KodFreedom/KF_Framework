//--------------------------------------------------------------------------------
//�@main_system.h
//  ���C���V�X�e���A�x�[�X�V�X�e�����Ǘ�����
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include <Windows.h>

//--------------------------------------------------------------------------------
//  forward declaration / �O���錾
//--------------------------------------------------------------------------------
class Mode;
class RenderSystem;
class RendererManager;
class Resources;
class Input;
class CollisionSystem;
class PhysicsSystem;
class GameObjectManager;
class SoundSystem;
class FadeSystem;
class CameraManager;
class LightManager;
class ShaderManager;
class ShadowMapSystem;
class ActorObserver;
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
    //  hinstance
    //  hwnd
    //  is_window_mode
    //  return�FMainSystem*
    //--------------------------------------------------------------------------------
    static MainSystem* Create(HINSTANCE hinstance, HWND hwnd, BOOL is_window_mode);
    
    //--------------------------------------------------------------------------------
    //  �j������
    //--------------------------------------------------------------------------------
    static void Release(void);

    //--------------------------------------------------------------------------------
    //  �C���X�^���X��Ԃ�
    //--------------------------------------------------------------------------------
    static MainSystem& Instance(void) { return *instance_; }

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
    //  �Q�b�^�[
    //--------------------------------------------------------------------------------
    auto& GetRenderSystem(void) const { return *render_system_; }
    auto& GetResources(void) const { return *resources_; }
    auto& GetLightManager(void) const { return *light_manager_; }
    auto& GetSoundSystem(void) const { return *sound_system_; }
    auto& GetRendererManager(void) const { return *renderer_manager_; }
    auto& GetShaderManager(void) const { return *shader_manager_; }
    auto& GetInput(void) const { return *input_; }
    auto& GetFadeSystem(void) const { return *fade_system_; }
    auto& GetCameraManager(void) const { return *camera_manager_; }
    auto& GetCollisionSystem(void) const { return *collision_system_; }
    auto& GetPhysicsSystem(void) const { return *physics_system_; }
    auto& GetGameObjectManager(void) const { return *game_object_manager_; }
    auto& GetShadowMapSystem(void) const { return *shadow_map_system_; }
    auto& GetActorObserver(void) const { return *actor_observer_; }
    auto& GetCurrentMode(void) const { return *current_mode_; }
#ifdef _DEBUG
    auto& GetDebugObserver(void) const { return *debug_observer_; }
#endif

private:
    //--------------------------------------------------------------------------------
    //  constructors for singleton / �V���O���g���̃R���X�g���N�^
    //--------------------------------------------------------------------------------
    MainSystem() {}
    MainSystem(const MainSystem& value) {}
    MainSystem& operator=(const MainSystem& value) {}
    ~MainSystem() {}

    //--------------------------------------------------------------------------------
    //  init the main system, return ture if is successed
    //  ����������
    //  hinstance
    //  hwnd
    //  is_window_mode
    //  return�Fbool
    //--------------------------------------------------------------------------------
    bool Init(HINSTANCE hinstance, HWND hwnd, BOOL is_window_mode);
    
    //--------------------------------------------------------------------------------
    //  �I������
    //--------------------------------------------------------------------------------
    void Uninit(void);

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    Mode*              current_mode_ = nullptr;        // ���̃��[�h
    RenderSystem*      render_system_ = nullptr;       // �����_�[�V�X�e��
    Resources*         resources_ = nullptr;           // ���\�[�X������
    SoundSystem*       sound_system_ = nullptr;        // �T�E���h�V�X�e��
    LightManager*      light_manager_ = nullptr;       // ���C�g�̊Ǘ���
    RendererManager*   renderer_manager_ = nullptr;    // �����_���[�Ǘ���
    ShaderManager*     shader_manager_ = nullptr;      // �V�F�[�_�Ǘ���
#ifdef _DEBUG
    DebugObserver*     debug_observer_ = nullptr;      // �f�o�b�O�ώ@��
#endif
    Input*             input_ = nullptr;               // ���͊Ǘ���
    FadeSystem*        fade_system_ = nullptr;         // �t�F�C�h�V�X�e��
    CameraManager*     camera_manager_ = nullptr;      // �J�����Ǘ���
    CollisionSystem*   collision_system_ = nullptr;    // �Փˏ����V�X�e��
    PhysicsSystem*     physics_system_ = nullptr;      // �����V�X�e��
    GameObjectManager* game_object_manager_ = nullptr; // �Q�[���I�u�W�F�N�g�Ǘ���
    ShadowMapSystem*   shadow_map_system_ = nullptr;   // �V���h�E�}�b�v�V�X�e��
    ActorObserver*     actor_observer_ = nullptr;      // �A�N�^�[�ώ@��
    static MainSystem* instance_;                      // �C���X�^���X
};
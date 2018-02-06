//--------------------------------------------------------------------------------
//�@modetitle.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode_title.h"
#include "main_system.h"
#include "input.h"
#include "mode_demo.h"
#include "camera.h"
#include "fade_system.h"
#include "light.h"
#include "resources.h"
#include "material_manager.h"
#include "game_object_spawner.h"
#include "game_object.h"
#include "flash_button_controller.h"
#include "game_time.h"
#include "sound_system.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void ModeTitle::Init(void)
{
    //�J�����̏�����
    auto camera = MY_NEW Camera;
    camera->Init();

    //���C�g�̏�����
    auto directional_light = MY_NEW DirectionalLight(Vector3(-1.0f, -4.0f, 1.0f).Normalized());

    //UI�̏�����
    GameObjectSpawner::CreateBasicPolygon2d(
        Vector3(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), 0.0f)
        , kDefaultLayer
        , L"title"
        , kDefault2dTextureShader
        , k2d);

    auto button = GameObjectSpawner::CreateFlashButton2d(
        1.0f
        , Vector3(475.0f, 73.0f, 0.0f)
        , kDefaultLayer
        , L"press_space"
        , kDefault2dTextureShader
        , k2d
        , 0.0f
        , Vector3(0.0f, SCREEN_HEIGHT * 0.25f, 0.0f));
    auto behavior = button->GetBehaviorBy(L"FlashButtonController");
    assert(behavior);
    flash_button_controller_ = static_cast<FlashButtonController*>(behavior);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void ModeTitle::Uninit(void)
{
    MainSystem::Instance().GetResources().GetMaterialManager().Disuse(L"title");
    MainSystem::Instance().GetSoundSystem().Stop(kTitleBgm);
    Mode::Uninit();
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void ModeTitle::Update(void)
{
    Mode::Update();
    
    auto& main_system = MainSystem::Instance();
    if (time_counter_ > 0.0f)
    {// ���U���g�ɂ����܂ŃJ�E���g����
        time_counter_ -= GameTime::Instance().ScaledDeltaTime();

        if (time_counter_ <= 0.0f)
        {
            main_system.GetFadeSystem().FadeTo(MY_NEW ModeDemo);
        }
        return;
    }

    if (main_system.GetInput().GetKeyTrigger(Key::kSubmit))
    {
        main_system.GetSoundSystem().Play(kSubmitSe);
        time_counter_ = kWaitTime;
        flash_button_controller_->SetFlashSpeed(15.0f);
    }
}

//--------------------------------------------------------------------------------
//  ���[�f�B���O�I���̒ʒm
//--------------------------------------------------------------------------------
void ModeTitle::OnCompleteLoading(void)
{
    MainSystem::Instance().GetSoundSystem().Play(kTitleBgm);
}
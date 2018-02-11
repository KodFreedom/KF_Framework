//--------------------------------------------------------------------------------
//�@modetitle.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode_title.h"
#include "main_system.h"
#include "input.h"
#include "mode_demo.h"
#include "mode_demo_play.h"
#include "camera.h"
#include "fade_system.h"
#include "light.h"
#include "resources.h"
#include "material_manager.h"
#include "game_object_spawner.h"
#include "game_object.h"
#include "button_controller.h"
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

    auto button = GameObjectSpawner::CreateButton2d(
        Vector3(400.0f, 70.0f, 0.0f) * 0.8f
        , Vector3(-200.0f, SCREEN_HEIGHT * 0.25f, 0.0f)
        , L"play_game.png"
        , kDefaultLayer
        , kDefault2dTextureShader
        , k2d
        , 0.0f);
    auto behavior = button->GetBehaviorBy(L"ButtonController");
    assert(behavior);
    button_controllers_[0] = static_cast<ButtonController*>(behavior);

    button = GameObjectSpawner::CreateButton2d(
        Vector3(400.0f, 70.0f, 0.0f) * 0.8f
        , Vector3(200.0f, SCREEN_HEIGHT * 0.25f, 0.0f)
        , L"demo_play.png"
        , kDefaultLayer
        , kDefault2dTextureShader
        , k2d
        , 0.0f);
    behavior = button->GetBehaviorBy(L"ButtonController");
    assert(behavior);
    button_controllers_[1] = static_cast<ButtonController*>(behavior);
    button_controllers_[1]->ChangeColor(Color::kGray, 0.1f);
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
            if (next_mode_ == kGame)
            {
                main_system.GetFadeSystem().FadeTo(MY_NEW ModeDemo);
            }
            else
            {
                main_system.GetFadeSystem().FadeTo(MY_NEW ModeDemoPlay);
            }
        }
        return;
    }

    // ���[�h�؂�ւ�
    auto& input = main_system.GetInput();
    if (input.GetKeyTrigger(Key::kLeft) || input.GetKeyTrigger(Key::kRight))
    {
        button_controllers_[next_mode_]->ChangeColor(Color::kGray);
        next_mode_ = static_cast<NextMode>(next_mode_ ^ 1);
        button_controllers_[next_mode_]->ChangeColor(Color::kWhite);
    }

    // ���[�h�m��
    if (input.GetKeyTrigger(Key::kSubmit))
    {
        main_system.GetSoundSystem().Play(kSubmitSe);
        time_counter_ = kWaitTime;
    }
}

//--------------------------------------------------------------------------------
//  ���[�f�B���O�I���̒ʒm
//--------------------------------------------------------------------------------
void ModeTitle::OnCompleteLoading(void)
{
    MainSystem::Instance().GetSoundSystem().Play(kTitleBgm);
}
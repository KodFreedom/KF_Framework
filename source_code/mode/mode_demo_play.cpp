//--------------------------------------------------------------------------------
//�@mode_demo_play.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode_demo_play.h"
#include "main_system.h"
#include "input.h"
#include "input_device_directX.h"
#include "sound_system.h"
#include "mode_title.h"
#include "fade_system.h"
#include "shadow_map_system.h"
#include "stage_spawner.h"
#include "actor_observer.h"
#include "player_controller.h"
#include "game_object.h"
#include "game_time.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
ModeDemoPlay::ModeDemoPlay() : Mode(L"DemoPlay")
    , time_counter_(0.0f)
{}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
ModeDemoPlay::~ModeDemoPlay()
{

}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void ModeDemoPlay::Init(void)
{    
    StageSpawner::LoadStage(L"demo");

    auto& main_system = MainSystem::Instance();
    auto player = main_system.GetActorObserver().GetPlayer();
    assert(player);
    main_system.GetShadowMapSystem().SetTarget(player->GetGameObject().GetTransform());
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void ModeDemoPlay::Uninit(void)
{
    MainSystem::Instance().GetSoundSystem().Stop(kGameBgm);
    MainSystem::Instance().GetInput().SetDemoPlayMode(false);
    Mode::Uninit();
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void ModeDemoPlay::Update(void)
{
    Mode::Update();
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void ModeDemoPlay::LateUpdate(void)
{
    Mode::LateUpdate();
    auto& main_system = MainSystem::Instance();

    if (time_counter_ > 0.0f)
    {// ���U���g�ɂ����܂ŃJ�E���g����
        time_counter_ -= GameTime::Instance().ScaledDeltaTime();

        if (time_counter_ <= 0.0f)
        {
            main_system.GetShadowMapSystem().SetTarget(nullptr);
            main_system.GetFadeSystem().FadeTo(MY_NEW ModeTitle);
        }
        return;
    }
   
    auto player = main_system.GetActorObserver().GetPlayer();
    if (player && player->GetCurrentStateName().find(L"Death") != String::npos)
    {// �v���C���[�����񂾂烊�U���g�ɂ���
        time_counter_ = kWaitTime;
        return;
    }

    if (main_system.GetActorObserver().GetEnemys().empty())
    {// �G�l�~�[�����񂾂烊�U���g�ɂ���
        time_counter_ = GameTime::kTimeInterval;
        return;
    }

    if (main_system.GetInput().GetJoystick()->GetButtonTrigger(XboxButton::kXboxMenu)
        || main_system.GetInput().GetKeyboard()->GetTrigger(DIK_RETURN))
    {
        main_system.GetSoundSystem().Play(kSubmitSe);
        time_counter_ = GameTime::kTimeInterval;
        return;
    }
}

//--------------------------------------------------------------------------------
//  ���[�f�B���O�I���̒ʒm
//--------------------------------------------------------------------------------
void ModeDemoPlay::OnCompleteLoading(void)
{
    MainSystem::Instance().GetSoundSystem().Play(kGameBgm);
    MainSystem::Instance().GetInput().SetDemoPlayMode(true);
}
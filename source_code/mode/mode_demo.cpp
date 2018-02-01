//--------------------------------------------------------------------------------
//�@modeDemo.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode_demo.h"
#include "main_system.h"
#include "input.h"
#include "sound_system.h"
#include "mode_result.h"
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
ModeDemo::ModeDemo() : Mode(L"Demo")
    , time_counter_(0.0f)
{}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
ModeDemo::~ModeDemo()
{

}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void ModeDemo::Init(void)
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
void ModeDemo::Uninit(void)
{
    MainSystem::Instance().GetSoundSystem().Stop(kGameBgm);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void ModeDemo::Update(void)
{
    Mode::Update();
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void ModeDemo::LateUpdate(void)
{
    Mode::LateUpdate();
    auto& main_system = MainSystem::Instance();

    if (time_counter_ > 0.0f)
    {// ���U���g�ɂ����܂ŃJ�E���g����
        time_counter_ -= GameTime::Instance().ScaledDeltaTime();

        if (time_counter_ <= 0.0f)
        {
            main_system.GetShadowMapSystem().SetTarget(nullptr);
            main_system.GetFadeSystem().FadeTo(MY_NEW ModeResult);
        }
        return;
    }
   
    auto player = main_system.GetActorObserver().GetPlayer();
    if (player && player->GetCurrentStateName().find(L"Dying") != String::npos)
    {// �v���C���[�����񂾂烊�U���g�ɂ���
        time_counter_ = kWaitTime;
        return;
    }

    if (main_system.GetActorObserver().GetEnemys().empty())
    {// �G�l�~�[�����񂾂烊�U���g�ɂ���
        time_counter_ = GameTime::kTimeInterval;
        return;
    }

#ifdef _DEBUG
    if (main_system.GetInput().GetKeyTrigger(Key::kStart))
    {
        main_system.GetSoundSystem().Play(kSubmitSe);
        time_counter_ = GameTime::kTimeInterval;
        return;
    }
#endif // _DEBUG
}

//--------------------------------------------------------------------------------
//  ���[�f�B���O�I���̒ʒm
//--------------------------------------------------------------------------------
void ModeDemo::OnCompleteLoading(void)
{
    MainSystem::Instance().GetSoundSystem().Play(kGameBgm);
}
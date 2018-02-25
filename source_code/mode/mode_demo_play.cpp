//--------------------------------------------------------------------------------
//　mode_demo_play.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode_demo_play.h"
#include "../main_system.h"
#include "../input/input.h"
#include "../input/input_device_directX.h"
#include "../sound/sound_system.h"
#include "mode_title.h"
#include "../fade_system.h"
#include "../render_system/shadow_map_system.h"
#include "../game_object/stage_spawner.h"
#include "../observer/actor_observer.h"
#include "../component/behavior/actor/player_controller.h"
#include "../game_object/game_object.h"
#include "../game_time.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
ModeDemoPlay::ModeDemoPlay() : Mode(L"DemoPlay")
    , time_counter_(0.0f)
{}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
ModeDemoPlay::~ModeDemoPlay()
{

}

//--------------------------------------------------------------------------------
//  初期化処理
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
//  終了処理
//--------------------------------------------------------------------------------
void ModeDemoPlay::Uninit(void)
{
    MainSystem::Instance().GetSoundSystem().Stop(kGameBgm);
    MainSystem::Instance().GetInput().SetDemoPlayMode(false);
    Mode::Uninit();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void ModeDemoPlay::Update(void)
{
    Mode::Update();
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void ModeDemoPlay::LateUpdate(void)
{
    Mode::LateUpdate();
    auto& main_system = MainSystem::Instance();

    if (time_counter_ > 0.0f)
    {// リザルトにいくまでカウントする
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
    {// プレイヤーが死んだらリザルトにいく
        time_counter_ = kWaitTime;
        return;
    }

    if (main_system.GetActorObserver().GetEnemys().empty())
    {// エネミーが死んだらリザルトにいく
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
//  ローディング終了の通知
//--------------------------------------------------------------------------------
void ModeDemoPlay::OnCompleteLoading(void)
{
    MainSystem::Instance().GetSoundSystem().Play(kGameBgm);
    MainSystem::Instance().GetInput().SetDemoPlayMode(true);
}
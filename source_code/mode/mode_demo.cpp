//--------------------------------------------------------------------------------
//　modeDemo.cpp
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
//  コンストラクタ
//--------------------------------------------------------------------------------
ModeDemo::ModeDemo() : Mode(L"Demo")
    , time_counter_(0.0f)
{}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
ModeDemo::~ModeDemo()
{

}

//--------------------------------------------------------------------------------
//  初期化処理
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
//  終了処理
//--------------------------------------------------------------------------------
void ModeDemo::Uninit(void)
{
    MainSystem::Instance().GetSoundSystem().Stop(kGameBgm);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void ModeDemo::Update(void)
{
    Mode::Update();
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void ModeDemo::LateUpdate(void)
{
    Mode::LateUpdate();
    auto& main_system = MainSystem::Instance();

    if (time_counter_ > 0.0f)
    {// リザルトにいくまでカウントする
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
    {// プレイヤーが死んだらリザルトにいく
        time_counter_ = kWaitTime;
        return;
    }

    if (main_system.GetActorObserver().GetEnemys().empty())
    {// エネミーが死んだらリザルトにいく
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
//  ローディング終了の通知
//--------------------------------------------------------------------------------
void ModeDemo::OnCompleteLoading(void)
{
    MainSystem::Instance().GetSoundSystem().Play(kGameBgm);
}
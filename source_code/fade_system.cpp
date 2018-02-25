//--------------------------------------------------------------------------------
//　fade_system.h
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "fade_system.h"
#include "main_system.h"
#include "mode/mode.h"
#include "game_object/game_object_spawner.h"
#include "game_object/game_object.h"
#include "resources/resources.h"
#include "resources/material_manager.h"
#include "game_time.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  生成処理
//--------------------------------------------------------------------------------
FadeSystem* FadeSystem::Create(void)
{
    auto instance = MY_NEW FadeSystem();
    instance->Init();
    return instance;
}

//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void FadeSystem::Release(void)
{
    Uninit();
    MY_DELETE this;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void FadeSystem::Update(void)
{
    switch (current_state_)
    {
    case FadeState::kFadeNone:
        break;
    case FadeState::kFadeIn:
        FadeIn();
        break;
    case FadeState::kFadeOut:
        FadeOut();
        break;
    case FadeState::kFadeWait:
        FadeWait();
        break;
    case FadeState::kFadeWaitIn:
        FadeWaitIn();
        break;
    case FadeState::kFadeWaitOut:
        FadeWaitOut();
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------------------------
//  次のモードにフェードする
//--------------------------------------------------------------------------------
void FadeSystem::FadeTo(Mode* next_mode, const float fade_time)
{
    if (current_state_ == FadeState::kFadeOut)
    {
        MY_DELETE next_mode;
        return;
    }
    current_state_ = FadeState::kFadeOut;
    time_counter_ = 0.0f;
    next_mode_ = next_mode;
    fade_time_ = fade_time;
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void FadeSystem::Init(void)
{
    GameObjectSpawner::CreateBasicPolygon2d(
        Vector3(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), 0.0f),
        kUnableAutoDelete,
        L"fade",
        kDefault2dShader,
        k2dMask);

    GameObjectSpawner::CreateScrollPolygon2d(
        Short2(2, 15),
        2,
        Vector3(412.0f, 64.0f, 0.0f),
        kUnableAutoDelete,
        L"loading",
        kDefault2dTextureShader,
        k2dMask,
        0.0f,
        Vector3(408.0f, 276.0f, 0.0f));
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void FadeSystem::Uninit(void)
{
}

//--------------------------------------------------------------------------------
//  fadeIn
//--------------------------------------------------------------------------------
void FadeSystem::FadeIn(void)
{
    time_counter_ -= GameTime::Instance().DeltaTime();
    UpdateAlpha(L"fade", time_counter_ / fade_time_);
    if (time_counter_ <= 0.0f)
    {
        time_counter_ = 0.0f;
        current_state_ = kFadeNone;
        GameTime::Instance().SetTimeScale(1.0f);
    }
}

//--------------------------------------------------------------------------------
//  fadeOut
//--------------------------------------------------------------------------------
void FadeSystem::FadeOut(void)
{
    if (time_counter_ >= fade_time_)
    {
        time_counter_ = fade_time_;
        current_state_ = kFadeWaitOut;
        GameTime::Instance().SetTimeScale(0.0f);
        MainSystem::Instance().Change(next_mode_);
    }
    time_counter_ += GameTime::Instance().DeltaTime();
    UpdateAlpha(L"fade", time_counter_ / fade_time_);
}

//--------------------------------------------------------------------------------
//  フェード待ち処理
//--------------------------------------------------------------------------------
void FadeSystem::FadeWait(void)
{
    if (MainSystem::Instance().GetResources().IsCompleteLoading())
    {
        current_state_ = kFadeWaitIn;
        MainSystem::Instance().GetCurrentMode().OnCompleteLoading();
    }
}

//--------------------------------------------------------------------------------
//  フェード待ち入る処理
//--------------------------------------------------------------------------------
void FadeSystem::FadeWaitIn(void)
{
    wait_time_counter_ -= GameTime::Instance().DeltaTime();
    UpdateAlpha(L"loading", wait_time_counter_ / wait_fade_time_);
    if (wait_time_counter_ <= 0.0f)
    {
        wait_time_counter_ = 0.0f;
        current_state_ = kFadeIn;
    }
}

//--------------------------------------------------------------------------------
//  フェード待ち出る処理
//--------------------------------------------------------------------------------
void FadeSystem::FadeWaitOut(void)
{
    wait_time_counter_ += GameTime::Instance().DeltaTime();
    UpdateAlpha(L"loading", wait_time_counter_ / wait_fade_time_);
    if (wait_time_counter_ >= wait_fade_time_)
    {
        wait_time_counter_ = wait_fade_time_;
        current_state_ = kFadeWait;
    } 
}

//--------------------------------------------------------------------------------
//  マテリアルのアルファ値更新処理
//--------------------------------------------------------------------------------
void FadeSystem::UpdateAlpha(const String& material_name, const float& alpha)
{
    auto material = MainSystem::Instance().GetResources().GetMaterialManager().Get(material_name);
    if (material)
    {
        material->diffuse_.a_ = alpha;
    }
}
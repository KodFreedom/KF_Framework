//--------------------------------------------------------------------------------
//�@fade_system.h
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "fade_system.h"
#include "main_system.h"
#include "mode.h"
#include "game_object_spawner.h"
#include "game_object.h"
#include "resources.h"
#include "material_manager.h"
#include "time.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
FadeSystem* FadeSystem::Create(void)
{
    auto instance = MY_NEW FadeSystem();
    instance->Init();
    return instance;
}

//--------------------------------------------------------------------------------
//  �j������
//--------------------------------------------------------------------------------
void FadeSystem::Release(void)
{
    Uninit();
    MY_DELETE this;
}

//--------------------------------------------------------------------------------
//  �X�V����
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
    default:
        break;
    }
}

//--------------------------------------------------------------------------------
//  ���̃��[�h�Ƀt�F�[�h����
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
//  ����������
//--------------------------------------------------------------------------------
void FadeSystem::Init(void)
{
    auto& material_manager = MainSystem::Instance().GetResources().GetMaterialManager();
    material_manager.Use(L"fade", Color::kBlack);
    material_ = material_manager.Get(L"fade");
    GameObjectSpawner::CreateBasicPolygon2d(Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), kUnableAutoDelete, L"fade", kDefault2dShader, k2dMask);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void FadeSystem::Uninit(void)
{
    material_ = nullptr;
    MainSystem::Instance().GetResources().GetMaterialManager().Disuse(L"fade");
}

//--------------------------------------------------------------------------------
//  fadeIn
//--------------------------------------------------------------------------------
void FadeSystem::FadeIn(void)
{
    time_counter_ -= Time::Instance()->DeltaTime();
    if (time_counter_ <= 0.0f)
    {
        time_counter_ = 0.0f;
        current_state_ = kFadeNone;
        Time::Instance()->SetTimeScale(1.0f);
    }
    material_->diffuse_.a_ = time_counter_ / fade_time_;
}

//--------------------------------------------------------------------------------
//  fadeOut
//--------------------------------------------------------------------------------
void FadeSystem::FadeOut(void)
{
    time_counter_ += Time::Instance()->DeltaTime();
    if (time_counter_ >= fade_time_)
    {
        time_counter_ = fade_time_;
        current_state_ = kFadeIn;
        Time::Instance()->SetTimeScale(0.0f);
        MainSystem::Instance().Change(next_mode_);
    }
    material_->diffuse_.a_ = time_counter_ / fade_time_;
}
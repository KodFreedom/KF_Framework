//--------------------------------------------------------------------------------
//�@fade_system.h
//  manage the materials' save, load
//	�}�e���A���Ǘ���
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "main.h"
#include "fade_system.h"
#include "main_system.h"
#include "mode.h"
#include "game_object_spawner.h"
#include "game_object.h"
#include "material_manager.h"

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
		delete next_mode;
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
    Material* material = MY_NEW Material;
    material->diffuse_ = Color::kBlack;
    MainSystem::Instance()->GetMaterialManager()->Use(L"fade", material);
    material_ = MainSystem::Instance()->GetMaterialManager()->GetMaterial(L"fade");
    GameObjectSpawner::CreateFade();
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void FadeSystem::Uninit(void)
{
    material_ = nullptr;
    MainSystem::Instance()->GetMaterialManager()->Disuse(L"fade");
}

//--------------------------------------------------------------------------------
//  fadeIn
//--------------------------------------------------------------------------------
void FadeSystem::FadeIn(void)
{
	time_counter_ -= DELTA_TIME;
	if (time_counter_ <= 0.0f)
	{
		time_counter_ = 0.0f;
		current_state_ = kFadeNone;
	}
    material_->diffuse_.a_ = time_counter_ / fade_time_;
}

//--------------------------------------------------------------------------------
//  fadeOut
//--------------------------------------------------------------------------------
void FadeSystem::FadeOut(void)
{
	time_counter_ += DELTA_TIME;
	if (time_counter_ >= fade_time_)
	{
		time_counter_ = fade_time_;

		current_state_ = kFadeIn;
		MainSystem::Instance()->Change(next_mode_);
	}
    material_->diffuse_.a_ = time_counter_ / fade_time_;
}
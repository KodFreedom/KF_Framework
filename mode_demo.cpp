//--------------------------------------------------------------------------------
//
//�@modeDemo.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode_demo.h"
#include "main_system.h"
#include "input.h"
#include "sound_manager.h"
#include "mode_result.h"
#include "fade_system.h"
#include "shadow_map_system.h"
#include "stage_spawner.h"
#include "actor_observer.h"
#include "player_controller.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
ModeDemo::ModeDemo() : Mode(L"Demo")
{

}

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

    auto main_system = MainSystem::Instance();
    auto player = main_system->GetActorObserver()->GetPlayer();
    assert(player);
    main_system->GetShadowMapSystem()->SetTarget(player->GetGameObject().GetTransform());
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

    auto main_system = MainSystem::Instance();
    auto player = main_system->GetActorObserver()->GetPlayer();
    if (player)
    {// �v���C���[�����񂾂烊�U���g�ɂ���
        if (player->GetCurrentStateName().find(L"Dying") != String::npos)
        {
            main_system->GetShadowMapSystem()->SetTarget(nullptr);
            main_system->GetFadeSystem()->FadeTo(MY_NEW ModeResult);
            return;
        }
    }

    if (main_system->GetActorObserver()->GetEnemys().empty())
    {// �G�l�~�[�����񂾂烊�U���g�ɂ���
        main_system->GetShadowMapSystem()->SetTarget(nullptr);
        main_system->GetFadeSystem()->FadeTo(MY_NEW ModeResult);
        return;
    }

#ifdef _DEBUG
    if (main_system->GetInput()->GetKeyTrigger(Key::kStart))
    {
        main_system->GetShadowMapSystem()->SetTarget(nullptr);
        main_system->GetFadeSystem()->FadeTo(MY_NEW ModeResult);
        return;
    }
#endif // _DEBUG
}
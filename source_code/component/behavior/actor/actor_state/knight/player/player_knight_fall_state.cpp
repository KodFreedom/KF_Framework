//--------------------------------------------------------------------------------
//  knight������X�e�[�g
//  player_knight_fall_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_fall_state.h"
#include "player_knight_idle_state.h"
#include "player_knight_hard_land_state.h"
#include "../../../../../../game_time.h"
#include "../../../player_controller.h"
#include "../../../../../animator/animator.h"
#include "../../../../../collider/collider.h"
#include "../../../../../../game_object/game_object.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerKnightFallState::Init(PlayerController& player)
{
    player.GetParameter().SetGroundCheckDistance(kGroundCheckDistance);
    player.GetAnimator().SetGrounded(false);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerKnightFallState::Uninit(PlayerController& player)
{
    player.GetAnimator().SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerKnightFallState::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();
    time_counter_ += GameTime::Instance().ScaledDeltaTime();
    if (player.GetCurrentGroundInfo().is_grounded)
    {
        if (time_counter_ > kHardLandTime)
        {
            player.Change(MY_NEW PlayerKnightHardLandState);
            return;
        }

        player.Change(MY_NEW PlayerKnightIdleState);
        return;
    }
}
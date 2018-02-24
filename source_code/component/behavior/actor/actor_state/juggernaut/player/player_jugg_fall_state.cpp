//--------------------------------------------------------------------------------
//  jugg������X�e�[�g
//�@player_jugg_fallen_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_jugg_fall_state.h"
#include "player_jugg_land_state.h"
#include "../../../player_controller.h"
#include "../../../../../animator/animator.h"
#include "../../../../../collider/collider.h"
#include "../../../../../../game_object/game_object.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerJuggFallState::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetGroundCheckDistance(kGroundCheckDistance);
    parameter.SetMovementMultiplier(kMovementMultiplier);
    auto& animator = player.GetAnimator();
    animator.SetGrounded(false);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerJuggFallState::Uninit(PlayerController& player)
{
    auto& animator = player.GetAnimator();
    animator.SetGrounded(true);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerJuggFallState::Update(PlayerController& player)
{
    PlayerState::Update(player);

    player.CheckGrounded();
    player.Move();

    if (player.GetAnimator().GetCurrentAnimationStateType() == kNormalMotionState)
    {
        if (player.GetCurrentGroundInfo().is_grounded)
        {
            player.Change(MY_NEW PlayerJuggLandState);
            return;
        }
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerJuggFallState::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�Փ˂̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerJuggFallState::OnCollision(PlayerController& player, CollisionInfo& info)
{

}
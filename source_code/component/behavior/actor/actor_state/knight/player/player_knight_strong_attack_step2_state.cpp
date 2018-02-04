//--------------------------------------------------------------------------------
//  knight�d�U���X�e�b�v2�X�e�[�g
//�@player_knight_strong_attack_step2_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_strong_attack_step2_state.h"
#include "player_knight_strong_attack_step3_state.h"
#include "player_knight_idle_state.h"
#include "player_controller.h"
#include "enemy_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerKnightStrongAttackStep2State::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetStrongAttack(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerKnightStrongAttackStep2State::Uninit(PlayerController& player)
{
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerKnightStrongAttackStep2State::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    auto& animator = player.GetAnimator();
    if (animator.GetCurrentAnimationStateType() == kBlendMotionState)
    {
        animator.SetStrongAttack(false);
        return;
    }

    int current_frame = animator.GetCurrentFrame();
    if (current_frame == kBeginAttackFrame)
    {
        // �U���pCollider��Awake����
        auto right_hand = player.GetGameObject().GetTransform()->FindChildBy(L"RightHand");
        if (right_hand)
        {
            auto& colliders = right_hand->GetGameObject().GetColliders();
            for (auto& collider : colliders)
            {
                collider->SetTag(L"Weapon");
                collider->SetMode(ColliderMode::kDynamic);
                collider->Awake();
            }
        }
    }
    else if (current_frame == kEndAttackFrame)
    {
        // �U���pCollider��Sleep����
        auto right_hand = player.GetGameObject().GetTransform()->FindChildBy(L"RightHand");
        if (right_hand)
        {
            auto& colliders = right_hand->GetGameObject().GetColliders();
            for (auto& collider : colliders)
            {
                collider->Sleep();
            }
        }
    }

    if (player.IsStrongAttack())
    {
        animator.SetStrongAttack(true);
    }

    if (current_frame >= kBeginStep3Frame && animator.GetIsStrongAttack() == true)
    {
        player.Change(MY_NEW PlayerKnightStrongAttackStep3State);
        return;
    }
}

//--------------------------------------------------------------------------------
//  ���[�V�����I���̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerKnightStrongAttackStep2State::OnAnimationOver(PlayerController& player)
{
    auto& animator = player.GetAnimator();
    if (animator.GetCurrentFrame() > kBeginStep3Frame
        && animator.GetCurrentAnimationName()._Equal(L"knight_strong_attack_step2"))
    {
        player.Change(MY_NEW PlayerKnightIdleState);
        return;
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerKnightStrongAttackStep2State::OnTrigger(PlayerController& player, Collider& self, Collider& other)
{
    if (self.GetTag()._Equal(L"Weapon"))
    {
        if (other.GetTag()._Equal(L"Body")
            && other.GetGameObject().GetTag()._Equal(L"Enemy"))
        {// �^�[�Q�b�g�ɓ�������, enemy controller���擾
            auto enemy_controller = other.GetGameObject().GetBehaviorBy(L"EnemyController");
            if (enemy_controller)
            {
                static_cast<EnemyController*>(enemy_controller)->Hit(player.GetParameter().GetAttack());
            }
        }
    }
}

//--------------------------------------------------------------------------------
//  �_���[�W�󂯂�����
//--------------------------------------------------------------------------------
void PlayerKnightStrongAttackStep2State::OnDamaged(PlayerController& player, const float& damage)
{
    player.ReceiveDamage(damage);

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        //player.Change(MY_NEW PlayerMutantDyingState);
        return;
    }

    //player.Change(MY_NEW PlayerMutantDamagedState);
}
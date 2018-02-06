//--------------------------------------------------------------------------------
//  knight�d�U���X�e�b�v1�X�e�[�g
//�@player_knight_strong_attack_step1_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_strong_attack_step1_state.h"
#include "player_knight_strong_attack_step2_state.h"
#include "player_knight_idle_state.h"
#include "player_knight_impact_state.h"
#include "player_knight_death_state.h"
#include "player_controller.h"
#include "enemy_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"
#include "main_system.h"
#include "sound_system.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerKnightStrongAttackStep1State::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetStrongAttack(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerKnightStrongAttackStep1State::Uninit(PlayerController& player)
{
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerKnightStrongAttackStep1State::Update(PlayerController& player)
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

        // Se
        auto& sound_system = MainSystem::Instance().GetSoundSystem();
        sound_system.Play(kAttackVoice1Se);
        sound_system.Play(kSordAttackSe);
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
    
    if (current_frame >= kBeginStep2Frame && animator.GetIsStrongAttack() == true)
    {
        player.Change(MY_NEW PlayerKnightStrongAttackStep2State);
        return;
    }
}

//--------------------------------------------------------------------------------
//  ���[�V�����I���̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerKnightStrongAttackStep1State::OnAnimationOver(PlayerController& player)
{
    auto& animator = player.GetAnimator();
    if (animator.GetCurrentFrame() > kBeginStep2Frame
        && animator.GetCurrentAnimationName()._Equal(L"knight_strong_attack_step1"))
    {
        player.Change(MY_NEW PlayerKnightIdleState);
        player.GetAnimator().SetStrongAttack(false);
        return;
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerKnightStrongAttackStep1State::OnTrigger(PlayerController& player, Collider& self, Collider& other)
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
void PlayerKnightStrongAttackStep1State::OnDamaged(PlayerController& player, const float& damage)
{
    player.ReceiveDamage(damage);

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        player.Change(MY_NEW PlayerKnightDeathState);
        return;
    }

    player.Change(MY_NEW PlayerKnightImpactState);
}
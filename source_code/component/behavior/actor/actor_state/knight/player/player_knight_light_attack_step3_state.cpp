//--------------------------------------------------------------------------------
//  knight�y�U���X�e�b�v3�X�e�[�g
//�@player_knight_light_attack_step3_state.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_knight_light_attack_step3_state.h"
#include "player_knight_idle_state.h"
#include "player_knight_impact_state.h"
#include "player_knight_death_state.h"
#include "player_controller.h"
#include "enemy_controller.h"
#include "animator.h"
#include "collider.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//  �������֐�
//--------------------------------------------------------------------------------
void PlayerKnightLightAttackStep3State::Init(PlayerController& player)
{
    auto& parameter = player.GetParameter();
    parameter.SetMovementMultiplier(kMovementMultiplier);
    player.GetAnimator().SetLightAttack(true);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void PlayerKnightLightAttackStep3State::Uninit(PlayerController& player)
{
    player.GetAnimator().SetLightAttack(false);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void PlayerKnightLightAttackStep3State::Update(PlayerController& player)
{
    PlayerState::Update(player);
    player.CheckGrounded();
    player.Move();

    int current_frame = player.GetAnimator().GetCurrentFrame();
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
}

//--------------------------------------------------------------------------------
//  ���[�V�����I���̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerKnightLightAttackStep3State::OnAnimationOver(PlayerController& player)
{
    if (player.GetAnimator().GetCurrentAnimationName()._Equal(L"knight_light_attack_step3"))
    {
        player.Change(MY_NEW PlayerKnightIdleState);
        return;
    }
}

//--------------------------------------------------------------------------------
//  �R���C�_�[�g���K�[�̎��Ă΂��
//--------------------------------------------------------------------------------
void PlayerKnightLightAttackStep3State::OnTrigger(PlayerController& player, Collider& self, Collider& other)
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
void PlayerKnightLightAttackStep3State::OnDamaged(PlayerController& player, const float& damage)
{
    player.ReceiveDamage(damage);

    if (player.GetParameter().GetCurrentLife() <= 0.0f)
    {
        player.Change(MY_NEW PlayerKnightDeathState);
        return;
    }

    player.Change(MY_NEW PlayerKnightImpactState);
}
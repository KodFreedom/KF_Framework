//--------------------------------------------------------------------------------
//  jugg�U���X�e�[�g
//�@player_jugg_attack_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "player_state.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class PlayerJuggAttackState : public PlayerState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    PlayerJuggAttackState() : PlayerState(L"PlayerJuggAttackState") {}
    ~PlayerJuggAttackState() {}

    //--------------------------------------------------------------------------------
    //  ����������
    //--------------------------------------------------------------------------------
    void Init(ActorController& actor) override;

    //--------------------------------------------------------------------------------
    //  �I������
    //--------------------------------------------------------------------------------
    void Uninit(ActorController& actor) override;

    //--------------------------------------------------------------------------------
    //  �X�V����
    //--------------------------------------------------------------------------------
    void Update(ActorController& actor) override;

    //--------------------------------------------------------------------------------
    //  ��X�V����
    //--------------------------------------------------------------------------------
    void LateUpdate(ActorController& actor) override {}

    //--------------------------------------------------------------------------------
    //  �R���C�_�[�g���K�[�̎��Ă΂��
    //--------------------------------------------------------------------------------
    void OnTrigger(ActorController& actor, Collider& self, Collider& other);

    //--------------------------------------------------------------------------------
    //  �R���C�_�[�Փ˂̎��Ă΂��
    //--------------------------------------------------------------------------------
    void OnCollision(ActorController& actor, CollisionInfo& info);

private:
    //--------------------------------------------------------------------------------
    //  �萔��`
    //--------------------------------------------------------------------------------
    static constexpr float kMovementMultiplier = 0.0f;
};
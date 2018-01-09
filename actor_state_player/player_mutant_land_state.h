//--------------------------------------------------------------------------------
//  mutant�����f�B���O�X�e�[�g
//�@player_mutant_land_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "player_state.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class PlayerMutantLandState : public PlayerState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    PlayerMutantLandState() : PlayerState(L"PlayerMutantLandState") {}
    ~PlayerMutantLandState() {}

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
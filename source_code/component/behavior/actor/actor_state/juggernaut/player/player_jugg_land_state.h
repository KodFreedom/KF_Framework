//--------------------------------------------------------------------------------
//  jugg�����f�B���O�X�e�[�g
//�@player_jugg_land_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "player_state.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class PlayerJuggLandState : public PlayerState
{
public:
    //--------------------------------------------------------------------------------
    //  constructors for singleton
    //--------------------------------------------------------------------------------
    PlayerJuggLandState() : PlayerState(L"PlayerJuggLandState") {}
    ~PlayerJuggLandState() {}

    //--------------------------------------------------------------------------------
    //  ����������
    //--------------------------------------------------------------------------------
    void Init(PlayerController& actor) override;

    //--------------------------------------------------------------------------------
    //  �I������
    //--------------------------------------------------------------------------------
    void Uninit(PlayerController& actor) override;

    //--------------------------------------------------------------------------------
    //  �X�V����
    //--------------------------------------------------------------------------------
    void Update(PlayerController& actor) override;

    //--------------------------------------------------------------------------------
    //  ��X�V����
    //--------------------------------------------------------------------------------
    void LateUpdate(PlayerController& actor) override {}

    //--------------------------------------------------------------------------------
    //  �R���C�_�[�g���K�[�̎��Ă΂��
    //--------------------------------------------------------------------------------
    void OnTrigger(PlayerController& actor, Collider& self, Collider& other) {}

    //--------------------------------------------------------------------------------
    //  �R���C�_�[�Փ˂̎��Ă΂��
    //--------------------------------------------------------------------------------
    void OnCollision(PlayerController& actor, CollisionInfo& info) {}

private:
    //--------------------------------------------------------------------------------
    //  �萔��`
    //--------------------------------------------------------------------------------
    static constexpr float kMovementMultiplier = 0.0f;
};
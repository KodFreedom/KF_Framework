//--------------------------------------------------------------------------------
//	�_���[�W�󂯂��X�e�[�g
//�@DamagedState.h
//	Author : Xu Wenjie
//	Date   : 2017-11-07
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "actorState.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class DamagedState : public ActorState
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	DamagedState();
	~DamagedState() {}

	void Init(ActorController& actor) override;
	void Act(ActorController& actor) override;
	void OnDamaged(ActorController& actor, const float& damage) override {}
};
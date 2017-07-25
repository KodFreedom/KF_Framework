//--------------------------------------------------------------------------------
//	�U���X�e�[�^�X
//�@attackStatus.h
//	Author : Xu Wenjie
//	Date   : 2017-07-19
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "status.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CAttackStatus : public CStatus
{
public:
	CAttackStatus() : CStatus(), m_bFirst(true) {}
	~CAttackStatus() override {}
	void Update(CActorBehaviorComponent& actor) override;
	void LateUpdate(CActorBehaviorComponent& actor) override;

private:
	bool m_bFirst;
};
//--------------------------------------------------------------------------------
//	�G�l�~�[�U�����[�h
//�@enemyAttackMode.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "AIMode.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class CEnemyAttackMode : public CAIMode
{
public:
	CEnemyAttackMode() : CAIMode() {}
	~CEnemyAttackMode() override {}
	void Update(CEnemyBehaviorComponent& enemy) override;
};
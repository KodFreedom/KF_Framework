//--------------------------------------------------------------------------------
//	�G�l�~�[�m�[�}�����[�h
//�@enemyNormalMode.h
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
class CEnemyNormalMode : public CAIMode
{
public:
	CEnemyNormalMode() : CAIMode() {}
	~CEnemyNormalMode() override {}
	void Update(CEnemyBehaviorComponent& enemy) override;
};
//--------------------------------------------------------------------------------
//�@stage_spawner.h
//	�X�e�[�W��������
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class StageSpawner
{
public:
	static void LoadStage(const String& stage_name);

private:
    static void LoadEnvironment(const String& stage_name);
    static void LoadPlayer(const String& stage_name);
    static void LoadEnemy(const String& stage_name);
};
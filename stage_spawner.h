//--------------------------------------------------------------------------------
//　stage_spawner.h
//	ステージ生成処理
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"

//--------------------------------------------------------------------------------
//  クラス宣言
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
//--------------------------------------------------------------------------------
//
//　motionManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-11-06
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "motionManager.h"
#include "motionInfo.h"

//--------------------------------------------------------------------------------
//  静的メンバー変数宣言
//--------------------------------------------------------------------------------
MotionManager* MotionManager::instance = nullptr;

//--------------------------------------------------------------------------------
//	
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  モーションをの読み込み
//--------------------------------------------------------------------------------
void MotionManager::Use(const string& motionName)
{
	if (motionName.empty()) return;

	//すでに読み込んだら処理終了
	auto iterator = motions.find(motionName);
	if (iterator != motions.end())
	{
		++iterator->second.UserNumber;
		return;
	}

	//モーションの読み込み
	//Todo:ファイルからモーションの読み込み
	MotionStruct info;
	info.UserNumber = 1;
	motions.emplace(motionName, info);
}

//--------------------------------------------------------------------------------
//  モーションをの破棄
//--------------------------------------------------------------------------------
void MotionManager::Disuse(const string& motionName)
{
	if (motionName.empty()) return;
	auto iterator = motions.find(motionName);
	if (iterator == motions.end()) return;
	--iterator->second.UserNumber;
	if (iterator->second.UserNumber == 0)
	{// 誰も使ってないので破棄する
		SAFE_DELETE(iterator->second.Pointer);
		motions.erase(iterator);
	}
}
//--------------------------------------------------------------------------------
//	
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  全てのモーションを破棄する
//--------------------------------------------------------------------------------
void MotionManager::uninit(void)
{
	for (auto iterator = motions.begin(); iterator != motions.end();)
	{
		SAFE_DELETE(iterator->second.Pointer);
		iterator = motions.erase(iterator);
	}
}

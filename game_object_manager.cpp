//--------------------------------------------------------------------------------
//　game_object_manager.h
//	ゲームオブジェクト管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "game_object_manager.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  リリース処理
//--------------------------------------------------------------------------------
void GameObjectManager::Clear(void)
{
	for (auto& game_objects : game_objects_array_)
	{
		for (auto iterator = game_objects.begin(); iterator != game_objects.end();)
		{
			(*iterator)->Release();
			iterator = game_objects.erase(iterator);
		}
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void GameObjectManager::Update(void)
{
	// 生きてないオブジェクトを削除する
	for (auto& game_objects : game_objects_array_)
	{
		for (auto iterator = game_objects.begin(); iterator != game_objects.end();)
		{
			if (!(*iterator)->IsAlive())
			{
				(*iterator)->Release();
				iterator = game_objects.erase(iterator);
			}
			else { ++iterator; }
		}
	}

	for (auto& game_objects : game_objects_array_)
	{
		for (auto game_object : game_objects)
		{
			game_object->Update();
		}
	}
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void GameObjectManager::LateUpdate(void)
{
	for (auto& game_objects : game_objects_array_)
	{
		for (auto game_object : game_objects)
		{
			game_object->LateUpdate();
		}
	}
}
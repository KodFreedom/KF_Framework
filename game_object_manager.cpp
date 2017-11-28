//--------------------------------------------------------------------------------
//�@game_object_manager.h
//	�Q�[���I�u�W�F�N�g�Ǘ���
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "game_object_manager.h"
#include "game_object.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �����[�X����
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
//  �X�V����
//--------------------------------------------------------------------------------
void GameObjectManager::Update(void)
{
	// �����ĂȂ��I�u�W�F�N�g���폜����
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
//  �X�V����(�`�撼�O)
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
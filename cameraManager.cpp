//--------------------------------------------------------------------------------
//  カメラマネージャ
//　cameraManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-11-05
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "cameraManager.h"
#include "camera.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
CameraManager* CameraManager::instance = nullptr;

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CameraManager::Update(void)
{
	for (auto camera : cameras) camera->Update();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CameraManager::LateUpdate(void)
{
	for (auto camera : cameras) camera->LateUpdate();
}

//--------------------------------------------------------------------------------
//  リリース処理
//--------------------------------------------------------------------------------
void CameraManager::ReleaseAll(void)
{
	for (auto iterator = cameras.begin(); iterator != cameras.end();)
	{
		(*iterator)->Release();
		iterator = cameras.erase(iterator);
	}
}
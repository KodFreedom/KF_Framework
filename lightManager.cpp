//--------------------------------------------------------------------------------
//
//　LightManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-11-02
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "lightManager.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
LightManager* LightManager::instance = nullptr;

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ライトの生成処理
//--------------------------------------------------------------------------------
int	LightManager::CreateLight(const LightType& type,
	const Vector3& position = Vector3::Zero, const Vector3& direction = Vector3::Forward,
	const Color& diffuse = Color::White, const Color& ambient = Color::Gray)
{
	int id = lights.size();
	auto light = new Light(type, position, direction, diffuse, ambient);
	lights.emplace(id, light);
	return id;
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void LightManager::init(void)
{
	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void LightManager::uninit(void)
{
	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();

	ReleaseAll();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

//--------------------------------------------------------------------------------
//  全てのライトを削除
//--------------------------------------------------------------------------------
void LightManager::ReleaseAll(void)
{
	for (auto iterator = lights.begin(); iterator != lights.end();)
	{
		delete iterator->second;
		iterator = lights.erase(iterator);
	}
}
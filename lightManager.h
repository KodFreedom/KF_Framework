//--------------------------------------------------------------------------------
//
//　LightManager.h
//	Author : Xu Wenjie
//	Date   : 2017-11-02
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "light.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class LightManager
{
public:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	static auto	Create(void)
	{
		if (instance) return instance;
		instance = new LightManager;
		instance->init();
		return instance;
	}
	static void Release(void) { SAFE_UNINIT(instance); }
	static auto Instance(void) { return instance; }

	int		CreateLight(const LightType& type,
		const Vector3& position = Vector3::Zero, const Vector3& direction = Vector3::Forward,
		const Color& diffuse = Color::White, const Color& ambient = Color::Gray);
	void	ReleaseLight(int lightID)
	{
		auto iterator = lights.find(lightID);
		if (iterator == lights.end()) return;
		delete iterator->second;
		lights.erase(iterator);
	}
	void	ReleaseAll(void);

private:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	LightManager() { lights.clear(); }
	~LightManager() {}
	void init(void);
	void uninit(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	unordered_map<int, Light*>	lights;
	static LightManager*		instance;
};
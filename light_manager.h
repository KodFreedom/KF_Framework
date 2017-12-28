//--------------------------------------------------------------------------------
//　light_manager.h
//  manage the lights' save, load
//	ライト管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "light.h"

//--------------------------------------------------------------------------------
//  ライト管理者クラス
//--------------------------------------------------------------------------------
class LightManager
{
public:
	//--------------------------------------------------------------------------------
	//  生成処理
	//  return : MaterialManager*
	//--------------------------------------------------------------------------------
	static LightManager* Create(void)
	{
		auto instance = MY_NEW LightManager();
		return instance;
	}

	//--------------------------------------------------------------------------------
	//  破棄処理
	//--------------------------------------------------------------------------------
	void Release(void) 
    {
        Clear();
        MY_DELETE this;
    }

	//--------------------------------------------------------------------------------
	//  ライトの削除
	//--------------------------------------------------------------------------------
	void Clear(void)
	{
        for (auto iterator = directional_lights_.begin(); iterator != directional_lights_.end();)
        {
            MY_DELETE (*iterator);
            iterator = directional_lights_.erase(iterator);
        }
	}

    //--------------------------------------------------------------------------------
    //  登録関数
    //--------------------------------------------------------------------------------
    void Register(DirectionalLight* directional_light) { directional_lights_.push_back(directional_light); }

    //--------------------------------------------------------------------------------
    //  Get関数
    //--------------------------------------------------------------------------------
    const auto& GetDirectionalLights(void) const { return directional_lights_; }

private:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	LightManager() {}
	LightManager(const LightManager& value) {}
	LightManager& operator=(const LightManager& value) {}
	~LightManager() {}

    list<DirectionalLight*> directional_lights_;
};
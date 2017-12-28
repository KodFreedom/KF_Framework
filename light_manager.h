//--------------------------------------------------------------------------------
//�@light_manager.h
//  manage the lights' save, load
//	���C�g�Ǘ���
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "light.h"

//--------------------------------------------------------------------------------
//  ���C�g�Ǘ��҃N���X
//--------------------------------------------------------------------------------
class LightManager
{
public:
	//--------------------------------------------------------------------------------
	//  ��������
	//  return : MaterialManager*
	//--------------------------------------------------------------------------------
	static LightManager* Create(void)
	{
		auto instance = MY_NEW LightManager();
		return instance;
	}

	//--------------------------------------------------------------------------------
	//  �j������
	//--------------------------------------------------------------------------------
	void Release(void) 
    {
        Clear();
        MY_DELETE this;
    }

	//--------------------------------------------------------------------------------
	//  ���C�g�̍폜
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
    //  �o�^�֐�
    //--------------------------------------------------------------------------------
    void Register(DirectionalLight* directional_light) { directional_lights_.push_back(directional_light); }

    //--------------------------------------------------------------------------------
    //  Get�֐�
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
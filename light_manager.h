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
	void Release(void) { Clear(); }

	//--------------------------------------------------------------------------------
	//  �S���C�g�̃Z�b�g����
	//--------------------------------------------------------------------------------
	void SetLight(void);

	//--------------------------------------------------------------------------------
	//  �f�B���N�V�������C�g�̍쐬
	//  direction : ���C�g�̕���
	//  diffuse : // ���ˌ��̐F
	//  ambient : // �����̐F
	//  specular : // ���ʌ��̐F
	//--------------------------------------------------------------------------------
	void CreateDirectionLight(const Vector3& direction, const Color& diffuse = Color::kWhite, const Color& ambient = Color::kGray, const Color& specular = Color::kWhite)
	{
		directional_lights_.push_back(MY_NEW DirectionalLight(direction, diffuse, ambient, specular));
	}

	//--------------------------------------------------------------------------------
	//  �f�B���N�V�������C�g�̎擾
	//  return : const list<DirectionalLight*>&
	//--------------------------------------------------------------------------------
	const auto& GetDirectionLights(void) const { return directional_lights_; }

	//--------------------------------------------------------------------------------
	//  ���C�g�̍폜
	//--------------------------------------------------------------------------------
	void Clear(void)
	{
		for (auto iterator = directional_lights_.begin(); iterator != directional_lights_.end();)
		{
			SAFE_DELETE(*iterator);
			iterator = directional_lights_.erase(iterator);
		}
	}

private:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	LightManager() {}
	LightManager(const LightManager& value) {}
	LightManager& operator=(const LightManager& value) {}
	~LightManager() {}

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	list<DirectionalLight*> directional_lights_;
	//list<SpotLight*> spot_lights_;
	//list<PointLight*> point_lights_;
};
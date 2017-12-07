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
	void Release(void) { Clear(); }

	//--------------------------------------------------------------------------------
	//  全ライトのセット処理
	//--------------------------------------------------------------------------------
	void SetLight(void);

	//--------------------------------------------------------------------------------
	//  ディレクションライトの作成
	//  direction : ライトの方向
	//  diffuse : // 漫射光の色
	//  ambient : // 環境光の色
	//  specular : // 鏡面光の色
	//--------------------------------------------------------------------------------
	void CreateDirectionLight(const Vector3& direction, const Color& diffuse = Color::kWhite, const Color& ambient = Color::kGray, const Color& specular = Color::kWhite)
	{
		directional_lights_.push_back(MY_NEW DirectionalLight(direction, diffuse, ambient, specular));
	}

	//--------------------------------------------------------------------------------
	//  ディレクションライトの取得
	//  return : const list<DirectionalLight*>&
	//--------------------------------------------------------------------------------
	const auto& GetDirectionLights(void) const { return directional_lights_; }

	//--------------------------------------------------------------------------------
	//  ライトの削除
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
	//  変数定義
	//--------------------------------------------------------------------------------
	list<DirectionalLight*> directional_lights_;
	//list<SpotLight*> spot_lights_;
	//list<PointLight*> point_lights_;
};
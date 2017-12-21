//--------------------------------------------------------------------------------
//　light.cpp
//	ライトクラス
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "light.h"
#include "main_system.h"
#include "light_manager.h"

//--------------------------------------------------------------------------------
//  ディレクショナルライト
//--------------------------------------------------------------------------------
DirectionalLight::DirectionalLight(const Vector3& direction)
    : Light(kDirectionalLight, Color::kWhite, Color::kGray, Color::kWhite, direction)
{
    MainSystem::Instance()->GetLightManager()->Register(this);
}
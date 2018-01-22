//--------------------------------------------------------------------------------
//�@light.cpp
//  ���C�g�N���X
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "light.h"
#include "main_system.h"
#include "light_manager.h"

//--------------------------------------------------------------------------------
//  �f�B���N�V���i�����C�g
//--------------------------------------------------------------------------------
DirectionalLight::DirectionalLight(const Vector3& direction)
    : Light(kDirectionalLight, Color(0.8f, 0.8f, 0.8f, 1.0f), Color::kGray, Color::kWhite, direction)
{
    MainSystem::Instance()->GetLightManager()->Register(this);
}
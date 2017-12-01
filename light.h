//--------------------------------------------------------------------------------
//�@light.h
//	���C�g�N���X
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  �񋓌^��`
//--------------------------------------------------------------------------------
enum LightType
{
	kPointLight = 1,
	kSpotLight = 2,
	kDirectionalLight = 3,
};

//--------------------------------------------------------------------------------
//  ���C�g���N���X
//--------------------------------------------------------------------------------
class Light
{
public:
	const LightType type_; // Type of light source
	Color diffuse_; // Diffuse color of light
	Color specular_; // Specular color of light
	Color ambient_; // Ambient color of light

protected:
	Light(const LightType& type, const Color& diffuse, const Color& ambient, const Color& specular)
		: type_(type), diffuse_(diffuse), specular_(specular), ambient_(ambient) {}
	Light(const Light& value) : type_(kDirectionalLight) {}
	Light& operator=(const Light& value) {}
	~Light() {}
};

//--------------------------------------------------------------------------------
//  �f�B���N�V�������C�g
//--------------------------------------------------------------------------------
class DirectionalLight : public Light
{
public:
	DirectionalLight(const Vector3& direction, const Color& diffuse, const Color& ambient, const Color& specular)
		: Light(kDirectionalLight, diffuse, ambient, specular)
		, direction_(direction) {}
	Vector3	direction_; // Direction in world space
};

////--------------------------------------------------------------------------------
////  �X�|�b�g���C�g
////--------------------------------------------------------------------------------
//class SpotLight : public Light
//{
//public:
//	SpotLight()
//		: Light(kSpot) {}
//	Vector3	position_; // Position in world space
//	float cutoffRange_; // Cutoff range
//	float falloff_; // Falloff
//	float constantAttenuation_; // Constant attenuation
//	float linearAttenuation_; // Linear attenuation
//	float quadraticAttenuation_; // Quadratic attenuation
//	float theta_; // Inner angle of spotlight cone
//	float phi_; // Outer angle of spotlight cone
//};
//
////--------------------------------------------------------------------------------
////  �|�C���g���C�g
////--------------------------------------------------------------------------------
//class PointLight : public Light
//{
//public:
//	PointLight() : Light(kPoint) {}
//	Vector3	position_; // Position in world space
//	float cutoffRange_; // Cutoff range
//	float falloff_; // Falloff
//	float constantAttenuation_; // Constant attenuation
//	float linearAttenuation_; // Linear attenuation
//	float quadraticAttenuation_; // Quadratic attenuation
//};
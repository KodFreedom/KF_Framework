//--------------------------------------------------------------------------------
//
//�@LightManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-11-02
//--------------------------------------------------------------------------------
#include "main.h"
#include "lightManager.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
LightManager* LightManager::instance = nullptr;

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ���C�g�̐�������
//--------------------------------------------------------------------------------
int	LightManager::CreateLight(const LightType& type,
	const Color& diffuse, const Color& specular, const Color& ambient,
	const Vector3& position, const Vector3& direction,
	const float& cutoffRange, const float& falloff, const float& constantAttenuation,
	const float& linearAttenuation, const float& quadraticAttenuation,
	const float& theta, const float& phi)
{
	int id = lights.size();
	auto& light = Light(type, diffuse, specular, ambient, position, direction, cutoffRange, falloff, constantAttenuation, linearAttenuation, quadraticAttenuation, theta, phi);
	lights.emplace(id, light);

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	D3DLIGHT9 lightDX = light;
	device->SetLight(id, &lightDX);
	device->LightEnable(id, TRUE);
#endif
	return id;
}

//--------------------------------------------------------------------------------
//  ���C�g�̔j������
//--------------------------------------------------------------------------------
void LightManager::ReleaseLight(const int lightID)
{
	auto iterator = lights.find(lightID);
	if (lights.end() == iterator) return;
	lights.erase(iterator);
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	device->LightEnable(lightID, FALSE);
#endif
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//  �L���X�g
//--------------------------------------------------------------------------------
LightManager::Light::operator D3DLIGHT9() const
{
	D3DLIGHT9 result;
	result.Type = (_D3DLIGHTTYPE)Type;
	result.Diffuse = Diffuse;
	result.Specular = Specular;
	result.Ambient = Ambient;
	result.Position = Position;
	result.Direction = Direction;
	result.Range = CutoffRange;
	result.Falloff = Falloff;
	result.Attenuation0 = ConstantAttenuation;
	result.Attenuation1 = LinearAttenuation;
	result.Attenuation2 = QuadraticAttenuation;
	result.Theta = Theta;
	result.Phi = Phi;
	return result;
}
#endif

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void LightManager::uninit(void)
{
	ReleaseAll();
}

//--------------------------------------------------------------------------------
//  �S�Ẵ��C�g���폜
//--------------------------------------------------------------------------------
void LightManager::ReleaseAll(void)
{
	lights.clear();
}
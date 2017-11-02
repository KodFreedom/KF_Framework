//--------------------------------------------------------------------------------
//
//�@light.h
//	Author : Xu Wenjie
//	Date   : 2017-11-02
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "light.h"

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
Light::Light(const int id, const LightType& type, const Vector3& position, const Vector3& direction, const Color& diffuse, const Color& ambient)
	: id(id)
	, type(type)
	, position(position)
	, direction(direction)
	, diffuse(diffuse)
	, ambient(ambient)
{
	set();
}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
Light::~Light()
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	pDevice->LightEnable(id, FALSE);
#endif
}

//--------------------------------------------------------------------------------
//  �ݒ菈��
//--------------------------------------------------------------------------------
void Light::set(void)
{
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	D3DLIGHT9 lightInfo;
	ZeroMemory(&lightInfo, sizeof(D3DLIGHT9));
	lightInfo.Type = (_D3DLIGHTTYPE)type;
	lightInfo.Diffuse = diffuse;
	lightInfo.Ambient = ambient;
	lightInfo.Position = position;
	lightInfo.Direction = direction;
	pDevice->SetLight(id, &lightInfo);
	pDevice->LightEnable(id, TRUE);
#endif
}

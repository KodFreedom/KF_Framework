//--------------------------------------------------------------------------------
//
//�@LightManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-11-02
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
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
	const Vector3& position, const Vector3& direction,
	const Color& diffuse, const Color& ambient)
{
	int id = lights.size();
	auto light = new Light(id, type, position, direction, diffuse, ambient);
	lights.emplace(id, light);
	return id;
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void LightManager::init(void)
{
	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void LightManager::uninit(void)
{
	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();

	ReleaseAll();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

//--------------------------------------------------------------------------------
//  �S�Ẵ��C�g���폜
//--------------------------------------------------------------------------------
void LightManager::ReleaseAll(void)
{
	for (auto iterator = lights.begin(); iterator != lights.end();)
	{
		delete iterator->second;
		iterator = lights.erase(iterator);
	}
}
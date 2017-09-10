//--------------------------------------------------------------------------------
//
//�@LightManager.cpp
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "lightManager.h"
#include "rendererDX.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//
//  ���C�g
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CLight::CLight()
	: m_nID(0)
	, m_cDiffuse(CKFColor(1.0f))
	, m_cAmbient(CKFColor(0.2f, 0.2f, 0.2f, 1.0f))
	, m_vDirection(CKFVec3(0.0f, -1.0f, 0.0f))
{

}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CLight::Init(const int &nID, const CKFVec3 &vDir)
{
	m_nID = nID;

	m_vDirection = vDir;
	CKFMath::VecNormalize(m_vDirection);

	SetLight();
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CLight::Uninit(void)
{
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();

	pDevice->LightEnable(m_nID, FALSE);
}

//--------------------------------------------------------------------------------
//  �����ݒ�
//--------------------------------------------------------------------------------
void CLight::SetDirection(const CKFVec3 &vDir)
{
	m_vDirection = vDir;
	CKFMath::VecNormalize(m_vDirection);

	SetLight();
}

//--------------------------------------------------------------------------------
//  Device�Ƀ��C�g�ݒ�
//--------------------------------------------------------------------------------
void CLight::SetLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	D3DLIGHT9 lightInfo;

	//Light Init
	ZeroMemory(&lightInfo, sizeof(D3DLIGHT9));
	lightInfo.Type = D3DLIGHT_DIRECTIONAL;//���s����
	lightInfo.Diffuse = m_cDiffuse;
	lightInfo.Ambient = m_cAmbient;
	lightInfo.Direction = m_vDirection;

	//�f�o�C�X�Ƀ��C�g�ݒ�
	pDevice->SetLight(m_nID, &lightInfo);
	pDevice->LightEnable(m_nID, TRUE);
}

//--------------------------------------------------------------------------------
//
//  ���C�g�}�l�[�W��
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CLightManager::CLightManager()
{
	m_listLight.clear();
}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
CLightManager::~CLightManager()
{
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool CLightManager::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CLightManager::Uninit(void)
{
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();

	ReleaseAll();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

//--------------------------------------------------------------------------------
//  Light Off
//--------------------------------------------------------------------------------
void CLightManager::TurnAllLightOff(void)
{
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

//--------------------------------------------------------------------------------
//  Light On
//--------------------------------------------------------------------------------
void CLightManager::TurnAllLightOn(void)
{
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//--------------------------------------------------------------------------------
//  �S�Ẵ��C�g���폜
//--------------------------------------------------------------------------------
void CLightManager::ReleaseAll(void)
{
	for_each(m_listLight.begin(), m_listLight.end(), [](CLight *pLight)
	{
		if (pLight != NULL)
		{
			pLight->Uninit();
			delete pLight;
			pLight = NULL;
		}
	});

	m_listLight.clear();
}

//--------------------------------------------------------------------------------
//  ���C�g�쐬
//--------------------------------------------------------------------------------
int CLightManager::CreateDirectionalLight(const CKFVec3 &vDir)
{
	int nID = m_listLight.size();

	CLight *pLight = new CLight;
	pLight->Init(nID, vDir);
	m_listLight.push_back(pLight);

	return nID;
}

//--------------------------------------------------------------------------------
//  ���C�g�����ݒ�
//--------------------------------------------------------------------------------
void CLightManager::SetLightDirection(const int &nLightID, const CKFVec3 &vDir)
{
	list<CLight*>::iterator itr = m_listLight.begin();
	advance(itr, nLightID);
	(*itr)->SetDirection(vDir);
}
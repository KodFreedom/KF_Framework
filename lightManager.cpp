//--------------------------------------------------------------------------------
//
//　LightManager.cpp
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
//  静的メンバ変数
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//
//  ライト
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CLight::CLight()
	: m_nID(0)
	, m_cDiffuse(CKFColor(1.0f))
	, m_cAmbient(CKFColor(0.2f, 0.2f, 0.2f, 1.0f))
	, m_vDirection(CKFVec3(0.0f, -1.0f, 0.0f))
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CLight::Init(const int &nID, const CKFVec3 &vDir)
{
	m_nID = nID;

	m_vDirection = vDir;
	CKFMath::VecNormalize(&m_vDirection);

	SetLight();
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CLight::Uninit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	pDevice->LightEnable(m_nID, FALSE);
}

//--------------------------------------------------------------------------------
//  方向設定
//--------------------------------------------------------------------------------
void CLight::SetDirection(const CKFVec3 &vDir)
{
	m_vDirection = vDir;
	CKFMath::VecNormalize(&m_vDirection);

	SetLight();
}

//--------------------------------------------------------------------------------
//  Deviceにライト設定
//--------------------------------------------------------------------------------
void CLight::SetLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	D3DLIGHT9 lightInfo;

	//Light Init
	ZeroMemory(&lightInfo, sizeof(D3DLIGHT9));
	lightInfo.Type = D3DLIGHT_DIRECTIONAL;//平行光源
	lightInfo.Diffuse = m_cDiffuse;
	lightInfo.Ambient = m_cAmbient;
	lightInfo.Direction = m_vDirection;

	//デバイスにライト設定
	pDevice->SetLight(m_nID, &lightInfo);
	pDevice->LightEnable(m_nID, TRUE);
}

//--------------------------------------------------------------------------------
//
//  ライトマネージャ
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CLightManager::CLightManager()
{
	m_listLight.clear();
}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CLightManager::~CLightManager()
{
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CLightManager::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CLightManager::Uninit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	ReleaseAll();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

//--------------------------------------------------------------------------------
//  Light Off
//--------------------------------------------------------------------------------
void CLightManager::TurnAllLightOff(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

//--------------------------------------------------------------------------------
//  Light On
//--------------------------------------------------------------------------------
void CLightManager::TurnAllLightOn(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//--------------------------------------------------------------------------------
//  全てのライトを削除
//--------------------------------------------------------------------------------
void CLightManager::ReleaseAll(void)
{
	std::for_each(m_listLight.begin(), m_listLight.end(), [](CLight *pLight)
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
//  ライト作成
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
//  ライト方向設定
//--------------------------------------------------------------------------------
void CLightManager::SetLightDirection(const int &nLightID, const CKFVec3 &vDir)
{
	std::list<CLight*>::iterator itr = m_listLight.begin();
	std::advance(itr, nLightID);
	(*itr)->SetDirection(vDir);
}
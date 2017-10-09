//--------------------------------------------------------------------------------
//
//　LightManager.h
//	Author : Xu Wenjie
//	Date   : 2017-04-30
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class CLight
{
public:
	CLight();
	~CLight() {}

	void Init(const int &nID, const CKFVec3 &vDir);
	void Uninit(void);

	void SetDirection(const CKFVec3 &vDir);

private:
	void SetLight(void);

	int			m_nID;
	CKFColor	m_cDiffuse;
	CKFColor	m_cAmbient;
	CKFVec3		m_vDirection;
};

class CLightManager
{
public:
	CLightManager();
	~CLightManager();

	bool	Init(void);
	void	Uninit(void);

	void	TurnAllLightOff(void);
	void	TurnAllLightOn(void);
	void	ReleaseAll(void);
	int		CreateDirectionalLight(const CKFVec3 &vDir);
	void	SetLightDirection(const int &nLightID, const CKFVec3 &vDir);

private:
	list<CLight*> m_listLight;
};
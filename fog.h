//--------------------------------------------------------------------------------
//
//�@fog.h
//	Author : Xu Wenjie
//	Date   : 2017-10-09
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class CFog
{
public:
	CFog();
	~CFog() {}

	static CFog*	Create(void)
	{
		auto pFog = new CFog;
		pFog->init();
		return pFog;
	}
	void			Release(void)
	{
		uninit();
		delete this;
	}
	
	// Get
	auto			GetEnable(void) const { return m_bEnable; }
	auto			GetEnableRangeFog(void) const { return m_bEnableRangeFog; }
	auto			GetStart(void) const { return m_fStart; }
	auto			GetEnd(void) const { return m_fEnd; }
	auto			GetDensity(void) const { return m_fDensity; }
	auto			GetColor(void) const { return m_cColor; }

	// Set
	void			SetEnable(const bool& bEnable);
	void			SetEnableRangeFog(const bool& bEnable);
	void			SetStart(const float& fStart);
	void			SetEnd(const float& fEnd);
	void			SetDensity(const float& fDensity);
	void			SetColor(const CKFColor& cColor);

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void			init(void);
	void			uninit(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	bool			m_bEnable;
	bool			m_bEnableRangeFog;
	float			m_fStart;
	float			m_fEnd;
	float			m_fDensity;
	CKFColor		m_cColor;
};
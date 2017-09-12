//--------------------------------------------------------------------------------
//	debug�\���Ǘ�����
//�@debugManager.h
//	Author : Xu Wenjie
//	Date   : 2017-09-07
//--------------------------------------------------------------------------------
#pragma once

#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  �N���X��`
//--------------------------------------------------------------------------------
class CDebugManager
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CDebugManager();
	~CDebugManager() {}

	static auto Create(void)
	{
		auto pDM = new CDebugManager;
		pDM->init();
		return pDM;
	}
	void		Update(void);
	void		Draw(void);
	void		Release(void)
	{
		uninit();
		delete this;
	}

	void		DisplayAlways(const string& strInfo);
	void		DisplayAlways(const char& cInfo);
	void		DisplayScroll(const string& strInfo);

private:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const unsigned short sc_usScrollTime = 1 * 30;

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void			init(void);
	void			uninit(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	string			m_strDebugInfo;
	list<string>	m_listStrDebugScroll;
	unsigned short	m_usCntScroll;

#ifdef USING_DIRECTX
	LPD3DXFONT		m_pFont;		// �t�H���g�ւ̃|�C���^
#endif//USING_DIRECTX
};
#endif//_DEBUG
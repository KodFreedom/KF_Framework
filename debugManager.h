//--------------------------------------------------------------------------------
//	debug表示管理処理
//　debugManager.h
//	Author : Xu Wenjie
//	Date   : 2017-09-07
//--------------------------------------------------------------------------------
#pragma once

#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  クラス定義
//--------------------------------------------------------------------------------
class CDebugManager
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
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
	//  定数定義
	//--------------------------------------------------------------------------------
	static const unsigned short sc_usScrollTime = 1 * 30;

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void			init(void);
	void			uninit(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	string			m_strDebugInfo;
	list<string>	m_listStrDebugScroll;
	unsigned short	m_usCntScroll;

#ifdef USING_DIRECTX
	LPD3DXFONT		m_pFont;		// フォントへのポインタ
#endif//USING_DIRECTX
};
#endif//_DEBUG
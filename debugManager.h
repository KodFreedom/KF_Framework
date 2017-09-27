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

	static auto Create(HWND hWnd)
	{
		auto pDM = new CDebugManager;
		pDM->init(hWnd);
		return pDM;
	}
	void		Update(void);
	void		LateUpdate(void);
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
	void			init(HWND hWnd);
	void			uninit(void);
	void			showMainWindow(void);
	void			showCollisionSystemWindow(void);
	void			showCameraWindow(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	string			m_strDebugInfo;
	list<string>	m_listStrDebugScroll;
	unsigned short	m_usCntScroll;
	bool			m_bCollisionSystemWindow;
	bool			m_bCameraWindow;
};
#endif//_DEBUG
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
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const unsigned short sc_usScrollTime = 1 * 30;

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void			init(HWND hWnd);
	void			uninit(void);
	void			showMainWindow(void);
	void			showCollisionSystemWindow(void);
	void			showCameraWindow(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	string			m_strDebugInfo;
	list<string>	m_listStrDebugScroll;
	unsigned short	m_usCntScroll;
	bool			m_bCollisionSystemWindow;
	bool			m_bCameraWindow;
};
#endif//_DEBUG
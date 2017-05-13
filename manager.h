//--------------------------------------------------------------------------------
//
//　manager.h
//	Author : Xu Wenjie
//	Date   : 2016-11-22
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MANAGER_H_
#define _MANAGER_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CRendererDX;
class CKeyboardDX;
class CMouseDX;
class CTextureManager;
class CLightManager;
class CMaterialManager;
class CModelManager;
class CGameObjectManager;
class CMode;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CManager
{
public:
	enum MODE
	{
		MODE_DEMO,
		MODE_MAX
	};

	CManager();
	~CManager() {}

	KFRESULT	Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void		Uninit(void);
	void		Update(void);
	void		LateUpdate(void);
	void		Draw(void);

	//Get関数
	CRendererDX*		GetRenderer(void) { return m_pRenderer; }
	CKeyboardDX*		GetKeyboard(void) { return m_pKeyboard; }
	CMouseDX*			GetMouse(void) { return m_pMouse; }
	CTextureManager*	GetTextureManager(void) { return m_pTextureManager; }
	CLightManager*		GetLightManager(void) { return m_pLightManager; }
	CMaterialManager*	GetMaterialManager(void) { return m_pMaterialManager; }
	CModelManager*		GetModelManager(void) { return m_pModelManager; }
	CGameObjectManager*	GetGameObjectManager(void) { return m_pGameObjectManager; }
	CMode*				GetModeNow(void) { return m_apMode[m_mode]; }

private:
	CRendererDX*		m_pRenderer;
	CKeyboardDX*		m_pKeyboard;
	CMouseDX*			m_pMouse;
	CTextureManager*	m_pTextureManager;
	CLightManager*		m_pLightManager;
	CMaterialManager*	m_pMaterialManager;
	CModelManager*		m_pModelManager;
	CGameObjectManager*	m_pGameObjectManager;
	CMode*				m_apMode[MODE_MAX];
	MODE				m_mode;
};

#endif
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
class CInputManager;
class CTextureManager;
class CLightManager;
class CMaterialManager;
class CModelManager;
class CGameObjectManager;
class CColliderManager;
class CMode;
class CFade;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CManager
{
public:
	CManager();
	~CManager() {}

	KFRESULT	Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void		Uninit(void);
	void		Update(void);
	void		LateUpdate(void);
	void		Draw(void);

	//Set関数
	void				SetMode(CMode* pMode);

	//Get関数
	CRendererDX*		GetRenderer(void) { return m_pRenderer; }
	CInputManager*		GetInputManager(void) { return m_pInputManager; }
	CTextureManager*	GetTextureManager(void) { return m_pTextureManager; }
	CLightManager*		GetLightManager(void) { return m_pLightManager; }
	CMaterialManager*	GetMaterialManager(void) { return m_pMaterialManager; }
	CModelManager*		GetModelManager(void) { return m_pModelManager; }
	CGameObjectManager*	GetGameObjectManager(void) { return m_pGameObjectManager; }
	CColliderManager*	GetColliderManager(void) { return m_pColliderManager; }
	CMode*				GetMode(void) { return m_pMode; }
	CFade*				GetFade(void) { return m_pFade; }

private:
	CRendererDX*		m_pRenderer;
	CInputManager*		m_pInputManager;
	CTextureManager*	m_pTextureManager;
	CLightManager*		m_pLightManager;
	CMaterialManager*	m_pMaterialManager;
	CModelManager*		m_pModelManager;
	CGameObjectManager*	m_pGameObjectManager;
	CColliderManager*	m_pColliderManager;
	CMode*				m_pMode;
	CFade*				m_pFade;
};

#endif
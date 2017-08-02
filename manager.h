//--------------------------------------------------------------------------------
//
//　manager.h
//	Author : Xu Wenjie
//	Date   : 2016-11-22
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CRendererDX;
class CInputManager;
class CMeshManager;
class CTextureManager;
class CLightManager;
class CMaterialManager;
class CModelManager;
class CGameObjectManager;
class CSoundManager;
class CUISystem;
class CMode;
class CFade;
class CKFCollisionSystem;
class CKFPhysicsSystem;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CManager
{
public:
	CManager();
	~CManager() {}

	bool	Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void	Uninit(void);
	void	Update(void);
	void	LateUpdate(void);
	void	Draw(void);

	//Set関数
	void				SetMode(CMode* pMode);

	//Get関数
	CRendererDX*		GetRenderer(void) { return m_pRenderer; }
	CInputManager*		GetInputManager(void) { return m_pInputManager; }
	CMeshManager*		GetMeshManager(void) { return m_pMeshManager; }
	CTextureManager*	GetTextureManager(void) { return m_pTextureManager; }
	CLightManager*		GetLightManager(void) { return m_pLightManager; }
	CMaterialManager*	GetMaterialManager(void) { return m_pMaterialManager; }
	CModelManager*		GetModelManager(void) { return m_pModelManager; }
	CGameObjectManager*	GetGameObjectManager(void) { return m_pGameObjectManager; }
	CSoundManager*		GetSoundManager(void) { return m_pSoundManager; }
	CUISystem*			GetUISystem(void) { return m_pUISystem; }
	CMode*				GetMode(void) { return m_pMode; }
	CFade*				GetFade(void) { return m_pFade; }
	CKFCollisionSystem*	GetCollisionSystem(void) { return m_pCollisionSystem; }
	CKFPhysicsSystem*	GetPhysicsSystem(void) { return m_pPhysicsSystem; }

private:
	CRendererDX*		m_pRenderer;
	CInputManager*		m_pInputManager;
	CMeshManager*		m_pMeshManager;
	CTextureManager*	m_pTextureManager;
	CLightManager*		m_pLightManager;
	CMaterialManager*	m_pMaterialManager;
	CModelManager*		m_pModelManager;
	CGameObjectManager*	m_pGameObjectManager;
	CSoundManager*		m_pSoundManager;
	CUISystem*			m_pUISystem;
	CMode*				m_pMode;
	CFade*				m_pFade;
	CKFCollisionSystem*	m_pCollisionSystem;
	CKFPhysicsSystem*	m_pPhysicsSystem;
};
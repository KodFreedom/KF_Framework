//--------------------------------------------------------------------------------
//
//�@manager.cpp
//	Author : Xu Wenjie
//	Date   : 2016-11-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "renderManager.h"
#include "inputManager.h"
#include "meshManager.h"
#include "textureManager.h"
#include "lightManager.h"
#include "materialManager.h"
#include "gameObjectManager.h"
#include "soundManager.h"
#include "UISystem.h"
#include "rendererDX.h"
#include "mode.h"
#include "modeTitle.h"
#include "modeDemo.h"
#include "fade.h"
#include "fog.h"

//�������Z����
#include "KF_CollisionSystem.h"
#include "KF_PhysicsSystem.h"

#ifdef _DEBUG
#include "modeEditor.h"
#include "debugManager.h"
#endif

//--------------------------------------------------------------------------------
//  �ÓI�����o�[�ϐ��錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CManager::CManager()
	: m_pRenderer(nullptr)
	, m_pRenderManager(nullptr)
	, m_pInputManager(nullptr)
	, m_pMeshManager(nullptr)
	, m_pTextureManager(nullptr)
	, m_pLightManager(nullptr)
	, m_pMaterialManager(nullptr)
	, m_pGameObjectManager(nullptr)
	, m_pSoundManager(nullptr)
	, m_pUISystem(nullptr)
	, m_pMode(nullptr)
	, m_pFade(nullptr)
	, m_pCollisionSystem(nullptr)
	, m_pPhysicsSystem(nullptr)
	, m_pFog(nullptr)
#ifdef _DEBUG
	, m_pDebugManager(nullptr)
#endif
{
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr = E_FAIL;

	//�����_��
	CKFMath::InitRandom();

	//�����_���[�̐���
	m_pRenderer = new CRendererDX;
	hr = m_pRenderer->Init(hWnd, bWindow);
	if (FAILED(hr))
	{
		MessageBox(NULL, "m_pRenderer->Init ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return false;
	}

#ifdef _DEBUG
	//Debug�}�l�[�W���̐���
	m_pDebugManager = CDebugManager::Create(hWnd);
#endif

	//�����_�[�}�l�[�W���̐���
	m_pRenderManager = CRenderManager::Create();

	//�t�H�O�̐���
	m_pFog = CFog::Create();

	//���͂̐���
	m_pInputManager = new CInputManager;
	if (!m_pInputManager->Init(hInstance, hWnd))
	{
		MessageBox(NULL, "m_pInputManager->Init ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return false;
	}

	//���b�V���}�l�[�W���̐���
	m_pMeshManager = new CMeshManager;

	//�e�N�X�`���}�l�[�W���̐���
	m_pTextureManager = new CTextureManager;

	//���C�g�}�l�[�W���̐���
	m_pLightManager = new CLightManager;
	if (!m_pLightManager->Init())
	{
		MessageBox(NULL, "m_pLightManager->Init ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return false;
	}

	//�}�e���A���}�l�[�W���̐���
	m_pMaterialManager = new CMaterialManager;

	//�R���W�����V�X�e��
	m_pCollisionSystem = CKFCollisionSystem::Create();

	//�������Z�V�X�e��
	m_pPhysicsSystem = CKFPhysicsSystem::Create();

	//�Q�[���I�u�W�F�N�g�}�l�[�W���̐���
	m_pGameObjectManager = CGameObjectManager::Create();

	//UI�V�X�e���̐���
	m_pUISystem = new CUISystem;

	//�T�E���h�}�l�[�W���̐���
	m_pSoundManager = new CSoundManager;
	m_pSoundManager->LoadAll();

	//Fade�̐���
	m_pFade = CFade::Create();

	//�������[�h�ݒ�
	SetMode(new CModeDemo);

	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CManager::Uninit(void)
{
	//���[�h�̔j��
	SAFE_RELEASE(m_pMode);

	//Fade�̔j��
	SAFE_RELEASE(m_pFade);

	//�T�E���h�}�l�[�W���̔j��
	if (m_pSoundManager)
	{
		m_pSoundManager->UnloadAll();
		delete m_pSoundManager;
		m_pSoundManager = nullptr;
	}

	//�Q�[���I�u�W�F�N�g�}�l�[�W���̔j��
	SAFE_RELEASE(m_pGameObjectManager);

	//UI�V�X�e���̔j��
	SAFE_RELEASE(m_pUISystem);

	//�������Z�V�X�e���̔j��
	SAFE_RELEASE(m_pPhysicsSystem);

	//�R���W�����V�X�e���̔j��
	SAFE_RELEASE(m_pCollisionSystem);

	//�}�e���A���}�l�[�W���̔j��
	SAFE_RELEASE(m_pMaterialManager);

	//���C�g�}�l�[�W���̔j��
	if (m_pLightManager)
	{
		m_pLightManager->Uninit();
		delete m_pLightManager;
		m_pLightManager = nullptr;
	}

	//�e�N�X�`���}�l�[�W���̔j��
	SAFE_RELEASE(m_pTextureManager);

	//���b�V���}�l�[�W���̔j��
	SAFE_RELEASE(m_pMeshManager);

	//���̓}�l�[�W���̔j��
	SAFE_RELEASE(m_pInputManager);

#ifdef _DEBUG
	//Debug�}�l�[�W���̔j��
	SAFE_RELEASE(m_pDebugManager);
#endif

	//�t�H�O�̔j��
	SAFE_RELEASE(m_pFog);

	//�����_�[�}�l�[�W��
	SAFE_RELEASE(m_pRenderManager);

	//�����_���[�̔j��
	SAFE_RELEASE(m_pRenderer);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CManager::Update(void)
{
#ifdef _DEBUG
	//Debug�}�l�[�W���̍X�V
	m_pDebugManager->Update();
#endif

	//���͍X�V
	m_pInputManager->Update();

	//���[�h�X�V
	m_pMode->Update();

	//�Q�[���I�u�W�F�N�g�}�l�[�W���X�V
	m_pGameObjectManager->Update();

	//�R���W�����X�V
	m_pCollisionSystem->Update();

	//�������Z�X�V
	m_pPhysicsSystem->Update();
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CManager::LateUpdate(void)
{
	//���[�h�X�V
	m_pMode->LateUpdate();

	//�Q�[���I�u�W�F�N�g�}�l�[�W���X�V
	m_pGameObjectManager->LateUpdate();

	//�R���W�����X�V
	m_pCollisionSystem->LateUpdate();

	//UI�X�V
	m_pUISystem->UpdateAll();

	//Fade�X�V
	m_pFade->Update();

	//�����_�[�}�l�[�W���X�V
	m_pRenderManager->Update();

#ifdef _DEBUG
	//Debug�}�l�[�W���̍X�V
	m_pDebugManager->LateUpdate();
#endif
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CManager::Draw(void)
{
	if (m_pRenderer->BeginRender())
	{
		//�J����
		m_pMode->CameraSet();

		//�����_�[
		m_pRenderManager->Render();

#ifdef _DEBUG
		//Debug�\��
		m_pCollisionSystem->DrawCollider();
#endif

		//UI�`��
		m_pUISystem->DrawAll();

		//Fade�`��
		m_pFade->Draw();

#ifdef _DEBUG
		//Debug�\��
		m_pDebugManager->Draw();
#endif
		//End Draw
		m_pRenderer->EndRender();
	}
}

//--------------------------------------------------------------------------------
//  ���[�h�؂�ւ�
//--------------------------------------------------------------------------------
void CManager::SetMode(CMode* pMode)
{
	if (!pMode) { return; }
	SAFE_RELEASE(m_pMode);
	m_pMode = pMode;
	m_pMode->Init();
}
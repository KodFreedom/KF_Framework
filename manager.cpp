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
#include "inputManager.h"
#include "meshManager.h"
#include "textureManager.h"
#include "modelManager.h"
#include "lightManager.h"
#include "materialManager.h"
#include "gameObjectManager.h"
#include "colliderManager.h"
#include "soundManager.h"
#include "UISystem.h"
#include "rendererDX.h"
#include "mode.h"
#include "modeDemo.h"
#include "fade.h"
#include "KF_PhysicsSystem.h"

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
	, m_pInputManager(nullptr)
	, m_pMeshManager(nullptr)
	, m_pTextureManager(nullptr)
	, m_pLightManager(nullptr)
	, m_pMaterialManager(nullptr)
	, m_pModelManager(nullptr)
	, m_pGameObjectManager(nullptr)
	, m_pColliderManager(nullptr)
	, m_pSoundManager(nullptr)
	, m_pUISystem(nullptr)
	, m_pMode(nullptr)
	, m_pFade(nullptr)
	, m_pPhysicsSystem(nullptr)
{
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr;

	//�����_���[�̐���
	m_pRenderer = new CRendererDX;

	hr = m_pRenderer->Init(hWnd, bWindow);
	if (FAILED(hr))
	{
		MessageBox(NULL, "m_pRenderer->Init ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return false;
	}

	//�����_��
	CKFMath::InitRandom();

	//�������Z�V�X�e��
	m_pPhysicsSystem = new CKFPhysicsSystem;
	m_pPhysicsSystem->Init();

	//���͂̐���
	m_pInputManager = new CInputManager;
	m_pInputManager->Init(hInstance, hWnd);

	//���b�V���}�l�[�W���̐���
	m_pMeshManager = new CMeshManager;

	//���f���}�l�[�W���̐���
	m_pModelManager = new CModelManager;
	m_pModelManager->Init();
	m_pModelManager->LoadAll();

	//�e�N�X�`���}�l�[�W���̐���
	m_pTextureManager = new CTextureManager;

	//���C�g�}�l�[�W���̐���
	m_pLightManager = new CLightManager;
	m_pLightManager->Init();

	//�}�e���A���}�l�[�W���̐���
	m_pMaterialManager = new CMaterialManager;
	m_pMaterialManager->Init();

	//�R���W�����}�l�[�W���̐���
	m_pColliderManager = new CColliderManager;
	m_pColliderManager->Init();

	//�Q�[���I�u�W�F�N�g�}�l�[�W���̐���
	m_pGameObjectManager = new CGameObjectManager;
	m_pGameObjectManager->Init();

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
	if (m_pGameObjectManager)
	{
		m_pGameObjectManager->Uninit();
		delete m_pGameObjectManager;
		m_pGameObjectManager = nullptr;
	}

	//UI�V�X�e���̔j��
	SAFE_RELEASE(m_pUISystem);

	//�R���W�����}�l�[�W���̔j��
	if (m_pColliderManager)
	{
		m_pColliderManager->Uninit();
		delete m_pColliderManager;
		m_pColliderManager = nullptr;
	}

	//�}�e���A���}�l�[�W���̔j��
	if (m_pMaterialManager)
	{
		delete m_pMaterialManager;
		m_pMaterialManager = nullptr;
	}

	//���C�g�}�l�[�W���̔j��
	if (m_pLightManager)
	{
		m_pLightManager->Uninit();
		delete m_pLightManager;
		m_pLightManager = nullptr;
	}

	//�e�N�X�`���}�l�[�W���̔j��
	SAFE_RELEASE(m_pTextureManager);

	//���f���}�l�[�W���̔j��
	if (m_pModelManager)
	{
		m_pModelManager->UnloadAll();
		delete m_pModelManager;
		m_pModelManager = nullptr;
	}

	//���b�V���}�l�[�W���̔j��
	SAFE_RELEASE(m_pMeshManager);

	//���̓}�l�[�W���̔j��
	if (m_pInputManager)
	{
		m_pInputManager->Uninit();
		delete m_pInputManager;
		m_pInputManager = nullptr;
	}

	//�������Z�V�X�e���̔j��
	SAFE_RELEASE(m_pPhysicsSystem);

	//�����_���[�̔j��
	SAFE_RELEASE(m_pRenderer);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CManager::Update(void)
{
	//���͍X�V
	m_pInputManager->Update();

	//�����_���[�X�V
	m_pRenderer->Update();

	//���[�h�X�V
	m_pMode->Update();

	//�R���W�����X�V
	m_pColliderManager->Update();

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

	//UI�X�V
	m_pUISystem->UpdateAll();

	//Fade�X�V
	m_pFade->Update();
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CManager::Draw(void)
{
	if (m_pRenderer)
	{
		if (m_pRenderer->BeginDraw() == true)
		{
			//���[�h�`��
			m_pMode->Draw();

#ifdef _DEBUG
			m_pColliderManager->DrawCollider();
#endif
			//UI�`��
			m_pUISystem->DrawAll();

			//Fade�`��
			m_pFade->Draw();

			m_pRenderer->EndDraw();
		}
	}
}

//--------------------------------------------------------------------------------
//  ���[�h�؂�ւ�
//--------------------------------------------------------------------------------
void CManager::SetMode(CMode* pMode)
{
	if (m_pMode)
	{
		m_pMode->Release();
	}

	m_pMode = pMode;

	if (m_pMode)
	{
		m_pMode->Init();
	}
}
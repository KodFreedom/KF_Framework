//--------------------------------------------------------------------------------
//
//�@manager.cpp
//	Author : Xu Wenjie
//	Date   : 2016-11-22
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "modelManager.h"
#include "lightManager.h"
#include "materialManager.h"
#include "gameObjectManager.h"
#include "colliderManager.h"
#include "rendererDX.h"
#include "inputDX.h"
#include "mode.h"
#include "modeDemo.h"
#include "fade.h"

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
	: m_pRenderer(NULL)
	, m_pKeyboard(NULL)
	, m_pMouse(NULL)
	, m_pTextureManager(NULL)
	, m_pLightManager(NULL)
	, m_pMaterialManager(NULL)
	, m_pModelManager(NULL)
	, m_pGameObjectManager(NULL)
	, m_pColliderManager(NULL)
	, m_pMode(NULL)
	, m_pFade(NULL)
{
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
KFRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr;

	//�����_���[�̐���
	m_pRenderer = new CRendererDX;

	hr = m_pRenderer->Init(hWnd, bWindow);
	if (FAILED(hr))
	{
		MessageBox(NULL, "m_pRenderer->Init ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return KF_FAILED;
	}

	//�����_��
	CKFMath::InitRandom();

	//���͂̐���
	m_pKeyboard = new CKeyboardDX;
	m_pKeyboard->Init(hInstance, hWnd);
	m_pMouse = new CMouseDX;
	m_pMouse->Init(hInstance, hWnd);

	//���f���}�l�[�W���̐���
	m_pModelManager = new CModelManager;
	m_pModelManager->Init();
	m_pModelManager->LoadAll();

	//�e�N�X�`���}�l�[�W���̐���
	m_pTextureManager = new CTextureManager;
	m_pTextureManager->Init();
	m_pTextureManager->LoadAll();

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

	//Fade�̐���
	m_pFade = CFade::Create();

	//�������[�h�ݒ�
	SetMode(new CModeDemo);

	return KF_SUCCEEDED;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CManager::Uninit(void)
{
	//���[�h�̔j��
	if (m_pMode)
	{
		m_pMode->Release();
		m_pMode = NULL;
	}

	//Fade�̔j��
	if (m_pFade)
	{
		m_pFade->Release();
		m_pFade = NULL;
	}

	//�Q�[���I�u�W�F�N�g�}�l�[�W���̔j��
	if (m_pGameObjectManager != NULL)
	{
		m_pGameObjectManager->Uninit();
		delete m_pGameObjectManager;
		m_pGameObjectManager = NULL;
	}

	//�R���W�����}�l�[�W���̔j��
	if (m_pColliderManager != NULL)
	{
		m_pColliderManager->Uninit();
		delete m_pColliderManager;
		m_pColliderManager = NULL;
	}

	//�}�e���A���}�l�[�W���̔j��
	if (m_pMaterialManager != NULL)
	{
		delete m_pMaterialManager;
		m_pMaterialManager = NULL;
	}

	//���C�g�}�l�[�W���̔j��
	if (m_pLightManager != NULL)
	{
		m_pLightManager->Uninit();
		delete m_pLightManager;
		m_pLightManager = NULL;
	}

	//�e�N�X�`���}�l�[�W���̔j��
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->UnloadAll();
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	//���f���}�l�[�W���̔j��
	if (m_pModelManager != NULL)
	{
		m_pModelManager->UnloadAll();
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	//�L�[�{�[�h�̔j��
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}

	//�}�E�X�̔j��
	if (m_pMouse != NULL)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = NULL;
	}

	//�����_���[�̔j��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CManager::Update(void)
{
	//�L�[�{�[�h�X�V
	if (m_pKeyboard)
	{
		m_pKeyboard->Update();
	}

	//�}�E�X�X�V
	if (m_pMouse)
	{
		m_pMouse->Update();
	}

	//�����_���[�X�V
	if (m_pRenderer)
	{
		m_pRenderer->Update();
	}

	//���[�h�X�V
	if (m_pMode)
	{
		m_pMode->Update();
	}

	//Fade�X�V
	if (m_pFade)
	{
		m_pFade->Update();
	}

	//�R���W�����X�V
	if (m_pColliderManager)
	{
		m_pColliderManager->Update();
	}
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CManager::LateUpdate(void)
{
	//���[�h�X�V
	if (m_pMode)
	{
		m_pMode->LateUpdate();
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CManager::Draw(void)
{
	if (m_pRenderer)
	{
		if (m_pRenderer->BeginDraw() == KF_SUCCEEDED)
		{
			//���[�h�`��
			if (m_pMode)
			{
				m_pMode->Draw();
			}

			//Fade�X�V
			if (m_pFade)
			{
				m_pFade->Draw();
			}

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
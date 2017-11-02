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
#include "renderer.h"
#include "renderManager.h"
#include "input.h"
#include "meshManager.h"
#include "textureManager.h"
#include "lightManager.h"
#include "materialManager.h"
#include "gameObjectManager.h"
#include "soundManager.h"
#include "UISystem.h"
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
#include "debugObserver.h"
#endif

//--------------------------------------------------------------------------------
//  �ÓI�����o�[�ϐ��錾
//--------------------------------------------------------------------------------
Manager* Manager::instance = nullptr;

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FCreate
//  �֐������F��������
//	�����F	hInstance�F�l
//			hWnd�F
//			isWindowMode�F
//	�߂�l�FManager*
//--------------------------------------------------------------------------------
Manager* Manager::Create(HINSTANCE hInstance, HWND hWnd, BOOL isWindowMode)
{
	if (instance) return instance;
	instance = new Manager;
	instance->init(hInstance, hWnd, isWindowMode);
	return instance;
}

//--------------------------------------------------------------------------------
//	�֐����FRelease
//  �֐������F�j������
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Manager::Release(void)
{
	SAFE_UNINIT(instance);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void Manager::Update(void)
{
#ifdef _DEBUG
	DebugObserver::Instance()->Update();
#endif

	//���͍X�V
	Input::Instance()->Update();

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
void Manager::LateUpdate(void)
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
	DebugObserver::Instance()->LateUpdate();
#endif
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void Manager::Draw(void)
{
	if (Renderer::Instance()->BeginRender())
	{
		m_pMode->CameraSet();
		m_pRenderManager->Render();
#ifdef _DEBUG
		m_pCollisionSystem->DrawCollider();
#endif
		m_pUISystem->DrawAll();
		m_pFade->Draw();
#ifdef _DEBUG
		DebugObserver::Instance()->Draw();
#endif
		Renderer::Instance()->EndRender();
	}
}

//--------------------------------------------------------------------------------
//  ���[�h�؂�ւ�
//--------------------------------------------------------------------------------
void Manager::ChangeMode(CMode* nextMode)
{
	if (!currentMode) { return; }
	SAFE_RELEASE(m_pMode);
	m_pMode = pMode;
	m_pMode->Init();
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool Manager::init(HINSTANCE hInstance, HWND hWnd, BOOL isWindowMode)
{
	HRESULT hr = E_FAIL;
	Random::Init();

	if (!Renderer::Create(hWnd, isWindowMode)) return false;

#ifdef _DEBUG
	DebugObserver::Create(hWnd);
#endif

	//�����_�[�}�l�[�W���̐���
	m_pRenderManager = CRenderManager::Create();

	//�t�H�O�̐���
	m_pFog = CFog::Create();

	Input::Create(hInstance, hWnd);
	MeshManager::Create();
	TextureManager::Create();
	LightManager::Create();
	MaterialManager::Create();

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
	SetMode(new CModeEditor);

	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void Manager::uninit(void)
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

	MaterialManager::Release();
	LightManager::Release();
	TextureManager::Release();
	MeshManager::Release();
	Input::Release();

#ifdef _DEBUG
	DebugObserver::Release();
#endif

	//�t�H�O�̔j��
	SAFE_RELEASE(m_pFog);

	//�����_�[�}�l�[�W��
	SAFE_RELEASE(m_pRenderManager);

	Renderer::Release();
}
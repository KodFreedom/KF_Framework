//--------------------------------------------------------------------------------
//
//�@modeDemo.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-28
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "lightManager.h"
#include "inputManager.h"
#include "soundManager.h"
#include "mode.h"
#include "modeDemo.h"
#include "modeResult.h"
#include "actionGameCamera.h"
#include "fade.h"

//gameobject
#include "stageSpawner.h"
#include "gameObjectSpawner.h"
#include "gameObjectActor.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CModeDemo::CModeDemo() : CMode()
	, m_bEndMode(false)
{

}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
CModeDemo::~CModeDemo()
{

}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CModeDemo::Init(void)
{	
	//���C�g�̏�����
	CMain::GetManager()->GetLightManager()->CreateDirectionalLight(CKFVec3(0.5f, -0.5f, 0.5f));

	//�J�����̏�����
	m_pCamera = new CActionGameCamera;
	m_pCamera->Init();

	//�Q�[���I�u�W�F�N�g�̏�����
	CGameObjectSpawner::CreateSkyBox(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	CStageSpawner::LoadStage("demo");
	auto pPlayer = CGameObjectActor::CreatePlayer("data/MODEL/motionPlayer.txt", CKFVec3(-188.0f, 10.0f, 196.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	pPlayer->SetName("Player");
	auto pEnemy = CGameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", CKFVec3(-50.0, 30.0f, -7.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	pEnemy->SetName("Enemy00");
	//pEnemy = CGameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", CKFVec3(-12.0, 45.0f, -46.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	//pEnemy->SetName("Enemy01");
	//pEnemy = CGameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", CKFVec3(117.0f, 80.0f, 1.6f), CKFVec3(0.0f), CKFVec3(1.0f));
	//pEnemy->SetName("Enemy02");
	CGameObjectSpawner::CreateGoal(CKFVec3(142.0f, 74.0f, 1.05f));
	m_pCamera->SetTarget(pPlayer);

	//CMain::GetManager()->GetSoundManager()->Play(CSM::BGM_GAME);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CModeDemo::Uninit(void)
{
	//�J�����ƃQ�[���I�u�W�F�N�g�̔j��
	CMode::Uninit();

	//���C�g�̔j��
	CMain::GetManager()->GetLightManager()->ReleaseAll();

	//BGM�̒�~
	CMain::GetManager()->GetSoundManager()->StopAll();
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CModeDemo::Update(void)
{
	CMode::Update();
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CModeDemo::LateUpdate(void)
{
	CMode::LateUpdate();

	if (m_bEndMode || CMain::GetManager()->GetInputManager()->GetKeyTrigger(CInputManager::KEY::K_START))
	{
		CMain::GetManager()->GetFade()->FadeToMode(new CModeResult);
	}
}
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

#ifdef _DEBUG
#include "debugManager.h"
#endif // _DEBUG

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
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
	Main::GetManager()->GetLightManager()->CreateDirectionalLight(Vector3(0.5f, -0.5f, 0.5f));

	//�J�����̏�����
	m_pCamera = new CActionGameCamera;
	m_pCamera->Init();

	//�Q�[���I�u�W�F�N�g�̏�����
	GameObjectSpawner::CreateSkyBox(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f));
	CStageSpawner::LoadStage("demo");
	//GameObjectSpawner::CreateCube(Vector3(0.0f), Vector3(0.0f), Vector3(1.0f));
	auto pPlayer = GameObjectActor::CreatePlayer("data/MODEL/motionPlayer.txt", Vector3(119.7f, 10.0f, -121.2f), Vector3(0.0f), Vector3(1.0f));
	pPlayer->SetName("Player");

#ifdef _DEBUG
	Main::GetManager()->GetDebugManager()->SetPlayer(pPlayer);
#endif // _DEBUG
	//auto pEnemy = GameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", Vector3(-50.0, 30.0f, -7.0f), Vector3(0.0f), Vector3(1.0f));
	//pEnemy->SetName("Enemy00");
	//pEnemy = GameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", Vector3(-12.0, 45.0f, -46.0f), Vector3(0.0f), Vector3(1.0f));
	//pEnemy->SetName("Enemy01");
	//pEnemy = GameObjectActor::CreateEnemy("data/MODEL/motionPlayer.txt", Vector3(117.0f, 80.0f, 1.6f), Vector3(0.0f), Vector3(1.0f));
	//pEnemy->SetName("Enemy02");
	GameObjectSpawner::CreateGoal(Vector3(112.0f, 16.0f, 51.0f));
	m_pCamera->SetTarget(pPlayer);

	//Main::GetManager()->GetSoundManager()->Play(CSM::BGM_GAME);
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

	if (Main::GetManager()->GetInputManager()->GetKeyTrigger(Input::KEY::Start))
	{
		Main::GetManager()->GetFade()->FadeToMode(new CModeResult);
	}
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CModeDemo::uninit(void)
{
	//�J�����ƃQ�[���I�u�W�F�N�g�̔j��
	CMode::uninit();

	//���C�g�̔j��
	Main::GetManager()->GetLightManager()->ReleaseAll();

	//BGM�̒�~
	Main::GetManager()->GetSoundManager()->StopAll();
}
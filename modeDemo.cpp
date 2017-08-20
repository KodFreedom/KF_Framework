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
#include "modelManager.h"
#include "lightManager.h"
#include "inputManager.h"
#include "soundManager.h"
#include "mode.h"
#include "modeDemo.h"
#include "modeResult.h"
#include "actionGameCamera.h"
#include "fade.h"

//gameobject
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
	CGameObjectSpawner::CreateField(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		CKFVec3 vPos = CKFMath::GetRandomVec3(CKFVec3(-10.0f, 10.0f, -10.0f), CKFVec3(10.0f, 12.0f, 10.0f));
		CKFVec3 vRot = CKFVec3(0.0f);//CKFMath::GetRandomVec3(CKFVec3(-KF_PI, -KF_PI, -KF_PI), CKFVec3(KF_PI, KF_PI, KF_PI));
		CGameObjectSpawner::CreateCube(vPos, vRot, CKFVec3(1.0f));
	}
	
	CGameObjectSpawner::CreateSkyBox(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	CGameObject* pPlayer = CGameObjectActor::CreatePlayer("data/MODEL/motionPlayer.txt", CKFVec3(0.0f, 10.0f, 0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	//CGameObject* pEnemy = CGameObjectActor::CreateEnemy(CMOM::MODEL_PLAYER, CKFVec3(0.0f, 10.0f, 10.0f), CKFVec3(0.0f, KF_PI, 0.0f), CKFVec3(1.0f));

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

	if (m_bEndMode || CMain::GetManager()->GetInputManager()->GetKeyTrigger(CInputManager::KEY::K_SUBMIT))
	{
		CMain::GetManager()->GetFade()->FadeToMode(new CModeResult);
	}
}
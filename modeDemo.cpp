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
#include "gameObject2D.h"
#include "gameObject3D.h"
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
	GetManager()->GetLightManager()->CreateDirectionalLight(CKFVec3(0.5f, -0.5f, 0.5f));

	//�J�����̏�����
	m_pCamera = new CActionGameCamera;
	m_pCamera->Init();

	//�Q�[���I�u�W�F�N�g�̏�����
	CGameObject3D::CreateField(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));

	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		CKFVec3 vPos = CKFMath::GetRandomVec3(CKFVec3(-10.0f, 4.0f, -10.0f), CKFVec3(10.0f, 5.0f, 10.0f));
		CKFVec3 vRot = CKFMath::GetRandomVec3(CKFVec3(-KF_PI, -KF_PI, -KF_PI), CKFVec3(KF_PI, KF_PI, KF_PI));
		CGameObject3D::CreateCube(vPos, vRot, CKFVec3(1.2f));
	}
	
	CGameObject3D::CreateSkyBox(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	CGameObject* pPlayer = CGameObjectActor::CreatePlayer(CMOM::MODEL_PLAYER, CKFVec3(0.0f, 10.0f, 0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	CGameObject* pEnemy = CGameObjectActor::CreateEnemy(CMOM::MODEL_PLAYER, CKFVec3(0.0f, 10.0f, 10.0f), CKFVec3(0.0f, KF_PI, 0.0f), CKFVec3(1.0f));

	m_pCamera->SetTarget(pPlayer);

	GetManager()->GetSoundManager()->Play(CSM::BGM_GAME);
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CModeDemo::Uninit(void)
{
	//�J�����ƃQ�[���I�u�W�F�N�g�̔j��
	CMode::Uninit();

	//���C�g�̔j��
	GetManager()->GetLightManager()->ReleaseAll();

	//BGM�̒�~
	GetManager()->GetSoundManager()->StopAll();
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CModeDemo::Update(void)
{
	CMode::Update();

	if (m_bEndMode || GetManager()->GetInputManager()->GetKeyTrigger(CInputManager::KEY::K_SUBMIT))
	{
		GetManager()->GetFade()->FadeToMode(new CModeResult);
	}
}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CModeDemo::LateUpdate(void)
{
	CMode::LateUpdate();
}
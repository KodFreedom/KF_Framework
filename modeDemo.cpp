//--------------------------------------------------------------------------------
//
//�@modeDemo.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-28
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
#include "mode.h"
#include "modeDemo.h"
#include "camera.h"

//gameobject
#include "gameObject2D.h"
#include "gameObject3D.h"
#include "gameObjectModel.h"
#include "gameObjectActor.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CModeDemo::CModeDemo() : CMode()
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
	m_pCamera = new CCamera;
	m_pCamera->Init();

	//�Q�[���I�u�W�F�N�g�̏�����
	CGameObject3D::CreateField(4, 4, CKFVec2(1.0f, 1.0f), CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	CGameObject2D::Create(CKFVec3(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.25f, 0.0f), 0.0f, CKFVec3(256.0f, 256.0f, 0.0f), CTM::TEX_DEMO_TEST);
	//CMeshCube::Create(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f), CKFColor(1.0f));
	//CGameObjectModel::Create(CKFVec3(0.0f), CKFVec3(0.0f), CMOM::MODEL_ROBOT);
	CGameObject3D::CreateSkyBox(CKFVec3(0.0f), CKFVec3(0.0f), CKFVec3(1.0f));
	//CGameObjectActor::Create(CKFVec3(0.0f), CKFVec3(0.0f), CMOM::MODEL_PLAYER);
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
}
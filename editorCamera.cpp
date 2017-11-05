//--------------------------------------------------------------------------------
//	�G�f�B�^�p�J�����N���X
//�@actionGameCamera.h
//	Author : Xu Wenjie
//	Date   : 2016-06-20
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "versionSetting.h"
#if defined(_DEBUG) || defined(EDITOR)
#include "main.h"
#include "manager.h"
#include "input.h"
#include "editorCamera.h"
#include "cameraManager.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
const float	EditorCamera::rotationSpeed = 0.05f;
const float	EditorCamera::startRotationMin = 0.2f;
const float	EditorCamera::zoomSpeed = 0.5f;
const float	EditorCamera::distanceMin = 3.0f;
const float	EditorCamera::distanceMax = 500.0f;

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
EditorCamera::EditorCamera() : Camera()
{
	CameraManager::Instance()->RegisterAsMain(this);
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void EditorCamera::Init(void)
{
	Camera::Init();
	SetPitch(45.0f / 180.0f * Pi);
	SetDistance(20.0f);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void EditorCamera::Update(void)
{
	auto input = Input::Instance();
	if (!input->GetKeyPress(Key::Lock)) return;
	
	Vector3 rotation;
	float zoomSpeed = 0.0f;
	float axisX = input->RotHorizontal();
	float axisY = input->RotVertical();
	float zoom = input->Zoom();

	if (fabsf(axisX) > startRotationMin)
	{//Y����]
		yaw(rotationSpeed * axisX);
	}
	if (fabsf(axisY) > startRotationMin)
	{//X����]
		pitch(rotationSpeed * axisY);
	}

	if (fabsf(zoom) > startRotationMin)
	{
		SetDistance(Math::Clamp(distance + zoomSpeed * zoom, distanceMin, distanceMax));
	}
}
#endif // _DEBUG
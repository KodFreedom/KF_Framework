//--------------------------------------------------------------------------------
//�@editor_camera.cpp
//	�G�f�B�^�p�J�����N���X
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "editor_camera.h"
#if defined(EDITOR)
#include "main_system.h"
#include "input.h"

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void EditorCamera::Init(void)
{
	Camera::Init();
	SetPitch(45.0f / 180.0f * kPi);
	SetDistance(20.0f);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void EditorCamera::Update(void)
{
	auto input = MainSystem::Instance()->GetInput();
	if (!input->GetKeyPress(Key::kLock)) return;
	
	Vector3 rotation;
	float zoomSpeed = 0.0f;
	float axisX = input->RotationHorizontal();
	float axisY = input->RotationVertical();
	float zoom = input->Zoom();

	if (fabsf(axisX) > kStartRotationMin)
	{//Y����]
		Yaw(kRotationSpeed * axisX);
	}
	if (fabsf(axisY) > kStartRotationMin)
	{//X����]
		Pitch(kRotationSpeed * axisY);
	}

	if (fabsf(zoom) > kStartRotationMin)
	{
		SetDistance(math::Clamp(distance_ + zoomSpeed * zoom, kDistanceMin, kDistanceMax));
	}
}
#endif // EDITOR
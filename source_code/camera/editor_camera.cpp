//--------------------------------------------------------------------------------
//�@editor_camera.cpp
//  �G�f�B�^�p�J�����N���X
//  Author : �����^(KodFreedom)
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
    auto& input = MainSystem::Instance().GetInput();
    if (!input.GetKeyPress(Key::kLock)) return;
    
    Vector3 rotation;
    float rotation_x = input.RotationHorizontal();
    float rotation_y = input.RotationVertical();
    float zoom = input.Zoom();

    if (fabsf(rotation_x) > kStartRotationMin)
    {//Y����]
        Yaw(kRotationSpeed * rotation_x);
    }
    if (fabsf(rotation_y) > kStartRotationMin)
    {//X����]
        Pitch(kRotationSpeed * rotation_y);
    }

    if (fabsf(zoom) > kStartRotationMin)
    {
        SetDistance(Math::Clamp(distance_ + kZoomSpeed * zoom, kDistanceMin, kDistanceMax));
    }
}
#endif // EDITOR
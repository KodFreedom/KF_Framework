//--------------------------------------------------------------------------------
//�@third_person_camera.h
//  �O�l�̃J�����N���X
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "camera.h"

//--------------------------------------------------------------------------------
//  �O�l�̃J�����N���X
//--------------------------------------------------------------------------------
class ThirdPersionCamera : public Camera
{
public:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    ThirdPersionCamera();
    ~ThirdPersionCamera() {}
    
    //--------------------------------------------------------------------------------
    //  ����������
    //--------------------------------------------------------------------------------
    void Init(void) override;

    //--------------------------------------------------------------------------------
    //  �X�V����
    //--------------------------------------------------------------------------------
    void Update(void) override;

    //--------------------------------------------------------------------------------
    //  ��X�V����
    //--------------------------------------------------------------------------------
    void LateUpdate(void) override;

private:
    //--------------------------------------------------------------------------------
    //  �萔��`
    //--------------------------------------------------------------------------------
    static constexpr float kRotationSpeed = 0.05f;
    static constexpr float kStartRotationMin = 0.2f;
    static constexpr float kRotationLerpTime = 0.1f;
    static constexpr float kDistanceDefault = 4.0f;
    static constexpr float kDistanceMin = 2.0f;
    static constexpr float kDistanceMax = 10.0f;
    static constexpr float kMoveLerpTime = 0.075f;
    static constexpr float kPitchDefault = 15.0f / 180.0f * kPi;
    static constexpr float kPitchMin = -5.0f / 180.0f * kPi;
    static constexpr float kPitchMax = 60.0f / 180.0f * kPi;
    static constexpr float kCollisionRadius = 0.1f;
    static constexpr float kOffsetY = 1.5f;

    //--------------------------------------------------------------------------------
    //  x����]
    //--------------------------------------------------------------------------------
    void Pitch(const float& radian) override;

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    float       pitch_speed_;
    float       yaw_speed_;
};
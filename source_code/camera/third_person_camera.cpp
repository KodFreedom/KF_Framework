//--------------------------------------------------------------------------------
//　third_person_camera.h
//  三人称カメラクラス
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "third_person_camera.h"
#include "../main_system.h"
#include "../input/input.h"
#include "../game_object/game_object.h"
#include "../observer/actor_observer.h"
#include "../component/behavior/actor/player_controller.h"

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
ThirdPersionCamera::ThirdPersionCamera() : Camera()
    , pitch_speed_(0.0f)
    , yaw_speed_(0.0f)
{}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void ThirdPersionCamera::Init(void)
{
    Camera::Init();
    SetOffsetY(kOffsetY);
    SetDistance(kDistanceDefault);
    SetPitch(kPitchDefault);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void ThirdPersionCamera::Update(void)
{
    auto& input = MainSystem::Instance().GetInput();
    float rotation_x = input.RotationHorizontal();
    float rotation_y = input.RotationVertical();

    float pitch_amount = 0.0f;
    float yaw_amount = 0.0f;

    if (fabsf(rotation_x) > kStartRotationMin)
    {//Y軸回転
        yaw_amount = kRotationSpeed * rotation_x;
    }
    if (fabsf(rotation_y) > kStartRotationMin)
    {//X軸回転
        pitch_amount = kRotationSpeed * rotation_y;
    }

    pitch_speed_ = Math::Lerp(pitch_speed_, pitch_amount, kRotationLerpTime);
    yaw_speed_ = Math::Lerp(yaw_speed_, yaw_amount, kRotationLerpTime);
    Pitch(pitch_speed_);
    Yaw(yaw_speed_);
}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void ThirdPersionCamera::LateUpdate(void)
{
    auto follow_target = MainSystem::Instance().GetActorObserver().GetPlayer();
    if (follow_target)
    {
        rig_.position = Math::Lerp(rig_.position, follow_target->GetGameObject().GetTransform()->GetPosition(), kMoveLerpTime);
    }
    Camera::LateUpdate();
}

//--------------------------------------------------------------------------------
//  X軸回転
//--------------------------------------------------------------------------------
void ThirdPersionCamera::Pitch(const float& radian)
{
    Camera::Pitch(radian);
    pivot_.rotation.x_ = Math::Clamp(pivot_.rotation.x_, kPitchMin, kPitchMax);
}
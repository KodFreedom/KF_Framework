//--------------------------------------------------------------------------------
//  ���ԃr�w�C�r�A�R���|�l���g
//�@windmillBehaviorComponent.h
//  Author : Xu Wenjie
//  Date   : 2017-09-12
//--------------------------------------------------------------------------------
#include "windmill_controller.h"
#include "game_object.h"
#include "transform.h"
#include "time.h"

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool WindmillController::Init(void)
{
    fan_ = owner_.GetTransform()->FindChildBy(L"Fan");
    assert(fan_);
    return true;
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void WindmillController::Update(void)
{
    fan_->RotateByRoll(rotate_speed_ * Time::Instance()->ScaledDeltaTime());
}
//--------------------------------------------------------------------------------
//  風車ビヘイビアコンポネント
//　windmillBehaviorComponent.h
//  Author : Xu Wenjie
//  Date   : 2017-09-12
//--------------------------------------------------------------------------------
#include "windmill_controller.h"
#include "../../../game_object/game_object.h"
#include "../../transform/transform.h"
#include "../../../game_time.h"

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool WindmillController::Init(void)
{
    fan_ = owner_.GetTransform()->FindChildBy(L"Fan");
    assert(fan_);
    return true;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void WindmillController::Update(void)
{
    fan_->RotateByRoll(rotate_speed_ * GameTime::Instance().ScaledDeltaTime());
}
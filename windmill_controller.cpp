//--------------------------------------------------------------------------------
//	���ԃr�w�C�r�A�R���|�l���g
//�@windmillBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-09-12
//--------------------------------------------------------------------------------
#include "windmill_controller.h"
#include "game_object.h"
#include "transform.h"

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool WindmillController::Init(void)
{
	fan_ = owner_.FindChildBy(L"Fan");
	if(!fan_)
	{
		assert("No Fan!!");
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void WindmillController::Update(void)
{
	if (!fan_) return;
	fan_->GetTransform()->RotateByRoll(rotate_speed_ * DELTA_TIME);
}
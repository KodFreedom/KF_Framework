//--------------------------------------------------------------------------------
//	���ԃr�w�C�r�A�R���|�l���g
//�@windmillBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-09-12
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "windmillController.h"
#include "gameObject.h"
#include "transform.h"
#include "KF_Utility.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool WindmillController::Init(void)
{
	fan = Utility::FindChildBy("Fan", owner);

	if(!fan)
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
	fan->GetTransform()->RotateByRoll(rotateSpeed * DELTA_TIME);
}
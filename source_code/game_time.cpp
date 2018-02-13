//--------------------------------------------------------------------------------
//�@time.cpp
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "game_time.h"
#include "common_setting.h"
#include "kf_utility.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�[�ϐ��錾
//--------------------------------------------------------------------------------
GameTime* GameTime::instance_ = nullptr;

//--------------------------------------------------------------------------------
//  instance��������
//--------------------------------------------------------------------------------
GameTime* GameTime::Create(void)
{
    if (instance_) return instance_;
    instance_ = MY_NEW GameTime;
    return instance_;
}

//--------------------------------------------------------------------------------
//  �j������
//--------------------------------------------------------------------------------
void GameTime::Release(void)
{
    SAFE_DELETE(instance_);
}

//--------------------------------------------------------------------------------
//  �t���[�����s�o���邩���`�F�b�N
//--------------------------------------------------------------------------------
bool GameTime::CanUpdateFrame(void)
{
    QueryPerformanceFrequency(&frequency_);
    QueryPerformanceCounter(&current_time_);
    //delta_time_ = static_cast<float>(current_time_.QuadPart - exec_last_time_.QuadPart)
    //     / static_cast<float>(frequency_.QuadPart);
    delta_time_ = kTimeInterval;

#ifdef _DEBUG
    // break point�̎����Ԃ���񂶂Ⴄ�̂Ő�������K�v������
    delta_time_ = delta_time_ >= kTimeInterval ? kTimeInterval : delta_time_;
#endif // _DEBUG

    scaled_delta_time_ = delta_time_ * time_scale_;

    if (delta_time_ >= kTimeInterval)
    {
        exec_last_time_ = current_time_;
        return true;
    }
    return false;
}

//--------------------------------------------------------------------------------
//  ���̎��Ԃ�Ԃ�
//--------------------------------------------------------------------------------
String GameTime::GetCurrentLocalTime(void) const
{
    SYSTEMTIME local_time = { 0 };
    GetLocalTime(&local_time);
    String& result = to_wstring(local_time.wHour) + L":"
        + to_wstring(local_time.wMinute) + L":"
        + to_wstring(local_time.wSecond);
    return result;
}
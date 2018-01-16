//--------------------------------------------------------------------------------
//�@time.cpp
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "time.h"
#include "common_setting.h"
#include "KF_Utility.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�[�ϐ��錾
//--------------------------------------------------------------------------------
Time* Time::instance_ = nullptr;

//--------------------------------------------------------------------------------
//  instance��������
//--------------------------------------------------------------------------------
Time* Time::Create(void)
{
    if (instance_) return instance_;
    instance_ = MY_NEW Time;
    return instance_;
}

//--------------------------------------------------------------------------------
//  �j������
//--------------------------------------------------------------------------------
void Time::Release(void)
{
    SAFE_DELETE(instance_);
}

//--------------------------------------------------------------------------------
//  �t���[�����s�o���邩���`�F�b�N
//--------------------------------------------------------------------------------
bool Time::CanUpdateFrame(void)
{
    QueryPerformanceFrequency(&frequency_);
    QueryPerformanceCounter(&current_time_);
    delta_time_ = static_cast<float>(current_time_.QuadPart - exec_last_time_.QuadPart)
         / static_cast<float>(frequency_.QuadPart);

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
String Time::GetCurrentLocalTime(void) const
{
    SYSTEMTIME local_time = { 0 };
    GetLocalTime(&local_time);
    String& result = to_wstring(local_time.wHour) + L":"
        + to_wstring(local_time.wMinute) + L":"
        + to_wstring(local_time.wSecond);
    return result;
}
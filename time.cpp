//--------------------------------------------------------------------------------
//　time.cpp
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "time.h"
#include "common_setting.h"
#include "KF_Utility.h"

//--------------------------------------------------------------------------------
//  静的メンバー変数宣言
//--------------------------------------------------------------------------------
Time* Time::instance_ = nullptr;

//--------------------------------------------------------------------------------
//  instance生成処理
//--------------------------------------------------------------------------------
Time* Time::Create(void)
{
    if (instance_) return instance_;
    instance_ = MY_NEW Time;
    return instance_;
}

//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void Time::Release(void)
{
    SAFE_DELETE(instance_);
}

//--------------------------------------------------------------------------------
//  フレーム実行出来るかをチェック
//--------------------------------------------------------------------------------
bool Time::CanUpdateFrame(void)
{
    QueryPerformanceFrequency(&frequency_);
    QueryPerformanceCounter(&current_time_);
    delta_time_ = static_cast<float>(current_time_.QuadPart - exec_last_time_.QuadPart)
         / static_cast<float>(frequency_.QuadPart);

#ifdef _DEBUG
    // break pointの時時間が飛んじゃうので制限する必要がある
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
//  今の時間を返す
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
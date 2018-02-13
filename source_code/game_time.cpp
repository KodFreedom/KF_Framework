//--------------------------------------------------------------------------------
//　time.cpp
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "game_time.h"
#include "common_setting.h"
#include "kf_utility.h"

//--------------------------------------------------------------------------------
//  静的メンバー変数宣言
//--------------------------------------------------------------------------------
GameTime* GameTime::instance_ = nullptr;

//--------------------------------------------------------------------------------
//  instance生成処理
//--------------------------------------------------------------------------------
GameTime* GameTime::Create(void)
{
    if (instance_) return instance_;
    instance_ = MY_NEW GameTime;
    return instance_;
}

//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void GameTime::Release(void)
{
    SAFE_DELETE(instance_);
}

//--------------------------------------------------------------------------------
//  フレーム実行出来るかをチェック
//--------------------------------------------------------------------------------
bool GameTime::CanUpdateFrame(void)
{
    QueryPerformanceFrequency(&frequency_);
    QueryPerformanceCounter(&current_time_);
    //delta_time_ = static_cast<float>(current_time_.QuadPart - exec_last_time_.QuadPart)
    //     / static_cast<float>(frequency_.QuadPart);
    delta_time_ = kTimeInterval;

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
String GameTime::GetCurrentLocalTime(void) const
{
    SYSTEMTIME local_time = { 0 };
    GetLocalTime(&local_time);
    String& result = to_wstring(local_time.wHour) + L":"
        + to_wstring(local_time.wMinute) + L":"
        + to_wstring(local_time.wSecond);
    return result;
}
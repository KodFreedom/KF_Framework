//--------------------------------------------------------------------------------
//　fade_system.h
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class Mode;
class Material;

//--------------------------------------------------------------------------------
//  フェイドシステムクラス
//--------------------------------------------------------------------------------
class FadeSystem
{
public:
    //--------------------------------------------------------------------------------
    //  生成処理
    //  return : FadeSystem*
    //--------------------------------------------------------------------------------
    static FadeSystem* Create(void);

    //--------------------------------------------------------------------------------
    //  破棄処理
    //--------------------------------------------------------------------------------
    void Release(void);
    
    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    void Update(void);

    //--------------------------------------------------------------------------------
    //  次のモードにフェードする
    //  next_mode：次のモード
    //  fade_time：フェードの時間（秒数）
    //--------------------------------------------------------------------------------
    void FadeTo(Mode* next_mode, const float fade_time = 1.0f);

private:
    //--------------------------------------------------------------------------------
    //  列挙型定義
    //--------------------------------------------------------------------------------
    enum FadeState
    {
        kFadeNone = 0,
        kFadeIn,
        kFadeOut
    };

    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    FadeSystem()
        : current_state_(kFadeIn), next_mode_(nullptr), material_(nullptr)
        , fade_time_(1.0f), time_counter_(1.0f) {}
    FadeSystem(const FadeSystem& value) {}
    FadeSystem& operator=(const FadeSystem& value) {}
    ~FadeSystem() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    void Init(void);

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    void Uninit(void);

    //--------------------------------------------------------------------------------
    //  フェイドイン処理
    //--------------------------------------------------------------------------------
    void FadeIn(void);

    //--------------------------------------------------------------------------------
    //  フェードアウト処理
    //--------------------------------------------------------------------------------
    void FadeOut(void);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    Material*   material_;
    FadeState   current_state_;
    Mode*       next_mode_;
    float       fade_time_;
    float       time_counter_;
};
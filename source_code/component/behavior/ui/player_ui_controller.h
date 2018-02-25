//--------------------------------------------------------------------------------
//  プレイヤーUIコントローラ
//  player_ui_controller.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "../behavior.h"
#include "../../../kf_utility/kf_math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class Transform;
class ActorParameter;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class PlayerUiController : public Behavior
{
public:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    PlayerUiController(GameObject& owner);
    ~PlayerUiController() {}

    //--------------------------------------------------------------------------------
    //  初期化処理
    //--------------------------------------------------------------------------------
    bool Init(void) override;

    //--------------------------------------------------------------------------------
    //  終了処理
    //--------------------------------------------------------------------------------
    void Uninit(void) override;

    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    void Update(void) override;

    //--------------------------------------------------------------------------------
    //  後更新処理
    //--------------------------------------------------------------------------------
    void LateUpdate(void) override;

private:
    //--------------------------------------------------------------------------------
    //  列挙型定義
    //--------------------------------------------------------------------------------
    enum ButtonType
    {
        kKeyboardButton = 0,
        kJoystickButton,
        kButtonTypeMax
    };

    //--------------------------------------------------------------------------------
    //  定数定義
    //--------------------------------------------------------------------------------
    static const Vector3 kLifeGaugeSize;
    static const Vector3 kLifeGaugeLeftTop;
    static const Vector3 kCoverSize;
    static const Vector3 kCoverLeftTop;
    static const Vector3 kButtonSize[kButtonTypeMax];
    static const Vector3 kButtonJumpLeftTop[kButtonTypeMax];
    static const Vector2 kButtonJumpUvOffset[kButtonTypeMax];
    static const Vector2 kButtonJumpUvScale[kButtonTypeMax];
    static const Vector3 kButtonLightAttackLeftTop[kButtonTypeMax];
    static const Vector2 kButtonLightAttackUvOffset[kButtonTypeMax];
    static const Vector2 kButtonLightAttackUvScale[kButtonTypeMax];
    static const Vector3 kButtonStrongAttackLeftTop[kButtonTypeMax];
    static const Vector2 kButtonStrongAttackUvOffset[kButtonTypeMax];
    static const Vector2 kButtonStrongAttackUvScale[kButtonTypeMax];
    static const Vector3 kButtonGuardLeftTop[kButtonTypeMax];
    static const Vector2 kButtonGuardUvOffset[kButtonTypeMax];
    static const Vector2 kButtonGuardUvScale[kButtonTypeMax];
    static const Vector3 kButtonTextSize;
    static const Vector3 kButtonTextJumpLeftTop;
    static const Vector3 kButtonTextLightAttackLeftTop;
    static const Vector3 kButtonTextStrongAttackLeftTop;
    static const Vector3 kButtonTextGuardLeftTop;
    static constexpr int kButtonNumber = 4;

    //--------------------------------------------------------------------------------
    //  ワーニングゲージの更新
    //--------------------------------------------------------------------------------
    void UpdateWarningGauge(const ActorParameter& parameter);

    //--------------------------------------------------------------------------------
    //  ライフゲージの更新
    //--------------------------------------------------------------------------------
    void UpdateLifeGauge(const ActorParameter& parameter);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    Transform*      warning_gauge_ = nullptr;
    Transform*      life_gauge_ = nullptr;
    float           warning_flash_speed_ = 2.0f;
    float           life_flash_speed_ = 0.4f;
};
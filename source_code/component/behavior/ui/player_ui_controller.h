//--------------------------------------------------------------------------------
//  �v���C���[UI�R���g���[��
//  player_ui_controller.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "../behavior.h"
#include "../../../kf_utility/kf_math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class Transform;
class ActorParameter;

//--------------------------------------------------------------------------------
//  �N���X�錾
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
    //  ����������
    //--------------------------------------------------------------------------------
    bool Init(void) override;

    //--------------------------------------------------------------------------------
    //  �I������
    //--------------------------------------------------------------------------------
    void Uninit(void) override;

    //--------------------------------------------------------------------------------
    //  �X�V����
    //--------------------------------------------------------------------------------
    void Update(void) override;

    //--------------------------------------------------------------------------------
    //  ��X�V����
    //--------------------------------------------------------------------------------
    void LateUpdate(void) override;

private:
    //--------------------------------------------------------------------------------
    //  �񋓌^��`
    //--------------------------------------------------------------------------------
    enum ButtonType
    {
        kKeyboardButton = 0,
        kJoystickButton,
        kButtonTypeMax
    };

    //--------------------------------------------------------------------------------
    //  �萔��`
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
    //  ���[�j���O�Q�[�W�̍X�V
    //--------------------------------------------------------------------------------
    void UpdateWarningGauge(const ActorParameter& parameter);

    //--------------------------------------------------------------------------------
    //  ���C�t�Q�[�W�̍X�V
    //--------------------------------------------------------------------------------
    void UpdateLifeGauge(const ActorParameter& parameter);

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    Transform*      warning_gauge_ = nullptr;
    Transform*      life_gauge_ = nullptr;
    float           warning_flash_speed_ = 2.0f;
    float           life_flash_speed_ = 0.4f;
};
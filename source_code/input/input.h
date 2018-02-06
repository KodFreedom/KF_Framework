//--------------------------------------------------------------------------------
//�@input.h
//  ���̓f�B�o�C�X���Ǘ�
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include "common_setting.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class KeyboardDirectX;
class MouseDirectX;
class JoystickDirectX;

//--------------------------------------------------------------------------------
//  �񋓌^��`
//--------------------------------------------------------------------------------
enum Key
{
    kSubmit = 0,
    kCancel,
    kStart,
    kUp,
    kDown,
    kLeft,
    kRight,
    kReset,
    kLock,
    kStrongAttack,
    kSkill,
    kBlock,
    kKeyMax,

    //�d�˂�L�[
    kJump = kSubmit,
    kLightAttack = kCancel,
    kSave = kStart,
    kExtend = kRight,
    kShrink = kLeft,
    kRaise = kUp,
    kReduce = kDown,
};

//--------------------------------------------------------------------------------
//  ���͏��N���X
//--------------------------------------------------------------------------------
class Input
{
public:
    //--------------------------------------------------------------------------------
    //  ��������
    //  return : Input*
    //--------------------------------------------------------------------------------
    static Input* Create(HINSTANCE hinstance, HWND hwnd)
    {
        auto instance = MY_NEW Input;
        instance->Init(hinstance, hwnd);
        return instance;
    }

    //--------------------------------------------------------------------------------
    //  �j������
    //--------------------------------------------------------------------------------
    void Release(void) 
    {
        Uninit();
        MY_DELETE this;
    }

    //--------------------------------------------------------------------------------
    //  �X�V����
    //--------------------------------------------------------------------------------
    void Update(void);

    //--------------------------------------------------------------------------------
    //  �A�N�Z�X���̎擾
    //--------------------------------------------------------------------------------
    void Acquire(void);

    //--------------------------------------------------------------------------------
    //  �A�N�Z�X���̕���
    //--------------------------------------------------------------------------------
    void Unacquire(void);

    //--------------------------------------------------------------------------------
    //  �ړ��̐����ʂ̎擾
    //--------------------------------------------------------------------------------
    float MoveHorizontal(void) const { return move_horizontal_; }

    //--------------------------------------------------------------------------------
    //  �ړ��̐����ʂ̎擾
    //--------------------------------------------------------------------------------
    float MoveVertical(void) const { return move_vertical_; }

    //--------------------------------------------------------------------------------
    //  ��]�̐����ʂ̎擾
    //--------------------------------------------------------------------------------
    float RotationHorizontal(void) const { return rotation_horizontal_; }

    //--------------------------------------------------------------------------------
    //  ��]�̐����ʂ̎擾
    //--------------------------------------------------------------------------------
    float RotationVertical(void) const { return rotation_vertical_; }

    //--------------------------------------------------------------------------------
    //  �Y�[���ʂ̎擾
    //--------------------------------------------------------------------------------
    float Zoom(void) const { return zoom_; }

    //--------------------------------------------------------------------------------
    //  �v���X�̎擾
    //--------------------------------------------------------------------------------
    bool GetKeyPress(const Key& key) const { return press_state_ & (1 << static_cast<int>(key)); }

    //--------------------------------------------------------------------------------
    //  �g���K�[�̎擾
    //--------------------------------------------------------------------------------
    bool GetKeyTrigger(const Key& key) const { return trigger_state_ & (1 << static_cast<int>(key)); }

    //--------------------------------------------------------------------------------
    //  �����[�X�̎擾
    //--------------------------------------------------------------------------------
    bool GetKeyRelease(const Key& key) const { return release_state_ & (1 << static_cast<int>(key)); }

    //--------------------------------------------------------------------------------
    //  �G�f�B�^���[�h�̐ݒ�
    //--------------------------------------------------------------------------------
    void SetEditorMode(const bool& enable) { is_editor_mode_ = enable; }

    //--------------------------------------------------------------------------------
    //  �L�[�{�[�h�̎擾
    //--------------------------------------------------------------------------------
    const auto GetKeyboard(void) const { return keyboard_; }

    //--------------------------------------------------------------------------------
    //  �}�E�X�̎擾
    //--------------------------------------------------------------------------------
    const auto GetMouse(void) const { return mouse_; }

    //--------------------------------------------------------------------------------
    //  �W���C�X�e�B�b�N�̎擾
    //--------------------------------------------------------------------------------
    const auto GetJoystick(void) const { return joystick_; }

private:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    Input();
    Input(const Input& value) {}
    Input& operator=(const Input& value) {}
    ~Input() {}

    //--------------------------------------------------------------------------------
    //  ����������
    //--------------------------------------------------------------------------------
    bool Init(HINSTANCE hinstance, HWND hwnd);

    //--------------------------------------------------------------------------------
    //  �I������
    //--------------------------------------------------------------------------------
    void Uninit(void);

    //--------------------------------------------------------------------------------
    //  ���͏��X�V�X�V
    //--------------------------------------------------------------------------------
    void UpdateInputInfo(void);

    //--------------------------------------------------------------------------------
    //  �ϐ���`
    //--------------------------------------------------------------------------------
    KeyboardDirectX* keyboard_; // �L�[�{�[�h
    MouseDirectX*    mouse_; // �}�E�X
    JoystickDirectX* joystick_; // �W���C�X�e�B�b�N
    float            move_horizontal_; // �ړ��̐�����
    float            move_vertical_; // �ړ��̐�����
    float            rotation_horizontal_; // ��]�̐�����
    float            rotation_vertical_; // ��]�̐�����
    float            zoom_; // �Y�[���̗�
    LONG             press_state_; // �v���X�̏��
    LONG             trigger_state_; // �g���K�[�̏��
    LONG             release_state_; // �����[�X�̏��
    bool             is_editor_mode_; // �G�f�B�^���[�V�����t���O
};
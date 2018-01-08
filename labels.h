//--------------------------------------------------------------------------------
//  �\�������ۑ�����Ƃ���
//�@labels.h
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �񋓌^��`
//--------------------------------------------------------------------------------
enum Language
{
    kEnglish = 0,
    kJapanese = 1,
    kLanguageMax
};

//--------------------------------------------------------------------------------
//  Labels
//--------------------------------------------------------------------------------
static const char* kSelectLanguage[kLanguageMax] =
{
    "Select language",
    u8"����I��"
};

static const char* kLanguage[kLanguageMax] =
{
    "English",
    u8"���{��"
};

static const char* kStageName[kLanguageMax] =
{
    "Stage name",
    u8"�X�e�[�W���O"
};

static const char* kOpenModelEditor[kLanguageMax] =
{
    "Open model editor",
    u8"���f���G�f�B�^���J��"
};

static const char* kCloseModelEditor[kLanguageMax] =
{
    "Close model editor",
    u8"���f���G�f�B�^�����"
};

static const char* kOpenFieldEditor[kLanguageMax] =
{
    "Open field editor",
    u8"�t�B�[���h�G�f�B�^���J��"
};

static const char* kCloseFieldEditor[kLanguageMax] =
{
    "Close field editor",
    u8"�t�B�[���h�G�f�B�^�����"
};

static const char* kTargetPosition[kLanguageMax] =
{
    "Target position",
    u8"�W�I�ʒu"
};

static const char* kAutoAdjustHeight[kLanguageMax] =
{
    "Auto adjust height",
    u8"�����̎�������"
};

static const char* kExplainMove[kLanguageMax] =
{
    "Move : W A S D",
    u8"�ړ� : W A S D"
};

static const char* kExplainRaiseReduce[kLanguageMax] =
{
    "Raise/Reduce : Up/Down",
    u8"�㏸/�~�� : �����/������"
};

static const char* kExplainCameraRotation[kLanguageMax] =
{
    "Camera rotation : Right click + Mouse move",
    u8"�J������] : �E�N���b�N + �}�E�X�ړ�"
};

static const char* kExplainCameraZoom[kLanguageMax] =
{
    "Camera zoom : Right click + Mouse wheel",
    u8"�J�����Y�[�� : �E�N���b�N + �}�E�X�E�B�[��"
};

static const char* kMoveRaiseSpeed[kLanguageMax] =
{
    "Move/Raise speed",
    u8"�ړ�/�N�����x"
};

static const char* kSaveStage[kLanguageMax] =
{
    "Save as stage name",
    u8"�X�e�[�W���𖼑O�Ƃ��ĕۑ�"
};

static const char* kLoadStage[kLanguageMax] =
{
    "Load from stage name",
    u8"�X�e�[�W���𖼑O�Ƃ��ēǍ�"
};

static const char* kRaiseModeRate[kLanguageMax] =
{
    "Raise mode rate of average and linear",
    u8"�N����ԃ��[�h���ςƐ��`�̔䗦"
};

static const char* kFieldRadius[kLanguageMax] =
{
    "Choose radius",
    u8"�I�𔼌a"
};

static const char* kFieldRange[kLanguageMax] =
{
    "Choose range",
    u8"�I��͈�"
};

static const char* kFieldRaiseSpeed[kLanguageMax] =
{
    "Raise speed",
    u8"�N�����x"
};

static const char* kFieldExtendSpeed[kLanguageMax] =
{
    "Extend speed",
    u8"�g�k���x"
};

static const char* kFieldChooseMode[kLanguageMax] =
{
    "Choose mode",
    u8"�I�����[�h"
};

static const char* kFieldChooseModeLabels[kLanguageMax][2] =
{
    {
        "Circle",
        "Square"
    },
    {
        u8"�~�`",
        u8"�����`"
    }
};

static const char* kBlockSize[kLanguageMax] =
{
    "Block size",
    u8"�u���b�N�T�C�Y"
};

static const char* kResetHeight[kLanguageMax] =
{
    "Set height to 0 in range",
    u8"�͈͓��̍�����0�ɂ���"
};

static const char* kPosition[kLanguageMax] =
{
    "Position",
    u8"�ʒu"
};

static const char* kRotation[kLanguageMax] =
{
    "Rotation",
    u8"��]"
};

static const char* kScale[kLanguageMax] =
{
    "Scale",
    u8"�X�P�[��"
};

static const char* kCreate[kLanguageMax] =
{
    "Create",
    u8"����"
};

static const char* kShowCreatedList[kLanguageMax] =
{
    "Show created list",
    u8"�������X�g�̕\��"
};

static const char* kModel[kLanguageMax] =
{
    "Model",
    u8"���f��"
};

static const char* kDelete[kLanguageMax] =
{
    "Delete",
    u8"�폜"
};

static const char* kSetPlayer[kLanguageMax] =
{
    "Set player to here",
    u8"�v���C���[�������ɐݒu����"
};
//--------------------------------------------------------------------------------
//  表示文字保存するところ
//　labels.h
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  列挙型定義
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
    u8"言語選択"
};

static const char* kLanguage[kLanguageMax] =
{
    "English",
    u8"日本語"
};

static const char* kStageName[kLanguageMax] =
{
    "Stage name",
    u8"ステージ名前"
};

static const char* kOpenModelEditor[kLanguageMax] =
{
    "Open model editor",
    u8"モデルエディタを開く"
};

static const char* kCloseModelEditor[kLanguageMax] =
{
    "Close model editor",
    u8"モデルエディタを閉じる"
};

static const char* kOpenFieldEditor[kLanguageMax] =
{
    "Open field editor",
    u8"フィールドエディタを開く"
};

static const char* kCloseFieldEditor[kLanguageMax] =
{
    "Close field editor",
    u8"フィールドエディタを閉じる"
};

static const char* kTargetPosition[kLanguageMax] =
{
    "Target position",
    u8"標的位置"
};

static const char* kAutoAdjustHeight[kLanguageMax] =
{
    "Auto adjust height",
    u8"高さの自動調節"
};

static const char* kExplainMove[kLanguageMax] =
{
    "Move : W A S D",
    u8"移動 : W A S D"
};

static const char* kExplainRaiseReduce[kLanguageMax] =
{
    "Raise/Reduce : Up/Down",
    u8"上昇/降下 : 上方向/下方向"
};

static const char* kExplainCameraRotation[kLanguageMax] =
{
    "Camera rotation : Right click + Mouse move",
    u8"カメラ回転 : 右クリック + マウス移動"
};

static const char* kExplainCameraZoom[kLanguageMax] =
{
    "Camera zoom : Right click + Mouse wheel",
    u8"カメラズーム : 右クリック + マウスウィール"
};

static const char* kMoveRaiseSpeed[kLanguageMax] =
{
    "Move/Raise speed",
    u8"移動/起伏速度"
};

static const char* kSaveStage[kLanguageMax] =
{
    "Save as stage name",
    u8"ステージ名を名前として保存"
};

static const char* kLoadStage[kLanguageMax] =
{
    "Load from stage name",
    u8"ステージ名を名前として読込"
};

static const char* kRaiseModeRate[kLanguageMax] =
{
    "Raise mode rate of average and linear",
    u8"起伏補間モード平均と線形の比率"
};

static const char* kFieldRadius[kLanguageMax] =
{
    "Choose radius",
    u8"選択半径"
};

static const char* kFieldRange[kLanguageMax] =
{
    "Choose range",
    u8"選択範囲"
};

static const char* kFieldRaiseSpeed[kLanguageMax] =
{
    "Raise speed",
    u8"起伏速度"
};

static const char* kFieldExtendSpeed[kLanguageMax] =
{
    "Extend speed",
    u8"拡縮速度"
};

static const char* kFieldChooseMode[kLanguageMax] =
{
    "Choose mode",
    u8"選択モード"
};

static const char* kFieldChooseModeLabels[kLanguageMax][2] =
{
    {
        "Circle",
        "Square"
    },
    {
        u8"円形",
        u8"長方形"
    }
};

static const char* kBlockSize[kLanguageMax] =
{
    "Block size",
    u8"ブロックサイズ"
};

static const char* kResetHeight[kLanguageMax] =
{
    "Set height to 0 in range",
    u8"範囲内の高さを0にする"
};

static const char* kPosition[kLanguageMax] =
{
    "Position",
    u8"位置"
};

static const char* kRotation[kLanguageMax] =
{
    "Rotation",
    u8"回転"
};

static const char* kScale[kLanguageMax] =
{
    "Scale",
    u8"スケール"
};

static const char* kCreate[kLanguageMax] =
{
    "Create",
    u8"生成"
};

static const char* kShowCreatedList[kLanguageMax] =
{
    "Show created list",
    u8"生成リストの表示"
};

static const char* kModel[kLanguageMax] =
{
    "Model",
    u8"モデル"
};

static const char* kDelete[kLanguageMax] =
{
    "Delete",
    u8"削除"
};

static const char* kSetPlayer[kLanguageMax] =
{
    "Set player to here",
    u8"プレイヤーをここに設置する"
};
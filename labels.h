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
    "Raise/Reduce : <- / ->",
    u8"上昇/降下 : <- / ->"
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
    "Save",
    u8"保存"
};

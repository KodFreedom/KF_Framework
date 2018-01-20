//--------------------------------------------------------------------------------
//  エディタビヘイビアコンポネント
//　EditorController.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#if defined(EDITOR)
#include "behavior.h"
#include "labels.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class FieldEditor;
class ModelEditor;
class EnemyEditor;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class EditorController : public Behavior
{
public:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    EditorController(GameObject& owner);
    ~EditorController() {}

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
    void LateUpdate(void) override {}
         
    //--------------------------------------------------------------------------------
    //  フィールドエディタの設定
    //--------------------------------------------------------------------------------
    void SetFieldEditor(FieldEditor* value) { field_editor_ = value; }

    //--------------------------------------------------------------------------------
    //  モデルエディタの設定
    //--------------------------------------------------------------------------------
    void SetModelEditor(ModelEditor* value) { model_editor_ = value; }

    //--------------------------------------------------------------------------------
    //  エネミーエディタの設定
    //--------------------------------------------------------------------------------
    void SetEnemyEditor(EnemyEditor* value) { enemy_editor_ = value; }

private:
    //--------------------------------------------------------------------------------
    //  保存処理
    //--------------------------------------------------------------------------------
    void Save(void);

    //--------------------------------------------------------------------------------
    //  読込処理
    //--------------------------------------------------------------------------------
    void Load(void);

    //--------------------------------------------------------------------------------
    //  メインウィンドウの表示
    //--------------------------------------------------------------------------------
    void ShowMainWindow(void);

    //--------------------------------------------------------------------------------
    //  モードウィンドウの表示
    //--------------------------------------------------------------------------------
    void ShowModeWindow(void);

    //--------------------------------------------------------------------------------
    //  位置ウィンドウの表示
    //--------------------------------------------------------------------------------
    void ShowPositonWindow(void);

    //--------------------------------------------------------------------------------
    //  フィールド情報を保存する関数
    //--------------------------------------------------------------------------------
    void SaveAsBinary(const String& name);

    //--------------------------------------------------------------------------------
    //  フィールド情報を読込関数
    //--------------------------------------------------------------------------------
    void LoadFrom(const String& name);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    bool         enable_auto_adjust_height_;
    FieldEditor* field_editor_;
    ModelEditor* model_editor_;
    EnemyEditor* enemy_editor_;
    GameObject*  player_;
    float        move_speed_;
    string       stage_name_;
    Language     current_language_;
};
#endif // _DEBUG
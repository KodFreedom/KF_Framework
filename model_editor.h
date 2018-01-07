//--------------------------------------------------------------------------------
//    モデルエディタビヘイビアコンポネント
//　ModelEditor.h
//    Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#if defined(EDITOR)
#include "behavior.h"
#include "kf_math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class Transform;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class ModelEditor : public Behavior
{
public:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    ModelEditor(GameObject& owner);
    ~ModelEditor() {}

    //--------------------------------------------------------------------------------
    //  初期化
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

    //--------------------------------------------------------------------------------
    //  アクティブフラグの取得
    //--------------------------------------------------------------------------------
    const bool& IsActive(void) const { return is_active_; }

    //--------------------------------------------------------------------------------
    //  アクティブフラグの設定
    //--------------------------------------------------------------------------------
    void SetActive(const bool& value) { is_active_ = value; }
    
    //--------------------------------------------------------------------------------
    //  位置の設定
    //--------------------------------------------------------------------------------
    void SetPosition(const Vector3& position);

    //--------------------------------------------------------------------------------
    //  ファイルに保存
    //--------------------------------------------------------------------------------
    void SaveAsBinary(const String& name);

    //--------------------------------------------------------------------------------
    //  情報を読込関数
    //--------------------------------------------------------------------------------
    void LoadFrom(const String& name);

private:
    //--------------------------------------------------------------------------------
    //  構造体定義
    //--------------------------------------------------------------------------------
    struct Info
    {
        Info() : my_transform(nullptr), rotation(Vector3::kZero) {}
        Transform* my_transform;
        Vector3    rotation;
    };

    //--------------------------------------------------------------------------------
    //  関数定義
    //--------------------------------------------------------------------------------
    void Create(void);
    void ShowMainWindow(void);
    void ShowTypeListBox(void);
    void ShowCreatedList(void);
    void ClearList(void);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    vector<String>     model_names_;
    vector<Info>       demo_model_infos_;
    vector<list<Info>> created_model_infos_;
    int                current_model_no_;
    bool               is_active_;
    bool               is_show_created_list_;
};
#endif // EDITOR
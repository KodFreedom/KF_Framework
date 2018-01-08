//--------------------------------------------------------------------------------
//    プレイヤービヘイビアコンポネント
//　FieldEditor.h
//    Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#if defined(EDITOR)
#include "behavior.h"
#include "render_system.h"
#include "kf_math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class FieldEditor : public Behavior
{
public:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    FieldEditor(GameObject& owner);
    ~FieldEditor() {}

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
    void LateUpdate(void) override {}

    //--------------------------------------------------------------------------------
    //  フィールドによって位置を調節する
    //--------------------------------------------------------------------------------
    Vector3 AdjustPositionInField(const Vector3& position, const bool& is_adjust_height);
    
    //--------------------------------------------------------------------------------
    //  アクティブフラグの取得
    //--------------------------------------------------------------------------------
    bool IsActive(void) const { return is_active_; }

    //--------------------------------------------------------------------------------
    //  アクティブフラグの設定
    //--------------------------------------------------------------------------------
    void SetActive(const bool& value);

    //--------------------------------------------------------------------------------
    //  位置の設定
    //--------------------------------------------------------------------------------
    void SetPosition(const Vector3& value) { editor_position_ = value; }

    //--------------------------------------------------------------------------------
    //  フィールド情報を保存する関数
    //--------------------------------------------------------------------------------
    void SaveAsBinary(const String& name);

    //--------------------------------------------------------------------------------
    //  フィールド情報を読込関数
    //--------------------------------------------------------------------------------
    void LoadFrom(const String& name);

private:
    //--------------------------------------------------------------------------------
    //  列挙型定義
    //--------------------------------------------------------------------------------
    enum ChooseMode
    {
        kCircle = 0,
        kSquare,
        kChooseModeMax
    };

    //--------------------------------------------------------------------------------
    //  フィールドの高さを取得
    //--------------------------------------------------------------------------------
    float GetHeight(const Vector3& position);

    //--------------------------------------------------------------------------------
    //  範囲内のインデックスを取得
    //--------------------------------------------------------------------------------
    list<int> GetChoosenIndexes(void);

    //--------------------------------------------------------------------------------
    //  メインウィンドウの表示
    //--------------------------------------------------------------------------------
    void ShowMainWindow(void);

    //--------------------------------------------------------------------------------
    //  頂点の更新
    //--------------------------------------------------------------------------------
    void UpdateVertexesBy(const float& raise_amount, const list<int>& choosen_indexes);

    //--------------------------------------------------------------------------------
    //  頂点の初期化
    //--------------------------------------------------------------------------------
    void InitVertexes(void);

    //--------------------------------------------------------------------------------
    //  初期化インデックスの取得
    //--------------------------------------------------------------------------------
    vector<int> GetInitMeshIndexes(void);

    //--------------------------------------------------------------------------------
    //  頂点の再計算
    //--------------------------------------------------------------------------------
    void RecalculateVertexes(void);

    //--------------------------------------------------------------------------------
    //  法線の計算
    //--------------------------------------------------------------------------------
    void RecalculateNormal(const list<int>& indexes);

    //--------------------------------------------------------------------------------
    //  全部のインデックスを取得
    //--------------------------------------------------------------------------------
    list<int> GetAllIndexes(void);

    //--------------------------------------------------------------------------------
    //  頂点を一定高さにセットする
    //--------------------------------------------------------------------------------
    void SetHeightTo(const float& height, const list<int>& indexes);

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    int              block_number_x_;
    int              block_number_z_;
    Vector2          block_size_;
    Vector3          min_position_;
    Vector3          max_position_;
    Vector3          editor_position_;
    Vector2          choose_range_;
    float            raise_speed_;
    float            extend_speed_;
    float            raise_mode_rate_; // average / linearの比率
    bool             is_active_;
    vector<Vertex3d> vertexes_;
    list<int>        previous_choosen_indexes_;
    ChooseMode       current_choose_mode_;
};
#endif // _DEBUG
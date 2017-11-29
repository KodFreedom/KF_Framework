//--------------------------------------------------------------------------------
//	エディタビヘイビアコンポネント
//　EditorController.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#if defined(EDITOR)
#include "behavior.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class FieldEditor;
class ModelEditor;

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
	void SetFieldEditor(GameObject* object);

	//--------------------------------------------------------------------------------
	//  モデルエディタの設定
	//--------------------------------------------------------------------------------
	void SetModelEditor(ModelEditor* value) { model_editor_ = value; }

private:
	//--------------------------------------------------------------------------------
	//  保存処理
	//--------------------------------------------------------------------------------
	void Save(void);

	//--------------------------------------------------------------------------------
	//  メインウィンドウの表示
	//--------------------------------------------------------------------------------
	void ShowMainWindow(void);

	//--------------------------------------------------------------------------------
	//  位置ウィンドウの表示
	//--------------------------------------------------------------------------------
	void ShowPositonWindow(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	bool         is_auto_height_;
	FieldEditor* field_editor_;
	ModelEditor* model_editor_;
	float        move_speed_;
};
#endif // _DEBUG
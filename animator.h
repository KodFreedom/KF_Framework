//--------------------------------------------------------------------------------
//
//　animator.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"
#include "component.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class MotionState;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class Animator : public Component
{
public:
	//--------------------------------------------------------------------------------
	//  constructors for singleton
	//--------------------------------------------------------------------------------
	Animator(GameObject& owner);
	~Animator() {}

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
	void Update(void);

	//--------------------------------------------------------------------------------
	//  今のアニメーションの名前の取得
	//--------------------------------------------------------------------------------
	const String& GetCurrentAnimationName(void);

	//--------------------------------------------------------------------------------
	//  アバターの設定
	//--------------------------------------------------------------------------------
	void SetAvatar(const String& file_name);

	//--------------------------------------------------------------------------------
	//  攻撃フラグの設定
	//--------------------------------------------------------------------------------
	void SetAttack(const bool& value) { is_attack_ = value; }

	//--------------------------------------------------------------------------------
	//  着地フラグの設定
	//--------------------------------------------------------------------------------
	void SetGrounded(const bool& value) { is_grounded_ = value; }

	//--------------------------------------------------------------------------------
	//  ジャンプフラグの設定
	//--------------------------------------------------------------------------------
	void SetJump(const bool& value) { is_jump_ = value; }

	//--------------------------------------------------------------------------------
	//  ダメージ受けたフラグの設定
	//--------------------------------------------------------------------------------
	void SetDamaged(const bool& value) { is_damaged_ = value; }

	//--------------------------------------------------------------------------------
	//  移動量の設定
	//--------------------------------------------------------------------------------
	void SetMovement(const float& value) { movement_ = value; }

	//--------------------------------------------------------------------------------
	//  モーションステートの変更
	//--------------------------------------------------------------------------------
	void Change(MotionState* new_motion_state);

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct Bone
	{
		Transform*  transform;
		String      name;
		Matrix44    bind_pose_inverse;
	};

	//--------------------------------------------------------------------------------
	//  ファイルから読み込む
	//--------------------------------------------------------------------------------
	void LoadFromFile(const String& file_name);

	//--------------------------------------------------------------------------------
	//  ボーンと子供を関連付ける
	//--------------------------------------------------------------------------------
	void AttachBonesToChildren(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	vector<Bone>   avatar_;
	vector<String> motion_names_;
	MotionState*   state_;
	bool           is_grounded_;
	bool           is_attack_;
	bool           is_jump_;
	bool           is_damaged_;
	float          movement_;
};

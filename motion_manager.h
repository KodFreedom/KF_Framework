//--------------------------------------------------------------------------------
//　motion_manager.h
//  manage the motiondatas' save, load
//	モーション管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class MotionData;

//--------------------------------------------------------------------------------
//  モーションマネージャクラス
//--------------------------------------------------------------------------------
class MotionManager
{
public:
	//--------------------------------------------------------------------------------
	//  生成処理
	//  return : MaterialManager*
	//--------------------------------------------------------------------------------
	static MotionManager* Create(void)
	{
		auto instance = MY_NEW MotionManager();
		return instance;
	}

	//--------------------------------------------------------------------------------
	//  破棄処理
	//--------------------------------------------------------------------------------
	void Release(void) { Clear(); }

	//--------------------------------------------------------------------------------
	//  与えられた名前のモーションを使う
	//  すでにあるの場合ユーザーを1たす、ないの場合ファイルから読み込む
	//  motion_name : モーション名
	//--------------------------------------------------------------------------------
	void Use(const String& motion_name);

	//--------------------------------------------------------------------------------
	//  与えられた名前のモーションを使わない
	//  ユーザーが-1になる、0になった場合メモリから破棄する
	//  motion_name : モーション名
	//--------------------------------------------------------------------------------
	void Disuse(const String& motion_name);

	//--------------------------------------------------------------------------------
	//  モーションデータを取得
	//--------------------------------------------------------------------------------
	MotionData* GetMotionDataBy(const String& motion_name)
	{ 
		auto iterator = motions.find(motion_name);
		if (motions.end() == iterator) return nullptr;
		return iterator->second.pointer;
	}

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct MotionInfo
	{
		MotionInfo() : user_number(1), pointer(nullptr) {}
		int			user_number;
		MotionData*	pointer;
	};

	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	MotionManager() {}
	MotionManager(const MotionManager& value) {}
	MotionManager& operator=(const MotionManager& value) {}
	~MotionManager() {}

	//--------------------------------------------------------------------------------
	//  クリア処理
	//--------------------------------------------------------------------------------
	void Clear(void);

	//--------------------------------------------------------------------------------
	//  ファイルから読み込む
	//--------------------------------------------------------------------------------
	MotionData* LoadFromFile(const String& motion_name);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	unordered_map<String, MotionInfo> motions;
};
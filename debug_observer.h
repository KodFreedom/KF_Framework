//--------------------------------------------------------------------------------
//　debug_observer.h
//	debug観察者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#ifdef _DEBUG
#include "common_setting.h"
#include "labels.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class GameObjectActor;

//--------------------------------------------------------------------------------
//  クラス定義
//--------------------------------------------------------------------------------
class DebugObserver
{
public:
	//--------------------------------------------------------------------------------
	//  生成処理
	//  return : MaterialManager*
	//--------------------------------------------------------------------------------
	static DebugObserver* Create(void)
	{
		auto instance = MY_NEW DebugObserver();
		return instance;
	}

	//--------------------------------------------------------------------------------
	//  破棄処理
	//--------------------------------------------------------------------------------
	void Release(void) 
    {
        Clear();
        MY_DELETE this;
    }

	//--------------------------------------------------------------------------------
	//  更新処理
	//--------------------------------------------------------------------------------
	void Update(void);

	//--------------------------------------------------------------------------------
	//  後更新処理
	//--------------------------------------------------------------------------------
	void LateUpdate(void);

	//--------------------------------------------------------------------------------
	//  描画処理
	//--------------------------------------------------------------------------------
	void Render(void);

	//--------------------------------------------------------------------------------
	//  DebugLogに表示したい文字列を追加する
	//	log : 表示したい文字列
	//--------------------------------------------------------------------------------
	void Display(const String& log);

	//--------------------------------------------------------------------------------
	//  プレイヤーの設定
	//	value : プレイヤーのオブジェクト
	//--------------------------------------------------------------------------------
	void SetPlayer(GameObjectActor* value) { player_ = value; }

	//--------------------------------------------------------------------------------
	//  DebugLogに表示したい文字列を追加する
	//	log : 表示したい文字列
	//--------------------------------------------------------------------------------
	void Clear(void)
	{
		player_ = nullptr;
		log_.clear();
	}

    //--------------------------------------------------------------------------------
    //  今の言語を取得する
    //--------------------------------------------------------------------------------
    const Language& GetCurrentLanguage(void) const { return current_language_; }

private:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	DebugObserver() : enable_collision_system_window_(false)
		, enable_camera_window_(false), enable_player_window_(false)
		, enable_fog_window_(false), enable_shadow_map_system_window_(false)
        , enable_log_window_(false), player_(nullptr)
        , current_language_(kEnglish) {}
	DebugObserver(const DebugObserver& value) {}
	DebugObserver& operator=(const DebugObserver& value) {}
	~DebugObserver() {}

	//--------------------------------------------------------------------------------
	//  DebugMainMenuの表示処理
	//--------------------------------------------------------------------------------
	void ShowMainWindow(void);

	//--------------------------------------------------------------------------------
	//  Collision情報の表示処理
	//--------------------------------------------------------------------------------
	void ShowCollisionSystemWindow(void);

	//--------------------------------------------------------------------------------
	//  Camera情報の表示処理
	//--------------------------------------------------------------------------------
	void ShowCameraWindow(void);

	//--------------------------------------------------------------------------------
	//  Player情報の表示処理
	//--------------------------------------------------------------------------------
	void ShowPlayerWindow(void);

	//--------------------------------------------------------------------------------
	//  Fog情報の表示処理
	//--------------------------------------------------------------------------------
	void ShowFogWindow(void);

    //--------------------------------------------------------------------------------
    //  ShadowMap情報の表示処理
    //--------------------------------------------------------------------------------
    void ShowShadowMapWindow(void);

    //--------------------------------------------------------------------------------
    //  Log情報の表示処理
    //--------------------------------------------------------------------------------
    void ShowLogWindow(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	String log_;
	bool enable_collision_system_window_;
	bool enable_camera_window_;
	bool enable_player_window_;
	bool enable_fog_window_;
    bool enable_shadow_map_system_window_;
    bool enable_log_window_;
	GameObjectActor* player_;
    Language current_language_;
};
#endif
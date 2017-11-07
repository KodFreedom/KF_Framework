//--------------------------------------------------------------------------------
//	debug観察者
//　debugObserver.h
//	Author : Xu Wenjie
//	Date   : 2017-09-07
//--------------------------------------------------------------------------------
#pragma once

#ifdef _DEBUG
#include "main.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class ActorController;

//--------------------------------------------------------------------------------
//  クラス定義
//--------------------------------------------------------------------------------
class DebugObserver
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	static DebugObserver* Create(HWND hWnd);
	static void			  Release(void);
	static auto			  Instance(void) { return instance; }

	void Update(void);
	void LateUpdate(void);
	void Draw(void);
	void DisplayAlways(const string& strInfo);
	void DisplayAlways(const char& cInfo);
	void DisplayScroll(const string& strInfo);
	void SetPlayer(ActorController* actor) { player = actor; }

private:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static const unsigned short scrollTime = 1 * 30;

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	DebugObserver();
	~DebugObserver() {}
	void init(HWND hWnd);
	void uninit(void);
	void showMainWindow(void);
	void showCollisionSystemWindow(void);
	void showCameraWindow(void);
	void showPlayerWindow(void);
	void showFogWindow(void);
	void showTextureManagerWindow(void);
	void showMeshManagerWindow(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	string						debugInfo;
	list<string>				debugLog;
	unsigned short				crollCounter;
	bool						enableCollisionSystemWindow;
	bool						enableCameraWindow;
	bool						enablePlayerWindow;
	bool						enableFogWindow;
	ActorController*	player;
	static DebugObserver*		instance;
};
#endif//_DEBUG
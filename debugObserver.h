//--------------------------------------------------------------------------------
//	debug�ώ@��
//�@debugObserver.h
//	Author : Xu Wenjie
//	Date   : 2017-09-07
//--------------------------------------------------------------------------------
#pragma once

#ifdef _DEBUG
#include "main.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class GameObjectActor;

//--------------------------------------------------------------------------------
//  �N���X��`
//--------------------------------------------------------------------------------
class DebugObserver
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
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
	void SetPlayer(GameObjectActor* value) { player = value; }

private:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const us scrollTime = 1 * 30;

	//--------------------------------------------------------------------------------
	//  �֐���`
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
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	string						debugInfo;
	list<string>				debugLog;
	us				scrollCounter;
	bool						enableCollisionSystemWindow;
	bool						enableCameraWindow;
	bool						enablePlayerWindow;
	bool						enableFogWindow;
	GameObjectActor*			player;
	static DebugObserver*		instance;
};
#endif
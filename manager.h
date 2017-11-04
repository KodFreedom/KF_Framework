//--------------------------------------------------------------------------------
//  メインマネージャ
//　manager.h
//	Author : Xu Wenjie
//	Date   : 2016-11-22
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class Mode;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class Manager
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	static Manager* Create(HINSTANCE hInstance, HWND hWnd, BOOL isWindowMode);
	static void		Release(void);
	static auto		Instance(void) { return instance; }

	void Update(void);
	void LateUpdate(void);
	void Render(void);
	void Change(Mode* nextMode);

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	Manager() : currentMode(nullptr) {}
	~Manager() {}
	bool init(HINSTANCE hInstance, HWND hWnd, BOOL isWindowMode);
	void uninit(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	Mode*				currentMode;
	static Manager*		instance;
};
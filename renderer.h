//--------------------------------------------------------------------------------
//
//　renderer.h
//	Author : Xu Wenjie
//	Date   : 2017-11-01
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class Renderer
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	static Renderer* Create(HWND hWnd, BOOL isWindowMode);
	static void		 Release(void);
	static auto		 Instance(void) { return instance; }

	virtual bool BeginRender(void) = 0;
	virtual void EndRender(void) = 0;

	//Get関数
	auto GetBackgroundColor(void) const { return backgroundColor; }
	auto GetWireFrameFlag(void) const { return enableWireFrameMode; }

	//Set関数
	void		 SetBackgroundColor(const Color& color) { backgroundColor = color; }
	virtual void SetWireFrameFlag(const bool& isEnable) { enableWireFrameMode = isEnable; }

protected:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	Renderer() : backgroundColor(Color::Black), enableWireFrameMode(false) {}
	~Renderer() {}
	virtual bool init(HWND hWnd, BOOL isWindowMode) = 0;
	virtual void uninit(void) = 0;

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	static Renderer* instance;
	Color			 backgroundColor;
	bool			 enableWireFrameMode;
};
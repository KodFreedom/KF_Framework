//--------------------------------------------------------------------------------
//
//　renderer.h
//	Author : Xu Wenjie
//	Date   : 2017-11-01
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "renderState.h"

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
	auto		 GetBackgroundColor(void) const { return backgroundColor; }
				 
	//Set関数	   
	void		 SetBackgroundColor(const Color& color) { backgroundColor = color; }
	virtual void SetPorjectionCamera(const Matrix44& view, const Matrix44& projection) = 0;
	virtual void SetRenderState(const Lighting& value) = 0;
	virtual void SetRenderState(const CullMode& value) = 0;
	virtual void SetRenderState(const Synthesis& value) = 0;
	virtual void SetRenderState(const FillMode& value) = 0;
	virtual void SetRenderState(const Alpha& value) = 0;
	virtual void SetRenderState(const Fog& value) = 0;

protected:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	Renderer() : backgroundColor(Color::Black) {}
	~Renderer() {}
	virtual bool init(HWND hWnd, BOOL isWindowMode) = 0;
	virtual void uninit(void) = 0;

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	static Renderer* instance;
	Color			 backgroundColor;
};
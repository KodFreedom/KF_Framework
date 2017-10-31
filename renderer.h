//--------------------------------------------------------------------------------
//
//�@renderer.h
//	Author : Xu Wenjie
//	Date   : 2017-11-01
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class Renderer
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	static Renderer* Create(HWND hWnd, BOOL isWindowMode);
	static void		 Release(void);
	static auto		 Instance(void) { return instance; }

	virtual bool BeginRender(void) = 0;
	virtual void EndRender(void) = 0;

	//Get�֐�
	auto GetBackgroundColor(void) const { return backgroundColor; }
	auto GetWireFrameFlag(void) const { return enableWireFrameMode; }

	//Set�֐�
	void		 SetBackgroundColor(const Color& color) { backgroundColor = color; }
	virtual void SetWireFrameFlag(const bool& isEnable) { enableWireFrameMode = isEnable; }

protected:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	Renderer() : backgroundColor(Color::Black), enableWireFrameMode(false) {}
	~Renderer() {}
	virtual bool init(HWND hWnd, BOOL isWindowMode) = 0;
	virtual void uninit(void) = 0;

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	static Renderer* instance;
	Color			 backgroundColor;
	bool			 enableWireFrameMode;
};
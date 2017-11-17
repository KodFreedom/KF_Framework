//--------------------------------------------------------------------------------
//
//�@renderSystem.h
//	Author : Xu Wenjie
//	Date   : 2017-11-01
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "renderState.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class Light;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class RenderSystem
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	static RenderSystem* Create(HWND hWnd, BOOL isWindowMode);
	static void			 Release(void);
	static auto			 Instance(void) { return instance; }

	virtual bool BeginRender(void) = 0;
	virtual void EndRender(void) = 0;
	virtual void Render(const string& meshName, const string& materialName, const Matrix44& worldMatrix) = 0;
				 
	//Get�֐�	   
	auto		 GetBackgroundColor(void) const { return backgroundColor; }
	auto		 GetFogColor(void) const { return fogColor; }

	//Set�֐�	   
	void		 SetBackgroundColor(const Color& color) { backgroundColor = color; }
	void		 SetFogColor(const Color& color) { fogColor = color; }
	virtual void SetPorjectionCamera(const Matrix44& view, const Matrix44& projection) = 0;
	virtual void SetRenderState(const Lighting& value) = 0;
	virtual void SetRenderState(const CullMode& value) = 0;
	virtual void SetRenderState(const Synthesis& value) = 0;
	virtual void SetRenderState(const FillMode& value) = 0;
	virtual void SetRenderState(const Fog& value) = 0;
	virtual void SetRenderState(const Alpha& value) = 0;

protected:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	RenderSystem()
		: backgroundColor(Color::Black), fogColor(Color::White)
		, fogStartZ(100.0f), fogEndZ(1000.0f), fogDensity(0.01f) {}
	~RenderSystem() {}
	virtual bool init(HWND hWnd, BOOL isWindowMode) = 0;
	virtual void uninit(void) = 0;

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	static RenderSystem*	instance;
	Color					backgroundColor;
	Color					fogColor;
	float					fogStartZ;
	float					fogEndZ;
	float					fogDensity;
};
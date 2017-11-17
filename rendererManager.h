//--------------------------------------------------------------------------------
//	描画用マネージャ
//　renderManager.h
//	Author : Xu Wenjie
//	Date   : 2017-09-20
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum RenderPriority
{
	RP_Default = 0,
	RP_AlphaTest,
	RP_ZSort,
	RP_Max
};

enum RenderStateType
{
	RS_Default = 0,					// LightOn_CCW_Multi_Solid_FogOn
	RS_NoLight_NoFog,				// LightOff_CCW_Multi_Solid_FogOff
	RS_NoCullMode,					// LightOn_None_Multi_Solid_FogOn
	RS_NoLight_NoFog_NoCullMode,	// LightOff_None_Multi_Solid_FogOff
	RS_Max
};

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class MeshRenderer;
class RenderState;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class RendererManager
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	static auto	Create(void)
	{
		if (instance) return instance;
		instance = new RendererManager;
		instance->init();
		return instance;
	}
	static void Release(void) { SAFE_UNINIT(instance); }
	static auto Instance(void) { return instance; }

	void Update(void);
	void Render(void);
	void Clear(void);
	void Register(MeshRenderer* renderer);

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	RendererManager();
	~RendererManager() {}
	void init(void);
	void uninit(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	list<MeshRenderer*>		renderersArrays[RP_Max][RS_Max];
	RenderState*			renderStates[RS_Max];
	static RendererManager*	instance;
};
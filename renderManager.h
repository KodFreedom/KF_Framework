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
#include "renderState.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class MeshRenderer;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class RenderManager
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	static auto	Create(void)
	{
		if (instance) return instance;
		instance = new RenderManager;
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
	//  列挙型定義
	//--------------------------------------------------------------------------------
	enum ListNoBase
	{
		LightingBase = 1,
		CullModeBase = Lighting::Max,
		SynthesisBase = CullMode::Max * Lighting::Max,
		FillModeBase = Synthesis::Max * CullMode::Max * Lighting::Max,
		FogBase = FillMode::Max * Synthesis::Max * CullMode::Max * Lighting::Max,
		AlphaBase = Fog::Max * FillMode::Max * Synthesis::Max * CullMode::Max * Lighting::Max,
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	RenderManager();
	~RenderManager() {}
	void uninit(void) { Clear(); }
	int  calculateListNo(const int alpha, const int fog, const int fillMode, const int synthesis, const int cullMode, const int lighting);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	list<MeshRenderer*>		meshRenderers[Alpha::Max * Fog::Max * FillMode::Max * Synthesis::Max * CullMode::Max * Lighting::Max];
	static RenderManager*	instance;
};
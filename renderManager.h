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
	//  関数定義
	//--------------------------------------------------------------------------------
	RenderManager();
	~RenderManager() {}
	void uninit(void) { Clear(); }

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	union
	{
		list<MeshRenderer*> renderersArray[Alpha::Max * Fog::Max * FillMode::Max * Synthesis::Max * CullMode::Max * Lighting::Max];
		list<MeshRenderer*> renderersArrays[Alpha::Max][Fog::Max][FillMode::Max][Synthesis::Max][CullMode::Max][Lighting::Max];
	};
	static RenderManager*	instance;
};
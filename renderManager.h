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
//  前方宣言
//--------------------------------------------------------------------------------
class CRenderComponent;
class CRenderState;

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum RenderPriority
{//レンダー優先度
	RP_3D = 0,
	RP_3D_ALPHATEST,
	RP_3D_ZSORT,
	RP_MAX
};

enum RenderState
{//レンダーステート
	RS_LIGHTOFF_CULLFACEON_MUL = 0,	//ライトオフ、両面描画、乗算合成
	RS_LIGHTOFF_CULLFACEOFF_MUL,	//ライトオフ、両面描画、乗算合成
	RS_LIGHTON_CULLFACEON_MUL,		//ライトオン、片面描画、乗算合成
	RS_LIGHTON_CULLFACEOFF_MUL,		//ライトオン、片面描画、乗算合成
	RS_MAX
};

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CRenderManager
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CRenderManager();
	~CRenderManager() {}

	static auto Create(void)
	{
		auto pRenderManager = new CRenderManager();
		pRenderManager->init();
		return pRenderManager;
	}
	void		Update(void);
	void		Render(void);
	void		Release(void)
	{
		uninit();
		delete this;
	}
	void		Clear(void);
	void		Register(CRenderComponent* pRender, const RenderPriority& rp, const RenderState& rs)
	{
		m_apRenderComponents[rp][rs].push_back(pRender);
	}

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	typedef list<CRenderComponent*> ListDraw;

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void		init(void);
	void		uninit(void);
	void		setRenderState(const RenderPriority& rp, const RenderState& rs);
	void		resetRenderState(const RenderPriority& rp, const RenderState& rs);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	ListDraw		m_apRenderComponents[RP_MAX][RS_MAX];
	CRenderState*	m_apRenderState[RS_MAX];
};
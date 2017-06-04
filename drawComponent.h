//--------------------------------------------------------------------------------
//	描画コンポネント
//　drawComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18	
//--------------------------------------------------------------------------------
#ifndef _DRAW_COMPONENT_H_
#define _DRAW_COMPONENT_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "component.h"
#include "renderState.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  描画コンポネントクラス
//--------------------------------------------------------------------------------
class CDrawComponent : public CComponent
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CDrawComponent(CGameObject* const pGameObj) : CComponent(pGameObj)
		,m_pRenderState(&s_nullRenderState)
	{}

	~CDrawComponent() {}

	virtual KFRESULT	Init(void) override { return KF_SUCCEEDED; }
	virtual void		Uninit(void) override {};
	virtual void		Draw(void) = 0;

	//Set関数
	void				SetRenderState(CRenderState* const pRenderState) 
	{
		if (pRenderState == NULL)
		{
			m_pRenderState = &s_nullRenderState;
			return;
		}

		m_pRenderState = pRenderState; 
	}

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	static CLightOffRenderState	s_lightOffRenderState;
	static CNullRenderState		s_nullRenderState;

protected:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CDrawComponent() : CComponent() {}

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	CRenderState*	m_pRenderState;
};

//--------------------------------------------------------------------------------
//  ヌル描画コンポネントクラス
//--------------------------------------------------------------------------------
class CNullDrawComponent : public CDrawComponent
{
public:
	CNullDrawComponent() : CDrawComponent() {}
	~CNullDrawComponent() {}

	KFRESULT	Init(void) override { return KF_SUCCEEDED; }
	void		Uninit(void) override {}
	void		Release(void) override {}
	void		Draw(void) override {}
	void		ReceiveMsg(const MESSAGE &msg) override {}
};

#endif

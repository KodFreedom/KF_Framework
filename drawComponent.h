//--------------------------------------------------------------------------------
//	描画コンポネント
//　drawComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18	
//--------------------------------------------------------------------------------
#pragma once

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
		,m_pRenderState(&s_nullRenderState), m_usMatID(0)
	{
		m_strTexName.clear();
	}

	~CDrawComponent() {}

	virtual bool	Init(void) override { return true; }
	virtual void	Uninit(void) override;
	virtual void	Draw(void) = 0;

	//Set関数
	void			SetRenderState(CRenderState* const pRenderState) 
	{
		if (!pRenderState)
		{
			m_pRenderState = &s_nullRenderState;
			return;
		}

		m_pRenderState = pRenderState; 
	}
	void			SetTexName(const string& strTexName);
	void			SetMatID(const unsigned short& usID) { m_usMatID = usID; }

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
	string			m_strTexName;	//テクスチャ
	unsigned short	m_usMatID;		//マテリアル
	CRenderState*	m_pRenderState;	//レンダーステート
};

//--------------------------------------------------------------------------------
//  ヌル描画コンポネントクラス
//--------------------------------------------------------------------------------
class CNullDrawComponent : public CDrawComponent
{
public:
	CNullDrawComponent() : CDrawComponent() {}
	~CNullDrawComponent() {}

	bool	Init(void) override { return true; }
	void	Uninit(void) override {}
	void	Release(void) override {}
	void	Draw(void) override {}
};
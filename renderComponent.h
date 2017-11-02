//--------------------------------------------------------------------------------
//	描画コンポネント
//　renderComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18	
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "component.h"
#include "renderManager.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  描画コンポネントクラス
//--------------------------------------------------------------------------------
class CRenderComponent : public Component
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CRenderComponent(GameObject* const pGameObj)
		: Component(pGameObj)
		, m_usMatID(0)
		, m_renderPriority(RP_3D)
		, m_renderState(RS_LIGHTON_CULLFACEON_MUL)
	{
		m_strTexName.clear();
	}

	~CRenderComponent() {}

	virtual bool	Init(void) override;
	virtual void	Uninit(void) override;
	virtual void	Update(void);
	virtual void	Render(void) = 0;

	//Set関数
	void			SetTexName(const string& strTexName);
	void			SetMatID(const unsigned short& usID) { m_usMatID = usID; }
	void			SetRenderPriority(const RenderPriority& rp) { m_renderPriority = rp; }
	void			SetRenderState(const RenderState& rs) { m_renderState = rs; }

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------

protected:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CRenderComponent() : Component() {}

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	string			m_strTexName;		//テクスチャ
	unsigned short	m_usMatID;			//マテリアル
	RenderPriority	m_renderPriority;
	RenderState	m_renderState;
};

//--------------------------------------------------------------------------------
//  ヌル描画コンポネントクラス
//--------------------------------------------------------------------------------
class CNullRenderComponent : public CRenderComponent
{
public:
	CNullRenderComponent() : CRenderComponent() {}
	~CNullRenderComponent() {}

	bool	Init(void) override { return true; }
	void	Uninit(void) override {}
	void	Release(void) override {}
	void	Update(void) override {}
	void	Render(void) override {}
};
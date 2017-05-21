//--------------------------------------------------------------------------------
//	レンダーステートクラス
//　renderState.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#ifndef _RENDERSTATE_H_
#define _RENDERSTATE_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  スーパークラス
//--------------------------------------------------------------------------------
class CRenderState
{
public:
	CRenderState() {}
	~CRenderState() {}

	virtual void	SetRenderState(void) = 0;
	virtual void	ResetRenderState(void) = 0;
};

//--------------------------------------------------------------------------------
//  LightOffクラス
//--------------------------------------------------------------------------------
class CLightOffRenderState : public CRenderState
{
public:
	CLightOffRenderState() : CRenderState() {}
	~CLightOffRenderState() {}

	void	SetRenderState(void) override;
	void	ResetRenderState(void) override;
};

//--------------------------------------------------------------------------------
//  ヌルクラス
//--------------------------------------------------------------------------------
class CNullRenderState : public CRenderState
{
public:
	CNullRenderState() : CRenderState() {}
	~CNullRenderState() {}

	void	SetRenderState(void) override {}
	void	ResetRenderState(void) override {}
};
#endif
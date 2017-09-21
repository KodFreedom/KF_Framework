//--------------------------------------------------------------------------------
//	レンダーステートクラス
//　renderState.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#pragma once

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

#ifdef USING_DIRECTX
	virtual void	SetRenderState(LPDIRECT3DDEVICE9 pDevice) = 0;
	virtual void	ResetRenderState(LPDIRECT3DDEVICE9 pDevice) = 0;
#endif // USING_DIRECTX
};

//--------------------------------------------------------------------------------
//  LIGHTON_CULLFACEON_MUL
//--------------------------------------------------------------------------------
class CLightOnCullFaceOnMulRenderState : public CRenderState
{
public:
	CLightOnCullFaceOnMulRenderState() {}
	~CLightOnCullFaceOnMulRenderState() {}

#ifdef USING_DIRECTX
	void	SetRenderState(LPDIRECT3DDEVICE9 pDevice) override;
	void	ResetRenderState(LPDIRECT3DDEVICE9 pDevice) override;
#endif // USING_DIRECTX
};

//--------------------------------------------------------------------------------
//  RS_LIGHTON_CULLFACEOFF_MUL
//--------------------------------------------------------------------------------
class CLightOnCullFaceOffMulRenderState : public CRenderState
{
public:
	CLightOnCullFaceOffMulRenderState() {}
	~CLightOnCullFaceOffMulRenderState() {}

#ifdef USING_DIRECTX
	void	SetRenderState(LPDIRECT3DDEVICE9 pDevice) override;
	void	ResetRenderState(LPDIRECT3DDEVICE9 pDevice) override;
#endif // USING_DIRECTX
};

//--------------------------------------------------------------------------------
//  RS_LIGHTOFF_CULLFACEON_MUL
//--------------------------------------------------------------------------------
class CLightOffCullFaceOnMulRenderState : public CRenderState
{
public:
	CLightOffCullFaceOnMulRenderState() {}
	~CLightOffCullFaceOnMulRenderState() {}

#ifdef USING_DIRECTX
	void	SetRenderState(LPDIRECT3DDEVICE9 pDevice) override;
	void	ResetRenderState(LPDIRECT3DDEVICE9 pDevice) override;
#endif // USING_DIRECTX
};

//--------------------------------------------------------------------------------
//  RS_LIGHTOFF_CULLFACEOFF_MUL
//--------------------------------------------------------------------------------
class CLightOffCullFaceOffMulRenderState : public CRenderState
{
public:
	CLightOffCullFaceOffMulRenderState() {}
	~CLightOffCullFaceOffMulRenderState() {}

#ifdef USING_DIRECTX
	void	SetRenderState(LPDIRECT3DDEVICE9 pDevice) override;
	void	ResetRenderState(LPDIRECT3DDEVICE9 pDevice) override;
#endif // USING_DIRECTX
};


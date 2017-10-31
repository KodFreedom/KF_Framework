//--------------------------------------------------------------------------------
//	レンダーステートクラス
//　renderState.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "renderState.h"
#include "manager.h"
#include "fog.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  LIGHTON_CULLFACEON_MUL
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  レンダーステート設定
//--------------------------------------------------------------------------------
void CLightOnCullFaceOnMulRenderState::SetRenderState(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//--------------------------------------------------------------------------------
//  レンダーステートリセット
//--------------------------------------------------------------------------------
void CLightOnCullFaceOnMulRenderState::ResetRenderState(LPDIRECT3DDEVICE9 pDevice)
{}

//--------------------------------------------------------------------------------
//  RS_LIGHTON_CULLFACEOFF_MUL
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  レンダーステート設定
//--------------------------------------------------------------------------------
void CLightOnCullFaceOffMulRenderState::SetRenderState(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

//--------------------------------------------------------------------------------
//  レンダーステートリセット
//--------------------------------------------------------------------------------
void CLightOnCullFaceOffMulRenderState::ResetRenderState(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//--------------------------------------------------------------------------------
//  RS_LIGHTOFF_CULLFACEON_MUL
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  レンダーステート設定
//--------------------------------------------------------------------------------
void CLightOffCullFaceOnMulRenderState::SetRenderState(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
}

//--------------------------------------------------------------------------------
//  レンダーステートリセット
//--------------------------------------------------------------------------------
void CLightOffCullFaceOnMulRenderState::ResetRenderState(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_FOGENABLE, Main::GetManager()->GetFog()->GetEnable());
}

//--------------------------------------------------------------------------------
//  RS_LIGHTOFF_CULLFACEOFF_MUL
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  レンダーステート設定
//--------------------------------------------------------------------------------
void CLightOffCullFaceOffMulRenderState::SetRenderState(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

//--------------------------------------------------------------------------------
//  レンダーステートリセット
//--------------------------------------------------------------------------------
void CLightOffCullFaceOffMulRenderState::ResetRenderState(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

#endif
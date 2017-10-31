//--------------------------------------------------------------------------------
//	�����_�[�X�e�[�g�N���X
//�@renderState.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "renderState.h"
#include "manager.h"
#include "fog.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  LIGHTON_CULLFACEON_MUL
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �����_�[�X�e�[�g�ݒ�
//--------------------------------------------------------------------------------
void CLightOnCullFaceOnMulRenderState::SetRenderState(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//--------------------------------------------------------------------------------
//  �����_�[�X�e�[�g���Z�b�g
//--------------------------------------------------------------------------------
void CLightOnCullFaceOnMulRenderState::ResetRenderState(LPDIRECT3DDEVICE9 pDevice)
{}

//--------------------------------------------------------------------------------
//  RS_LIGHTON_CULLFACEOFF_MUL
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �����_�[�X�e�[�g�ݒ�
//--------------------------------------------------------------------------------
void CLightOnCullFaceOffMulRenderState::SetRenderState(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

//--------------------------------------------------------------------------------
//  �����_�[�X�e�[�g���Z�b�g
//--------------------------------------------------------------------------------
void CLightOnCullFaceOffMulRenderState::ResetRenderState(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//--------------------------------------------------------------------------------
//  RS_LIGHTOFF_CULLFACEON_MUL
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �����_�[�X�e�[�g�ݒ�
//--------------------------------------------------------------------------------
void CLightOffCullFaceOnMulRenderState::SetRenderState(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
}

//--------------------------------------------------------------------------------
//  �����_�[�X�e�[�g���Z�b�g
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
//  �����_�[�X�e�[�g�ݒ�
//--------------------------------------------------------------------------------
void CLightOffCullFaceOffMulRenderState::SetRenderState(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

//--------------------------------------------------------------------------------
//  �����_�[�X�e�[�g���Z�b�g
//--------------------------------------------------------------------------------
void CLightOffCullFaceOffMulRenderState::ResetRenderState(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

#endif
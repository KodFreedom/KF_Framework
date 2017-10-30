//--------------------------------------------------------------------------------
//
//�@fog.cpp
//	Author : Xu Wenjie
//	Date   : 2017-10-09
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "fog.h"
#include "manager.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif // USING_DIRECTX


//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Public
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CFog::CFog()
	: m_bEnable(true)
	, m_bEnableRangeFog(true)
	, m_fStart(1.0f)
	, m_fEnd(1000.0f)
	, m_fDensity(0.01f)
	, m_cColor(CKFMath::sc_cWhite)
{

}

//--------------------------------------------------------------------------------
//  �t�H�O�̎g�p�̐ݒ�
//--------------------------------------------------------------------------------
void CFog::SetEnable(const bool& bEnable)
{
	m_bEnable = bEnable;
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_FOGENABLE, m_bEnable);
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  �͈̓x�[�X�̃t�H�O�̎g�p�̐ݒ�
//--------------------------------------------------------------------------------
void CFog::SetEnableRangeFog(const bool& bEnable)
{
	m_bEnableRangeFog = bEnable;
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, m_bEnableRangeFog);
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  �t�H�O�J�n�_�ݒ�
//--------------------------------------------------------------------------------
void CFog::SetStart(const float& fStart)
{
	m_fStart = fStart;
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_FOGSTART, *((LPDWORD)(&m_fStart)));
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  �t�H�O�I���_�ݒ�
//--------------------------------------------------------------------------------
void CFog::SetEnd(const float& fEnd)
{
	m_fEnd = fEnd;
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_FOGEND, *((LPDWORD)(&m_fEnd)));
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  �t�H�O���x�ݒ�
//--------------------------------------------------------------------------------
void CFog::SetDensity(const float& fDensity)
{
	m_fDensity = fDensity;
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *((LPDWORD)(&m_fDensity)));
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  �t�H�O�F�ݒ�
//--------------------------------------------------------------------------------
void CFog::SetColor(const Color& cColor)
{
	m_cColor = cColor;
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_FOGCOLOR, m_cColor);
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  Private
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CFog::init(void)
{
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	
	// �t�H�O��L���ɂ���
	pDevice->SetRenderState(D3DRS_FOGENABLE, m_bEnable);

	// �t�H�O�J���[�ݒ�
	pDevice->SetRenderState(D3DRS_FOGCOLOR, m_cColor);

	// �o�[�e�b�N�X�t�H�O(���`����)���g�p
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);

	// �t�H�O�͈͐ݒ�
	pDevice->SetRenderState(D3DRS_FOGSTART, *((LPDWORD)(&m_fStart)));
	pDevice->SetRenderState(D3DRS_FOGEND, *((LPDWORD)(&m_fEnd)));

	// �s�N�Z���t�H�O(�w���̂Q )���g�p
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP2);

	// �t�H�O���x�ݒ�
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *((LPDWORD)(&m_fDensity)));

	// �͈̓x�[�X�̃t�H�O���g�p
	pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, m_bEnableRangeFog);
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CFog::uninit(void)
{
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();

	// �t�H�O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	// �͈̓x�[�X�̃t�H�O���֎~
	pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
#endif // USING_DIRECTX
}
//--------------------------------------------------------------------------------
//	�`��p�}�l�[�W��
//�@renderManager.h
//	Author : Xu Wenjie
//	Date   : 2017-09-20
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "renderManager.h"
#include "renderComponent.h"
#include "renderState.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif // USING_DIRECTX

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CRenderManager::CRenderManager()
{
	for (auto& pRS : m_apRenderState) { pRS = nullptr; }
	clear();
}

//--------------------------------------------------------------------------------
//  Update
//--------------------------------------------------------------------------------
void CRenderManager::Update(void)
{
	//To do : Z sort
}

//--------------------------------------------------------------------------------
//  Render
//--------------------------------------------------------------------------------
void CRenderManager::Render(void)
{
	//Render
	for (int nCntRP = 0; nCntRP < RP_MAX; ++nCntRP)
	{
		for (int nCntRS = 0; nCntRS < RS_MAX; ++nCntRS)
		{
			if (m_apRenderComponents[nCntRP][nCntRS].empty()) { continue; }

			setRenderState((RENDER_PRIORITY)nCntRP, (RENDER_STATE)nCntRS);

			for (auto pRender : m_apRenderComponents[nCntRP][nCntRS])
			{
				pRender->Render();
			}

			resetRenderState((RENDER_PRIORITY)nCntRP, (RENDER_STATE)nCntRS);
		}
	}

	//Clear
	clear();
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
void CRenderManager::init(void)
{
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ���f�X�e�B�l�[�V�����J���[�̎w��
																		// �T���v���[�X�e�[�g�̐ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���t�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���u�l�̌J��Ԃ��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���g�厞�̕�Ԑݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���k�����̕�Ԑݒ�
																		
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����(�����l��D3DTA_TEXTURE�A�e�N�X�`�����Ȃ��ꍇ��D3DTA_DIFFUSE)
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����(�����l��D3DTOP_SELECTARG1)
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����(�����l��D3DTA_CURRENT)
#endif // USING_DIRECTX

	//RenderState������
	m_apRenderState[RS_LIGHTON_CULLFACEON_MUL] = new CLightOnCullFaceOnMulRenderState;
	m_apRenderState[RS_LIGHTON_CULLFACEOFF_MUL] = new CLightOnCullFaceOffMulRenderState;
	m_apRenderState[RS_LIGHTOFF_CULLFACEON_MUL] = new CLightOffCullFaceOnMulRenderState;
	m_apRenderState[RS_LIGHTOFF_CULLFACEOFF_MUL] = new CLightOffCullFaceOffMulRenderState;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CRenderManager::uninit(void)
{
	for (auto& pRS : m_apRenderState) { SAFE_DELETE(pRS); }
	clear();
}

//--------------------------------------------------------------------------------
//  �z��N���A����
//--------------------------------------------------------------------------------
void CRenderManager::clear(void)
{
	for (auto& aListRender : m_apRenderComponents)
	{
		for (auto& listRender : aListRender)
		{
			listRender.clear();
		}
	}
}

//--------------------------------------------------------------------------------
//  setRenderState
//--------------------------------------------------------------------------------
void CRenderManager::setRenderState(const RENDER_PRIORITY& rp, const RENDER_STATE& rs)
{
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();

	if (rp == RP_3D_ALPHATEST)
	{//Alpha Test

		pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}
	
	m_apRenderState[rs]->SetRenderState(pDevice);
#endif // USING_DIRECTX
}

//--------------------------------------------------------------------------------
//  resetRenderState
//--------------------------------------------------------------------------------
void CRenderManager::resetRenderState(const RENDER_PRIORITY& rp, const RENDER_STATE& rs)
{
#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();

	if (rp == RP_3D_ALPHATEST)
	{//ZTEST
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}

	m_apRenderState[rs]->ResetRenderState(pDevice);
#endif // USING_DIRECTX
}
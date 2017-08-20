//--------------------------------------------------------------------------------
//
//�@fade.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-09
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "fade.h"
#include "manager.h"
#include "textureManager.h"
#include "mode.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
const float CFade::sc_fFadeRate = 1.0f / (120.0f * 0.5f);

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CFade::Init(void)
{
#ifdef USING_DIRECTX
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	CMain::GetManager()->GetTextureManager()->UseTexture("polygon.jpg");

	//���_�o�b�t�@
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,				//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,					//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,						//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,					//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&m_pVtxBuffer,						//���_�o�b�t�@�̃A�h���X
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CFade : CreateVertexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
	}

	// ���_����ݒ�
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ʒu�ݒ�
	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].vPos = CKFVec3(0.0f, 0.0f, 0.0f);
	pVtx[1].vPos = CKFVec3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].vPos = CKFVec3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].vPos = CKFVec3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//���_�J���[�ݒ�
	pVtx[0].ulColor = m_cColor;
	pVtx[1].ulColor = m_cColor;
	pVtx[2].ulColor = m_cColor;
	pVtx[3].ulColor = m_cColor;

	//���_UV�ݒ�
	pVtx[0].vUV = CKFVec2(0.0f, 0.0f);
	pVtx[1].vUV = CKFVec2(1.0f, 0.0f);
	pVtx[2].vUV = CKFVec2(0.0f, 1.0f);
	pVtx[3].vUV = CKFVec2(1.0f, 1.0f);

	//rhw�̐ݒ�(�K��1.0f)
	pVtx[0].fRhw = 1.0f;
	pVtx[1].fRhw = 1.0f;
	pVtx[2].fRhw = 1.0f;
	pVtx[3].fRhw = 1.0f;

	//���z�A�h���X���
	m_pVtxBuffer->Unlock();
#endif
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CFade::Uninit(void)
{
#ifdef USING_DIRECTX
	CMain::GetManager()->GetTextureManager()->DisuseTexture("polygon.jpg");
	SAFE_RELEASE(m_pVtxBuffer);
#endif
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CFade::Update(void)
{
	if (m_fade == FADE_NONE) { return; }

	//fade���ԃJ�E���g
	if (m_nCnt != 0)
	{
		m_nCnt -= 1;
		return;
	}

	if (m_fade == FADE_IN)
	{
		m_cColor.m_fA -= sc_fFadeRate;//alpha�����Z���Č��̉�ʂ𕂂��オ�点��

		if (m_cColor.m_fA <= 0.0f)
		{
			m_cColor.m_fA = 0.0f;
			m_fade = FADE_NONE;
		}
	}
	else if (m_fade == FADE_OUT)
	{
		m_cColor.m_fA += sc_fFadeRate;

		if (m_cColor.m_fA >= 1.0f)
		{
			m_cColor.m_fA = 1.0f;

			//Check SE & BGM(fade out effect)
			m_fade = FADE_IN;
			CMain::GetManager()->SetMode(m_pModeNext);
		}
	}

	SetColorFade(m_cColor);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CFade::Draw(void)
{
#ifdef USING_DIRECTX
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	LPDIRECT3DTEXTURE9 pTexture = CMain::GetManager()->GetTextureManager()->GetTexture("polygon.jpg");
	pDevice->SetTexture(0, pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
#endif
}

//--------------------------------------------------------------------------------
//  FadeToMode
//--------------------------------------------------------------------------------
void CFade::FadeToMode(CMode* pModeNext)
{
	if (m_fade == FADE_OUT) 
	{
		delete pModeNext;
		return;
	}

	m_fade = FADE_OUT;
	m_pModeNext = pModeNext;
	m_nCnt = 10;
}

//--------------------------------------------------------------------------------
//  SetColorFade
//--------------------------------------------------------------------------------
void CFade::SetColorFade(const CKFColor &cColor)
{
#ifdef USING_DIRECTX
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�(0�`255�̐����l)
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//���z�A�h���X���
	m_pVtxBuffer->Unlock();
#endif
}

//--------------------------------------------------------------------------------
//  �쐬
//--------------------------------------------------------------------------------
CFade *CFade::Create(void)
{
	CFade *pFade = new CFade;
	pFade->Init();
	return pFade;
}
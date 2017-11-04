//--------------------------------------------------------------------------------
//
//�@fade.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-09
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "fadeSystem.h"
#include "manager.h"
#include "textureManager.h"
#include "mode.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void FadeSystem::Update(void)
{
	switch (currentState)
	{
	case State::None:
		break;
	case State::FadeIn:
		fadeIn();
		break;
	case State::FadeOut:
		fadeOut();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void FadeSystem::Draw(void)
{
	//#ifdef USING_DIRECTX
	//	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	//
	//	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	//	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));
	//
	//	// ���_�t�H�[�}�b�g�̐ݒ�
	//	pDevice->SetFVF(FVF_VERTEX_2D);
	//
	//	// �e�N�X�`���̐ݒ�
	//	LPDIRECT3DTEXTURE9 texture = Main::GetManager()->GetTextureManager()->GetTexture("polygon.jpg");
	//	pDevice->SetTexture(0, texture);
	//
	//	// �|���S���̕`��
	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	//#endif
}

//--------------------------------------------------------------------------------
//	�֐����FFadeTo
//  �֐������F���̃��[�h�Ƀt�F�[�h����
//	�����F	nextMode�F���̃��[�h
//			fadeTime�F�t�F�[�h�̎��ԁi�b���j
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void FadeSystem::FadeTo(Mode* nextMode, const float fadeTime)
{
	if (currentState == State::FadeOut)
	{
		delete nextMode;
		return;
	}
	currentState = State::FadeOut;
	timeCounter = 0.0f;
	FadeSystem::nextMode = nextMode;
	FadeSystem::fadeTime = fadeTime;
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void FadeSystem::init(void)
{
//#ifdef USING_DIRECTX
//	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();
//	HRESULT hr;
//
//	Main::GetManager()->GetTextureManager()->UseTexture("polygon.jpg");
//
//	//���_�o�b�t�@
//	hr = pDevice->CreateVertexBuffer(
//		sizeof(VERTEX_2D) * 4,				//�쐬���������_�o�b�t�@�̃T�C�Y
//		D3DUSAGE_WRITEONLY,					//���_�o�b�t�@�̎g�p���@
//		FVF_VERTEX_2D,						//�����Ȃ��Ă����v
//		D3DPOOL_MANAGED,					//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
//		&m_pVtxBuffer,						//���_�o�b�t�@�̃A�h���X
//		NULL);
//
//	if (FAILED(hr))
//	{
//		MessageBox(NULL, "FadeSystem : CreateVertexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
//	}
//
//	// ���_����ݒ�
//	//���z�A�h���X���擾���邽�߂̃|�C���^
//	VERTEX_2D *pVtx;
//
//	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
//	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
//
//	//���_�ʒu�ݒ�
//	//���_���W�̐ݒ�i2D���W�A�E���j
//	pVtx[0].Position = Vector3(0.0f, 0.0f, 0.0f);
//	pVtx[1].Position = Vector3(SCREEN_WIDTH, 0.0f, 0.0f);
//	pVtx[2].Position = Vector3(0.0f, SCREEN_HEIGHT, 0.0f);
//	pVtx[3].Position = Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
//
//	//���_�J���[�ݒ�
//	pVtx[0].Color = color;
//	pVtx[1].Color = color;
//	pVtx[2].Color = color;
//	pVtx[3].Color = color;
//
//	//���_UV�ݒ�
//	pVtx[0].UV = Vector2(0.0f, 0.0f);
//	pVtx[1].UV = Vector2(1.0f, 0.0f);
//	pVtx[2].UV = Vector2(0.0f, 1.0f);
//	pVtx[3].UV = Vector2(1.0f, 1.0f);
//
//	//rhw�̐ݒ�(�K��1.0f)
//	pVtx[0].Rhw = 1.0f;
//	pVtx[1].Rhw = 1.0f;
//	pVtx[2].Rhw = 1.0f;
//	pVtx[3].Rhw = 1.0f;
//
//	//���z�A�h���X���
//	m_pVtxBuffer->Unlock();
//#endif
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void FadeSystem::uninit(void)
{
//#ifdef USING_DIRECTX
//	Main::GetManager()->GetTextureManager()->DisuseTexture("polygon.jpg");
//	SAFE_RELEASE(m_pVtxBuffer);
//#endif
}

//--------------------------------------------------------------------------------
//  fadeIn
//--------------------------------------------------------------------------------
void FadeSystem::fadeIn(void)
{
	timeCounter -= DELTA_TIME;
	if (timeCounter <= 0.0f)
	{
		timeCounter = 0.0f;
		currentState = State::None;
	}
	setColor();
}

//--------------------------------------------------------------------------------
//  fadeOut
//--------------------------------------------------------------------------------
void FadeSystem::fadeOut(void)
{
	timeCounter += DELTA_TIME;
	if (timeCounter >= fadeTime)
	{
		timeCounter = fadeTime;

		// Todo : Check SE & BGM(fade out effect)
		currentState = State::FadeIn;
		Manager::Instance()->Change(nextMode);
	}
	setColor();
}

//--------------------------------------------------------------------------------
//	�F�̐ݒ�
//--------------------------------------------------------------------------------
void FadeSystem::setColor(void)
{
	color.A = timeCounter / fadeTime;
//#ifdef USING_DIRECTX
//	//���z�A�h���X���擾���邽�߂̃|�C���^
//	VERTEX_2D *pVtx = NULL;
//
//	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
//	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
//
//	//���_�J���[�̐ݒ�(0�`255�̐����l)
//	pVtx[0].Color = cColor;
//	pVtx[1].Color = cColor;
//	pVtx[2].Color = cColor;
//	pVtx[3].Color = cColor;
//
//	//���z�A�h���X���
//	m_pVtxBuffer->Unlock();
//#endif
}
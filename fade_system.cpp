//--------------------------------------------------------------------------------
//�@fade_system.h
//  manage the materials' save, load
//	�}�e���A���Ǘ���
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "fade_system.h"
#include "main_system.h"
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
	switch (current_state_)
	{
	case FadeState::kFadeNone:
		break;
	case FadeState::kFadeIn:
		FadeIn();
		break;
	case FadeState::kFadeOut:
		FadeOut();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void FadeSystem::Render(void)
{
	//#ifdef USING_DIRECTX
	//	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	//
	//	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	//	pDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(VERTEX_2D));
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
//  ���̃��[�h�Ƀt�F�[�h����
//--------------------------------------------------------------------------------
void FadeSystem::FadeTo(Mode* next_mode, const float fade_time)
{
	if (current_state_ == FadeState::kFadeOut)
	{
		delete next_mode;
		return;
	}
	current_state_ = FadeState::kFadeOut;
	time_counter_ = 0.0f;
	next_mode_ = next_mode;
	fade_time_ = fade_time;
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void FadeSystem::Init(void)
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
//		&vertexBuffer,						//���_�o�b�t�@�̃A�h���X
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
//	vertexBuffer->Lock(0, 0, (void**)&pVtx, 0);
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
//	vertexBuffer->Unlock();
//#endif
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void FadeSystem::Uninit(void)
{
//#ifdef USING_DIRECTX
//	Main::GetManager()->GetTextureManager()->DisuseTexture("polygon.jpg");
//	SAFE_RELEASE(vertexBuffer);
//#endif
}

//--------------------------------------------------------------------------------
//  fadeIn
//--------------------------------------------------------------------------------
void FadeSystem::FadeIn(void)
{
	time_counter_ -= DELTA_TIME;
	if (time_counter_ <= 0.0f)
	{
		time_counter_ = 0.0f;
		current_state_ = kFadeNone;
	}
	color_.a_ = time_counter_ / fade_time_;
}

//--------------------------------------------------------------------------------
//  fadeOut
//--------------------------------------------------------------------------------
void FadeSystem::FadeOut(void)
{
	time_counter_ += DELTA_TIME;
	if (time_counter_ >= fade_time_)
	{
		time_counter_ = fade_time_;

		// Todo : Check SE & BGM(fade out effect)
		current_state_ = kFadeIn;
		MainSystem::Instance()->Change(next_mode_);
	}
	color_.a_ = time_counter_ / fade_time_;
}
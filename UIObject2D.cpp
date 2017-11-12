//--------------------------------------------------------------------------------
//
//�@UIObject2D.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "UIObject2D.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
UIObject2D::UIObject2D(const us order)
	: UIObject(order)
{
	sprites.clear();
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UIObject2D::Uninit(void)
{
	for (auto itr = sprites.begin(); itr != sprites.end();)
	{
		TextureManager::Instance()->Disuse(itr->texture);
#ifdef USING_DIRECTX
		SAFE_RELEASE(itr->pVtxBuffer);
#endif
		itr = sprites.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void UIObject2D::Draw(void)
{
	for (auto itr = sprites.begin(); itr != sprites.end(); ++itr)
	{
		/*
		LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(
			0,							//�X�g���[���ԍ�
			itr->pVtxBuffer,			//���_�o�b�t�@
			0,							//�I�t�Z�b�g
			sizeof(VERTEX_2D));			//�X�g���C�h��

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		auto texture = TextureManager::Instance()->GetTexture(itr->texture);
		pDevice->SetTexture(0, texture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,							//�I�t�Z�b�g
			itr->polygonNumber);			//�|���S����
		*/
	}
}
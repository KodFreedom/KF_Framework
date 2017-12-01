//--------------------------------------------------------------------------------
//�@ui_object_2d.cpp
//	2dui�I�u�W�F�N�g
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "ui_object_2d.h"
#include "main_system.h"
#include "texture_manager.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void UIObject2D::Uninit(void)
{
	auto texture_manager = MainSystem::Instance()->GetTextureManager();
	for (auto iterator = sprites_.begin(); iterator != sprites_.end();)
	{
		texture_manager->Disuse(iterator->texture_name);
		iterator = sprites_.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void UIObject2D::Render(void)
{
	//for (auto itr = sprites_.begin(); itr != sprites_.end(); ++itr)
	//{
	//	/*
	//	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	//
	//	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	//	pDevice->SetStreamSource(
	//		0,							//�X�g���[���ԍ�
	//		itr->pVtxBuffer,			//���_�o�b�t�@
	//		0,							//�I�t�Z�b�g
	//		sizeof(VERTEX_2D));			//�X�g���C�h��
	//
	//	// ���_�t�H�[�}�b�g�̐ݒ�
	//	pDevice->SetFVF(FVF_VERTEX_2D);
	//
	//	// �e�N�X�`���̐ݒ�
	//	auto texture = TextureManager::Instance()->GetTexture(itr->texture);
	//	pDevice->SetTexture(0, texture);
	//
	//	// �|���S���̕`��
	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
	//		0,							//�I�t�Z�b�g
	//		itr->polygonNumber);			//�|���S����
	//	*/
	//}
}
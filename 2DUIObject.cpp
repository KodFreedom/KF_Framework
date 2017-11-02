//--------------------------------------------------------------------------------
//
//�@2DUIObject.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "2DUIObject.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
C2DUIObject::C2DUIObject(const unsigned short& usPriority)
	: CUIObject(usPriority)
{
	m_listSprite.clear();
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void C2DUIObject::Uninit(void)
{
	for (auto itr = m_listSprite.begin(); itr != m_listSprite.end();)
	{
		Main::GetManager()->GetTextureManager()->DisuseTexture(itr->strTexName);
		SAFE_RELEASE(itr->pVtxBuffer);
		itr = m_listSprite.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void C2DUIObject::Draw(void)
{
	for (auto itr = m_listSprite.begin(); itr != m_listSprite.end(); ++itr)
	{
#ifdef USING_DIRECTX
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
		LPDIRECT3DTEXTURE9 texture = Main::GetManager()->GetTextureManager()->GetTexture(itr->strTexName);
		pDevice->SetTexture(0, texture);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,							//�I�t�Z�b�g
			itr->usNumPolygon);			//�|���S����
#endif
	}
}
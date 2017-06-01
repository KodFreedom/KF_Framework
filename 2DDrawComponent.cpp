//--------------------------------------------------------------------------------
//	2D�`��R���|�l���g
//�@2DDrawComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-21	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "2DDrawComponent.h"
#include "2DMeshComponent.h"
#include "gameObject.h"
#include "manager.h"
#include "textureManager.h"

#ifdef USING_DIRECTX9
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void C2DDrawComponent::Draw(CGameObject &gameObj, const CMeshComponent &meshComponent)
{
	C2DMeshComponent *pMesh = (C2DMeshComponent*)&meshComponent;

#ifdef USING_DIRECTX9
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	// �����_�[�X�e�[�g�ݒ�
	m_pRenderState->SetRenderState();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,						//�X�g���[���ԍ�
		pMesh->GetVtxBuffer(),	//���_�o�b�t�@
		0,						//�I�t�Z�b�g
		sizeof(VERTEX_2D));		//�X�g���C�h��

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture(pMesh->GetTexName());
	pDevice->SetTexture(0, pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,							//�I�t�Z�b�g
		pMesh->GetNumPolygon());	//�|���S����

	// �����_�[�X�e�[�g���Z�b�g
	m_pRenderState->ResetRenderState();
#endif
}
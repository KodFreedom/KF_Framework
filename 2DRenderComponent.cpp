////--------------------------------------------------------------------------------
////	2D�`��R���|�l���g
////�@2DRenderComponent.h
////	Author : Xu Wenjie
////	Date   : 2017-05-21	
////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
////  �C���N���[�h�t�@�C��
////--------------------------------------------------------------------------------
//#include "2DRenderComponent.h"
//#include "2DMeshComponent.h"
//#include "gameObject.h"
//#include "manager.h"
//#include "textureManager.h"
//
//#ifdef USING_DIRECTX
//#include "rendererDX.h"
//#endif
//
////--------------------------------------------------------------------------------
////  �N���X
////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
////  �`�揈��
////--------------------------------------------------------------------------------
//void C2DRenderComponent::Render(void)
//{
//#ifdef USING_DIRECTX
//	auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
//
//	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
//	pDevice->SetStreamSource(
//		0,							//�X�g���[���ԍ�
//		c_mesh->GetVtxBuffer(),	//���_�o�b�t�@
//		0,							//�I�t�Z�b�g
//		sizeof(VERTEX_2D));			//�X�g���C�h��
//
//	// ���_�t�H�[�}�b�g�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	// �e�N�X�`���̐ݒ�
//	LPDIRECT3DTEXTURE9 pTexture = Main::GetManager()->GetTextureManager()->GetTexture(m_strTexName);
//	pDevice->SetTexture(0, pTexture);
//
//	// �|���S���̕`��
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
//		0,							//�I�t�Z�b�g
//		c_mesh->GetNumPolygon());	//�|���S����
//#endif
//}
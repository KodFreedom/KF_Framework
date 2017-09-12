//--------------------------------------------------------------------------------
//	3D���b�V���`��R���|�l���g
//�@3DMeshDrawComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-21	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "3DMeshDrawComponent.h"
#include "3DMeshComponent.h"
#include "gameObject.h"
#include "manager.h"
#include "meshManager.h"
#include "textureManager.h"
#include "materialManager.h"
#include "mesh.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void C3DMeshDrawComponent::Draw(void)
{
	const auto pMeshInfo = CMain::GetManager()->GetMeshManager()->GetMesh(c_pMesh->GetMeshName());

	//�}�g���N�X�Z�o
	auto mtxWorld = m_pGameObj->GetTransformComponent()->GetMatrix();

#ifdef USING_DIRECTX
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();

	//RenderState�ݒ�
	m_pRenderState->SetRenderState();

	//�}�g���b�N�X�ݒ�
	D3DXMATRIX mtx = mtxWorld;
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,						//�X�g���[���ԍ�
		pMeshInfo->m_pVtxBuffer,//���_�o�b�t�@
		0,						//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_3D));		//�X�g���C�h��

	// ���_�C���f�b�N�X�̐ݒ�
	pDevice->SetIndices(pMeshInfo->m_pIdxBuffer);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	auto pTexture = CMain::GetManager()->GetTextureManager()->GetTexture(m_strTexName);
	pDevice->SetTexture(0, pTexture);

	// �}�e���A���̐ݒ�
	D3DMATERIAL9 mat = CMain::GetManager()->GetMaterialManager()->GetMaterial(m_usMatID);
	pDevice->SetMaterial(&mat);

	//�v���~�e�B�u�`��
	pDevice->DrawIndexedPrimitive(
		(_D3DPRIMITIVETYPE)pMeshInfo->m_drawType,
		0,
		0,
		pMeshInfo->m_nNumVtx,
		0,
		pMeshInfo->m_nNumPolygon);

	//RenderState�߂�
	m_pRenderState->ResetRenderState();
#endif
}

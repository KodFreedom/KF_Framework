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
void C3DMeshDrawComponent::Draw(const CGameObject &gameObj, const CMeshComponent &meshComponent)
{
	C3DMeshComponent *pMesh = (C3DMeshComponent*)&meshComponent;
	CKFMtx44& mtxWorld = gameObj.GetMatrix();
	CTM::TEX_NAME texName = pMesh->GetTexName();
	CMM::MATERIAL matType = pMesh->GetMatType();
	C3DMeshComponent::MESH3D meshInfo = pMesh->GetMeshInfo();

#ifdef USING_DIRECTX9
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//RenderState�ݒ�
	m_pRenderState->SetRenderState();

	//�}�g���b�N�X�ݒ�
	D3DXMATRIX mtx = mtxWorld;
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,						//�X�g���[���ԍ�
		meshInfo.pVtxBuffer,	//���_�o�b�t�@
		0,						//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_3D));		//�X�g���C�h��

	// ���_�C���f�b�N�X�̐ݒ�
	pDevice->SetIndices(meshInfo.pIdxBuffer);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture(texName);
	pDevice->SetTexture(0, pTexture);

	// �}�e���A���̐ݒ�
	D3DMATERIAL9 mat = GetManager()->GetMaterialManager()->GetMaterial(matType);
	pDevice->SetMaterial(&mat);

	//�v���~�e�B�u�`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		meshInfo.nNumVtx,
		0,
		meshInfo.nNumPolygon);

	//RenderState�߂�
	m_pRenderState->ResetRenderState();
#endif
}

//--------------------------------------------------------------------------------
//	���f�����b�V���`��R���|�l���g
//�@modelMeshDrawComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "modelMeshDrawComponent.h"
#include "modelMeshComponent.h"
#include "manager.h"
#include "textureManager.h"
#include "materialManager.h"
#include "modelManager.h"
#include "gameObject.h"
#include "model.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CModelMeshDrawComponent::Draw(void)
{
	//�`�F�b�NMeshComponent�̃^�C�v
	CMeshComponent* pMesh = m_pGameObj->GetMeshComponent();
	if (pMesh->GetType() != CMeshComponent::MESH_MODEL) { return; }
	CModelMeshComponent* pModelMesh = (CModelMeshComponent*)pMesh;

	//�}�g���N�X�Z�o
	CKFMtx44 mtxWorld;
	CKFMtx44 mtxRot;
	CKFMtx44 mtxPos;

	//�P�ʍs��ɏ�����
	CKFMath::MtxIdentity(&mtxWorld);

	//��](Y->X->Z)
	mtxRot = m_pGameObj->GetMatrixRot();
	//CKFMath::MtxRotationYawPitchRoll(&mtxRot, m_pGameObj->GetRot());
	//mtxWorld *= mtxRot;
	mtxWorld *= mtxRot;

	//���s�ړ�
	CKFMath::MtxTranslation(&mtxPos, m_pGameObj->GetPos());
	mtxWorld *= mtxPos;

	m_pGameObj->SetMatrix(mtxWorld);

	const CTM::TEX_NAME& texName = pModelMesh->GetTexName();
	const CMM::MATERIAL& matType = pModelMesh->GetMatType();
	CModel* pModel = GetManager()->GetModelManager()->GetModel(pModelMesh->GetModelName());
	if (pModel != NULL)
	{
		//RenderState�ݒ�
		m_pRenderState->SetRenderState();

		pModel->Draw(mtxWorld, matType, texName);

		//RenderState�߂�
		m_pRenderState->ResetRenderState();
	}
}

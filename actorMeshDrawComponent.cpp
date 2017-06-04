//--------------------------------------------------------------------------------
//	�A�N�^�[�`��R���|�l���g
//�@actorMeshDrawComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "actorMeshDrawComponent.h"
#include "actorMeshComponent.h"
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
void CActorMeshDrawComponent::Draw(void)
{
	//�`�F�b�NMeshComponent�̃^�C�v
	CMeshComponent* pMesh = m_pGameObj->GetMeshComponent();
	if (pMesh->GetType() != CMeshComponent::MESH_ACTOR) { return; }
	CActorMeshComponent* pActorMesh = (CActorMeshComponent*)pMesh;

	//�}�g���N�X�Z�o
	CKFMtx44 mtxWorld;
	CKFMtx44 mtxRot;
	CKFMtx44 mtxPos;

	//�P�ʍs��ɏ�����
	CKFMath::MtxIdentity(&mtxWorld);

	//��](Y->X->Z)
	CKFMath::MtxRotationYawPitchRoll(&mtxRot, m_pGameObj->GetRot());
	mtxWorld *= mtxRot;

	//���s�ړ�
	CKFMath::MtxTranslation(&mtxPos, m_pGameObj->GetPos());
	mtxWorld *= mtxPos;

	m_pGameObj->SetMatrix(mtxWorld);

	const CTM::TEX_NAME& texName = pActorMesh->GetTexName();
	const CMM::MATERIAL& matType = pActorMesh->GetMatType();
	const CMOM::MODEL_NAME& modelName = pActorMesh->GetModelName();
	CActorMeshComponent::ACTOR_MOTION_INFO& info = pActorMesh->GetMotionInfo();

	//���f���̎擾
	CModel* pModel = GetManager()->GetModelManager()->GetModel(modelName);
	if (pModel == NULL) { return; }
	CModelActorX* pModelActor = (CModelActorX*)pModel;

	//RenderState�ݒ�
	m_pRenderState->SetRenderState();

	pModelActor->Draw(mtxWorld, info.vectorPartsInfo, matType, texName);

	//RenderState�߂�
	m_pRenderState->ResetRenderState();
}

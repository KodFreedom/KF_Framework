//--------------------------------------------------------------------------------
//
//�@gameObjectModel.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "rendererDX.h"
#include "model.h"
#include "modelX.h"
#include "gameObject.h"
#include "gameObjectModel.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObjectModel::CGameObjectModel() : CGameObject()
	, m_vRot(CKFVec3(0.0f))
	, m_modelName(CMOM::MODEL_NONE)
	, m_matType(CMM::MAT_MAX)
{

}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
CGameObjectModel::~CGameObjectModel()
{

}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CGameObjectModel::Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName)
{
	m_vPos = vPos;
	m_vRot = vRot;
	m_modelName = modelName;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CGameObjectModel::Uninit(void)
{

}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CGameObjectModel::Update(void)
{

}

//--------------------------------------------------------------------------------
//  �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CGameObjectModel::LateUpdate(void)
{

}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CGameObjectModel::Draw(void)
{
	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);

	if (pModel != NULL)
	{
		//�`�F�b�N���f���^�C�v
		if (pModel->GetModelType() != CMOM::XFILE) { return; }

		//�L���X�g
		CModelX* pModelX = (CModelX*)pModel;

		//RenderState�ݒ�
		SetRenderState();

		//�}�g���b�N�X�ݒ�
		SetMatrix();

		//�`��
		if (m_matType != CMM::MAT_MAX)
		{
			pModelX->Draw(m_matType);
		}
		else
		{
			pModelX->Draw();
		}

		//RenderState�߂�
		ResetRenderState();
	}
}

//--------------------------------------------------------------------------------
// SetWorldMatrix
//--------------------------------------------------------------------------------
void CGameObjectModel::SetMatrix(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//���[���h���΍��W
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxPos;

	//�P�ʍs��ɏ�����
	D3DXMatrixIdentity(&mtxWorld);

	//��](Y->X->Z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vRot.m_fY, m_vRot.m_fX, m_vRot.m_fZ);
	mtxWorld *= mtxRot;

	//���s�ړ�
	D3DXMatrixTranslation(&mtxPos, m_vPos.m_fX, m_vPos.m_fY, m_vPos.m_fZ);
	mtxWorld *= mtxPos;

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
}

//--------------------------------------------------------------------------------
// SetRenderState
//--------------------------------------------------------------------------------
void CGameObjectModel::SetRenderState(void)
{

}

//--------------------------------------------------------------------------------
// ResetRenderState
//--------------------------------------------------------------------------------
void CGameObjectModel::ResetRenderState(void)
{

}

//--------------------------------------------------------------------------------
// ����
//--------------------------------------------------------------------------------
CGameObjectModel* CGameObjectModel::Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName)
{
	CGameObjectModel *pObjModel = NULL;
	pObjModel = new CGameObjectModel;
	pObjModel->Init(vPos, vRot, modelName);
	pObjModel->m_pri = GOM::PRI_3D;
	pObjModel->m_nID = GetManager()->GetGameObjectManager()->SaveGameObj(GOM::PRI_3D, pObjModel);
	return pObjModel;
}
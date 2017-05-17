//--------------------------------------------------------------------------------
//
//�@gameObjectActor.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-15
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "gameObjectActor.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObjectActor::CGameObjectActor()
{
	m_nCntFrame = 0;
	m_nKeyNow = 0;
	m_motionNow = MOTION_NEUTAL;
	m_vectorPartsInfo.clear();
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CGameObjectActor::Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName)
{
	CGameObjectModel::Init(vPos, vRot, modelName);

	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);
	if (pModel != NULL)
	{
		CModelActorX* pModelActor = (CModelActorX*)pModel;
		m_vectorPartsInfo = pModelActor->GetDefaultPartsInfo();
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CGameObjectActor::Uninit(void)
{
	CGameObjectModel::Uninit();
	m_vectorPartsInfo.clear();
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CGameObjectActor::Update(void)
{
	
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CGameObjectActor::LateUpdate(void)
{
	UpdateMotion();
}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CGameObjectActor::Draw(void)
{
	//���f���̎擾
	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);
	if (pModel == NULL) { return; }
	CModelActorX* pModelActor = (CModelActorX*)pModel;

	//���[���h���΍��W
	CKFMtx44 mtxWorld;
	CKFMtx44 mtxRot;
	CKFMtx44 mtxPos;

	//�P�ʍs��ɏ�����
	CKFMath::MtxIdentity(&mtxWorld);

	//��](Y->X->Z)
	CKFMath::MtxRotationYawPitchRoll(&mtxRot, m_vRot);
	mtxWorld *= mtxRot;

	//���s�ړ�
	CKFMath::MtxTranslation(&mtxPos, m_vPos);
	mtxWorld *= mtxPos;

	//�`��
	pModelActor->Draw(mtxWorld, m_vectorPartsInfo);
}

//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
CGameObjectActor*	CGameObjectActor::Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName)
{
	CGameObjectActor *pObjChar = NULL;
	pObjChar = new CGameObjectActor;
	pObjChar->Init(vPos, vRot, modelName);
	pObjChar->m_pri = GOM::PRI_3D;
	pObjChar->m_nID = GetManager()->GetGameObjectManager()->SaveGameObj(GOM::PRI_3D, pObjChar);
	return pObjChar;
}

//--------------------------------------------------------------------------------
//  ���[�V�����X�V����
//--------------------------------------------------------------------------------
void CGameObjectActor::UpdateMotion(void)
{
	//���[�V�������擾
	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);
	if (pModel == NULL) { return; }
	CModelActorX* pModelActor = (CModelActorX*)pModel;
	const std::vector<CModelActorX::VEC_MOTION> &vectorMotion = pModelActor->GetPartsMotionInfo();

	//�p�[�c�X�V
	int nFrame;
	int nNumKey;
	bool bLoop;
	int nNumParts = (int)m_vectorPartsInfo.size();
	for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
	{
		CKFVec3 &vPartPos = m_vectorPartsInfo[nCntPart].vPos;
		CKFVec3 &vPartRot = m_vectorPartsInfo[nCntPart].vRot;
		CKFVec3 vKeyPos = vectorMotion[nCntPart][m_motionNow].vectorKeyFrame[m_nKeyNow].vPos;
		CKFVec3 vKeyRot = vectorMotion[nCntPart][m_motionNow].vectorKeyFrame[m_nKeyNow].vRot;
		CKFVec3 vRotDifference = vKeyRot - m_vectorPartsInfo[nCntPart].vRot;
		CKFMath::NormalizeRotInPi(&vRotDifference);

		nFrame = vectorMotion[nCntPart][m_motionNow].vectorKeyFrame[m_nKeyNow].nFrame;
		nNumKey = vectorMotion[nCntPart][m_motionNow].vectorKeyFrame.size();
		bLoop = vectorMotion[nCntPart][m_motionNow].bLoop;

		if ((nFrame - m_nCntFrame) != 0)
		{
			vPartPos += (vKeyPos - vPartPos) / (float)(nFrame - m_nCntFrame);
			vPartRot += vRotDifference / (float)(nFrame - m_nCntFrame);
		}

		CKFMath::NormalizeRotInTwoPi(&vPartRot);
	}

	//���[�V�����X�V
	m_nCntFrame++;

	if (m_nCntFrame == nFrame)
	{
		m_nCntFrame = 0;
		m_nKeyNow++;

		if (m_nKeyNow == nNumKey)
		{
			m_nKeyNow = 0;

			if (bLoop != true)
			{
				//SetMotionNext(0);
			}
		}
	}
}
//--------------------------------------------------------------------------------
//
//�@gameObjectCharacter.cpp
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
#include "gameObjectCharacter.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObjectCharacter::CGameObjectCharacter()
{
	m_nCntFrame = 0;
	m_nKeyNow = 0;
	m_motionNow = MOTION_NEUTAL;
	m_vectorPartsInfo.clear();
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CGameObjectCharacter::Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName)
{
	CGameObjectModel::Init(vPos, vRot, modelName);

	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);
	if (pModel != NULL)
	{
		CModelCharacterX* pChar = (CModelCharacterX*)pModel;
		m_vectorPartsInfo = pChar->GetDefaultPartsInfo();
	}
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CGameObjectCharacter::Uninit(void)
{
	CGameObjectModel::Uninit();
	m_vectorPartsInfo.clear();
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CGameObjectCharacter::Update(void)
{
	//���[�V�������擾
	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);
	if (pModel == NULL) { return; }
	CModelCharacterX* pChar = (CModelCharacterX*)pModel;
	const std::vector<CModelCharacterX::VEC_MOTION> &vectorMotion = pChar->GetPartsMotionInfo();

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

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CGameObjectCharacter::LateUpdate(void)
{

}

//--------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------
void CGameObjectCharacter::Draw(void)
{
	//���f���̎擾
	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);
	if (pModel == NULL) { return; }
	CModelCharacterX* pChar = (CModelCharacterX*)pModel;

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
	pChar->Draw(mtxWorld, m_vectorPartsInfo);
}

//--------------------------------------------------------------------------------
//  ��������
//--------------------------------------------------------------------------------
CGameObjectCharacter*	CGameObjectCharacter::Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName)
{
	CGameObjectCharacter *pObjChar = NULL;
	pObjChar = new CGameObjectCharacter;
	pObjChar->Init(vPos, vRot, modelName);
	pObjChar->m_pri = GOM::PRI_3D;
	pObjChar->m_nID = GetManager()->GetGameObjectManager()->SaveGameObj(GOM::PRI_3D, pObjChar);
	return pObjChar;
}
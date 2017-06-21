//--------------------------------------------------------------------------------
//	アクターメッシュコンポネント
//　actorMeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "actorMeshComponent.h"
#include "manager.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// 初期化処理
//--------------------------------------------------------------------------------
KFRESULT CActorMeshComponent::Init(void)
{
	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);
	if (pModel != NULL)
	{
		CModelActorX* pModelActor = (CModelActorX*)pModel;
		m_motionInfo.vectorPartsInfo = pModelActor->GetDefaultPartsInfo();
		SetMotionAtNow(MOTION_NEUTAL);
	}

	return KF_SUCCEEDED;
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void CActorMeshComponent::Uninit(void)
{
	CModelMeshComponent::Uninit();
	m_motionInfo.vectorPartsInfo.clear();
}

//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void CActorMeshComponent::Update(void)
{
	//モーション情報取得
	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);
	if (pModel == NULL) { return; }
	CModelActorX* pModelActor = (CModelActorX*)pModel;
	const std::vector<CModelActorX::VEC_MOTION>& vectorMotion = pModelActor->GetPartsMotionInfo();

	//パーツ更新
	int nFrame;
	int nNumKey;
	bool bLoop;
	int nNumParts = (int)m_motionInfo.vectorPartsInfo.size();
	for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
	{
		CKFVec3& vPartPos = m_motionInfo.vectorPartsInfo[nCntPart].vPos;
		CKFVec3& vPartRot = m_motionInfo.vectorPartsInfo[nCntPart].vRot;
		CKFVec3 vKeyPos = vectorMotion[nCntPart][m_motionInfo.motionNow].vectorKeyFrame[m_motionInfo.nKeyNow].vPos;
		CKFVec3 vKeyRot = vectorMotion[nCntPart][m_motionInfo.motionNow].vectorKeyFrame[m_motionInfo.nKeyNow].vRot;
		CKFVec3 vRotDifference = vKeyRot - vPartRot;
		CKFMath::NormalizeRotInPi(&vRotDifference);

		nFrame = vectorMotion[nCntPart][m_motionInfo.motionNow].vectorKeyFrame[m_motionInfo.nKeyNow].nFrame;
		nNumKey = vectorMotion[nCntPart][m_motionInfo.motionNow].vectorKeyFrame.size();
		bLoop = vectorMotion[nCntPart][m_motionInfo.motionNow].bLoop;

		if ((nFrame - m_motionInfo.nCntFrame) != 0)
		{
			vPartPos += (vKeyPos - vPartPos) / (float)(nFrame - m_motionInfo.nCntFrame);
			vPartRot += vRotDifference / (float)(nFrame - m_motionInfo.nCntFrame);
		}

		CKFMath::NormalizeRotInTwoPi(&vPartRot);
	}
	
	//モーション更新
	m_motionInfo.nCntFrame++;

	if (m_motionInfo.nCntFrame == nFrame)
	{
		m_motionInfo.nCntFrame = 0;
		m_motionInfo.nKeyNow++;

		if (m_motionInfo.nKeyNow == nNumKey)
		{
			m_motionInfo.nKeyNow = 0;

			if (bLoop != true)
			{
				SetMotionNext(m_motionInfo.motionNext);
			}
		}
	}
}

//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void CActorMeshComponent::SetMotionNext(const MOTION& motion)
{
	if (motion >= MOTION_MAX || motion == m_motionInfo.motionNow) { return; }

	switch (motion)
	{
	case MOTION_JUMP:
		m_motionInfo.motionNext = MOTION_LANDING;
		break;
	case MOTION_MOVE:
		if (m_motionInfo.motionNow != MOTION_NEUTAL
			&& m_motionInfo.motionNow != MOTION_MOVE) 
		{
			return;
		}
		m_motionInfo.motionNext = MOTION_NEUTAL;
		break;
	case MOTION_NEUTAL:
		if (m_motionInfo.motionNow != MOTION_NEUTAL
			&& m_motionInfo.motionNow != MOTION_MOVE)
		{
			return;
		}
		m_motionInfo.motionNext = MOTION_NEUTAL;
		break;
	default:
		m_motionInfo.motionNext = MOTION_NEUTAL;
		break;
	}

	m_motionInfo.motionNow = motion;
	m_motionInfo.nCntFrame = 0;
	m_motionInfo.nKeyNow = 0;
}

//--------------------------------------------------------------------------------
// モーション設定
//--------------------------------------------------------------------------------
void CActorMeshComponent::SetMotionAtNow(const MOTION& motion)
{
	m_motionInfo.motionNow = motion;
	m_motionInfo.nCntFrame = 0;
	m_motionInfo.nKeyNow = 0;

	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);
	if (pModel == NULL) { return; }
	CModelActorX* pModelActor = (CModelActorX*)pModel;
	const std::vector<CModelActorX::VEC_MOTION>& vectorMotion = pModelActor->GetPartsMotionInfo();

	//パーツ更新
	int nNumParts = (int)m_motionInfo.vectorPartsInfo.size();
	for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
	{
		CKFVec3& vPartPos = m_motionInfo.vectorPartsInfo[nCntPart].vPos;
		CKFVec3& vPartRot = m_motionInfo.vectorPartsInfo[nCntPart].vRot;
		CKFVec3 vKeyPos = vectorMotion[nCntPart][m_motionInfo.motionNow].vectorKeyFrame[m_motionInfo.nKeyNow].vPos;
		CKFVec3 vKeyRot = vectorMotion[nCntPart][m_motionInfo.motionNow].vectorKeyFrame[m_motionInfo.nKeyNow].vRot;
		
		vPartPos = vKeyPos;
		vPartRot = vKeyRot;
	}
}
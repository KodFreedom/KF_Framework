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
bool CActorMeshComponent::Init(void)
{
	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);
	if (pModel != NULL)
	{
		CModelActorX* pModelActor = (CModelActorX*)pModel;
		m_motionInfo.vectorPartsInfo = pModelActor->GetDefaultPartsInfo();
		SetMotionAtNow(MOTION_NEUTAL);
	}

	return true;
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
	const vector<CModelActorX::VEC_MOTION>& vectorMotion = pModelActor->GetPartsMotionInfo();

	//パーツ更新
	int nFrame = 0;
	int nNumKey = 0;
	bool bLoop = false;
	int nNumParts = (int)m_motionInfo.vectorPartsInfo.size();
	for (int nCntPart = 0; nCntPart < nNumParts; nCntPart++)
	{
		CKFVec3& vPartPos = m_motionInfo.vectorPartsInfo[nCntPart].vPos;
		CKFVec3& vPartRot = m_motionInfo.vectorPartsInfo[nCntPart].vRot;
		CKFVec3 vKeyPos = vectorMotion[nCntPart][m_motionInfo.motionNow].vectorKeyFrame[m_motionInfo.nKeyNow].vPos;
		CKFVec3 vKeyRot = vectorMotion[nCntPart][m_motionInfo.motionNow].vectorKeyFrame[m_motionInfo.nKeyNow].vRot;
		CKFVec3 vRotDifference = vKeyRot - vPartRot;
		CKFMath::NormalizeRotInPi(vRotDifference);

		nFrame = (int)m_status * sc_nChangeFrame + (1 - (int)m_status) * vectorMotion[nCntPart][m_motionInfo.motionNow].vectorKeyFrame[m_motionInfo.nKeyNow].nFrame;
		nNumKey = vectorMotion[nCntPart][m_motionInfo.motionNow].vectorKeyFrame.size();
		bLoop = vectorMotion[nCntPart][m_motionInfo.motionNow].bLoop;

		if ((nFrame - m_motionInfo.nCntFrame) != 0)
		{
			vPartPos += (vKeyPos - vPartPos) / (float)(nFrame - m_motionInfo.nCntFrame);
			vPartRot += vRotDifference / (float)(nFrame - m_motionInfo.nCntFrame);
		}

		CKFMath::NormalizeRotInTwoPi(vPartRot);
	}

	//フレームカウント
	m_motionInfo.nCntFrame++;

	//キーフレーム切り替え
	if (m_motionInfo.nCntFrame == nFrame)
	{
		m_status = MS_NORMAL;
		m_motionInfo.nCntFrame = 0;
		m_motionInfo.nKeyNow++;

		//モーション切り替え
		if (m_motionInfo.nKeyNow == nNumKey)
		{
			m_motionInfo.nKeyNow = 0;

			if (bLoop != true)
			{
				ChangeMotion(m_motionInfo.motionNext);
			}
		}
	}
}

//--------------------------------------------------------------------------------
// モーション設定
//--------------------------------------------------------------------------------
void CActorMeshComponent::SetMotion(const MOTION& motion)
{
	if (!CanChangeMotion(motion)) { return; }
	ChangeMotion(motion);
}

//--------------------------------------------------------------------------------
// モーション設定
//--------------------------------------------------------------------------------
void CActorMeshComponent::SetMotionAtNow(const MOTION& motion)
{
	ChangeMotion(motion);
	m_status = MS_NORMAL;

	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);
	if (pModel == NULL) { return; }
	CModelActorX* pModelActor = (CModelActorX*)pModel;
	const vector<CModelActorX::VEC_MOTION>& vectorMotion = pModelActor->GetPartsMotionInfo();

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

//--------------------------------------------------------------------------------
// モーションの切り替え
//--------------------------------------------------------------------------------
void  CActorMeshComponent::ChangeMotion(const MOTION& motion)
{
	m_status = MS_CHANGE;
	m_motionInfo.motionNow = motion;
	m_motionInfo.nCntFrame = 0;
	m_motionInfo.nKeyNow = 0;

	//Motion Next
	switch (motion)
	{
	case MOTION_JUMP:
		m_motionInfo.motionNext = MOTION_LANDING;
		break;
	default:
		m_motionInfo.motionNext = MOTION_NEUTAL;
		break;
	}
}

//--------------------------------------------------------------------------------
// モーションの変更できるかをチェック
//--------------------------------------------------------------------------------
bool  CActorMeshComponent::CanChangeMotion(const MOTION& motion)
{
	if (m_motionInfo.motionNow == motion) { return false; }

	switch (m_motionInfo.motionNow)
	{
	case MOTION_JUMP:
		return false;
	case MOTION_LANDING:
		if (motion == MOTION_NEUTAL) { return false; }
		return true;
	case MOTION_ATTACK:
		return false;
	default:
		break;
	}

	return true;
}

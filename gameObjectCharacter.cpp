//--------------------------------------------------------------------------------
//
//　gameObjectCharacter.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-15
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "gameObjectCharacter.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObjectCharacter::CGameObjectCharacter()
{
	m_nCntFrame = 0;
	m_nKeyNow = 0;
	m_motionNow = MOTION_NEUTAL;
	m_vectorPartsInfo.clear();
}

//--------------------------------------------------------------------------------
//  初期化処理
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
//  終了処理
//--------------------------------------------------------------------------------
void CGameObjectCharacter::Uninit(void)
{
	CGameObjectModel::Uninit();
	m_vectorPartsInfo.clear();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CGameObjectCharacter::Update(void)
{
	//モーション情報取得
	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);
	if (pModel == NULL) { return; }
	CModelCharacterX* pChar = (CModelCharacterX*)pModel;
	const std::vector<CModelCharacterX::VEC_MOTION> &vectorMotion = pChar->GetPartsMotionInfo();

	//パーツ更新
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

	//モーション更新
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
//  更新処理
//--------------------------------------------------------------------------------
void CGameObjectCharacter::LateUpdate(void)
{

}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CGameObjectCharacter::Draw(void)
{
	//モデルの取得
	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);
	if (pModel == NULL) { return; }
	CModelCharacterX* pChar = (CModelCharacterX*)pModel;

	//ワールド相対座標
	CKFMtx44 mtxWorld;
	CKFMtx44 mtxRot;
	CKFMtx44 mtxPos;

	//単位行列に初期化
	CKFMath::MtxIdentity(&mtxWorld);

	//回転(Y->X->Z)
	CKFMath::MtxRotationYawPitchRoll(&mtxRot, m_vRot);
	mtxWorld *= mtxRot;

	//平行移動
	CKFMath::MtxTranslation(&mtxPos, m_vPos);
	mtxWorld *= mtxPos;

	//描画
	pChar->Draw(mtxWorld, m_vectorPartsInfo);
}

//--------------------------------------------------------------------------------
//  生成処理
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
//--------------------------------------------------------------------------------
//	
//　motionStatus.cpp
//	Author : Xu Wenjie
//	Date   : 2017-09-20
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "motionStatus.h"
#include "animatorComponent.h"
#include "motionInfo.h"
#include "gameObject.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  CNormalMotionStatus
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Update
//--------------------------------------------------------------------------------
void CNormalMotionStatus::Update(CAnimatorComponent& animator)
{
	auto pMotion = animator.m_apMotionData[animator.m_motionNow];
	auto nNumFrame = pMotion->m_vecMotionKey[m_nKeyNow].m_nFrame;
	auto itrNodeKey = pMotion->m_vecMotionKey[m_nKeyNow].m_listNodesKey.begin();
	auto itrNodeKeyNext = pMotion->m_vecMotionKey[(m_nKeyNow + 1) % (int)pMotion->m_vecMotionKey.size()].m_listNodesKey.begin();
	float fRate = (float)m_nFrameNow / nNumFrame;

	for (auto pObj : animator.m_vecBorns)
	{//Nodeごと位置回転更新
		auto pTrans = pObj->GetTransformComponent();
		auto& vPosNew = CKFMath::LerpVec3(itrNodeKey->c_vPos, itrNodeKeyNext->c_vPos, fRate);
		auto& qRotNew = CKFMath::SlerpQuaternion(itrNodeKey->c_qRot, itrNodeKeyNext->c_qRot, fRate);
		pTrans->SetPosNext(vPosNew);
		pTrans->SetRotNext(qRotNew);
		++itrNodeKey;
		++itrNodeKeyNext;
	}

	//フレームカウント
	m_nFrameNow++;

	//キーフレーム切り替え
	if (m_nFrameNow == nNumFrame)
	{
		m_nFrameNow = 0;
		m_nKeyNow = (m_nKeyNow + 1) % (int)pMotion->m_vecMotionKey.size();

		if (m_nKeyNow == (int)pMotion->m_vecMotionKey.size() - 1
			&& !pMotion->m_bLoop)
		{//モーション切り替え
			ChangeMotion(animator.m_motionNext);
		}
	}
}

//--------------------------------------------------------------------------------
//  ChangeMotion
//--------------------------------------------------------------------------------
void CNormalMotionStatus::ChangeMotion(const MOTION_PATTERN& motion)
{
	//if (m_status == MS_BLEND) { return; }
	//if (!checkCanChange(motion)) { return; }

	////Motion State
	//auto pMotionInfo = m_apMotionData[m_motionNow];
	//if (pMotionInfo->m_bLoop
	//	|| m_nKeyNow < (int)pMotionInfo->m_vecMotionKey.size() - 1)
	//{
	//	m_status = MS_BLEND;
	//	m_motionBlend = motion;
	//	m_nBlendKey = 0;
	//	m_nCntBlendFrame = 0;
	//	m_nBlendFrame = pMotionInfo->m_vecMotionKey[m_nKeyNow].m_nFrame - m_nCntFrame;
	//}
}

//--------------------------------------------------------------------------------
//
//  CAwaitMotionStatus
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ChangeMotion
//--------------------------------------------------------------------------------
void CAwaitMotionStatus::ChangeMotion(const MOTION_PATTERN& motion)
{

}

//--------------------------------------------------------------------------------
//
//  CBlendMotionStatus
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Update
//--------------------------------------------------------------------------------
void CBlendMotionStatus::Update(CAnimatorComponent& animator)
{
	auto pMotion = animator.m_apMotionData[animator.m_motionNow];
	auto nFrame = pMotion->m_vecMotionKey[m_nKeyNow].m_nFrame;
	auto itrNodeKey = pMotion->m_vecMotionKey[m_nKeyNow].m_listNodesKey.begin();
	auto itrNodeKeyNext = pMotion->m_vecMotionKey[(m_nKeyNow + 1) % (int)pMotion->m_vecMotionKey.size()].m_listNodesKey.begin();
	float fRate = (float)m_nFrameNow / nFrame;

	auto pBlendMotion = animator.m_apMotionData[animator.m_motionNext];
	auto nBlendFrame = pBlendMotion->m_vecMotionKey[m_nBlendKeyNow].m_nFrame;
	auto itrBlendKey = pBlendMotion->m_vecMotionKey[m_nBlendKeyNow].m_listNodesKey.begin();
	auto itrBlendKeyNext = pBlendMotion->m_vecMotionKey[m_nBlendKeyNow + 1].m_listNodesKey.begin();
	float fBlendRate = (float)m_nBlendFrameNow / nBlendFrame;
	float fMotionBlendRate = (float)m_nBlendFrameNow / sc_nBlendFrame;

	for (auto pObj : animator.m_vecBorns)
	{//Nodeごと位置回転更新
		auto pTrans = pObj->GetTransformComponent();
		auto vPosNew = CKFMath::LerpVec3(itrNodeKey->c_vPos, itrNodeKeyNext->c_vPos, fRate);
		auto qRotNew = CKFMath::SlerpQuaternion(itrNodeKey->c_qRot, itrNodeKeyNext->c_qRot, fRate);
		auto vBlendPosNew = CKFMath::LerpVec3(itrBlendKey->c_vPos, itrBlendKeyNext->c_vPos, fBlendRate);
		auto qBlendRotNew = CKFMath::SlerpQuaternion(itrBlendKey->c_qRot, itrBlendKeyNext->c_qRot, fBlendRate);
		auto vTruePos = vPosNew * (1.0f - fMotionBlendRate) + vBlendPosNew * fMotionBlendRate;
		auto qTrueRot = qRotNew * (1.0f - fMotionBlendRate) + qBlendRotNew * fMotionBlendRate;
		pTrans->SetPosNext(vTruePos);
		pTrans->SetRotNext(qTrueRot);
		++itrNodeKey;
		++itrNodeKeyNext;
		++itrBlendKey;
		++itrBlendKeyNext;
	}

	++m_nFrameNow;
	++m_nBlendFrameNow;

	if (m_nBlendFrameNow == m_nBlendFrame)
	{//モーションステータスの切り替え
		animator.m_motionNow = animator.m_motionNext;
		animator.changeMotionStatus(new CNormalMotionStatus(m_nBlendKeyNow, m_nBlendFrameNow));
		animator.m_motionNext = animator.getMotionNext(animator.m_motionNow);
	}
}

//--------------------------------------------------------------------------------
//  ChangeMotion
//--------------------------------------------------------------------------------
void CBlendMotionStatus::ChangeMotion(const MOTION_PATTERN& motion)
{

}
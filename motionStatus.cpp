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
	++m_nFrameNow;

	if (m_nFrameNow == nNumFrame)
	{//キーフレーム切り替え
		m_nFrameNow = 0;
		++m_nKeyNow;

		if (m_nKeyNow == (int)pMotion->m_vecMotionKey.size())
		{
			m_nKeyNow = 0;

			if (!pMotion->m_bLoop)
			{//モーション切り替え
				animator.changeMotionStatus(new CAwaitMotionStatus(m_nKeyNow, m_nFrameNow));
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  ChangeMotion
//--------------------------------------------------------------------------------
void CNormalMotionStatus::ChangeMotion(CAnimatorComponent& animator, const MOTION_PATTERN& motion)
{
	if (!checkCanChange(animator, motion)) { return; }
	animator.m_motionNext = motion;
	animator.changeMotionStatus(new CBlendMotionStatus(m_nKeyNow, m_nFrameNow));
}

//--------------------------------------------------------------------------------
//  checkCanChange
//--------------------------------------------------------------------------------
bool CNormalMotionStatus::checkCanChange(CAnimatorComponent& animator, const MOTION_PATTERN& motion)
{
	if (animator.m_motionNow == motion) { return false; }

	switch (animator.m_motionNow)
	{
	case MP_NEUTAL:
		return true;
	case MP_RUN:
		return true;
	case MP_JUMP:
		if (motion == MP_LAND && animator.m_bIsGrounded) { return true; }
		return false;
	case MP_LAND:
		if (motion == MP_NEUTAL) { return false; }
		return true;
	case MP_ATTACK:
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------
//
//  CAwaitMotionStatus
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Update
//--------------------------------------------------------------------------------
void CAwaitMotionStatus::Update(CAnimatorComponent& animator)
{
	ChangeMotion(animator, animator.m_motionNext);
}

//--------------------------------------------------------------------------------
//  ChangeMotion
//--------------------------------------------------------------------------------
void CAwaitMotionStatus::ChangeMotion(CAnimatorComponent& animator, const MOTION_PATTERN& motion)
{
	if (!checkCanChange(animator, motion)) { return; }
	animator.m_motionNext = motion;
	animator.changeMotionStatus(new CBlendMotionStatus(m_nKeyNow, m_nFrameNow));
}

//--------------------------------------------------------------------------------
//  checkCanChange
//--------------------------------------------------------------------------------
bool CAwaitMotionStatus::checkCanChange(CAnimatorComponent& animator, const MOTION_PATTERN& motion)
{
	if (animator.m_motionNow == motion) { return false; }

	switch (animator.m_motionNow)
	{
	case MP_JUMP:
		if (motion == MP_LAND && animator.m_bIsGrounded) { return true; }
		return false;
	}

	return true;
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
	auto nNumFrame = pMotion->m_vecMotionKey[m_nKeyNow].m_nFrame;
	auto itrNodeKey = pMotion->m_vecMotionKey[m_nKeyNow].m_listNodesKey.begin();
	auto itrNodeKeyNext = pMotion->m_vecMotionKey[(m_nKeyNow + 1) % (int)pMotion->m_vecMotionKey.size()].m_listNodesKey.begin();
	float fRate = (float)m_nFrameNow / nNumFrame;

	auto pBlendMotion = animator.m_apMotionData[animator.m_motionNext];
	auto nNumBlendFrame = pBlendMotion->m_vecMotionKey[m_nBlendKeyNow].m_nFrame;
	auto itrBlendKey = pBlendMotion->m_vecMotionKey[m_nBlendKeyNow].m_listNodesKey.begin();
	auto itrBlendKeyNext = pBlendMotion->m_vecMotionKey[m_nBlendKeyNow + 1].m_listNodesKey.begin();
	float fBlendRate = (float)m_nBlendFrameNow / nNumBlendFrame;
	float fMotionBlendRate = (float)m_nBlendFrameCnt / sc_nBlendFrame;

	for (auto pObj : animator.m_vecBorns)
	{//Nodeごと位置回転更新
		Vector3 vPosNew;
		Quaternion qRotNew;

		if (!pMotion->m_bLoop)
		{//ループしないなら最後の1フレームのままにする
			vPosNew = itrNodeKey->c_vPos;
			qRotNew = itrNodeKey->c_qRot;
		}
		else
		{
			vPosNew = CKFMath::LerpVec3(itrNodeKey->c_vPos, itrNodeKeyNext->c_vPos, fRate);
			qRotNew = CKFMath::SlerpQuaternion(itrNodeKey->c_qRot, itrNodeKeyNext->c_qRot, fRate);
		}
		
		auto vBlendPosNew = CKFMath::LerpVec3(itrBlendKey->c_vPos, itrBlendKeyNext->c_vPos, fBlendRate);
		auto qBlendRotNew = CKFMath::SlerpQuaternion(itrBlendKey->c_qRot, itrBlendKeyNext->c_qRot, fBlendRate);
		auto vTruePos = vPosNew * (1.0f - fMotionBlendRate) + vBlendPosNew * fMotionBlendRate;
		auto qTrueRot = qRotNew * (1.0f - fMotionBlendRate) + qBlendRotNew * fMotionBlendRate;
		
		auto pTrans = pObj->GetTransformComponent();
		pTrans->SetPosNext(vTruePos);
		pTrans->SetRotNext(qTrueRot);
		++itrNodeKey;
		++itrNodeKeyNext;
		++itrBlendKey;
		++itrBlendKeyNext;
	}

	if(pMotion->m_bLoop)
	{
		++m_nFrameNow; 

		if (m_nFrameNow == nNumFrame)
		{//キーフレーム切り替え
			m_nFrameNow = 0;
			m_nKeyNow = (m_nKeyNow + 1) % (int)pMotion->m_vecMotionKey.size();
		}
	}
	
	++m_nBlendFrameNow;
	++m_nBlendFrameCnt;

	if (m_nBlendFrameNow == nNumBlendFrame)
	{//キーフレーム切り替え
		m_nBlendFrameNow = 0;
		++m_nBlendKeyNow;
	}

	if (m_nBlendFrameCnt == sc_nBlendFrame)
	{//モーションステータスの切り替え
		animator.m_motionNow = animator.m_motionNext;
		animator.changeMotionStatus(new CNormalMotionStatus(m_nBlendKeyNow, m_nBlendFrameNow));
		animator.m_motionNext = animator.getMotionNext(animator.m_motionNow);
	}
}

//--------------------------------------------------------------------------------
//  ChangeMotion
//--------------------------------------------------------------------------------
void CBlendMotionStatus::ChangeMotion(CAnimatorComponent& animator, const MOTION_PATTERN& motion)
{
	if (!checkCanChange(animator, motion)) { return; }
	animator.m_motionNow = animator.m_motionNext;
	animator.m_motionNext = motion;
	animator.changeMotionStatus(new CBlendMotionStatus(m_nBlendKeyNow, m_nBlendFrameNow));
}

//--------------------------------------------------------------------------------
//  checkCanChange
//--------------------------------------------------------------------------------
bool CBlendMotionStatus::checkCanChange(CAnimatorComponent& animator, const MOTION_PATTERN& motion)
{
	if (animator.m_motionNow == motion) { return false; }

	switch (animator.m_motionNow)
	{
	case MP_LAND:
		if (motion == MP_JUMP && animator.m_bIsGrounded) { return true; }
		return false;
	}

	return false;
}
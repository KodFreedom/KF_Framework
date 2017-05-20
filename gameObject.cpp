//--------------------------------------------------------------------------------
//
//　gameObject.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "gameObject.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
CGameObject::CGameObject(const GOM::PRIORITY &pri = GOM::PRI_3D)
	: m_pri(pri)
	, m_bActive(true)
	, m_bAlive(true)
	, m_pInput(&s_nullInput)
	, m_pPhysic(&s_nullPhysic)
	, m_pCollision(&s_nullCollision)
	, m_pMesh(&s_nullMesh)
	, m_pDraw(&s_nullDraw)
	, m_vPos(CKFVec3(0.0f))
	, m_vPosNext(CKFVec3(0.0f))
	, m_vRot(CKFVec3(0.0f))
	, m_vRotNext(CKFVec3(0.0f))
	, m_vScale(CKFVec3(0.0f))
	, m_vScaleNext(CKFVec3(0.0f))
{
	GetManager()->GetGameObjectManager()->SaveGameObj(m_pri, this);
}

//--------------------------------------------------------------------------------
//  破棄処理
//--------------------------------------------------------------------------------
void CGameObject::Release(void)
{
	GetManager()->GetGameObjectManager()->ReleaseGameObj(m_pri, this);
}

//--------------------------------------------------------------------------------
//  パラメーター交換処理
//--------------------------------------------------------------------------------
void CGameObject::SwitchParam(void)
{
	m_vPos = m_vPosNext;
	m_vRot = m_vRotNext;
	m_vScale = m_vScaleNext;
}
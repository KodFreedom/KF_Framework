//--------------------------------------------------------------------------------
//	コリジョンコンポネント
//　collisionComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-04
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "colliderComponent.h"
#include "manager.h"
#include "gameObject.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CColliderComponent::CColliderComponent(CGameObject* const pGameObj, const CS::COL_TYPE& type, const CS::COL_MODE& mode)
	: CComponent(pGameObj)
	, m_type(type)
	, m_mode(mode)
	, m_bTrigger(false)
{
	CKFMath::MtxIdentity(m_mtxWorld);
	CKFMath::MtxIdentity(m_mtxOffset);
	GetManager()->GetCollisionSystem()->RegisterCollider(m_mode, m_type, this);
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CColliderComponent::Uninit(void)
{
	GetManager()->GetCollisionSystem()->DeregisterCollider(m_mode, m_type, this);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CColliderComponent::Update(void)
{
	m_mtxWorld = m_mtxOffset;
	m_mtxWorld *= m_pGameObj->GetTransformComponent()->GetMatrixWorldNext();
}

//--------------------------------------------------------------------------------
//  Offset設定
//--------------------------------------------------------------------------------
void CColliderComponent::SetOffset(const CKFVec3& vPos, const CKFVec3& vRot)
{
	CKFMath::MtxRotationYawPitchRoll(m_mtxOffset, vRot);
	m_mtxOffset.m_af[3][0] = vPos.m_fX;
	m_mtxOffset.m_af[3][1] = vPos.m_fY;
	m_mtxOffset.m_af[3][2] = vPos.m_fZ;
}

//--------------------------------------------------------------------------------
//  Offset設定
//--------------------------------------------------------------------------------
void CColliderComponent::Sleep(void)
{
	GetManager()->GetCollisionSystem()->DeregisterCollider(m_mode, m_type, this);
}

//--------------------------------------------------------------------------------
//  Offset設定
//--------------------------------------------------------------------------------
void CColliderComponent::Awake(void)
{
	GetManager()->GetCollisionSystem()->RegisterCollider(m_mode, m_type, this);
}
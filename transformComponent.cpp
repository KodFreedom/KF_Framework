//--------------------------------------------------------------------------------
//	トランスフォームコンポネント
//　transformComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "transformComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CTransformComponent::CTransformComponent(CGameObject* const pGameObj) : CComponent(pGameObj)
	, m_vPos(CKFVec3(0.0f))
	, m_vPosNext(CKFVec3(0.0f))
	, m_vScale(CKFVec3(0.0f))
	, m_vScaleNext(CKFVec3(0.0f))
	, m_vForward(CKFVec3(0.0f, 0.0f, 1.0f))
	, m_vUp(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vRight(CKFVec3(1.0f, 0.0f, 0.0f))
	, m_vForwardNext(CKFVec3(0.0f, 0.0f, 1.0f))
	, m_vUpNext(CKFVec3(0.0f, 1.0f, 0.0f))
	, m_vRightNext(CKFVec3(1.0f, 0.0f, 0.0f))
	, m_pParent(nullptr)
{
	m_listChildren.clear();
	CKFMath::MtxIdentity(m_mtxThis);
}

//--------------------------------------------------------------------------------
//  マトリクス算出(親がないの場合呼び出される)
//--------------------------------------------------------------------------------
void  CTransformComponent::UpdateMatrix(void)
{
	CalculateMtxThis();

	//子供たちの行列も算出
	for (auto itr = m_listChildren.begin(); itr != m_listChildren.end(); itr++)
	{
		(*itr)->UpdateMatrix(m_mtxThis);
	}
}

//--------------------------------------------------------------------------------
//  マトリクス算出(親があるの場合呼び出される)
//--------------------------------------------------------------------------------
void  CTransformComponent::UpdateMatrix(const CKFMtx44& mtxParent)
{
	CalculateMtxThis(mtxParent);

	//子供たちの行列も算出
	for (auto itr = m_listChildren.begin(); itr != m_listChildren.end(); itr++)
	{
		(*itr)->UpdateMatrix(m_mtxThis);
	}
}

//--------------------------------------------------------------------------------
//  パラメーター交換処理
//--------------------------------------------------------------------------------
void CTransformComponent::SwapParam(void)
{
	m_vPos = m_vPosNext;
	//m_vRot = m_vRotNext;
	m_vScale = m_vScaleNext;
	m_vForward = m_vForwardNext;
	m_vRight = m_vRightNext;
	m_vUp = m_vUpNext;
}

//--------------------------------------------------------------------------------
//  Childリストに追加
//--------------------------------------------------------------------------------
void CTransformComponent::RegisterChild(CTransformComponent* pChild)
{
	m_listChildren.push_back(pChild);
}

//--------------------------------------------------------------------------------
//  Childリストから削除
//--------------------------------------------------------------------------------
void CTransformComponent::DeregisterChild(CTransformComponent* pChild)
{
	m_listChildren.remove(pChild);
}

//--------------------------------------------------------------------------------
//  パラメーター交換処理
//--------------------------------------------------------------------------------
void CTransformComponent::RegisterParent(CTransformComponent* pParent, const CKFVec3& vOffsetPos, const CKFVec3& vOffsetRot)
{
	if (m_pParent)
	{//親があるの場合前の親から削除
		m_pParent->DeregisterChild(this);
	}
	m_pParent = pParent;
	m_vOffsetPos = vOffsetPos;
	m_vOffsetRot = vOffsetRot;
}

//--------------------------------------------------------------------------------
//  パラメーター交換処理
//--------------------------------------------------------------------------------
CKFQuaternion CTransformComponent::GetRot(void)
{
	CKFMtx44 mtxRot = GetMatrixRot();
	CKFQuaternion qRot = CKFMath::MtxToQuaternion(mtxRot);
	return qRot;
}

//--------------------------------------------------------------------------------
//  パラメーター交換処理
//--------------------------------------------------------------------------------
CKFQuaternion CTransformComponent::GetRotNext(void)
{
	CKFMtx44 mtxRot = GetMatrixRotNext();
	CKFQuaternion qRot = CKFMath::MtxToQuaternion(mtxRot);
	return qRot;
}

//--------------------------------------------------------------------------------
//	回転行列の取得
//--------------------------------------------------------------------------------
CKFMtx44 CTransformComponent::GetMatrixRot(void)
{
	CKFMtx44 mtxRot;
	mtxRot.m_af[0][0] = m_vRight.m_fX;
	mtxRot.m_af[0][1] = m_vRight.m_fY;
	mtxRot.m_af[0][2] = m_vRight.m_fZ;
	mtxRot.m_af[1][0] = m_vUp.m_fX;
	mtxRot.m_af[1][1] = m_vUp.m_fY;
	mtxRot.m_af[1][2] = m_vUp.m_fZ;
	mtxRot.m_af[2][0] = m_vForward.m_fX;
	mtxRot.m_af[2][1] = m_vForward.m_fY;
	mtxRot.m_af[2][2] = m_vForward.m_fZ;
	return mtxRot;
}

//--------------------------------------------------------------------------------
//	回転行列の取得
//--------------------------------------------------------------------------------
CKFMtx44 CTransformComponent::GetMatrixRotNext(void)
{
	CKFMtx44 mtxRot;
	mtxRot.m_af[0][0] = m_vRightNext.m_fX;
	mtxRot.m_af[0][1] = m_vRightNext.m_fY;
	mtxRot.m_af[0][2] = m_vRightNext.m_fZ;
	mtxRot.m_af[1][0] = m_vUpNext.m_fX;
	mtxRot.m_af[1][1] = m_vUpNext.m_fY;
	mtxRot.m_af[1][2] = m_vUpNext.m_fZ;
	mtxRot.m_af[2][0] = m_vForwardNext.m_fX;
	mtxRot.m_af[2][1] = m_vForwardNext.m_fY;
	mtxRot.m_af[2][2] = m_vForwardNext.m_fZ;
	return mtxRot;
}

//--------------------------------------------------------------------------------
//	行列の取得
//--------------------------------------------------------------------------------
CKFMtx44 CTransformComponent::GetMatrixNext(void)
{
	//マトリクス算出
	CKFMtx44 mtxWorld;

	//回転
	mtxWorld = GetMatrixRotNext();

	//平行移動
	mtxWorld.m_af[3][0] = m_vPosNext.m_fX;
	mtxWorld.m_af[3][1] = m_vPosNext.m_fY;
	mtxWorld.m_af[3][2] = m_vPosNext.m_fZ;

	//親のマトリクス取得
	if (m_pParent)
	{
		mtxWorld *= m_pParent->GetMatrixNext();
	}

	return mtxWorld;
}

//--------------------------------------------------------------------------------
//  上方向により回転
//--------------------------------------------------------------------------------
void CTransformComponent::RotByUp(const CKFVec3& vUp)
{
	m_vUpNext = vUp;
	m_vRightNext = m_vUpNext * m_vForwardNext;
	m_vForwardNext = m_vRightNext * m_vUpNext;
	CKFMath::VecNormalize(m_vUpNext);
	CKFMath::VecNormalize(m_vForwardNext);
	CKFMath::VecNormalize(m_vRightNext);
}

//--------------------------------------------------------------------------------
//  前方向により回転
//--------------------------------------------------------------------------------
void CTransformComponent::RotByForward(const CKFVec3& vForward)
{
	m_vForwardNext = vForward;
	m_vUpNext = m_vForwardNext * m_vRightNext;
	m_vRightNext = m_vUpNext * m_vForwardNext;
	CKFMath::VecNormalize(m_vUpNext);
	CKFMath::VecNormalize(m_vForwardNext);
	CKFMath::VecNormalize(m_vRightNext);
}

//--------------------------------------------------------------------------------
//  右方向により回転
//--------------------------------------------------------------------------------
void CTransformComponent::RotByRight(const CKFVec3& vRight)
{
	m_vRightNext = vRight;
	m_vForwardNext = m_vRightNext * m_vUpNext;
	m_vUpNext = m_vForwardNext * m_vRightNext;
	CKFMath::VecNormalize(m_vUpNext);
	CKFMath::VecNormalize(m_vForwardNext);
	CKFMath::VecNormalize(m_vRightNext);
}

//--------------------------------------------------------------------------------
//  ZXY回転
//--------------------------------------------------------------------------------
void CTransformComponent::RotByEuler(const CKFVec3& vRot)
{
	CKFMtx44 mtxRot;
	CKFMath::MtxRotationYawPitchRoll(mtxRot, vRot);
	CKFMath::Vec3TransformNormal(m_vUpNext, mtxRot);
	CKFMath::Vec3TransformNormal(m_vForwardNext, mtxRot);
	CKFMath::Vec3TransformNormal(m_vRightNext, mtxRot);
}

//--------------------------------------------------------------------------------
//  X軸(vVecRight)回転
//--------------------------------------------------------------------------------
void CTransformComponent::RotByPitch(const float& fRadian)
{
	CKFMtx44 mtxPitch;
	CKFMath::MtxRotAxis(mtxPitch, m_vRightNext, fRadian);
	CKFMath::Vec3TransformNormal(m_vUpNext, mtxPitch);
	CKFMath::Vec3TransformNormal(m_vForwardNext, mtxPitch);
}

//--------------------------------------------------------------------------------
//  Y軸(vVecUp)回転
//--------------------------------------------------------------------------------
void CTransformComponent::RotByYaw(const float& fRadian)
{
	CKFMtx44 mtxYaw;
	CKFMath::MtxRotAxis(mtxYaw, m_vUpNext, fRadian);
	CKFMath::Vec3TransformNormal(m_vRightNext, mtxYaw);
	CKFMath::Vec3TransformNormal(m_vForwardNext, mtxYaw);
}

//--------------------------------------------------------------------------------
//  Z軸(vLook)回転
//--------------------------------------------------------------------------------
void CTransformComponent::RotByRoll(const float& fRadian)
{
	CKFMtx44 mtxRoll;
	CKFMath::MtxRotAxis(mtxRoll, m_vForwardNext, fRadian);
	CKFMath::Vec3TransformNormal(m_vForwardNext, mtxRoll);
	CKFMath::Vec3TransformNormal(m_vRightNext, mtxRoll);
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  マトリクス算出
//--------------------------------------------------------------------------------
void  CTransformComponent::CalculateMtxThis(void)
{
	//単位行列に初期化
	CKFMath::MtxIdentity(m_mtxThis);

	//回転
	m_mtxThis *= GetMatrixRot();

	//平行移動
	CKFMtx44 mtxPos;
	CKFMath::MtxTranslation(mtxPos, m_vPos);
	m_mtxThis *= mtxPos;
}

//--------------------------------------------------------------------------------
//  マトリクス算出(親子関係)
//--------------------------------------------------------------------------------
void  CTransformComponent::CalculateMtxThis(const CKFMtx44& mtxParent)
{
	//自分の行列算出
	CalculateMtxThis();

	//親に対する相対位置行列
	//回転
	CKFMtx44 mtxRot;
	CKFMath::MtxRotationYawPitchRoll(mtxRot, m_vOffsetRot);
	m_mtxThis *= mtxRot;

	//平行移動
	CKFMtx44 mtxPos;
	CKFMath::MtxTranslation(mtxPos, m_vOffsetPos);
	m_mtxThis *= mtxPos;

	//親の行列とかける
	m_mtxThis *= mtxParent;
}
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
	, m_vPos(CKFMath::sc_vZero)
	, m_vPosNext(CKFMath::sc_vZero)
	, m_vScale(CKFMath::sc_vZero)
	, m_vScaleNext(CKFMath::sc_vZero)
	, m_vForward(CKFMath::sc_vForward)
	, m_vUp(CKFMath::sc_vUp)
	, m_vRight(CKFMath::sc_vRight)
	, m_vForwardNext(CKFMath::sc_vForward)
	, m_vUpNext(CKFMath::sc_vUp)
	, m_vRightNext(CKFMath::sc_vRight)
	, m_pParent(nullptr)
{
	m_listChildren.clear();
	CKFMath::MtxIdentity(m_mtxThis);
}

//--------------------------------------------------------------------------------
//	関数名：UpdateMatrix
//  関数説明：マトリクス算出(親がないの場合呼び出される)
//	引数：	なし
//	戻り値：なし
//--------------------------------------------------------------------------------
void  CTransformComponent::UpdateMatrix(void)
{
	//親がある場合計算しない
	if (m_pParent) { return; }

	calculateMtxThis();

	//子供たちの行列も算出
	for (auto pChild : m_listChildren)
	{
		pChild->UpdateMatrix(m_mtxThis);
	}
}

//--------------------------------------------------------------------------------
//	関数名：UpdateMatrix
//  関数説明：マトリクス算出(親があるの場合呼び出される)
//	引数：	mtxParent
//	戻り値：なし
//--------------------------------------------------------------------------------
void  CTransformComponent::UpdateMatrix(const CKFMtx44& mtxParent)
{
	calculateMtxThis(mtxParent);

	//子供たちの行列も算出
	for (auto pChild : m_listChildren)
	{
		pChild->UpdateMatrix(m_mtxThis);
	}
}

//--------------------------------------------------------------------------------
//	関数名：UpdateMatrix
//  関数説明：パラメーター交換処理
//	引数：	なし
//	戻り値：なし
//--------------------------------------------------------------------------------
void CTransformComponent::SwapParam(void)
{
	m_vPos = m_vPosNext;
	m_vScale = m_vScaleNext;
	m_vForward = m_vForwardNext;
	m_vRight = m_vRightNext;
	m_vUp = m_vUpNext;
}

//--------------------------------------------------------------------------------
//	関数名：RegisterChild
//  関数説明：Childリストに追加
//	引数：	pChild
//	戻り値：なし
//--------------------------------------------------------------------------------
void CTransformComponent::RegisterChild(CTransformComponent* pChild)
{
	m_listChildren.push_back(pChild);
}

//--------------------------------------------------------------------------------
//	関数名：RegisterChild
//  関数説明：Childリストから削除
//	引数：	pChild
//	戻り値：なし
//--------------------------------------------------------------------------------
void CTransformComponent::DeregisterChild(CTransformComponent* pChild)
{
	m_listChildren.remove(pChild);
}

//--------------------------------------------------------------------------------
//	関数名：RegisterParent
//  関数説明：親の追加
//	引数：	pParent
//			vOffsetPos
//			vOffsetRot
//	戻り値：なし
//--------------------------------------------------------------------------------
void CTransformComponent::RegisterParent(CTransformComponent* pParent, const CKFVec3& vOffsetPos, const CKFVec3& vOffsetRot)
{
	if (m_pParent)
	{//親があるの場合前の親から削除
		m_pParent->DeregisterChild(this);
	}
	m_pParent = pParent;
	m_pParent->RegisterChild(this);
	m_vOffsetPos = vOffsetPos;
	m_vOffsetRot = vOffsetRot;
}

//--------------------------------------------------------------------------------
//	関数名：GetRot
//  関数説明：今のフレームの回転の取得
//	引数：	なし
//	戻り値：CKFQuaternion
//--------------------------------------------------------------------------------
const CKFQuaternion CTransformComponent::GetRot(void) const
{
	return CKFMath::MtxToQuaternion(GetMatrixRot());
}

//--------------------------------------------------------------------------------
//	関数名：GetRotNext
//  関数説明：次のフレームの回転の取得
//	引数：	なし
//	戻り値：CKFQuaternion
//--------------------------------------------------------------------------------
const CKFQuaternion CTransformComponent::GetRotNext(void) const
{
	return CKFMath::MtxToQuaternion(GetMatrixRotNext());
}

//--------------------------------------------------------------------------------
//	関数名：GetMatrixRot
//  関数説明：今のフレームの回転行列の取得
//	引数：	なし
//	戻り値：CKFMtx44
//--------------------------------------------------------------------------------
const CKFMtx44 CTransformComponent::GetMatrixRot(void) const
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
//	関数名：GetMatrixRotNext
//  関数説明：次のフレームの回転行列の取得
//	引数：	なし
//	戻り値：CKFMtx44
//--------------------------------------------------------------------------------
const CKFMtx44 CTransformComponent::GetMatrixRotNext(void) const
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
//	関数名：GetMatrixWorldNext
//  関数説明：次のフレームの世界行列の取得
//	引数：	なし
//	戻り値：CKFMtx44
//--------------------------------------------------------------------------------
const CKFMtx44 CTransformComponent::GetMatrixWorldNext(void) const
{
	//回転
	auto mtxWorld = GetMatrixRotNext();

	//平行移動
	mtxWorld.m_af[3][0] = m_vPosNext.m_fX;
	mtxWorld.m_af[3][1] = m_vPosNext.m_fY;
	mtxWorld.m_af[3][2] = m_vPosNext.m_fZ;

	//親のマトリクス取得
	if (m_pParent)
	{
		//親に対する相対位置行列
		//回転
		CKFMtx44 mtxRot;
		CKFMath::MtxRotationYawPitchRoll(mtxRot, m_vOffsetRot);
		mtxWorld *= mtxRot;

		//平行移動
		CKFMtx44 mtxPos;
		CKFMath::MtxTranslation(mtxPos, m_vOffsetPos);
		mtxWorld *= mtxPos;

		mtxWorld *= m_pParent->GetMatrixWorldNext();
	}

	return mtxWorld;
}

//--------------------------------------------------------------------------------
//	関数名：SetRot
//  関数説明：今の回転の設定
//	引数：	qRot：回転情報
//	戻り値：なし
//--------------------------------------------------------------------------------
void CTransformComponent::SetRot(const CKFQuaternion& qRot)
{
	auto mtxRot = CKFMath::QuaternionToMtx(qRot);
	m_vRight.m_fX = mtxRot.m_af[0][0];
	m_vRight.m_fY = mtxRot.m_af[0][1];
	m_vRight.m_fZ = mtxRot.m_af[0][2];
	m_vUp.m_fX = mtxRot.m_af[1][0];
	m_vUp.m_fY = mtxRot.m_af[1][1];
	m_vUp.m_fZ = mtxRot.m_af[1][2];
	m_vForward.m_fX = mtxRot.m_af[2][0];
	m_vForward.m_fY = mtxRot.m_af[2][1];
	m_vForward.m_fZ = mtxRot.m_af[2][2];
}

//--------------------------------------------------------------------------------
//	関数名：SetRotNext
//  関数説明：次の回転の設定
//	引数：	qRotNext：回転情報
//	戻り値：なし
//--------------------------------------------------------------------------------
void CTransformComponent::SetRotNext(const CKFQuaternion& qRotNext)
{
	auto mtxRot = CKFMath::QuaternionToMtx(qRotNext);
	m_vRightNext.m_fX = mtxRot.m_af[0][0];
	m_vRightNext.m_fY = mtxRot.m_af[0][1];
	m_vRightNext.m_fZ = mtxRot.m_af[0][2];
	m_vUpNext.m_fX = mtxRot.m_af[1][0];
	m_vUpNext.m_fY = mtxRot.m_af[1][1];
	m_vUpNext.m_fZ = mtxRot.m_af[1][2];
	m_vForwardNext.m_fX = mtxRot.m_af[2][0];
	m_vForwardNext.m_fY = mtxRot.m_af[2][1];
	m_vForwardNext.m_fZ = mtxRot.m_af[2][2];
}

//--------------------------------------------------------------------------------
//	関数名：RotByUp
//  関数説明：上方向により回転
//	引数：	vUp：上方向
//	戻り値：なし
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
//	関数名：RotByForward
//  関数説明：前方向により回転
//	引数：	vForward：前方向
//	戻り値：なし
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
//	関数名：RotByRight
//  関数説明：右方向により回転
//	引数：	vRight：右方向
//	戻り値：なし
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
//	関数名：RotByEuler
//  関数説明：オーラー角より回転
//	引数：	vRot：オーラー角
//	戻り値：なし
//--------------------------------------------------------------------------------
void CTransformComponent::RotByEuler(const CKFVec3& vRot)
{
	CKFMtx44 mtxRot;
	CKFMath::MtxRotationYawPitchRoll(mtxRot, vRot);
	m_vUpNext = CKFMath::Vec3TransformNormal(m_vUpNext, mtxRot);
	m_vForwardNext = CKFMath::Vec3TransformNormal(m_vForwardNext, mtxRot);
	m_vRightNext = CKFMath::Vec3TransformNormal(m_vRightNext, mtxRot);
}

//--------------------------------------------------------------------------------
//	関数名：RotByPitch
//  関数説明：自分のX軸(vVecRight)より回転
//	引数：	fRadian：ラジアン角
//	戻り値：なし
//--------------------------------------------------------------------------------
void CTransformComponent::RotByPitch(const float& fRadian)
{
	CKFMtx44 mtxPitch;
	CKFMath::MtxRotAxis(mtxPitch, m_vRightNext, fRadian);
	m_vUpNext = CKFMath::Vec3TransformNormal(m_vUpNext, mtxPitch);
	m_vForwardNext = CKFMath::Vec3TransformNormal(m_vForwardNext, mtxPitch);
}

//--------------------------------------------------------------------------------
//	関数名：RotByYaw
//  関数説明：自分のY軸(vVecUp)より回転
//	引数：	fRadian：ラジアン角
//	戻り値：なし
//--------------------------------------------------------------------------------
void CTransformComponent::RotByYaw(const float& fRadian)
{
	CKFMtx44 mtxYaw;
	CKFMath::MtxRotAxis(mtxYaw, m_vUpNext, fRadian);
	m_vRightNext = CKFMath::Vec3TransformNormal(m_vRightNext, mtxYaw);
	m_vForwardNext = CKFMath::Vec3TransformNormal(m_vForwardNext, mtxYaw);
}

//--------------------------------------------------------------------------------
//	関数名：RotByRoll
//  関数説明：自分のZ軸(vLook)より回転
//	引数：	fRadian：ラジアン角
//	戻り値：なし
//--------------------------------------------------------------------------------
void CTransformComponent::RotByRoll(const float& fRadian)
{
	CKFMtx44 mtxRoll;
	CKFMath::MtxRotAxis(mtxRoll, m_vForwardNext, fRadian);
	m_vForwardNext = CKFMath::Vec3TransformNormal(m_vForwardNext, mtxRoll);
	m_vRightNext = CKFMath::Vec3TransformNormal(m_vRightNext, mtxRoll);
}

//--------------------------------------------------------------------------------
//	関数名：TransformDirectionToLocal
//  関数説明：世界軸の方向ベクトルを自分の軸に変換する
//	引数：	vDirection：方向ベクトル
//	戻り値：なし
//--------------------------------------------------------------------------------
CKFVec3 CTransformComponent::TransformDirectionToLocal(const CKFVec3& vDirection)
{
	auto mtxInverse = CKFMath::MtxTranspose(m_mtxThis);
	mtxInverse.m_af[3][0] = 0.0f;
	mtxInverse.m_af[3][1] = 0.0f;
	mtxInverse.m_af[3][2] = 0.0f;
	auto vAnswer = CKFMath::Vec3TransformNormal(vDirection, mtxInverse);
	return vAnswer;
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：calculateMtxThis
//  関数説明：行列の算出(親なし)
//	引数：	なし
//	戻り値：なし
//--------------------------------------------------------------------------------
void  CTransformComponent::calculateMtxThis(void)
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
//	関数名：calculateMtxThis
//  関数説明：行列の算出(親あり)
//	引数：	mtxParent
//	戻り値：なし
//--------------------------------------------------------------------------------
void  CTransformComponent::calculateMtxThis(const CKFMtx44& mtxParent)
{
	//自分の行列算出
	calculateMtxThis();

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
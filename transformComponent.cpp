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
	, m_Position(CKFMath::sc_vZero)
	, m_PositionNext(CKFMath::sc_vZero)
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
void  CTransformComponent::UpdateMatrix(const Matrix44& mtxParent)
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
	m_Position = m_PositionNext;
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
void CTransformComponent::RegisterParent(CTransformComponent* pParent, const Vector3& vOffsetPos, const Vector3& vOffsetRot)
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
//	戻り値：Quaternion
//--------------------------------------------------------------------------------
const Quaternion CTransformComponent::GetRot(void) const
{
	return CKFMath::MtxToQuaternion(GetMatrixRot());
}

//--------------------------------------------------------------------------------
//	関数名：GetRotNext
//  関数説明：次のフレームの回転の取得
//	引数：	なし
//	戻り値：Quaternion
//--------------------------------------------------------------------------------
const Quaternion CTransformComponent::GetRotNext(void) const
{
	return CKFMath::MtxToQuaternion(GetMatrixRotNext());
}

//--------------------------------------------------------------------------------
//	関数名：GetEulerRot
//  関数説明：今のフレームの回転の取得
//	引数：	なし
//	戻り値：Vector3
//--------------------------------------------------------------------------------
const Vector3 CTransformComponent::GetEulerRot(void) const
{
	return CKFMath::MtxToEular(GetMatrixRot());
}

//--------------------------------------------------------------------------------
//	関数名：GetMatrixRot
//  関数説明：今のフレームの回転行列の取得
//	引数：	なし
//	戻り値：Matrix44
//--------------------------------------------------------------------------------
const Matrix44 CTransformComponent::GetMatrixRot(void) const
{
	Matrix44 mtxRot;
	mtxRot.Elements[0][0] = m_vRight.X;
	mtxRot.Elements[0][1] = m_vRight.Y;
	mtxRot.Elements[0][2] = m_vRight.Z;
	mtxRot.Elements[1][0] = m_vUp.X;
	mtxRot.Elements[1][1] = m_vUp.Y;
	mtxRot.Elements[1][2] = m_vUp.Z;
	mtxRot.Elements[2][0] = m_vForward.X;
	mtxRot.Elements[2][1] = m_vForward.Y;
	mtxRot.Elements[2][2] = m_vForward.Z;
	return mtxRot;
}

//--------------------------------------------------------------------------------
//	関数名：GetMatrixRotNext
//  関数説明：次のフレームの回転行列の取得
//	引数：	なし
//	戻り値：Matrix44
//--------------------------------------------------------------------------------
const Matrix44 CTransformComponent::GetMatrixRotNext(void) const
{
	Matrix44 mtxRot;
	mtxRot.Elements[0][0] = m_vRightNext.X;
	mtxRot.Elements[0][1] = m_vRightNext.Y;
	mtxRot.Elements[0][2] = m_vRightNext.Z;
	mtxRot.Elements[1][0] = m_vUpNext.X;
	mtxRot.Elements[1][1] = m_vUpNext.Y;
	mtxRot.Elements[1][2] = m_vUpNext.Z;
	mtxRot.Elements[2][0] = m_vForwardNext.X;
	mtxRot.Elements[2][1] = m_vForwardNext.Y;
	mtxRot.Elements[2][2] = m_vForwardNext.Z;
	return mtxRot;
}

//--------------------------------------------------------------------------------
//	関数名：GetMatrixWorldNext
//  関数説明：次のフレームの世界行列の取得
//	引数：	なし
//	戻り値：Matrix44
//--------------------------------------------------------------------------------
const Matrix44 CTransformComponent::GetMatrixWorldNext(void) const
{
	//回転
	auto mtxWorld = GetMatrixRotNext();

	//平行移動
	mtxWorld.Elements[3][0] = m_PositionNext.X;
	mtxWorld.Elements[3][1] = m_PositionNext.Y;
	mtxWorld.Elements[3][2] = m_PositionNext.Z;

	//親のマトリクス取得
	if (m_pParent)
	{
		//親に対する相対位置行列
		//回転
		Matrix44 mtxRot;
		CKFMath::MtxRotationYawPitchRoll(mtxRot, m_vOffsetRot);
		mtxWorld *= mtxRot;

		//平行移動
		Matrix44 mtxPos;
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
void CTransformComponent::SetRot(const Quaternion& qRot)
{
	auto mtxRot = CKFMath::QuaternionToMtx(qRot);
	m_vRight.X = mtxRot.Elements[0][0];
	m_vRight.Y = mtxRot.Elements[0][1];
	m_vRight.Z = mtxRot.Elements[0][2];
	m_vUp.X = mtxRot.Elements[1][0];
	m_vUp.Y = mtxRot.Elements[1][1];
	m_vUp.Z = mtxRot.Elements[1][2];
	m_vForward.X = mtxRot.Elements[2][0];
	m_vForward.Y = mtxRot.Elements[2][1];
	m_vForward.Z = mtxRot.Elements[2][2];
}

//--------------------------------------------------------------------------------
//	関数名：SetRotNext
//  関数説明：次の回転の設定
//	引数：	qRotNext：回転情報
//	戻り値：なし
//--------------------------------------------------------------------------------
void CTransformComponent::SetRotNext(const Quaternion& qRotNext)
{
	auto mtxRot = CKFMath::QuaternionToMtx(qRotNext);
	m_vRightNext.X = mtxRot.Elements[0][0];
	m_vRightNext.Y = mtxRot.Elements[0][1];
	m_vRightNext.Z = mtxRot.Elements[0][2];
	m_vUpNext.X = mtxRot.Elements[1][0];
	m_vUpNext.Y = mtxRot.Elements[1][1];
	m_vUpNext.Z = mtxRot.Elements[1][2];
	m_vForwardNext.X = mtxRot.Elements[2][0];
	m_vForwardNext.Y = mtxRot.Elements[2][1];
	m_vForwardNext.Z = mtxRot.Elements[2][2];
}

//--------------------------------------------------------------------------------
//	関数名：SetRotNext
//  関数説明：次の回転の設定
//	引数：	vRot：回転情報
//	戻り値：なし
//--------------------------------------------------------------------------------
void CTransformComponent::SetRotNext(const Vector3& vRot)
{
	auto qRot = CKFMath::EulerToQuaternion(vRot);
	SetRotNext(qRot);
}

//--------------------------------------------------------------------------------
//	関数名：RotByUp
//  関数説明：上方向により回転
//	引数：	vUp：上方向
//	戻り値：なし
//--------------------------------------------------------------------------------
void CTransformComponent::RotByUp(const Vector3& vUp)
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
void CTransformComponent::RotByForward(const Vector3& vForward)
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
void CTransformComponent::RotByRight(const Vector3& vRight)
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
void CTransformComponent::RotByEuler(const Vector3& vRot)
{
	Matrix44 mtxRot;
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
	Matrix44 mtxPitch;
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
	Matrix44 mtxYaw;
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
	Matrix44 mtxRoll;
	CKFMath::MtxRotAxis(mtxRoll, m_vForwardNext, fRadian);
	m_vUpNext = CKFMath::Vec3TransformNormal(m_vUpNext, mtxRoll);
	m_vRightNext = CKFMath::Vec3TransformNormal(m_vRightNext, mtxRoll);
}

//--------------------------------------------------------------------------------
//	関数名：TransformDirectionToLocal
//  関数説明：世界軸の方向ベクトルを自分の軸に変換する
//	引数：	vDirection：方向ベクトル
//	戻り値：なし
//--------------------------------------------------------------------------------
Vector3 CTransformComponent::TransformDirectionToLocal(const Vector3& vDirection)
{
	auto mtxInverse = CKFMath::MtxTranspose(m_mtxThis);
	mtxInverse.Elements[3][0] = 0.0f;
	mtxInverse.Elements[3][1] = 0.0f;
	mtxInverse.Elements[3][2] = 0.0f;
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
	Matrix44 mtxPos;
	CKFMath::MtxTranslation(mtxPos, m_Position);
	m_mtxThis *= mtxPos;
}

//--------------------------------------------------------------------------------
//	関数名：calculateMtxThis
//  関数説明：行列の算出(親あり)
//	引数：	mtxParent
//	戻り値：なし
//--------------------------------------------------------------------------------
void  CTransformComponent::calculateMtxThis(const Matrix44& mtxParent)
{
	//自分の行列算出
	calculateMtxThis();

	//親に対する相対位置行列
	//回転
	Matrix44 mtxRot;
	CKFMath::MtxRotationYawPitchRoll(mtxRot, m_vOffsetRot);
	m_mtxThis *= mtxRot;

	//平行移動
	Matrix44 mtxPos;
	CKFMath::MtxTranslation(mtxPos, m_vOffsetPos);
	m_mtxThis *= mtxPos;

	//親の行列とかける
	m_mtxThis *= mtxParent;
}
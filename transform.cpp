//--------------------------------------------------------------------------------
//	トランスフォームコンポネント
//　transform.h
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "transform.h"

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
Transform::Transform(GameObject* const owner) : Component(owner)
	, currentPosition(Vector3::Zero)
	, currentScale(Vector3::One)
	, currentForward(Vector3::Forward)
	, currentUp(Vector3::Up)
	, currentRight(Vector3::Right)
	, currentWorldMatrix(Matrix44::Identity)
	, nextPosition(Vector3::Zero)
	, nextScale(Vector3::One)
	, nextForward(Vector3::Forward)
	, nextUp(Vector3::Up)
	, nextRight(Vector3::Right)
	, parent(nullptr)
	, offset(Matrix44::Identity)
{
	children.clear();
}

//--------------------------------------------------------------------------------
//	関数名：UpdateMatrix
//  関数説明：マトリクス算出(親がないの場合呼び出される)
//	引数：	なし
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::UpdateMatrix(void)
{
	if (parent) return;
	calculateWorldMatrix();
	for (auto child : children) 
	{
		child->UpdateMatrix(currentWorldMatrix);
	}
}

//--------------------------------------------------------------------------------
//	関数名：UpdateMatrix
//  関数説明：マトリクス算出(親があるの場合呼び出される)
//	引数：	parent：親のマトリクス
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::UpdateMatrix(const Matrix44& parent)
{
	calculateWorldMatrix(parent);
	for (auto child : children)
	{
		child->UpdateMatrix(currentWorldMatrix);
	}
}

//--------------------------------------------------------------------------------
//	関数名：SwapParamater
//  関数説明：パラメーター交換処理
//	引数：	なし
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::SwapParamater(void)
{
	currentPosition = nextPosition;
	currentScale = nextScale;
	currentForward = nextForward;
	currentRight = nextRight;
	currentUp = nextUp;
}

//--------------------------------------------------------------------------------
//	関数名：RegisterParent
//  関数説明：親の追加
//	引数：	value：親のポインタ
//			offsetPosition：相対位置
//			offsetRotation：相対回転
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::RegisterParent(Transform* value, const Vector3& offsetPosition, const Vector3& offsetRotation)
{
	if (parent)
	{//親があるの場合前の親から削除
		parent->DeregisterChild(this);
	}
	parent = value;
	parent->RegisterChild(this);
	offset = Matrix44::Transform(offsetRotation, offsetPosition);
}

//--------------------------------------------------------------------------------
//	関数名：GetNextWorldMatrix
//  関数説明：次のフレームの世界行列の取得
//	引数：	なし
//	戻り値：Matrix44
//--------------------------------------------------------------------------------
Matrix44 Transform::GetNextWorldMatrix(void) const
{
	auto& nextWorldMatrix = Matrix44::Transform(nextRight, nextUp, nextForward, nextPosition, nextScale);
	if (parent)
	{
		nextWorldMatrix *= offset;
		nextWorldMatrix *= parent->GetNextWorldMatrix();
	}
	return nextWorldMatrix;
}

//--------------------------------------------------------------------------------
//	関数名：SetNextRotation
//  関数説明：次の回転の設定
//	引数：	value：回転情報
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::SetNextRotation(const Quaternion& value)
{
	auto& rotation = value.ToMatrix();
	nextRight.X = rotation.Elements[0][0];
	nextRight.Y = rotation.Elements[0][1];
	nextRight.Z = rotation.Elements[0][2];
	nextUp.X = rotation.Elements[1][0];
	nextUp.Y = rotation.Elements[1][1];
	nextUp.Z = rotation.Elements[1][2];
	nextForward.X = rotation.Elements[2][0];
	nextForward.Y = rotation.Elements[2][1];
	nextForward.Z = rotation.Elements[2][2];
}

//--------------------------------------------------------------------------------
//	関数名：SetRotNext
//  関数説明：次の回転の設定
//	引数：	value：回転情報
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::SetNextRotation(const Vector3& euler)
{
	SetNextRotation(euler.ToQuaternion());
}

//--------------------------------------------------------------------------------
//	関数名：SetOffset
//  関数説明：親に対する相対行列の設定
//	引数：	position：位置情報
//			rotation：回転情報
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::SetOffset(const Vector3& position, const Vector3& rotation)
{
	if (!parent) return;
	offset = Matrix44::Transform(rotation, position);
}

//--------------------------------------------------------------------------------
//	関数名：RotateByUp
//  関数説明：上方向により回転
//	引数：	up：上方向
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::RotateByUp(const Vector3& up)
{
	nextUp = up.Normalized();
	nextRight = (nextUp * nextForward).Normalized();
	nextForward = (nextRight * nextUp).Normalized();
}

//--------------------------------------------------------------------------------
//	関数名：RotateByForward
//  関数説明：前方向により回転
//	引数：	forward：前方向
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::RotateByForward(const Vector3& forward)
{
	nextForward = forward.Normalized();
	nextUp = (nextForward * nextRight).Normalized();
	nextRight = (nextUp * nextForward).Normalized();
}

//--------------------------------------------------------------------------------
//	関数名：RotateByRight
//  関数説明：右方向により回転
//	引数：	right：右方向
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::RotateByRight(const Vector3& right)
{
	nextRight = right.Normalized();
	nextForward = (nextRight * nextUp).Normalized();
	nextUp = (nextForward * nextRight).Normalized();
}

//--------------------------------------------------------------------------------
//	関数名：RotateByEuler
//  関数説明：オーラー角より回転
//	引数：	euler：オーラー角
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::RotateByEuler(const Vector3& euler)
{
	auto& rotation = Matrix44::RotationYawPitchRoll(euler);
	nextUp = Vector3::TransformNormal(nextUp, rotation);
	nextForward = Vector3::TransformNormal(nextForward, rotation);
	nextRight = Vector3::TransformNormal(nextRight, rotation);
}

//--------------------------------------------------------------------------------
//	関数名：RotByPitch
//  関数説明：自分のX軸(nextRight)より回転
//	引数：	radian：ラジアン角
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::RotateByPitch(const float& radian)
{
	auto& rotation = Matrix44::RotationAxis(nextRight, radian);
	nextUp = Vector3::TransformNormal(nextUp, rotation);
	nextForward = Vector3::TransformNormal(nextForward, rotation);
}

//--------------------------------------------------------------------------------
//	関数名：RotateByYaw
//  関数説明：自分のY軸(nextUp)より回転
//	引数：	radian：ラジアン角
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::RotateByYaw(const float& radian)
{
	auto& rotation = Matrix44::RotationAxis(nextUp, radian);
	nextForward = Vector3::TransformNormal(nextForward, rotation);
	nextRight = Vector3::TransformNormal(nextRight, rotation);
}

//--------------------------------------------------------------------------------
//	関数名：RotateByRoll
//  関数説明：自分のZ軸(nextForward)より回転
//	引数：	radian：ラジアン角
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::RotateByRoll(const float& radian)
{
	auto& rotation = Matrix44::RotationAxis(nextForward, radian);
	nextUp = Vector3::TransformNormal(nextUp, rotation);
	nextRight = Vector3::TransformNormal(nextRight, rotation);
}

//--------------------------------------------------------------------------------
//	関数名：TransformDirectionToLocal
//  関数説明：世界軸の方向ベクトルを自分の軸に変換する
//	引数：	direction：方向ベクトル
//	戻り値：なし
//--------------------------------------------------------------------------------
Vector3 Transform::TransformDirectionToLocal(const Vector3& direction)
{
	auto& inverse = currentWorldMatrix.Transpose();
	return Vector3::TransformNormal(direction, inverse);
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：calculateWorldMatrix
//  関数説明：行列の算出(親なし)
//	引数：	なし
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::calculateWorldMatrix(void)
{
	currentWorldMatrix = Matrix44::Transform(currentRight, currentUp, currentForward, currentPosition, currentScale);
}

//--------------------------------------------------------------------------------
//	関数名：calculateWorldMatrix
//  関数説明：行列の算出(親あり)
//	引数：	mtxParent
//	戻り値：なし
//--------------------------------------------------------------------------------
void Transform::calculateWorldMatrix(const Matrix44& parent)
{
	calculateWorldMatrix();
	currentWorldMatrix *= offset;
	currentWorldMatrix *= parent;
}
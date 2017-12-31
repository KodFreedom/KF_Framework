//--------------------------------------------------------------------------------
//　kf_math.cpp
//  classes and methods for math
//	演算用のクラスとメソッド
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "kf_math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  constant variables / 定数
//--------------------------------------------------------------------------------
const Vector2 Vector2::kZero = Vector2(0.0f);
const Vector2 Vector2::kOne = Vector2(1.0f);

const Vector3 Vector3::kZero = Vector3(0.0f);				
const Vector3 Vector3::kOne = Vector3(1.0f);				
const Vector3 Vector3::kUp = Vector3(0.0f, 1.0f, 0.0f);	
const Vector3 Vector3::kDown = Vector3(0.0f, -1.0f, 0.0f);	
const Vector3 Vector3::kLeft = Vector3(-1.0f, 0.0f, 0.0f);	
const Vector3 Vector3::kRight = Vector3(1.0f, 0.0f, 0.0f);	
const Vector3 Vector3::kForward = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::kBack = Vector3(0.0f, 0.0f, -1.0f);	
const Vector3 Vector3::kAxisX = Vector3::kRight;
const Vector3 Vector3::kAxisY = Vector3::kUp;
const Vector3 Vector3::kAxisZ = Vector3::kForward;

const Vector4 Vector4::kZero = Vector4(0.0f);
const Vector4 Vector4::kOne = Vector4(1.0f);

const Matrix44 Matrix44::kZero = Matrix44(0.0f);
const Matrix44 Matrix44::kIdentity = Matrix44();

const Quaternion Quaternion::kIdentity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

const Color Color::kWhite = Color(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::kBlack = Color(0.0f, 0.0f, 0.0f, 1.0f);
const Color Color::kGray = Color(0.2f, 0.2f, 0.2f, 1.0f);
const Color Color::kRed = Color(1.0f, 0.0f, 0.0f, 1.0f);
const Color Color::kBlue = Color(0.0f, 0.0f, 1.0f, 1.0f);
const Color Color::kGreen = Color(0.0f, 1.0f, 0.0f, 1.0f);

////--------------------------------------------------------------------------------
////	関数名：ToPickingRay
////  関数説明：スクリーン座標からレイの算出
////	引数：	viewportSize：viewportの副幅
////			projectMatrix00：射影行列の00番
////			projectMatrix11：射影行列の11番
////			viewInverse：view行列の逆行列
////	戻り値：Ray
////--------------------------------------------------------------------------------
//Ray Vector2::ToPickingRay(const Vector2& viewportSize, const float& projectMatrix00, const float& projectMatrix11, const Matrix44& viewInverse)
//{
//	auto& position3D = Vector2(
//		(((2.0f * X) / viewportSize.X) - 1.0f) / projectMatrix00,
//		(((-2.0f * Y) / viewportSize.Y) + 1.0f) / projectMatrix11);
//	Ray result;
//	result.Origin = Vector3::Zero;
//	result.Direction = Vector3(position3D.X, position3D.Y, 1.0f);
//	result.Transform(viewInverse);
//	return result;
//}

//--------------------------------------------------------------------------------
//
//  Vector2
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  return the radian between two vector2 / Vector2間の角度を返す
//--------------------------------------------------------------------------------
float Vector2::RadianBetween(const Vector2& from, const Vector2& to)
{
    if (from == to) return 0.0f;
    float square_magnitude_from = from.SquareMagnitude();
    float square_magnitude_to = to.SquareMagnitude();
    if (square_magnitude_from * square_magnitude_to <= 0.0f) return 0.0f;
    float dot = (from * (1.0f / sqrtf(square_magnitude_from))).Dot(to * (1.0f / sqrtf(square_magnitude_to)));
    dot = Math::Clamp(dot, -1.0f, 1.0f);
    return acosf(dot);
    //float cross = from * to;
    //float sign = cross >= 0.0f ? 1.0f : -1.0f;
    //return acosf(dot / (sqrtf(square_magnitude_from) * sqrtf(square_magnitude_to)) * sign);
}

//--------------------------------------------------------------------------------
//
//  Vector3
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  change the euler to quaternion / Euler角をQuaternionに変換
//--------------------------------------------------------------------------------
Quaternion Vector3::ToQuaternion(void) const
{
	// TODO: 計算式を調べる
	Quaternion result;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	D3DXQUATERNION resultDX;
	D3DXQuaternionRotationYawPitchRoll(&resultDX, y_, x_, z_);
	result.x_ = resultDX.x;
	result.y_ = resultDX.y;
	result.z_ = resultDX.z;
	result.w_ = resultDX.w;
#endif
	return result;
}

//--------------------------------------------------------------------------------
//  ポイントを回転する
//--------------------------------------------------------------------------------
Vector3 Vector3::TransformCoord(const Vector3& point, const Matrix44& transform)
{
	auto& work = Vector4(point, 1.0f);
	work *= transform;
	if (work.w_ == 0.0f) return Vector3::kZero;
	return Vector3(work.x_ / work.w_, work.y_ / work.w_, work.z_ / work.w_);
}

//--------------------------------------------------------------------------------
//  法線ベクトルを回転する
//--------------------------------------------------------------------------------
Vector3 Vector3::TransformNormal(const Vector3& normal, const Matrix44& transform)
{
	auto& work = Vector4(normal, 0.0f);
	work *= transform;
	return Vector3(work.x_, work.y_, work.z_);
}

//--------------------------------------------------------------------------------
//  ポイントを逆行列により変換する
//--------------------------------------------------------------------------------
Vector3 Vector3::TransformInverse(const Vector3& point, const Matrix44& transform)
{
	auto work = point;
	work.x_ -= transform.m30_;
	work.y_ -= transform.m31_;
	work.z_ -= transform.m32_;
	Vector3 result;
	result.x_ = work.x_ * transform.m00_ +
		work.y_ * transform.m01_ +
		work.z_ * transform.m02_;
	result.y_ = work.x_ * transform.m10_ +
		work.y_ * transform.m11_ +
		work.z_ * transform.m12_;
	result.z_ = work.x_ * transform.m20_ +
		work.y_ * transform.m21_ +
		work.z_ * transform.m22_;
	return result;
}

//--------------------------------------------------------------------------------
//  ベクトルを回転する
//--------------------------------------------------------------------------------
Vector3 Vector3::Rotate(const Vector3& direction, const Quaternion& rotation)
{
	Quaternion& direction_quaternion = Quaternion(direction.x_, direction.y_, direction.z_, 0.0f);
	Quaternion& result = rotation.Conjugate() * direction_quaternion;
	result *= rotation;
	return Vector3(result.x_, result.y_, result.z_);
}

//--------------------------------------------------------------------------------
//
//  Vector4
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  mult with matrix / マトリクスとの乗算
//--------------------------------------------------------------------------------
Vector4 Vector4::operator*(const Matrix44& matrix) const
{
	Vector4 result;
	result.x_ = x_ * matrix.m00_ +
		y_ * matrix.m10_ +
		z_ * matrix.m20_ +
		w_ * matrix.m30_;
	result.y_ = x_ * matrix.m01_ +
		y_ * matrix.m11_ +
		z_ * matrix.m21_ +
		w_ * matrix.m31_;
	result.z_ = x_ * matrix.m02_ +
		y_ * matrix.m12_ +
		z_ * matrix.m22_ +
		w_ * matrix.m32_;
	result.w_ = x_ * matrix.m03_ +
		y_ * matrix.m13_ +
		z_ * matrix.m23_ +
		w_ * matrix.m33_;
	return result;
}

//--------------------------------------------------------------------------------
//  mult this with matrix / マトリクスとの乗算
//  matrix : 乗算相手
//--------------------------------------------------------------------------------
void Vector4::operator*=(const Matrix44& matrix)
{
	Vector4 copy = *this;
	x_ = copy.x_ * matrix.m_[0][0] +
		copy.y_ * matrix.m_[1][0] +
		copy.z_ * matrix.m_[2][0] +
		copy.w_ * matrix.m_[3][0];
	y_ = copy.x_ * matrix.m_[0][1] +
		copy.y_ * matrix.m_[1][1] +
		copy.z_ * matrix.m_[2][1] +
		copy.w_ * matrix.m_[3][1];
	z_ = copy.x_ * matrix.m_[0][2] +
		copy.y_ * matrix.m_[1][2] +
		copy.z_ * matrix.m_[2][2] +
		copy.w_ * matrix.m_[3][2];
	w_ = copy.x_ * matrix.m_[0][3] +
		copy.y_ * matrix.m_[1][3] +
		copy.z_ * matrix.m_[2][3] +
		copy.w_ * matrix.m_[3][3];
}

//--------------------------------------------------------------------------------
//
//  Matrix44
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
Matrix44::Matrix44()
	: m00_(1.0f), m01_(0.0f), m02_(0.0f), m03_(0.0f)
	, m10_(0.0f), m11_(1.0f), m12_(0.0f), m13_(0.0f)
	, m20_(0.0f), m21_(0.0f), m22_(1.0f), m23_(0.0f)
	, m30_(0.0f), m31_(0.0f), m32_(0.0f), m33_(1.0f) {}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
Matrix44::Matrix44(const float& value)
	: m00_(value), m01_(value), m02_(value), m03_(value)
	, m10_(value), m11_(value), m12_(value), m13_(value)
	, m20_(value), m21_(value), m22_(value), m23_(value)
	, m30_(value), m31_(value), m32_(value), m33_(value) {}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
Matrix44::Matrix44(const float& m00, const float& m01, const float& m02, const float& m03,
				   const float& m10, const float& m11, const float& m12, const float& m13,
				   const float& m20, const float& m21, const float& m22, const float& m23,
				   const float& m30, const float& m31, const float& m32, const float& m33)
	: m00_(m00), m01_(m01), m02_(m02), m03_(m03)
	, m10_(m10), m11_(m11), m12_(m12), m13_(m13)
	, m20_(m20), m21_(m21), m22_(m22), m23_(m23)
	, m30_(m30), m31_(m31), m32_(m32), m33_(m33) {}

//--------------------------------------------------------------------------------
//  change to euler / オーラー角に変換
//--------------------------------------------------------------------------------
Vector3 Matrix44::ToEuler(void) const
{
	return this->ToQuaternion().ToEuler();
}

//--------------------------------------------------------------------------------
//  change to quaternion / Quaternionに変換
//--------------------------------------------------------------------------------
Quaternion Matrix44::ToQuaternion(void) const
{
	Quaternion result;
	if (m_[2][2] <= 0.0f)  // x^2 + y^2 >= z^2 + w^2
	{
		float difference_1100 = m_[1][1] - m_[0][0];
		float one_minus_22 = 1.0f - m_[2][2];
		if (difference_1100 <= 0.0f)  // x^2 >= y^2
		{
			float square_4x = one_minus_22 - difference_1100;
			float inverse_4x = 0.5f / sqrtf(square_4x);
			result.x_ = square_4x * inverse_4x;
			result.y_ = (m_[0][1] + m_[1][0]) * inverse_4x;
			result.z_ = (m_[0][2] + m_[2][0]) * inverse_4x;
			result.w_ = (m_[1][2] - m_[2][1]) * inverse_4x;
		}
		else  // y^2 >= x^2
		{
			float square_4y = one_minus_22 + difference_1100;
			float inverse_4y = 0.5f / sqrtf(square_4y);
			result.x_ = (m_[0][1] + m_[1][0]) * inverse_4y;
			result.y_ = square_4y * inverse_4y;
			result.z_ = (m_[1][2] + m_[2][1]) * inverse_4y;
			result.w_ = (m_[2][0] - m_[0][2]) * inverse_4y;
		}
	}
	else  // z^2 + w^2 >= x^2 + y^2
	{
		float sum_1100 = m_[1][1] + m_[0][0];
		float one_plus_22 = 1.0f + m_[2][2];
		if (sum_1100 <= 0.0f)  // z^2 >= w^2
		{
			float square_4z = one_plus_22 - sum_1100;
			float inverse_4z = 0.5f / sqrtf(square_4z);
			result.x_ = (m_[0][2] + m_[2][0]) * inverse_4z;
			result.y_ = (m_[1][2] + m_[2][1]) * inverse_4z;
			result.z_ = square_4z * inverse_4z;
			result.w_ = (m_[0][1] - m_[1][0]) * inverse_4z;
		}
		else  // w^2 >= z^2
		{
			float square_4w = one_plus_22 + sum_1100;
			float inverse_4w = 0.5f / sqrtf(square_4w);
			result.x_ = (m_[1][2] - m_[2][1]) * inverse_4w;
			result.y_ = (m_[2][0] - m_[0][2]) * inverse_4w;
			result.z_ = (m_[0][1] - m_[1][0]) * inverse_4w;
			result.w_ = square_4w * inverse_4w;
		}
	}
	return result;
}

//--------------------------------------------------------------------------------
//  create rotation matrix with axis and radian / 与えられた軸と角で回転行列の作成
//--------------------------------------------------------------------------------
Matrix44 Matrix44::RotationAxis(const Vector3& axis, const float& radian)
{
	Matrix44 result;
	const float& x = axis.x_;
	const float& y = axis.y_;
	const float& z = axis.z_;
	const float& sin = sinf(radian);
	const float& cos = cosf(radian);
	result.m_[0][0] = cos + x * x * (1.0f - cos);
	result.m_[0][1] = x * y * (1.0f - cos) + z * sin;
	result.m_[0][2] = x * z * (1.0f - cos) - y * sin;
	result.m_[1][0] = x * y * (1.0f - cos) - z * sin;
	result.m_[1][1] = cos + y * y * (1.0f - cos);
	result.m_[1][2] = z * y * (1.0f - cos) + x * sin;
	result.m_[2][0] = x * z * (1.0f - cos) + y * sin;
	result.m_[2][1] = y * z * (1.0f - cos) - x * sin;
	result.m_[2][2] = cos + z * z * (1.0f - cos);
	return result;
}

//--------------------------------------------------------------------------------
//  create rotation matrix with euler / 与えられたeuler角で回転行列の作成
//--------------------------------------------------------------------------------
Matrix44 Matrix44::RotationYawPitchRoll(const Vector3& euler)
{
	Matrix44 result;
	const float& sinx = sinf(euler.x_);
	const float& cosx = cosf(euler.x_);
	const float& siny = sinf(euler.y_);
	const float& cosy = cosf(euler.y_);
	const float& sinz = sinf(euler.z_);
	const float& cosz = cosf(euler.z_);
	result.m_[0][0] = cosy * cosz + sinx * siny * sinz;
	result.m_[0][1] = cosx * sinz;
	result.m_[0][2] = -siny * cosz + sinx * cosy * sinz;
	result.m_[1][0] = -cosy * sinz + sinx * siny * cosz;
	result.m_[1][1] = cosx * cosz;
	result.m_[1][2] = siny * sinz + sinx * cosy * cosz;
	result.m_[2][0] = cosx * siny;
	result.m_[2][1] = -sinx;
	result.m_[2][2] = cosx * cosy;
	return result;
}

//--------------------------------------------------------------------------------
//  create transform matrix with given right, up, forward, scale, translation
//  与えられた回転と移動量で行列の作成
//--------------------------------------------------------------------------------
Matrix44 Matrix44::Transform(const Vector3& right, const Vector3& up, const Vector3& forward, const Vector3& translation, const Vector3& scale)
{
	return Matrix44(right.x_ * scale.x_, right.y_ * scale.x_, right.z_ * scale.x_, 0.0f,
					up.x_ * scale.y_, up.y_ * scale.y_, up.z_ * scale.y_, 0.0f,
					forward.x_ * scale.z_, forward.y_ * scale.z_, forward.z_ * scale.z_, 0.0f,
					translation.x_, translation.y_, translation.z_, 1.0f);
}

//--------------------------------------------------------------------------------
//  create transform matrix with given rotation, scale, translation
//  与えられた回転、移動量、スケールで行列の作成
//--------------------------------------------------------------------------------
Matrix44 Matrix44::Transform(const Quaternion& rotation, const Vector3& translation, const Vector3& scale)
{
	auto& result = Scale(scale);
	result *= rotation.ToMatrix();
	result *= Translation(translation);
	return result;
}

//--------------------------------------------------------------------------------
//  create perspective matrix / パースペクティブ行列の作成(左手座標系)
//--------------------------------------------------------------------------------
Matrix44 Matrix44::PerspectiveLeftHand(const float& fov_radian_y, const float& aspect_ratio, const float& near_z, const float& far_z)
{
	assert(near_z < far_z);
	float fov_cos = cosf(fov_radian_y * 0.5f);
	float fov_sin = sqrtf(1.0f - fov_cos * fov_cos);
	if (fov_radian_y * 0.5f > kPi) fov_sin = -fov_sin;

	float height = fov_cos / fov_sin;
	float width = height / aspect_ratio;
	float range = far_z / (far_z - near_z);

	Matrix44 result;
	result.m_[0][0] = width;
	result.m_[0][1] = 0.0f;
	result.m_[0][2] = 0.0f;
	result.m_[0][3] = 0.0f;
	result.m_[1][0] = 0.0f;
	result.m_[1][1] = height;
	result.m_[1][2] = 0.0f;
	result.m_[1][3] = 0.0f;
	result.m_[2][0] = 0.0f;
	result.m_[2][1] = 0.0f;
	result.m_[2][2] = range;
	result.m_[2][3] = 1.0f;
	result.m_[3][0] = 0.0f;
	result.m_[3][1] = 0.0f;
	result.m_[3][2] = -range * near_z;
	result.m_[3][3] = 0.0f;
	return result;
}

//--------------------------------------------------------------------------------
//
//  Quaternion
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	euler角に変換
//--------------------------------------------------------------------------------
Vector3 Quaternion::ToEuler(void) const
{
	Vector3 result;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	D3DXQUATERNION quaternion = *this;
	D3DXQuaternionToAxisAngle(&quaternion, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), &result.x_);
	D3DXQuaternionToAxisAngle(&quaternion, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &result.y_);
	D3DXQuaternionToAxisAngle(&quaternion, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &result.z_);
#else
	result.X = atan2f(2.0f * (W * Z + X * Y), 1.0f - 2.0f * (Z * Z + X * X));
	result.Y = asinf(2.0f * (W * X - Y * Z));
	result.Z = atan2f(2.0f * (W * Y + Z * X), 1.0f - 2.0f * (X * X + Y * Y));
#endif
	return result;
}

//--------------------------------------------------------------------------------
//	関数名：ToMatrix
//  関数説明：行列に変換
//	引数：	なし
//	戻り値：Matrix44
//--------------------------------------------------------------------------------
Matrix44 Quaternion::ToMatrix(void) const
{
	Matrix44 result;
	static const auto quaternion1110 = Quaternion(1.0f, 1.0f, 1.0f, 0.0f);
	auto& this_x2 = *this + *this;
	auto& this_x_this_x2 = this->MultiplySeparately(this_x2);
	auto& work0 = quaternion1110
		- Quaternion(this_x_this_x2.y_, this_x_this_x2.x_, this_x_this_x2.x_, quaternion1110.w_)
		- Quaternion(this_x_this_x2.z_, this_x_this_x2.z_, this_x_this_x2.y_, quaternion1110.w_);
	auto& work1 = Quaternion(x_, x_, y_, w_).MultiplySeparately(Quaternion(this_x2.z_, this_x2.y_, this_x2.z_, this_x2.w_));
	auto& work2 = Quaternion(w_, w_, w_, w_).MultiplySeparately(Quaternion(this_x2.y_, this_x2.z_, this_x2.x_, this_x2.w_));
	auto& one_plus_two = work1 + work2;
	auto& one_minus_two = work1 - work2;
	result.m_[0][0] = work0.x_;
	result.m_[0][1] = one_plus_two.y_;
	result.m_[0][2] = one_minus_two.x_;
	result.m_[0][3] = work0.w_;
	result.m_[1][0] = one_minus_two.y_;
	result.m_[1][1] = work0.y_;
	result.m_[1][2] = one_plus_two.z_;
	result.m_[1][3] = work0.w_;
	result.m_[2][0] = one_plus_two.x_;
	result.m_[2][1] = one_minus_two.z_;
	result.m_[2][2] = work0.z_;
	result.m_[2][3] = work0.w_;
	return result;
}

//--------------------------------------------------------------------------------
//
//  Math
//
//--------------------------------------------------------------------------------

/*
//--------------------------------------------------------------------------------
//  Others
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  NormalizeRotInTwoPi(float)
//	回転を-2Piから2Piの間にする
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInTwoPi(float& fRot)
{
	while (fRot >= KF_PI * 2.0f)
	{
		fRot -= KF_PI * 2.0f;
	}

	while (fRot <= -KF_PI * 2.0f)
	{
		fRot += KF_PI * 2.0f;
	}
}

//--------------------------------------------------------------------------------
//  NormalizeRotInTwoPi(Vec3)
//	回転を-2Piから2Piの間にする
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInTwoPi(Vector3& vRot)
{
	NormalizeRotInTwoPi(vRot.X);
	NormalizeRotInTwoPi(vRot.Y);
	NormalizeRotInTwoPi(vRot.Z);
}

//--------------------------------------------------------------------------------
//  NormalizeRotInPi(float)
//	回転を-PiからPiの間にする
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInPi(float& fRot)
{
	if (fRot > KF_PI)
	{
		fRot -= KF_PI * 2.0f;
	}
	else if (fRot < -KF_PI)
	{
		fRot += KF_PI * 2.0f;
	}
}

//--------------------------------------------------------------------------------
//  NormalizeRotInPi(Vec3)
//	回転を-PiからPiの間にする
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInPi(Vector3& vRot)
{
	NormalizeRotInPi(vRot.X);
	NormalizeRotInPi(vRot.Y);
	NormalizeRotInPi(vRot.Z);
}

//--------------------------------------------------------------------------------
//  NormalizeRotInPi(Vec3)
//	回転を-PiからPiの間にする
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInZeroToTwoPi(float& fRot)
{
	while (fRot >= KF_PI * 2.0f)
	{
		fRot -= KF_PI * 2.0f;
	}

	while (fRot < 0.0f)
	{
		fRot += KF_PI * 2.0f;
	}
}

//--------------------------------------------------------------------------------
//  NormalizeRotInPi(Vec3)
//	回転を-PiからPiの間にする
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInZeroToTwoPi(Vector3& vRot)
{
	NormalizeRotInZeroToTwoPi(vRot.X);
	NormalizeRotInZeroToTwoPi(vRot.Y);
	NormalizeRotInZeroToTwoPi(vRot.Z);
}

//--------------------------------------------------------------------------------
//  CalculateZDepth
//	Z深度情報算出
//--------------------------------------------------------------------------------
float CKFMath::CalculateZDepth(const Vector3& Position, const Vector3& vCameraEye, const Vector3& vCameraAt)
{
	Vector3 vCameraForward = vCameraAt - vCameraEye;
	Vector3 PositionToCamera = Position - vCameraEye;
	float fDepth = Vec3Dot(vCameraForward, PositionToCamera);
	return fDepth;
}
*/
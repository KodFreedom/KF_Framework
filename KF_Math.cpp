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
//  Vector3
//
//--------------------------------------------------------------------------------
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
	const float& sinz = cosf(euler.z_);
	result.m_[0][0] = cosy * sinz + sinx * siny * sinz;
	result.m_[0][1] = cosx * sinz;
	result.m_[0][2] = -siny * sinz + sinx * cosy * sinz;
	result.m_[1][0] = -cosy * sinz + sinx * siny * sinz;
	result.m_[1][1] = cosx * sinz;
	result.m_[1][2] = siny * sinz + sinx * cosy * sinz;
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
//  create projection matrix / 射影行列の作成(左手座標系)
//--------------------------------------------------------------------------------
Matrix44 Matrix44::ProjectionLeftHand(const float& fov_radian_y, const float& aspect_ratio, const float& near_z, const float& far_z)
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
//--------------------------------------------------------------------------------
//	Quaternionを球形補間方式で補間する
//--------------------------------------------------------------------------------
Quaternion math::Slerp(const Quaternion& from, const Quaternion& to, const float& time)
{
	if (time <= 0.0f) { return from; }
	if (time >= 1.0f) { return to; }
	Quaternion result;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	D3DXQUATERNION from_dx = from;
	D3DXQUATERNION to_dx = to;
	D3DXQUATERNION result_dx;
	D3DXQuaternionSlerp(&result_dx, &from_dx, &to_dx, time);
	result.x_ = result_dx.x;
	result.y_ = result_dx.y;
	result.z_ = result_dx.z;
	result.w_ = result_dx.w;
#else
	//Quaternion qFromCpy = qFrom;
	//Quaternion qToCpy = qTo;

	//// Only unit quaternions are valid rotations.
	//// Normalize to avoid undefined behavior.

	//// Compute the cosine of the angle between the two vectors.
	//float fDot = QuaternionDot(qFromCpy, qToCpy);

	//if (fabs(fDot) > 0.9995f)
	//{
	//	// If the inputs are too close for comfort, linearly interpolate
	//	// and normalize the result.
	//	Quaternion qResult = qFromCpy + (qToCpy - qFromCpy) * fTime;
	//	QuaternionNormalize(qResult);
	//	return qResult;
	//}

	//// If the dot product is negative, the quaternions
	//// have opposite handed-ness and slerp won't take
	//// the shorter path. Fix by reversing one quaternion.
	//if (fDot < 0.0f) 
	//{
	//	qToCpy *= -1.0f;
	//	fDot = -fDot;
	//}

	//ClampFloat(fDot, -1.0f, 1.0f);		// Robustness: Stay within domain of acos()
	//float fTheta = acosf(fDot) * fTime;	// theta = angle between v0 and result 

	//Quaternion qWork = qToCpy - qFromCpy * fDot;
	//QuaternionNormalize(qWork);			// { v0, v2 } is now an orthonormal basis
	//Quaternion qResult = qFromCpy * cosf(fTheta) + qWork * sinf(fTheta);
#endif
	return result;
}

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
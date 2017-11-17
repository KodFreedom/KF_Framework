//--------------------------------------------------------------------------------
//	���Z�p�֐�
//	KF_Math.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "KF_Math.h"
using namespace KF;

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
const Vector2		Vector2::Zero = Vector2(0.0f);				
const Vector2		Vector2::One = Vector2(1.0f);				
const Vector3		Vector3::Zero = Vector3(0.0f);				
const Vector3		Vector3::One = Vector3(1.0f);				
const Vector3		Vector3::Up = Vector3(0.0f, 1.0f, 0.0f);	
const Vector3		Vector3::Down = Vector3(0.0f, -1.0f, 0.0f);	
const Vector3		Vector3::Left = Vector3(-1.0f, 0.0f, 0.0f);	
const Vector3		Vector3::Right = Vector3(1.0f, 0.0f, 0.0f);	
const Vector3		Vector3::Forward = Vector3(0.0f, 0.0f, 1.0f);
const Vector3		Vector3::Back = Vector3(0.0f, 0.0f, -1.0f);	
const Vector3		Vector3::AxisX = Vector3::Right;
const Vector3		Vector3::AxisY = Vector3::Up;
const Vector3		Vector3::AxisZ = Vector3::Forward;
const Matrix44		Matrix44::Identity = Matrix44();
const Quaternion	Quaternion::Identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
const Color			Color::White = Color(1.0f, 1.0f, 1.0f, 1.0f);
const Color			Color::Black = Color(0.0f, 0.0f, 0.0f, 1.0f);
const Color			Color::Gray = Color(0.2f, 0.2f, 0.2f, 1.0f);
const Color			Color::Red = Color(1.0f, 0.0f, 0.0f, 1.0f);
const Color			Color::Blue = Color(0.0f, 0.0f, 1.0f, 1.0f);
const Color			Color::Green = Color(0.0f, 1.0f, 0.0f, 1.0f);

//--------------------------------------------------------------------------------
//
//  Vector2
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����Foperator D3DXVECTOR2()
//  �֐������F�L���X�g(D3DXVECTOR2)
//	�����F	�Ȃ�
//	�߂�l�FD3DXVECTOR2
//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
Vector2::operator D3DXVECTOR2() const
{
	D3DXVECTOR2 result;
	result.x = X;
	result.y = Y;
	return result;
}
#endif

//--------------------------------------------------------------------------------
//	�֐����Foperator=
//  �֐������F�l�̑������
//	�����F	value : �l
//	�߂�l�FVector2
//--------------------------------------------------------------------------------
Vector2& Vector2::operator=(const Vector2& value)
{
	X = value.X;
	Y = value.Y;
	return *this;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator==
//  �֐������F���l���菈��
//	�����F	value : ����
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool Vector2::operator==(const Vector2& value) const
{
	return (X == value.X && Y == value.Y);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator+
//  �֐������F�����Z
//	�����F	value : ����
//	�߂�l�FVector2
//--------------------------------------------------------------------------------
Vector2 Vector2::operator+(const Vector2& value) const
{
	return Vector2(X + value.X, Y + value.Y);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator+=
//  �֐������F�����Z
//	�����F	value : ����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Vector2::operator+=(const Vector2& value)
{
	X += value.X;
	Y += value.Y;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator-
//  �֐������F�����Z
//	�����F	value : ����
//	�߂�l�FVector2
//--------------------------------------------------------------------------------
Vector2 Vector2::operator-(const Vector2& value) const
{
	return Vector2(X - value.X, Y - value.Y);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator-=
//  �֐������F�����Z
//	�����F	value : ����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Vector2::operator-=(const Vector2& value)
{
	X -= value.X;
	Y -= value.Y;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*
//  �֐������F�O��
//	�����F	value : ����
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Vector2::operator*(const Vector2& value) const
{
	return (X * value.Y - Y * value.X);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*
//  �֐������F�X�P�[��
//	�����F	value : �X�P�[���l
//	�߂�l�FVector2
//--------------------------------------------------------------------------------
Vector2 Vector2::operator*(const float& value) const
{
	return Vector2(X * value, Y * value);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*=
//  �֐������F�X�P�[��
//	�����F	value : �X�P�[���l
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Vector2::operator*=(const float& value)
{
	X *= value;
	Y *= value;
}

//--------------------------------------------------------------------------------
//	�֐����FDot
//  �֐������FVector2�̓��όv�Z
//	�����F	value�F���όv�Z�̉E�葤
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Vector2::Dot(const Vector2& value) const
{
	return (X * value.X + Y * value.Y);
}

//--------------------------------------------------------------------------------
//	�֐����FMagnitude
//  �֐������F�����v�Z
//	�����F	�Ȃ�
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Vector2::Magnitude(void) const
{
	return sqrtf(X * X + Y * Y);
}

//--------------------------------------------------------------------------------
//	�֐����FSquareMagnitude
//  �֐������F�����̕����̌v�Z
//	�����F	�Ȃ�
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Vector2::SquareMagnitude(void) const
{
	return (X * X + Y * Y);
}

//--------------------------------------------------------------------------------
//	�֐����FNormalize
//  �֐������F���K��
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Vector2::Normalize(void)
{
	*this = this->Normalized();
}

//--------------------------------------------------------------------------------
//	�֐����FNormalized
//  �֐������F���K�������̒l��Ԃ�
//	�����F	�Ȃ�
//	�߂�l�FVector2
//--------------------------------------------------------------------------------
Vector2 Vector2::Normalized(void) const
{
	float magnitude = this->Magnitude();
	if (magnitude <= 0.0f) return Vector2(0.0f);
	return Vector2(X / magnitude, Y / magnitude);
}

//--------------------------------------------------------------------------------
//	�֐����FToRadian
//  �֐������F�p�x�̎Z�o
//	�����F	�Ȃ�
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Vector2::ToRadian(void) const
{
	return atan2f(Y, X);
}

//--------------------------------------------------------------------------------
//	�֐����FToPickingRay
//  �֐������F�X�N���[�����W���烌�C�̎Z�o
//	�����F	viewportSize�Fviewport�̕���
//			projectMatrix00�F�ˉe�s���00��
//			projectMatrix11�F�ˉe�s���11��
//			viewInverse�Fview�s��̋t�s��
//	�߂�l�FRay
//--------------------------------------------------------------------------------
Ray Vector2::ToPickingRay(const Vector2& viewportSize, const float& projectMatrix00, const float& projectMatrix11, const Matrix44& viewInverse)
{
	auto& position3D = Vector2(
		(((2.0f * X) / viewportSize.X) - 1.0f) / projectMatrix00,
		(((-2.0f * Y) / viewportSize.Y) + 1.0f) / projectMatrix11);
	Ray result;
	result.Origin = Vector3::Zero;
	result.Direction = Vector3(position3D.X, position3D.Y, 1.0f);
	result.Transform(viewInverse);
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FRadianBetween
//  �֐������FVector2�Ԃ̊p�x�̎Z�o
//	�����F	valueL�F���葤
//			valueR�F�E�葤
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Vector2::RadianBetween(const Vector2& valueL, const Vector2& valueR)
{
	if (valueL == valueR) return 0.0f;

	float squareMagnitudeL = valueL.SquareMagnitude();
	float squareMagnitudeR = valueR.SquareMagnitude();
	if (squareMagnitudeL <= 0.0f || squareMagnitudeR <= 0.0f) return 0.0f;
	
	float dot = valueL.Dot(valueR);
	float cross = valueL * valueR;
	float sign = cross >= 0.0f ? 1.0f : -1.0f;
	return acosf(dot / (sqrtf(squareMagnitudeL) * sqrtf(squareMagnitudeR)) * sign);
}

//--------------------------------------------------------------------------------
//
//  Vector3
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����Foperator Vector2()
//  �֐������F�L���X�g(Vector2)
//	�����F	�Ȃ�
//	�߂�l�FVector2
//--------------------------------------------------------------------------------
Vector3::operator Vector2() const
{
	return Vector2(X, Y);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator D3DXVECTOR3()
//  �֐������F�L���X�g(D3DXVECTOR3)
//	�����F	�Ȃ�
//	�߂�l�FD3DXVECTOR3
//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
Vector3::operator D3DXVECTOR3() const
{
	D3DXVECTOR3 result;
	result.x = X;
	result.y = Y;
	result.z = Z;
	return result;
}
#endif

//--------------------------------------------------------------------------------
//	�֐����Foperator=
//  �֐������F�l�̑������
//	�����F	value : �l
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3& Vector3::operator=(const Vector3& value)
{
	X = value.X;
	Y = value.Y;
	Z = value.Z;
	return *this;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator==
//  �֐������F���l���菈��
//	�����F	value : ����
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool Vector3::operator==(const Vector3& value) const
{
	return (X == value.X && Y == value.Y && Z == value.Z);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator!=
//  �֐������F�ْl���菈��
//	�����F	value : ����
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool Vector3::operator!=(const Vector3& value) const
{
	return (X != value.X || Y != value.Y || Z != value.Z);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator+
//  �֐������F�����Z
//	�����F	value : ����
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Vector3::operator+(const Vector3& value) const
{
	return Vector3(X + value.X, Y + value.Y, Z + value.Z);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator+=
//  �֐������F�����Z
//	�����F	value : ����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Vector3::operator+=(const Vector3& value)
{
	X += value.X;
	Y += value.Y;
	Z += value.Z;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator-
//  �֐������F�����Z
//	�����F	value : ����
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Vector3::operator-(const Vector3& value) const
{
	return Vector3(X - value.X, Y - value.Y, Z - value.Z);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator-=
//  �֐������F�����Z
//	�����F	value : ����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Vector3::operator-=(const Vector3& value)
{
	X -= value.X;
	Y -= value.Y;
	Z -= value.Z;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*
//  �֐������F�X�P�[��
//	�����F	value : �X�P�[���l
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Vector3::operator*(const float& value) const
{
	return Vector3(X * value, Y * value, Z * value);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*=
//  �֐������F�X�P�[��
//	�����F	value : �X�P�[���l
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Vector3::operator*=(const float& value)
{
	X *= value;
	Y *= value;
	Z *= value;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*
//  �֐������F�O��
//	�����F	value : ����
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Vector3::operator*(const Vector3& value) const
{
	return Vector3(Y * value.Z - value.Y * Z, Z * value.X - value.Z * X, X * value.Y - value.X * Y);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*=
//  �֐������F�O��
//	�����F	value : ����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Vector3::operator*=(const Vector3& value)
{
	*this = *this * value;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator/
//  �֐������F�X�P�[��
//	�����F	value : �X�P�[���l
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Vector3::operator/(const float& value) const
{
	return Vector3(X / value, Y / value, Z / value);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator/=
//  �֐������F�X�P�[��
//	�����F	value : �X�P�[���l
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Vector3::operator/=(const float& value)
{
	X /= value;
	Y /= value;
	Z /= value;
}

//--------------------------------------------------------------------------------
//	�֐����FDot
//  �֐������F����
//	�����F	value : ����
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Vector3::Dot(const Vector3& value) const
{
	return (X * value.X + Y * value.Y + Z * value.Z);
}

//--------------------------------------------------------------------------------
//	�֐����FMagnitude
//  �֐������F�����v�Z
//	�����F	�Ȃ�
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Vector3::Magnitude(void) const
{
	return sqrtf(X * X + Y * Y + Z * Z);
}

//--------------------------------------------------------------------------------
//	�֐����FSquareMagnitude
//  �֐������F�����̕����̌v�Z
//	�����F	�Ȃ�
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Vector3::SquareMagnitude(void) const
{
	return (X * X + Y * Y + Z * Z);
}

//--------------------------------------------------------------------------------
//	�֐����FNormalize
//  �֐������F���K��
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Vector3::Normalize(void)
{
	*this = this->Normalized();
}

//--------------------------------------------------------------------------------
//	�֐����FNormalized
//  �֐������F���K�������̒l��Ԃ�
//	�����F	�Ȃ�
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Vector3::Normalized(void) const
{
	float magnitude = this->Magnitude();
	if (magnitude <= 0.0f) return Vector3(0.0f);
	return *this / magnitude;
}

//--------------------------------------------------------------------------------
//	�֐����FToQuaternion
//  �֐������FEuler�p��Quaternion�ɕϊ�
//	�����F	�Ȃ�
//	�߂�l�FQuaternion
//--------------------------------------------------------------------------------
Quaternion Vector3::ToQuaternion(void) const
{
	// Todo : �v�Z���𒲂ׂ�
	Quaternion result;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	D3DXQUATERNION resultDX;
	D3DXQuaternionRotationYawPitchRoll(&resultDX, Y, X, Z);
	result.X = resultDX.x;
	result.Y = resultDX.y;
	result.Z = resultDX.z;
	result.W = resultDX.w;
#endif
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FDistanceBetween
//  �֐������F���_�̊Ԃ̋����̎Z�o
//	�����F	pointA�ApointB�F�v�Z����
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Vector3::DistanceBetween(const Vector3& pointA, const Vector3& pointB)
{
	return (pointA - pointB).Magnitude();
}

//--------------------------------------------------------------------------------
//	�֐����FSquareDistanceBetween
//  �֐������F���_�̊Ԃ̋����̕����̎Z�o
//	�����F	pointA�ApointB�F�v�Z����
//	�߂�l�FQuaternion
//--------------------------------------------------------------------------------
float Vector3::SquareDistanceBetween(const Vector3& pointA, const Vector3& pointB)
{
	return (pointA - pointB).SquareMagnitude();
}

//--------------------------------------------------------------------------------
//	�֐����FTransformCoord
//  �֐������F�|�C���g����]����
//	�����F	point�F��]����
//			rotation�F��]�s��
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Vector3::TransformCoord(const Vector3& point, const Matrix44& transform)
{
	auto& work = Vector4(point, 1.0f);
	work *= transform;
	auto result = Vector3::Zero;
	if (work.W != 0.0f)
	{
		result.X = work.X / work.W;
		result.Y = work.Y / work.W;
		result.Z = work.Z / work.W;
	}
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FTransformNormal
//  �֐������F�x�N�g������]����
//	�����F	normal�F��]����
//			rotation�F��]�s��
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Vector3::TransformNormal(const Vector3& normal, const Matrix44& transform)
{
	auto& work = Vector4(normal, 0.0f);
	work *= transform;
	return Vector3(work.X, work.Y, work.Z);
}

//--------------------------------------------------------------------------------
//	�֐����FTransformInverse
//  �֐������F�|�C���g���t�s��ɂ��ϊ�����
//	�����F	point�F�ϊ�����
//			transform�F�ϊ��s��
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Vector3::TransformInverse(const Vector3& point, const Matrix44& transform)
{
	auto work = point;
	work.X -= transform.Elements[3][0];
	work.Y -= transform.Elements[3][1];
	work.Z -= transform.Elements[3][2];
	Vector3 result;
	result.X = work.X * transform.Elements[0][0] +
		work.Y * transform.Elements[0][1] +
		work.Z * transform.Elements[0][2];
	result.Y = work.X * transform.Elements[1][0] +
		work.Y * transform.Elements[1][1] +
		work.Z * transform.Elements[1][2];
	result.Z = work.X * transform.Elements[2][0] +
		work.Y * transform.Elements[2][1] +
		work.Z * transform.Elements[2][2];
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FScale
//  �֐������FXYZ�ʁX�ŃX�P�[������
//	�����F	value�F�X�P�[���������l
//			scale�F�X�P�[���l
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Vector3::Scale(const Vector3& value, const Vector3& scale)
{
	return Vector3(value.X * scale.X, value.Y * scale.Y, value.Z * scale.Z);
}

//--------------------------------------------------------------------------------
//	�֐����FEulerBetween
//  �֐������F�x�N�g���Ԃ�euler�p�̎Z�o
//	�����F	directionA�AdirectionB�F�x�N�g��
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Vector3::EulerBetween(const Vector3& directionA, const Vector3& directionB)
{
	Vector3 result;

	//X����]
	result.X = Vector2::RadianBetween(Vector2(directionA.Y, directionA.Z), Vector2(directionB.Y, directionB.Z));

	//Y����]
	result.Y = Vector2::RadianBetween(Vector2(directionA.Z, directionA.X), Vector2(directionB.Z, directionB.X));

	//Z����]
	result.Z = Vector2::RadianBetween(Vector2(directionA.X, directionA.Y), Vector2(directionB.X, directionB.Y));

	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FProjectOnPlane
//  �֐������F�����𕽖ʂƕ��s�̕����ɉ�]����
//	�����F	direction�F��]����������
//			planeNormal�F���ʂ̏����
//			currentNormal�F���̏����
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Vector3::ProjectOnPlane(const Vector3& direction, const Vector3& planeNormal, const Vector3& currentNormal)
{
	return (currentNormal * direction) * planeNormal;
}

//--------------------------------------------------------------------------------
//
//  Vector4
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����Foperator*
//  �֐������F�}�g���N�X�Ƃ̏�Z
//	�����F	matrix : ��Z����
//	�߂�l�FVector4
//--------------------------------------------------------------------------------
Vector4 Vector4::operator*(const Matrix44 &matrix) const
{
	Vector4 result;
	result.X =	X * matrix.Elements[0][0] +
				Y * matrix.Elements[1][0] +
				Z * matrix.Elements[2][0] +
				W * matrix.Elements[3][0];
	result.Y =	X * matrix.Elements[0][1] +
				Y * matrix.Elements[1][1] +
				Z * matrix.Elements[2][1] +
				W * matrix.Elements[3][1];
	result.Z =	X * matrix.Elements[0][2] +
				Y * matrix.Elements[1][2] +
				Z * matrix.Elements[2][2] +
				W * matrix.Elements[3][2];
	result.W =	X * matrix.Elements[0][3] +
				Y * matrix.Elements[1][3] +
				Z * matrix.Elements[2][3] +
				W * matrix.Elements[3][3];
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*=
//  �֐������F�}�g���N�X�Ƃ̏�Z
//	�����F	matrix : ��Z����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Vector4::operator*=(const Matrix44 &matrix)
{
	Vector4 copy = *this;
	X = copy.X * matrix.Elements[0][0] +
		copy.Y * matrix.Elements[1][0] +
		copy.Z * matrix.Elements[2][0] +
		copy.W * matrix.Elements[3][0];
	Y = copy.X * matrix.Elements[0][1] +
		copy.Y * matrix.Elements[1][1] +
		copy.Z * matrix.Elements[2][1] +
		copy.W * matrix.Elements[3][1];
	Z = copy.X * matrix.Elements[0][2] +
		copy.Y * matrix.Elements[1][2] +
		copy.Z * matrix.Elements[2][2] +
		copy.W * matrix.Elements[3][2];
	W = copy.X * matrix.Elements[0][3] +
		copy.Y * matrix.Elements[1][3] +
		copy.Z * matrix.Elements[2][3] +
		copy.W * matrix.Elements[3][3];
}

//--------------------------------------------------------------------------------
//
//  Matrix44
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
Matrix44::Matrix44()
{
	float elements[4][4] = {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f } };
	memcpy(Elements, elements, sizeof(Elements));
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
Matrix44::Matrix44(
	const float& element11, const float& element12, const float& element13, const float& element14,
	const float& element21, const float& element22, const float& element23, const float& element24,
	const float& element31, const float& element32, const float& element33, const float& element34,
	const float& element41, const float& element42, const float& element43, const float& element44)
{
	Element11 = element11;
	Element12 = element12;
	Element13 = element13;
	Element14 = element14;
	Element21 = element21;
	Element22 = element22;
	Element23 = element23;
	Element24 = element24;
	Element31 = element31;
	Element32 = element32;
	Element33 = element33;
	Element34 = element34;
	Element41 = element41;
	Element42 = element42;
	Element43 = element43;
	Element44 = element44;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator D3DXMATRIX()
//  �֐������F�L���X�g(D3DXMATRIX)
//	�����F	�Ȃ�
//	�߂�l�FD3DXMATRIX
//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
Matrix44::operator D3DXMATRIX() const
{
	D3DXMATRIX result;
	for (int countY = 0; countY < 4; ++countY)
	{
		for (int countX = 0; countX < 4; ++countX)
		{
			result(countY, countX) = Elements[countY][countX];
		}
	}
	return result;
}
#endif

//--------------------------------------------------------------------------------
//	�֐����Foperator=
//  �֐������F�l�̑������
//	�����F	value : �l
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44& Matrix44::operator=(const Matrix44 &value)
{
	memcpy(Elements, value.Elements, sizeof(Elements));
	return *this;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*
//  �֐������Ffloat�Ə�Z����
//	�����F	value : ����
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44 Matrix44::operator*(const float& value) const
{
	Matrix44 result;
	for (int countY = 0; countY < 4; ++countY)
	{
		for (int countX = 0; countX < 4; ++countX)
		{
			result.Elements[countY][countX] = Elements[countY][countX] * value;
		}
	}
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator+
//  �֐������F���Z����
//	�����F	value : ����
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44 Matrix44::operator+(const Matrix44 &value) const
{
	Matrix44 result;
	for (int countY = 0; countY < 4; ++countY)
	{
		for (int countX = 0; countX < 4; ++countX)
		{
			result.Elements[countY][countX] = Elements[countY][countX] + value.Elements[countY][countX];
		}
	}
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*
//  �֐������F��Z����
//	�����F	value : ����
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44 Matrix44::operator*(const Matrix44 &value) const
{
	Matrix44 result;
	for (int countY = 0; countY < 4; ++countY)
	{
		for (int countX = 0; countX < 4; ++countX)
		{
			result.Elements[countY][countX] =
				Elements[countY][0] * value.Elements[0][countX] + 
				Elements[countY][1] * value.Elements[1][countX] + 
				Elements[countY][2] * value.Elements[2][countX] + 
				Elements[countY][3] * value.Elements[3][countX];
		}
	}
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*
//  �֐������F��Z����
//	�����F	value : ����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Matrix44::operator*=(const Matrix44 &value)
{
	Matrix44 copy = *this;
	for (int countY = 0; countY < 4; ++countY)
	{
		for (int countX = 0; countX < 4; ++countX)
		{
			Elements[countY][countX] =
				copy.Elements[countY][0] * value.Elements[0][countX] +
				copy.Elements[countY][1] * value.Elements[1][countX] +
				copy.Elements[countY][2] * value.Elements[2][countX] +
				copy.Elements[countY][3] * value.Elements[3][countX];
		}
	}
}

//--------------------------------------------------------------------------------
//	�֐����FToEular
//  �֐������FEular�ɕϊ�
//	�����F	�Ȃ�
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Matrix44::ToEular(void) const
{
	return this->ToQuaternion().ToEuler();
}

//--------------------------------------------------------------------------------
//	�֐����FToQuaternion
//  �֐������FQuaternion�ɕϊ�
//	�����F	�Ȃ�
//	�߂�l�FQuaternion
//--------------------------------------------------------------------------------
Quaternion Matrix44::ToQuaternion(void) const
{
	Quaternion result;
	if (Elements[2][2] <= 0.0f)  // x^2 + y^2 >= z^2 + w^2
	{
		float difference1100 = Elements[1][1] - Elements[0][0];
		float oneMinus22 = 1.0f - Elements[2][2];
		if (difference1100 <= 0.0f)  // x^2 >= y^2
		{
			float square4X = oneMinus22 - difference1100;
			float inverse4X = 0.5f / sqrtf(square4X);
			result.X = square4X * inverse4X;
			result.Y = (Elements[0][1] + Elements[1][0]) * inverse4X;
			result.Z = (Elements[0][2] + Elements[2][0]) * inverse4X;
			result.W = (Elements[1][2] - Elements[2][1]) * inverse4X;
		}
		else  // y^2 >= x^2
		{
			float square4Y = oneMinus22 + difference1100;
			float inverse4Y = 0.5f / sqrtf(square4Y);
			result.X = (Elements[0][1] + Elements[1][0]) * inverse4Y;
			result.Y = square4Y * inverse4Y;
			result.Z = (Elements[1][2] + Elements[2][1]) * inverse4Y;
			result.W = (Elements[2][0] - Elements[0][2]) * inverse4Y;
		}
	}
	else  // z^2 + w^2 >= x^2 + y^2
	{
		float sum1100 = Elements[1][1] + Elements[0][0];
		float onePlus22 = 1.0f + Elements[2][2];
		if (sum1100 <= 0.0f)  // z^2 >= w^2
		{
			float square4Z = onePlus22 - sum1100;
			float inverse4Z = 0.5f / sqrtf(square4Z);
			result.X = (Elements[0][2] + Elements[2][0]) * inverse4Z;
			result.Y = (Elements[1][2] + Elements[2][1]) * inverse4Z;
			result.Z = square4Z * inverse4Z;
			result.W = (Elements[0][1] - Elements[1][0]) * inverse4Z;
		}
		else  // w^2 >= z^2
		{
			float square4W = onePlus22 + sum1100;
			float inverse4W = 0.5f / sqrtf(square4W);
			result.X = (Elements[1][2] - Elements[2][1]) * inverse4W;
			result.Y = (Elements[2][0] - Elements[0][2]) * inverse4W;
			result.Z = (Elements[0][1] - Elements[1][0]) * inverse4W;
			result.W = square4W * inverse4W;
		}
	}
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FTranspose
//  �֐������FTranspose�s��ɕϊ�
//	�����F	�Ȃ�
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44 Matrix44::Transpose(void) const
{
	Matrix44 result;
	for (int countY = 0; countY < 4; ++countY)
	{
		for (int countX = 0; countX < 4; ++countX)
		{
			result.Elements[countY][countX] = Elements[countX][countY];
		}
	}
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FScale
//  �֐������F�^����ꂽ�l�ŃX�P�[���s��̍쐬
//	�����F	scale�F�X�P�[���l
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44 Matrix44::Scale(const Vector3& scale)
{
	return Matrix44(
		scale.X, 0.0f, 0.0f, 0.0f,
		0.0f, scale.Y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.Z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

//--------------------------------------------------------------------------------
//	�֐����FRotation
//  �֐������F�^����ꂽ��]�ōs��̍쐬
//	�����F	right�F�E����
//			up�F�����
//			forward�F�O����
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44 Matrix44::Rotation(const Vector3& right, const Vector3& up, const Vector3& forward)
{
	return Matrix44(
		right.X, right.Y, right.Z, 0.0f,
		up.X, up.Y, up.Z, 0.0f,
		forward.X, forward.Y, forward.Z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

//--------------------------------------------------------------------------------
//	�֐����FRotationAxis
//  �֐������F�^����ꂽ���Ɗp�ŉ�]�s��̍쐬
//	�����F	axis�F��
//			radian�F�p�x
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44 Matrix44::RotationAxis(const Vector3& axis, const float& radian)
{
	Matrix44 result;
	float x = axis.X;
	float y = axis.Y;
	float z = axis.Z;
	float sin = sinf(radian);
	float cos = cosf(radian);
	result.Elements[0][0] = cos + x * x * (1.0f - cos);
	result.Elements[0][1] = x * y * (1.0f - cos) + z * sin;
	result.Elements[0][2] = x * z * (1.0f - cos) - y * sin;
	result.Elements[1][0] = x * y * (1.0f - cos) - z * sin;
	result.Elements[1][1] = cos + y * y * (1.0f - cos);
	result.Elements[1][2] = z * y * (1.0f - cos) + x * sin;
	result.Elements[2][0] = x * z * (1.0f - cos) + y * sin;
	result.Elements[2][1] = y * z * (1.0f - cos) - x * sin;
	result.Elements[2][2] = cos + z * z * (1.0f - cos);
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FRotationYawPitchRoll
//  �֐������F�^����ꂽeuler�p�ŉ�]�s��̍쐬
//	�����F	euler�F�p�x
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44 Matrix44::RotationYawPitchRoll(const Vector3& euler)
{
	Matrix44 result;
	float sinX = sinf(euler.X);
	float cosX = cosf(euler.X);
	float sinY = sinf(euler.Y);
	float cosY = cosf(euler.Y);
	float sinZ = sinf(euler.Z);
	float cosZ = cosf(euler.Z);
	result.Elements[0][0] = cosY * cosZ + sinX * sinY * sinZ;
	result.Elements[0][1] = cosX * sinZ;
	result.Elements[0][2] = -sinY * cosZ + sinX * cosY * sinZ;
	result.Elements[1][0] = -cosY * sinZ + sinX * sinY * cosZ;
	result.Elements[1][1] = cosX * cosZ;
	result.Elements[1][2] = sinY * sinZ + sinX * cosY * cosZ;
	result.Elements[2][0] = cosX * sinY;
	result.Elements[2][1] = -sinX;
	result.Elements[2][2] = cosX * cosY;
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FTranslation
//  �֐������F�^����ꂽ�ړ��ʂŕ��s�ړ��s��̍쐬
//	�����F	translation�F�ړ���
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44 Matrix44::Translation(const Vector3& translation)
{
	Matrix44 result;
	result.Elements[3][0] = translation.X;
	result.Elements[3][1] = translation.Y;
	result.Elements[3][2] = translation.Z;
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FTransform
//  �֐������F�^����ꂽ��]�ƈړ��ʂōs��̍쐬
//	�����F	right�F�E����
//			up�F�����
//			forward�F�O����
//			translation�F�ړ���
//			scale�F�X�P�[����
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44 Matrix44::Transform(const Vector3& right, const Vector3& up, const Vector3& forward, const Vector3& translation, const Vector3& scale)
{
	return Matrix44(
		right.X * scale.X, right.Y * scale.X, right.Z * scale.X, 0.0f,
		up.X * scale.Y, up.Y * scale.Y, up.Z * scale.Y, 0.0f,
		forward.X * scale.Z, forward.Y * scale.Z, forward.Z * scale.Z, 0.0f,
		translation.X, translation.Y, translation.Z, 1.0f);
}

//--------------------------------------------------------------------------------
//	�֐����FTransform
//  �֐������F�^����ꂽ��]�ƈړ��ʂōs��̍쐬
//	�����F	rotation�F��]��
//			translation�F�ړ���
//			scale�F�X�P�[����
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44 Matrix44::Transform(const Vector3& rotation, const Vector3& translation, const Vector3& scale)
{
	auto& result = Matrix44(
		scale.X, 0.0f, 0.0f, 0.0f,
		0.0f, scale.Y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.Z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	result *= RotationYawPitchRoll(rotation);
	result *= Translation(translation);
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FProjectionLeftHand
//  �֐������F�ˉe�s��̍쐬(������W�n)
//	�����F	fovAngleY�F����p�x
//			aspectRatio�F�A�X�y�N�g��
//			nearZ�F��ԋ߂�����
//			farZ�F��ԉ�������
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44 Matrix44::ProjectionLeftHand(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ)
{
	float cosFov = cosf(fovAngleY * 0.5f);
	float sinFov = sqrtf(1.0f - cosFov * cosFov);
	if (fovAngleY * 0.5f > Pi) sinFov = -sinFov;

	float height = cosFov / sinFov;
	float width = height / aspectRatio;
	float range = farZ / (farZ - nearZ);

	Matrix44 result;
	result.Elements[0][0] = width;
	result.Elements[0][1] = 0.0f;
	result.Elements[0][2] = 0.0f;
	result.Elements[0][3] = 0.0f;
	result.Elements[1][0] = 0.0f;
	result.Elements[1][1] = height;
	result.Elements[1][2] = 0.0f;
	result.Elements[1][3] = 0.0f;
	result.Elements[2][0] = 0.0f;
	result.Elements[2][1] = 0.0f;
	result.Elements[2][2] = range;
	result.Elements[2][3] = 1.0f;
	result.Elements[3][0] = 0.0f;
	result.Elements[3][1] = 0.0f;
	result.Elements[3][2] = -range * nearZ;
	result.Elements[3][3] = 0.0f;
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FToMatrix44
//  �֐������FD3DXMATRIX��Matrix44�ɕϊ�
//	�����F	value�FD3DXMATRIX
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
Matrix44 Matrix44::ToMatrix44(const D3DXMATRIX& value)
{
	Matrix44 result;
	for (int countY = 0; countY < 4; ++countY)
	{
		for (int countX = 0; countX < 4; ++countX)
		{
			result.Elements[countY][countX] = value(countY, countX);
		}
	}
	return result;
}
#endif

//--------------------------------------------------------------------------------
//  Quaternion
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����Foperator D3DXQUATERNION()
//  �֐������F�L���X�g(D3DXQUATERNION)
//	�����F	�Ȃ�
//	�߂�l�FD3DXQUATERNION
//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
Quaternion::operator D3DXQUATERNION() const
{
	D3DXQUATERNION result;
	result.x = X;
	result.y = Y;
	result.z = Z;
	result.w = W;
	return result;
}
#endif

//--------------------------------------------------------------------------------
//	�֐����Foperator+
//  �֐������F�����Z
//	�����F	value : ����
//	�߂�l�FQuaternion
//--------------------------------------------------------------------------------
Quaternion Quaternion::operator+(const Quaternion& value) const
{
	return Quaternion(X + value.X, Y + value.Y, Z + value.Z, W + value.W);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator+=
//  �֐������F�����Z
//	�����F	value : ����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Quaternion::operator+=(const Quaternion& value)
{
	X += value.X;
	Y += value.Y;
	Z += value.Z;
	W += value.W;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator-
//  �֐������F�����Z
//	�����F	value : ����
//	�߂�l�FQuaternion
//--------------------------------------------------------------------------------
Quaternion Quaternion::operator-(const Quaternion& value) const
{
	return Quaternion(X - value.X, Y - value.Y, Z - value.Z, W - value.W);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator-=
//  �֐������F�����Z
//	�����F	value : ����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Quaternion::operator-=(const Quaternion& value)
{
	X -= value.X;
	Y -= value.Y;
	Z -= value.Z;
	W -= value.W;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*
//  �֐������F��Z(float)
//	�����F	value : ����
//	�߂�l�FQuaternion
//--------------------------------------------------------------------------------
Quaternion Quaternion::operator*(const float& value) const
{
	return Quaternion(X * value, Y * value, Z * value, W * value);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*=
//  �֐������F��Z(float)
//	�����F	value : ����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Quaternion::operator*=(const float& value)
{
	X *= value;
	Y *= value;
	Z *= value;
	W *= value;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator/
//  �֐������F���Z(float)
//	�����F	value : ����
//	�߂�l�FQuaternion
//--------------------------------------------------------------------------------
Quaternion Quaternion::operator/(const float& value) const
{
	return Quaternion(X / value, Y / value, Z / value, W / value);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator/=
//  �֐������F���Z(float)
//	�����F	value : ����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Quaternion::operator/=(const float& value)
{
	X /= value;
	Y /= value;
	Z /= value;
	W /= value;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*
//  �֐������F��Z
//	�����F	value : ����
//	�߂�l�FQuaternion
//--------------------------------------------------------------------------------
Quaternion Quaternion::operator*(const Quaternion& value) const
{
	return Quaternion(
		(value.W * this->X) + (value.X * this->W) + (value.Y * this->Z) - (value.Z * this->Y),
		(value.W * this->Y) - (value.X * this->Z) + (value.Y * this->W) + (value.Z * this->X),
		(value.W * this->Z) + (value.X * this->Y) - (value.Y * this->X) + (value.Z * this->W),
		(value.W * this->W) - (value.X * this->X) - (value.Y * this->Y) - (value.Z * this->Z));
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*=
//  �֐������F��Z
//	�����F	value : ����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Quaternion::operator*=(const Quaternion& value)
{
	auto copy = *this;
	this->X = (value.W * copy.X) + (value.X * copy.W) + (value.Y * copy.Z) - (value.Z * copy.Y);
	this->Y = (value.W * copy.Y) - (value.X * copy.Z) + (value.Y * copy.W) + (value.Z * copy.X);
	this->Z = (value.W * copy.Z) + (value.X * copy.Y) - (value.Y * copy.X) + (value.Z * copy.W);
	this->W = (value.W * copy.W) - (value.X * copy.X) - (value.Y * copy.Y) - (value.Z * copy.Z);
}

//--------------------------------------------------------------------------------
//	�֐����FDot
//  �֐������F��Z
//	�����F	value : ����
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Quaternion::Dot(const Quaternion& value) const
{
	return (X * value.X + Y * value.Y + Z * value.Z + W * value.W);
}

//--------------------------------------------------------------------------------
//	�֐����FSquareMagnitude
//  �֐������F�����̕����̎Z�o
//	�����F	�Ȃ�
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Quaternion::SquareMagnitude(void) const
{
	return this->Dot(*this);
}

//--------------------------------------------------------------------------------
//	�֐����FMagnitude
//  �֐������F�����̎Z�o
//	�����F	�Ȃ�
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Quaternion::Magnitude(void) const
{
	return sqrtf(this->SquareMagnitude());
}

//--------------------------------------------------------------------------------
//	�֐����FNormalize
//  �֐������F���K��
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Quaternion::Normalize(void)
{
	*this = this->Normalized();
}

//--------------------------------------------------------------------------------
//	�֐����FNormalize
//  �֐������F���K�������̒l��Ԃ�
//	�����F	�Ȃ�
//	�߂�l�FQuaternion
//--------------------------------------------------------------------------------
Quaternion Quaternion::Normalized(void) const
{
	float magnitude = this->Magnitude();
	if (magnitude <= 0.0f) return Quaternion();
	return *this / magnitude;
}

//--------------------------------------------------------------------------------
//	�֐����FMultiplySeparately
//  �֐������F�ʁX�ŏ�Z
//	�����F	value�F����
//	�߂�l�FQuaternion
//--------------------------------------------------------------------------------
Quaternion Quaternion::MultiplySeparately(const Quaternion& value) const
{
	return Quaternion(X * value.X, Y * value.Y, Z * value.Z, W * value.W);
}

//--------------------------------------------------------------------------------
//	�֐����FToEuler
//  �֐������Feuler�p�ɕϊ�
//	�����F	�Ȃ�
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Quaternion::ToEuler(void) const
{
	Vector3 result;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	D3DXQUATERNION quaternion = *this;
	D3DXQuaternionToAxisAngle(&quaternion, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), &result.X);
	D3DXQuaternionToAxisAngle(&quaternion, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &result.Y);
	D3DXQuaternionToAxisAngle(&quaternion, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &result.Z);
#else
	result.X = atan2f(2.0f * (W * Z + X * Y), 1.0f - 2.0f * (Z * Z + X * X));
	result.Y = asinf(2.0f * (W * X - Y * Z));
	result.Z = atan2f(2.0f * (W * Y + Z * X), 1.0f - 2.0f * (X * X + Y * Y));
#endif
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FToMatrix
//  �֐������F�s��ɕϊ�
//	�����F	�Ȃ�
//	�߂�l�FMatrix44
//--------------------------------------------------------------------------------
Matrix44 Quaternion::ToMatrix(void) const
{
	Matrix44 result;
	static const auto quaternion1110 = Quaternion(1.0f, 1.0f, 1.0f, 0.0f);
	auto& this_x2 = *this + *this;
	auto& this_x_this_x2 = this->MultiplySeparately(this_x2);
	auto& work0 = quaternion1110
		- Quaternion(this_x_this_x2.Y, this_x_this_x2.X, this_x_this_x2.X, quaternion1110.W)
		- Quaternion(this_x_this_x2.Z, this_x_this_x2.Z, this_x_this_x2.Y, quaternion1110.W);
	auto& work1 = Quaternion(X, X, Y, W).MultiplySeparately(Quaternion(this_x2.Z, this_x2.Y, this_x2.Z, this_x2.W));
	auto& work2 = Quaternion(W, W, W, W).MultiplySeparately(Quaternion(this_x2.Y, this_x2.Z, this_x2.X, this_x2.W));
	auto& OnePlusTwo = work1 + work2;
	auto& OneMinusTwo = work1 - work2;
	result.Elements[0][0] = work0.X;
	result.Elements[0][1] = OnePlusTwo.Y;
	result.Elements[0][2] = OneMinusTwo.X;
	result.Elements[0][3] = work0.W;
	result.Elements[1][0] = OneMinusTwo.Y;
	result.Elements[1][1] = work0.Y;
	result.Elements[1][2] = OnePlusTwo.Z;
	result.Elements[1][3] = work0.W;
	result.Elements[2][0] = OnePlusTwo.X;
	result.Elements[2][1] = OneMinusTwo.Z;
	result.Elements[2][2] = work0.Z;
	result.Elements[2][3] = work0.W;
	return result;
}

//--------------------------------------------------------------------------------
//
//  Color
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����Foperator=
//  �֐������F�l�̑������
//	�����F	value : �l
//	�߂�l�FColor
//--------------------------------------------------------------------------------
Color& Color::operator=(const Color& value)
{
	R = value.R;
	G = value.G;
	B = value.B;
	A = value.A;
	return *this;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator==
//  �֐������F���l���菈��
//	�����F	value : ����
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool Color::operator==(const Color& value)
{
	return (R == value.R && G == value.G && B == value.B && A == value.A);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator+
//  �֐������F�����Z����
//	�����F	value : �l
//	�߂�l�FColor
//--------------------------------------------------------------------------------
Color Color::operator+(const Color& value) const
{
	return Color(R + value.R, G + value.G, B + value.B, A + value.A);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator+=
//  �֐������F�����Z����
//	�����F	value : �l
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Color::operator+=(const Color& value)
{
	R += value.R;
	G += value.G;
	B += value.B;
	A += value.A;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator-
//  �֐������F�����Z����
//	�����F	value : �l
//	�߂�l�FColor
//--------------------------------------------------------------------------------
Color Color::operator-(const Color& value) const
{
	return Color(R - value.R, G - value.G, B - value.B, A - value.A);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator-=
//  �֐������F�����Z����
//	�����F	value : �l
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Color::operator-=(const Color& value)
{
	R -= value.R;
	G -= value.G;
	B -= value.B;
	A -= value.A;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*
//  �֐������F�|���Z(float)
//	�����F	value : �l
//	�߂�l�FColor
//--------------------------------------------------------------------------------
Color Color::operator*(const float& value) const
{
	return Color(R * value, G * value, B * value, A * value);
}

//--------------------------------------------------------------------------------
//	�֐����Foperator*=
//  �֐������F�|���Z(float)
//	�����F	value : �l
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Color::operator*=(const float& value)
{
	R *= value;
	G *= value;
	B *= value;
	A *= value;
}

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
//--------------------------------------------------------------------------------
//	�֐����Foperator D3DCOLORVALUE()
//  �֐������F�L���X�g(D3DCOLORVALUE)
//	�����F	�Ȃ�
//	�߂�l�FD3DCOLORVALUE
//--------------------------------------------------------------------------------
Color::operator D3DCOLORVALUE() const
{
	D3DCOLORVALUE result;
	result.r = R;
	result.g = G;
	result.b = B;
	result.a = A;
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����Foperator unsigned long()
//  �֐������F�L���X�g(unsigned long)
//	�����F	�Ȃ�
//	�߂�l�Funsigned long
//--------------------------------------------------------------------------------
Color::operator unsigned long() const
{
	return (unsigned long)((((unsigned long)(A * 255.0f) & 0xff) << 24) 
		| (((unsigned long)(R * 255.0f) & 0xff) << 16) 
		| (((unsigned long)(G * 255.0f) & 0xff) << 8) 
		| ((unsigned long)(B * 255.0f) & 0xff));
}
#endif

//--------------------------------------------------------------------------------
//
//  Ray
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FTransform
//  �֐������F���C�̕ϊ�
//	�����F	transform : �ϊ��s��
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Ray::Transform(const Matrix44& transform)
{
	Origin = Vector3::TransformCoord(Origin, transform);
	Direction = Vector3::TransformNormal(Direction, transform);
}

//--------------------------------------------------------------------------------
//
//  Random
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FInit
//  �֐������F�����_���V�X�e��������
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void Random::Init(void)
{
	srand((unsigned)time(NULL));
}

//--------------------------------------------------------------------------------
//	�֐����FRange
//  �֐������F�����_����Int�l���擾
//	�����F	min�F�ŏ��l
//			max�F�ő�l
//	�߂�l�Fint
//--------------------------------------------------------------------------------
int	Random::Range(const int& min, const int& max)
{
	if (min >= max) return min;
	return rand() % (max - min) + min;
}

//--------------------------------------------------------------------------------
//	�֐����FRange
//  �֐������F�����_����float�l���擾
//	�����F	min�F�ŏ��l
//			max�F�ő�l
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Random::Range(const float& min, const float& max)
{
	if (min >= max) { return min; }
	return (rand() % 10000) * 0.0001f * (max - min) + min;
}

//--------------------------------------------------------------------------------
//	�֐����FRange
//  �֐������F�����_����Vector3�l���擾
//	�����F	min�F�ŏ��l
//			max�F�ő�l
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Random::Range(const Vector3& min, const Vector3& max)
{
	return Vector3(Range(min.X, max.X), Range(min.Y, max.Y), Range(min.Z, max.Z));
}

//--------------------------------------------------------------------------------
//
//  Math
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FLerp
//  �֐������FVector3����`��ԕ����ŕ�Ԃ���
//	�����F	from�F�n�_
//			to�F�I�_
//			time�F����(0 - 1)
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Math::Lerp(const Vector3& from, const Vector3& to, const float& time)
{
	if (time <= 0.0f) return from;
	if (time >= 1.0f) return to;
	return (from + (to - from) * time);
}

//--------------------------------------------------------------------------------
//	�֐����FLerp
//  �֐������Ffloat����`��ԕ����ŕ�Ԃ���
//	�����F	from�F�n�_
//			to�F�I�_
//			time�F����(0 - 1)
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Math::Lerp(const float& from, const float& to, const float& time)
{
	if (time <= 0.0f) return from;
	if (time >= 1.0f) return to;
	return (from + (to - from) * time);
}

//--------------------------------------------------------------------------------
//	�֐����FLerp
//  �֐������FColor����`��ԕ����ŕ�Ԃ���
//	�����F	from�F�n�_
//			to�F�I�_
//			time�F����(0 - 1)
//	�߂�l�FColor
//--------------------------------------------------------------------------------
Color Math::Lerp(const Color& from, const Color& to, const float& time)
{
	if (time <= 0.0f) return from;
	if (time >= 1.0f) return to;
	return (from + (to - from) * time);
}

//--------------------------------------------------------------------------------
//	�֐����FSlerp
//  �֐������FVector3�����`��ԕ����ŕ�Ԃ���
//	�����F	from�F�n�_
//			to�F�I�_
//			time�F����(0 - 1)
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Vector3 Math::Slerp(const Vector3& from, const Vector3& to, const float& time)
{
	if (time <= 0.0f) return from;
	if (time >= 1.0f) return to;
	return (from + (to - from) * time).Normalized();
}

//--------------------------------------------------------------------------------
//	�֐����FSlerp
//  �֐������FQuaternion�����`��ԕ����ŕ�Ԃ���
//	�����F	from�F�n�_
//			to�F�I�_
//			time�F����(0 - 1)
//	�߂�l�FVector3
//--------------------------------------------------------------------------------
Quaternion Math::Slerp(const Quaternion& from, const Quaternion& to, const float& time)
{
	if (time <= 0.0f) { return from; }
	if (time >= 1.0f) { return to; }
	Quaternion result;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	D3DXQUATERNION fromDX = from;
	D3DXQUATERNION toDX = to;
	D3DXQUATERNION resultDX;
	D3DXQuaternionSlerp(&resultDX, &fromDX, &toDX, time);
	result.X = resultDX.x;
	result.Y = resultDX.y;
	result.Z = resultDX.z;
	result.W = resultDX.w;
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

//--------------------------------------------------------------------------------
//	�֐����FClamp
//  �֐������Ffloat��min��max�̊Ԃɂ���
//	�����F	value�F���ݒl
//			min�F�ŏ��l
//			max�F�ő�l
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Math::Clamp(const float& value, const float& min, const float& max)
{
	return value < min ? min : value > max ? max : value;
}

//--------------------------------------------------------------------------------
//	�֐����FAbsMax
//  �֐������F��Βl���傫������Ԃ�
//	�����F	valueA�AvalueB�F��r�l
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float Math::AbsMax(const float& valueA, const float& valueB)
{
	return fabsf(valueA) >= fabsf(valueB) ? valueA : valueB;
}

/*
//--------------------------------------------------------------------------------
//  Others
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  NormalizeRotInTwoPi(float)
//	��]��-2Pi����2Pi�̊Ԃɂ���
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
//	��]��-2Pi����2Pi�̊Ԃɂ���
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInTwoPi(Vector3& vRot)
{
	NormalizeRotInTwoPi(vRot.X);
	NormalizeRotInTwoPi(vRot.Y);
	NormalizeRotInTwoPi(vRot.Z);
}

//--------------------------------------------------------------------------------
//  NormalizeRotInPi(float)
//	��]��-Pi����Pi�̊Ԃɂ���
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
//	��]��-Pi����Pi�̊Ԃɂ���
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInPi(Vector3& vRot)
{
	NormalizeRotInPi(vRot.X);
	NormalizeRotInPi(vRot.Y);
	NormalizeRotInPi(vRot.Z);
}

//--------------------------------------------------------------------------------
//  NormalizeRotInPi(Vec3)
//	��]��-Pi����Pi�̊Ԃɂ���
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
//	��]��-Pi����Pi�̊Ԃɂ���
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInZeroToTwoPi(Vector3& vRot)
{
	NormalizeRotInZeroToTwoPi(vRot.X);
	NormalizeRotInZeroToTwoPi(vRot.Y);
	NormalizeRotInZeroToTwoPi(vRot.Z);
}

//--------------------------------------------------------------------------------
//  CalculateZDepth
//	Z�[�x���Z�o
//--------------------------------------------------------------------------------
float CKFMath::CalculateZDepth(const Vector3& Position, const Vector3& vCameraEye, const Vector3& vCameraAt)
{
	Vector3 vCameraForward = vCameraAt - vCameraEye;
	Vector3 PositionToCamera = Position - vCameraEye;
	float fDepth = Vec3Dot(vCameraForward, PositionToCamera);
	return fDepth;
}
*/
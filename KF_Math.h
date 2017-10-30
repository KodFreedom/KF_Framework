//--------------------------------------------------------------------------------
//	���Z�p�w�b�_�[
//�@KF_Math.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
#pragma once
#define USING_DIRECTX

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include <math.h>
#include <time.h>

#ifdef USING_DIRECTX
#include <d3dx9.h>
#endif

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define KF_PI (3.1415926358979f)	//�~����

namespace KF
{
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const float Pi = 3.1415926358979f; //�~����

	//--------------------------------------------------------------------------------
	//  �x�N�g��2�N���X
	//--------------------------------------------------------------------------------
	class Vector2
	{
	public:
		Vector2() : X(0.0f), Y(0.0f) {}
		Vector2(const float& value) : X(value), Y(value) {}
		Vector2(const float& x, const float& y) : X(x), Y(y) {}
		~Vector2() {}

		// �����o�[�ϐ�
		float X;
		float Y;

		// �L���X�g
#ifdef USING_DIRECTX
		operator D3DXVECTOR2() const;
#endif

		// �Z�p���Z�q
		Vector2& operator=(const Vector2& value);
		bool operator==(const Vector2& value) const;

		Vector2 operator+(const Vector2& value) const;
		void operator+=(const Vector2& value);

		Vector2 operator-(const Vector2& value) const;
		void operator-=(const Vector2& value);

		Vector2 operator*(const float& value) const;
		void operator*=(const float& value);
		float operator*(const Vector2& value) const;

		// ���\�b�h
		float		Dot(const Vector2& value) const;
		float		Magnitude(void) const;
		float		SquareMagnitude(void) const;
		void		Normalize(void);
		Vector2&	Normalized(void) const;
		float		Radian(void) const;

		// �ÓI���\�b�h
		static float RadianBetween(const Vector2& valueL, const Vector2& valueR);
	};

	//--------------------------------------------------------------------------------
	//  �x�N�g��3�N���X
	//--------------------------------------------------------------------------------
	class Vector3
	{
	public:
		Vector3() : X(0.0f), Y(0.0f), Z(0.0f) {}
		Vector3(const float& value) : X(value), Y(value), Z(value) {}
		Vector3(const float& x, const float& y, const float& z) : X(x), Y(y), Z(z) {}
		~Vector3() {}

		// �����o�[�ϐ�
		float X;
		float Y;
		float Z;

		// �萔��`
		static const Vector3 Zero;
		static const Vector3 One;
		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Left;
		static const Vector3 Right;
		static const Vector3 Forward;
		static const Vector3 Back;

		// �L���X�g
		operator Vector2() const;
#ifdef USING_DIRECTX
		operator D3DXVECTOR3() const;
#endif

		// �Z�p���Z�q
		Vector3& operator=(const Vector3& value);
		bool operator==(const Vector3& value) const;
		bool operator!=(const Vector3& value) const;

		Vector3 operator+(const Vector3& value) const;
		void operator+=(const Vector3& value);

		Vector3 operator-(const Vector3& value) const;
		void operator-=(const Vector3& value);

		Vector3 operator*(const float& value) const;
		void operator*=(const float& value);

		Vector3 operator*(const Vector3& value) const;
		void operator*=(const Vector3& value);

		Vector3 operator/(const float& value) const;
		void operator/=(const float& value);

		// ���\�b�h
		float			Dot(const Vector3& value) const;
		float			Magnitude(void) const;
		float			SquareMagnitude(void) const;
		void			Normalize(void);
		Vector3&		Normalized(void) const;
		
		// �ÓI���\�b�h
		static float	Distance(const Vector3& valueL, const Vector3& valueR);
		static float	SquareDistance(const Vector3& valueL, const Vector3& valueR);
		static Vector3&	TransformCoord(const Vector3& value, const Matrix44& rotation);
		static Vector3&	TransformNormal(const Vector3& value, const Matrix44& rotation);
		static Vector3&	TransformInverse(const Vector3& value, const Matrix44& matrix);
		static Vector3&	Scale(const Vector3& value, const Vector3& scale);
		static float	RadianBetween(const Vector3& valueL, const Vector3& valueR);
		static Vector3&	EulerBetween(const Vector3& from, const Vector3& to);
		static Vector3&	ProjectOnPlane(const Vector3& value, const Vector3& planeNormal, const Vector3& currentNormal = Vector3::Up);
	};

	//--------------------------------------------------------------------------------
	//  Matrix4*4
	//--------------------------------------------------------------------------------
	class Matrix44
	{
	public:
		Matrix44();
		Matrix44(
			const float& element11, const float& element12, const float& element13, const float& element14,
			const float& element21, const float& element22, const float& element23, const float& element24,
			const float& element31, const float& element32, const float& element33, const float& element34,
			const float& element41, const float& element42, const float& element43, const float& element44);
		~Matrix44() {}

		// �萔��`
		static const Matrix44 Identity;

		// �����o�[�ϐ�
		union 
		{
			struct 
			{
				float Element11, Element12, Element13, Element14;
				float Element21, Element22, Element23, Element24;
				float Element31, Element32, Element33, Element34;
				float Element41, Element42, Element43, Element44;
			};
			float Elements[4][4];
		};

		// �L���X�g
#ifdef USING_DIRECTX
		operator D3DXMATRIX() const;
#endif

		// �Z�p���Z�q
		Matrix44& operator=(const Matrix44& value);
		Matrix44 operator*(const Matrix44& value) const;
		void operator*=(const Matrix44& value);

		// ���\�b�h
		Vector3&			ToEular(void) const;
		Matrix44&			Transpose(void) const;

		// �ÓI���\�b�h
		static Matrix44&	RotationByAxis(const Vector3& axis, const float& radian);
		static Matrix44&	RotationYawPitchRoll(const Vector3& euler);
		static Matrix44&	Translation(const Vector3& translation);

#ifdef USING_DIRECTX
		static Matrix44&	ToMatrix44(const D3DXMATRIX& value);
#endif
	};

	//--------------------------------------------------------------------------------
	//  �x�N�g��4�N���X
	//--------------------------------------------------------------------------------
	class Vector4
	{
	public:
		Vector4() : X(0.0f), Y(0.0f), Z(0.0f), W(0.0f) {}
		Vector4(const float& x, const float& y, const float& z, const float& w) : X(x), Y(y), Z(z), W(w) {}
		Vector4(const Vector3& value, const float& w) : X(value.X), Y(value.Y), Z(value.Z), W(w) {}
		~Vector4() {}

		// �����o�[�ϐ�
		float X;
		float Y;
		float Z;
		float W;

		//�Z�p���Z�q
		Vector4 operator*(const Matrix44& mtxValue) const;
		void operator*=(const Matrix44& mtxValue);
	};

	//--------------------------------------------------------------------------------
	//  �N�H�[�^�j�I��
	//--------------------------------------------------------------------------------
	class Quaternion
	{
	public:
		Quaternion() : X(0.0f), Y(0.0f), Z(0.0f), W(1.0f) {}
		Quaternion(const float& value) : X(value), Y(value), Z(value), W(1.0f) {}
		Quaternion(const float& x, const float& y, const float& z, const float& w) : X(x), Y(y), Z(z), W(w) {}
		~Quaternion() {}

		// �萔��`
		static const Quaternion Identity;

		// �����o�[�ϐ�
		float X;
		float Y;
		float Z;
		float W;

		//�L���X�g
#ifdef USING_DIRECTX
		operator D3DXQUATERNION () const;
#endif

		//�Z�p���Z�q
		Quaternion operator+(const Quaternion& value) const;
		void operator+=(const Quaternion& value);
		Quaternion operator-(const Quaternion& value) const;
		void operator-=(const Quaternion& value);
		Quaternion operator*(const float& value) const;
		void operator*=(const float& value);
		void operator/=(const float& value);
		Quaternion operator*(const Quaternion& value) const;
		void operator*=(const Quaternion& value);
	};

	//--------------------------------------------------------------------------------
	//  �F
	//--------------------------------------------------------------------------------
	class Color
	{
	public:
		Color() : R(0.0f), G(0.0f), B(0.0f), A(0.0f) {}
		Color(const float& value) : R(value), G(value), B(value), A(value) {}
		Color(const float& r, const float& g, const float& b, const float& a) : R(r), G(g), B(b), A(a) {}
		~Color() {}

		// �萔��`
		static const Color White;
		static const Color Black;
		static const Color Red;
		static const Color Blue;
		static const Color Green;

		// �����o�[�ϐ�
		float R;
		float G;
		float B;
		float A;

		//�L���X�g
#ifdef USING_DIRECTX
		operator D3DCOLORVALUE () const;
		operator unsigned long() const;
#endif

		//�Z�p���Z�q
		Color& operator=(const Color& value);
		bool operator==(const Color& value);

		Color operator+(const Color& value) const;
		void operator+=(const Color& value);

		Color operator-(const Color& value) const;
		void operator-=(const Color& value);

		Color operator*(const float& value) const;
		void operator*=(const float& value);
	};

	//--------------------------------------------------------------------------------
	//  ���C
	//--------------------------------------------------------------------------------
	class Ray
	{
	public:
		Ray()
			: Origin(Vector3(0.0f))
			, Direction(Vector3(0.0f))
		{}
		Ray(const Vector3& vOrigin, const Vector3& vDirection)
			: Origin(vOrigin)
			, Direction(vDirection)
		{}
		~Ray() {}

		Vector3 Origin;
		Vector3 Direction;
	};

	//--------------------------------------------------------------------------------
	//
	//  �v�Z����`
	//
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	//	�����_��
	//--------------------------------------------------------------------------------
	namespace Random
	{
		void	Init(void);
		int		Range(const int& min, const int& max);
		float	Range(const float& min, const float& max);
		Vector3	Range(const Vector3& min, const Vector3& max);
	};

	//--------------------------------------------------------------------------------
	//	�v�Z��
	//--------------------------------------------------------------------------------
	class CKFMath
	{
	public:
		//--------------------------------------------------------------------------------
		//  �֐���`
		//--------------------------------------------------------------------------------
		//Quaternion�v�Z
		static void				QuaternionIdentity(Quaternion& qValue);
		static float			QuaternionMagnitudeSquare(const Quaternion& qValue);
		static float			QuaternionMagnitude(const Quaternion& qValue);
		static void				QuaternionNormalize(Quaternion& qValue);
		static float			QuaternionDot(const Quaternion& qL, const Quaternion& qR);
		static Vector3			QuaternionToEuler(const Quaternion& quaternion);
		static Matrix44			QuaternionToMtx(const Quaternion& qValue);
		static Quaternion		QuaternionMultiplyXYZW(const Quaternion& qL, const Quaternion& qR);
		static Quaternion		EulerToQuaternion(const Vector3& vVec);
		static Quaternion		MtxToQuaternion(const Matrix44& mtxRot);

		//Ray�v�Z
		static Ray			CalculatePickingRay(const Vector2& vScreenPos, const float& fViewportWidth, const float& fViewportHeight, const float& fProjMtx00, const float& fProjMtx11, const Matrix44& mtxViewInverse);
		static Ray			ChangePosToRay(const Vector2& vScreenPos, const float& fViewportWidth, const float& fViewportHeight, const float& fProjMtx00, const float& fProjMtx11);
		static void				TransformRay(Ray& ray, const Matrix44& mtxTrans);

		//Lerp�֐�
		static Vector3			LerpVec3(const Vector3& vVecFrom, const Vector3& vVecTo, const float& fTime);
		static Vector3			LerpNormal(const Vector3& vNormalFrom, const Vector3& vNormalTo, const float& fTime);
		static float			LerpFloat(const float& fFrom, const float& fTo, const float& fTime);
		static Color			LerpColor(const Color& cFrom, const Color& cTo, const float& fTime);
		static Quaternion	SlerpQuaternion(const Quaternion& qFrom, const Quaternion& qTo, const float& fTime);

		//Clamp�֐�
		static void				ClampFloat(float& fValue, const float& fMin, const float& fMax);

		//�ق��̌v�Z
		static void				NormalizeColor(Color& cColor);
		static void				NormalizeRotInTwoPi(float& fRot);
		static void				NormalizeRotInTwoPi(Vector3& vRot);
		static void				NormalizeRotInPi(float& fRot);
		static void				NormalizeRotInPi(Vector3& vRot);
		static void				NormalizeRotInZeroToTwoPi(float& fRot);
		static void				NormalizeRotInZeroToTwoPi(Vector3& vRot);
		static float			CalculateZDepth(const Vector3& vPos, const Vector3& vCameraEye, const Vector3& vCameraAt);

	private:
		CKFMath() {}
		~CKFMath() {}
	};
}
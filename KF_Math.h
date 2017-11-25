//--------------------------------------------------------------------------------
//　kf_math.h
//  classes and methods for math
//	演算用のクラスとメソッド
//  用于计算的类和函数
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"

namespace KodFreedom
{
	//--------------------------------------------------------------------------------
	//  forward declaration / 前方宣言 / 前置声明
	//--------------------------------------------------------------------------------
	class Quaternion;
	class Matrix44;
	class Ray;

	//--------------------------------------------------------------------------------
	//  constant variables / 定数 / 常量
	//--------------------------------------------------------------------------------
	constexpr float kPi = 3.1415926358979f; // Pi / 円周率 / 圆周率

	//--------------------------------------------------------------------------------
	//  vector2 / ベクトル2 / 二次元向量
	//--------------------------------------------------------------------------------
	class Vector2
	{
	public:
		Vector2() : x_(0.0f), y_(0.0f) {}
		Vector2(const float& value) : x_(value), y_(value) {}
		Vector2(const float& x, const float& y) : x_(x), y_(y) {}
		~Vector2() {}

		// x component of the vector2 / ベクトル2のx要素 / 二次元向量的x要素
		float x_;

		// y component of the vector2 / ベクトル2のy要素 / 二次元向量的y要素
		float y_;

		// vector2(0, 0) / x,yが全部0のベクトル2 / 要素为0的二次元向量
		static const Vector2 kZero;

		// vector2(1, 1) / x,yが全部1のベクトル2 / 要素为1的二次元向量
		static const Vector2 kOne;

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		//--------------------------------------------------------------------------------
		//  operator D3DXVECTOR2()
		//  cast to D3DXVECTOR2 / D3DXVECTOR2にキャストする / 转换成D3DXVECTOR2
		//  return：D3DXVECTOR2
		//--------------------------------------------------------------------------------
		operator D3DXVECTOR2() const { return D3DXVECTOR2(x_, y_); }
#endif

		//--------------------------------------------------------------------------------
		//  operator=
		//  override operator= / 代入処理 / 赋值
		//  value : 値 / 值
		//  return：Vector2
		//--------------------------------------------------------------------------------
		Vector2& operator=(const Vector2& value)
		{
			x_ = value.x_;
			y_ = value.y_;
			return *this;
		}

		//--------------------------------------------------------------------------------
		//  operator==
		//  check if this equal value / 同値判定処理 / 判断两边变量是否相等
		//  value : 値 / 值
		//  return：bool
		//--------------------------------------------------------------------------------
		bool operator==(const Vector2& value) const
		{
			return (x_ == value.x_ && y_ == value.y_);
		}

		//--------------------------------------------------------------------------------
		//  operator+
		//  add / 足し算 / 加算
		//  value : 値 / 值
		//  return：Vector2
		//--------------------------------------------------------------------------------
		Vector2 operator+(const Vector2& value) const
		{
			return Vector2(x_ + value.x_, y_ + value.y_);
		}
		
		//--------------------------------------------------------------------------------
		//  operator+=
		//  add to this / 自分に足す / 加算
		//  value : 値 / 值
		//--------------------------------------------------------------------------------
		void operator+=(const Vector2& value)
		{
			x_ += value.x_;
			y_ += value.y_;
		}

		//--------------------------------------------------------------------------------
		//  operator-
		//  minus / 引き算 / 减算
		//  value : 値 / 值
		//  return：Vector2
		//--------------------------------------------------------------------------------
		Vector2 operator-(const Vector2& value) const
		{
			return Vector2(x_ - value.x_, y_ - value.y_);
		}
		
		//--------------------------------------------------------------------------------
		//  operator-=
		//  minus to this / 自分に引く / 减算
		//  value : 値 / 值
		//--------------------------------------------------------------------------------
		void operator-=(const Vector2& value)
		{
			x_ -= value.x_;
			y_ -= value.y_;
		}

		//--------------------------------------------------------------------------------
		//  operator*
		//  scale x and y / スケール / 缩放
		//  value : scale value / スケール値 / 缩放值
		//  return：Vector2
		//--------------------------------------------------------------------------------
		Vector2 operator*(const float& value) const
		{
			return Vector2(x_ * value, y_ * value);
		}

		//--------------------------------------------------------------------------------
		//  operator*=
		//  scale x and y / スケール / 缩放
		//  value : scale value / スケール値 / 缩放值
		//--------------------------------------------------------------------------------
		void operator*=(const float& value)
		{
			x_ *= value;
			y_ *= value;
		}

		//--------------------------------------------------------------------------------
		//  operator*
		//  cross / 外積 / 外积
		//  value : 値 / 值
		//  return：float
		//--------------------------------------------------------------------------------
		float operator*(const Vector2& value) const
		{
			return (x_ * value.y_ - y_ * value.x_);
		}

		//--------------------------------------------------------------------------------
		//  Dot
		//  dot / 内積 / 内积
		//  value : 値 / 值
		//  return：float
		//--------------------------------------------------------------------------------
		float Dot(const Vector2& value) const
		{
			return (x_ * value.x_ + y_ * value.y_);
		}

		//--------------------------------------------------------------------------------
		//  Magnitude
		//  compute the magnitude / 長さの計算 / 长度计算
		//  return：float
		//--------------------------------------------------------------------------------
		float Magnitude(void) const
		{
			return sqrtf(x_ * x_ + y_ * y_);
		}

		//--------------------------------------------------------------------------------
		//  SquareMagnitude
		//  compute the square magnitude / 長さ平方の計算 / 平方长度计算
		//  return：float
		//--------------------------------------------------------------------------------
		float SquareMagnitude(void) const
		{
			return (x_ * x_ + y_ * y_);
		}
		
		//--------------------------------------------------------------------------------
		//  Normalized
		//  return the normalized vector / 正規化したの値を返す / 返回正规化后的值
		//  return：Vector2
		//--------------------------------------------------------------------------------
		Vector2 Normalized(void) const
		{
			float magnitude = Magnitude();
			if (magnitude <= 0.0f) return Vector2::kZero;
			return Vector2(x_ / magnitude, y_ / magnitude);
		}

		//--------------------------------------------------------------------------------
		//  Normalize
		//  normalize the vector / 正規化 / 正规化
		//--------------------------------------------------------------------------------
		void Normalize(void)
		{
			*this = this->Normalized();
		}

		//--------------------------------------------------------------------------------
		//  ToRadian
		//  return the radian between x, y / xy間の角度の算出 / 返回xy的夹角
		//  return：float
		//--------------------------------------------------------------------------------
		float ToRadian(void) const
		{
			return atan2f(y_, x_);
		}

		//--------------------------------------------------------------------------------
		//  RadianBetween
		//  return the radian between two vector2 / Vector2間の角度を返す / 返回向量夹角
		//  from：from vector / 開始ベクトル / 开始向量
		//  to：to vector / 終了ベクトル / 结束向量
		//  return：float
		//--------------------------------------------------------------------------------
		static float RadianBetween(const Vector2& from, const Vector2& to)
		{
			if (from == to) return 0.0f;
			float square_magnitude_from = from.SquareMagnitude();
			float square_magnitude_to = to.SquareMagnitude();
			if (square_magnitude_from * square_magnitude_to <= 0.0f) return 0.0f;
			float dot = from.Dot(to);
			float cross = from * to;
			float sign = cross >= 0.0f ? 1.0f : -1.0f;
			return acosf(dot / (sqrtf(square_magnitude_from) * sqrtf(square_magnitude_to)) * sign);
		}
	};

	//--------------------------------------------------------------------------------
	//  ベクトル3クラス
	//--------------------------------------------------------------------------------
	class Vector3
	{
	public:
		Vector3() : X(0.0f), Y(0.0f), Z(0.0f) {}
		Vector3(const float& value) : X(value), Y(value), Z(value) {}
		Vector3(const float& x, const float& y, const float& z) : X(x), Y(y), Z(z) {}
		~Vector3() {}

		// メンバー変数
		float X;
		float Y;
		float Z;

		// 定数定義
		static const Vector3 Zero;
		static const Vector3 One;
		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Left;
		static const Vector3 Right;
		static const Vector3 Forward;
		static const Vector3 Back;
		static const Vector3 AxisX;
		static const Vector3 AxisY;
		static const Vector3 AxisZ;

		// キャスト
		operator Vector2() const;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		operator D3DXVECTOR3() const;
#endif

		// 算術演算子
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

		// メソッド
		float			Dot(const Vector3& value) const;
		float			Magnitude(void) const;
		float			SquareMagnitude(void) const;
		void			Normalize(void);
		Vector3			Normalized(void) const;
		Quaternion		ToQuaternion(void) const;
		
		// 静的メソッド
		static float	DistanceBetween(const Vector3& pointA, const Vector3& pointB);
		static float	SquareDistanceBetween(const Vector3& pointA, const Vector3& pointB);
		static Vector3	TransformCoord(const Vector3& point, const Matrix44& transform);
		static Vector3	TransformNormal(const Vector3& normal, const Matrix44& transform);
		static Vector3	TransformInverse(const Vector3& point, const Matrix44& transform);
		static Vector3	Scale(const Vector3& value, const Vector3& scale);
		static Vector3	EulerBetween(const Vector3& directionA, const Vector3& directionB);
		static Vector3	ProjectOnPlane(const Vector3& direction, const Vector3& planeNormal, const Vector3& currentNormal = Vector3::Up);
	};

	//--------------------------------------------------------------------------------
	//  ベクトル4クラス
	//--------------------------------------------------------------------------------
	class Vector4
	{
	public:
		Vector4() : X(0.0f), Y(0.0f), Z(0.0f), W(0.0f) {}
		Vector4(const float& x, const float& y, const float& z, const float& w) : X(x), Y(y), Z(z), W(w) {}
		Vector4(const Vector3& value, const float& w) : X(value.X), Y(value.Y), Z(value.Z), W(w) {}
		~Vector4() {}

		// メンバー変数
		float X;
		float Y;
		float Z;
		float W;

		// 算術演算子
		Vector4 operator*(const Matrix44& matrix) const;
		void operator*=(const Matrix44& matrix);
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

		// メンバー変数
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

		// 定数定義
		static const Matrix44 Identity;

		// キャスト
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		operator D3DXMATRIX() const;
#endif

		// 算術演算子
		Matrix44& operator=(const Matrix44& value);
		Matrix44 operator*(const float& value) const;
		Matrix44 operator+(const Matrix44& value) const;
		Matrix44 operator*(const Matrix44& value) const;
		void operator*=(const Matrix44& value);

		// メソッド
		Vector3			ToEular(void) const;
		Quaternion		ToQuaternion(void) const;
		Matrix44		Transpose(void) const;

		// 静的メソッド
		static Matrix44 Scale(const Vector3& scale);
		static Matrix44 Rotation(const Vector3& right, const Vector3& up, const Vector3& forward);
		static Matrix44	RotationAxis(const Vector3& axis, const float& radian);
		static Matrix44	RotationYawPitchRoll(const Vector3& euler);
		static Matrix44	Translation(const Vector3& translation);
		static Matrix44	Transform(const Vector3& right, const Vector3& up, const Vector3& forward, const Vector3& translation, const Vector3& scale = Vector3::One);
		static Matrix44 Transform(const Vector3& rotation, const Vector3& translation, const Vector3& scale = Vector3::One);
		static Matrix44 ProjectionLeftHand(const float fovAngleY, const float aspectRatio, const float nearZ, const float farZ);
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		static Matrix44	ToMatrix44(const D3DXMATRIX& value);
#endif
	};

	//--------------------------------------------------------------------------------
	//  クォータニオン
	//--------------------------------------------------------------------------------
	class Quaternion
	{
	public:
		Quaternion() : X(0.0f), Y(0.0f), Z(0.0f), W(1.0f) {}
		Quaternion(const float& value) : X(value), Y(value), Z(value), W(1.0f) {}
		Quaternion(const float& x, const float& y, const float& z, const float& w) : X(x), Y(y), Z(z), W(w) {}
		~Quaternion() {}

		// メンバー変数
		float X;
		float Y;
		float Z;
		float W;

		// 定数定義
		static const Quaternion Identity;

		// キャスト
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		operator D3DXQUATERNION () const;
#endif

		// 算術演算子
		Quaternion operator+(const Quaternion& value) const;
		void operator+=(const Quaternion& value);
		Quaternion operator-(const Quaternion& value) const;
		void operator-=(const Quaternion& value);
		Quaternion operator*(const float& value) const;
		void operator*=(const float& value);
		Quaternion operator/(const float& value) const;
		void operator/=(const float& value);
		Quaternion operator*(const Quaternion& value) const;
		void operator*=(const Quaternion& value);

		// メソッド
		float		Dot(const Quaternion& value) const;
		float		SquareMagnitude(void) const;
		float		Magnitude(void) const;
		void		Normalize(void);
		Quaternion	Normalized(void) const;
		Quaternion	MultiplySeparately(const Quaternion& value) const;
		Vector3		ToEuler(void) const;
		Matrix44	ToMatrix(void) const;
	};

	//--------------------------------------------------------------------------------
	//  色
	//--------------------------------------------------------------------------------
	class Color
	{
	public:
		Color() : R(0.0f), G(0.0f), B(0.0f), A(0.0f) {}
		Color(const float& value) : R(value), G(value), B(value), A(value) {}
		Color(const float& r, const float& g, const float& b, const float& a) : R(r), G(g), B(b), A(a) {}
		~Color() {}

		// 定数定義
		static const Color White;
		static const Color Black;
		static const Color Gray;
		static const Color Red;
		static const Color Blue;
		static const Color Green;

		// メンバー変数
		float R;
		float G;
		float B;
		float A;

		//キャスト
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		operator D3DCOLORVALUE () const;
		operator unsigned long() const;
#endif

		//算術演算子
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
	//  レイ
	//--------------------------------------------------------------------------------
	class Ray
	{
	public:
		Ray()
			: Origin(Vector3::Zero)
			, Direction(Vector3::Zero)
		{}
		Ray(const Vector3& vOrigin, const Vector3& vDirection)
			: Origin(vOrigin)
			, Direction(vDirection)
		{}
		~Ray() {}

		// メンバー変数
		Vector3 Origin;
		Vector3 Direction;

		// メソッド
		void	Transform(const Matrix44& transform);
	};

	//--------------------------------------------------------------------------------
	//	ランダム
	//--------------------------------------------------------------------------------
	namespace Random
	{
		void		Init(void);
		int			Range(const int& min, const int& max);
		float		Range(const float& min, const float& max);
		Vector3		Range(const Vector3& min, const Vector3& max);
	};

	//--------------------------------------------------------------------------------
	//	計算式
	//--------------------------------------------------------------------------------
	namespace Math
	{
		// Lerp
		Vector3		Lerp(const Vector3& from, const Vector3& to, const float& time);
		float		Lerp(const float& from, const float& to, const float& time);
		Color		Lerp(const Color& from, const Color& to, const float& time);

		// Slerp
		Vector3		Slerp(const Vector3& from, const Vector3& to, const float& time);
		Quaternion	Slerp(const Quaternion& from, const Quaternion& to, const float& time);

		// Clamp
		float		Clamp(const float& value, const float& min, const float& max);

		// Max
		float		AbsMax(const float& valueA, const float& valueB);
	}
}
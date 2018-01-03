﻿//--------------------------------------------------------------------------------
//　kf_math.h
//  classes and methods for math
//  演算用のクラスとメソッド
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include <time.h>
#include <math.h>
#include "common_setting.h"
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
#include <d3dx9.h>
#endif

namespace kodfreedom
{
    //--------------------------------------------------------------------------------
    //  forward declaration / 前方宣言
    //--------------------------------------------------------------------------------
    class Quaternion;
    class Matrix44;
    class Ray;
    class Vector4;

    //--------------------------------------------------------------------------------
    //  constant variables / 定数
    //--------------------------------------------------------------------------------
    static constexpr float kPi = 3.1415926f; // Pi / 円周率
    static constexpr float kDotMin = 0.05f;

    //--------------------------------------------------------------------------------
    //  Short2
    //--------------------------------------------------------------------------------
    class Short2
    {
    public:
        Short2() : x_(0), y_(0) {}
        ~Short2() {}

        union
        {
            struct
            {
                short x_;
                short y_;
            };
            short m_[2];
        };
    };

    //--------------------------------------------------------------------------------
    //  ベクトル2
    //--------------------------------------------------------------------------------
    class Vector2
    {
    public:
        Vector2() : x_(0.0f), y_(0.0f) {}
        ~Vector2() {}

        //--------------------------------------------------------------------------------
        //  set x and y with given value
        //  xとyにvalueを入れる
        //--------------------------------------------------------------------------------
        Vector2(const float& value) : x_(value), y_(value) {}

        //--------------------------------------------------------------------------------
        //  set x and y with given x and y
        //  xとyに与えられた値を入れるを入れる
        //--------------------------------------------------------------------------------
        Vector2(const float& x, const float& y) : x_(x), y_(y) {}
        
        float x_; // x component of the vector2 / ベクトル2のx要素
        float y_; // y component of the vector2 / ベクトル2のy要素

        static const Vector2 kZero; // vector2(0, 0)
        static const Vector2 kOne; // vector2(1, 1)

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
        //--------------------------------------------------------------------------------
        //  cast to D3DXVECTOR2 / D3DXVECTOR2にキャストする
        //  return：D3DXVECTOR2
        //--------------------------------------------------------------------------------
        operator D3DXVECTOR2() const { return D3DXVECTOR2(x_, y_); }
#endif

        //--------------------------------------------------------------------------------
        //  代入処理
        //  value : 値
        //  return：Vector2
        //--------------------------------------------------------------------------------
        Vector2& operator=(const Vector2& value)
        {
            x_ = value.x_;
            y_ = value.y_;
            return *this;
        }

        //--------------------------------------------------------------------------------
        //  check if this equal value / 同値判定処理
        //  value : 値
        //  return：bool
        //--------------------------------------------------------------------------------
        bool operator==(const Vector2& value) const
        {
            return (x_ == value.x_ && y_ == value.y_);
        }

        //--------------------------------------------------------------------------------
        //  add / 足し算
        //  value : 値
        //  return：Vector2
        //--------------------------------------------------------------------------------
        Vector2 operator+(const Vector2& value) const
        {
            return Vector2(x_ + value.x_, y_ + value.y_);
        }
        
        //--------------------------------------------------------------------------------
        //  add to this / 自分に足す
        //  value : 値
        //--------------------------------------------------------------------------------
        void operator+=(const Vector2& value)
        {
            x_ += value.x_;
            y_ += value.y_;
        }

        //--------------------------------------------------------------------------------
        //  minus / 引き算
        //  value : 値
        //  return：Vector2
        //--------------------------------------------------------------------------------
        Vector2 operator-(const Vector2& value) const
        {
            return Vector2(x_ - value.x_, y_ - value.y_);
        }
        
        //--------------------------------------------------------------------------------
        //  minus to this / 自分に引く
        //  value : 値
        //--------------------------------------------------------------------------------
        void operator-=(const Vector2& value)
        {
            x_ -= value.x_;
            y_ -= value.y_;
        }

        //--------------------------------------------------------------------------------
        //  scale x and y / スケール
        //  value : scale value / スケール値
        //  return：Vector2
        //--------------------------------------------------------------------------------
        Vector2 operator*(const float& value) const
        {
            return Vector2(x_ * value, y_ * value);
        }

        //--------------------------------------------------------------------------------
        //  scale x and y / スケール
        //  value : scale value / スケール値
        //--------------------------------------------------------------------------------
        void operator*=(const float& value)
        {
            x_ *= value;
            y_ *= value;
        }

        //--------------------------------------------------------------------------------
        //  cross / 外積
        //  value : 値
        //  return：float
        //--------------------------------------------------------------------------------
        float operator*(const Vector2& value) const
        {
            return (x_ * value.y_ - y_ * value.x_);
        }

        //--------------------------------------------------------------------------------
        //  dot / 内積
        //  value : 値
        //  return：float
        //--------------------------------------------------------------------------------
        float Dot(const Vector2& value) const
        {
            return (x_ * value.x_ + y_ * value.y_);
        }

        //--------------------------------------------------------------------------------
        //  compute the magnitude / 長さの計算
        //  return：float
        //--------------------------------------------------------------------------------
        float Magnitude(void) const
        {
            return sqrtf(x_ * x_ + y_ * y_);
        }

        //--------------------------------------------------------------------------------
        //  compute the square magnitude / 長さ平方の計算
        //  return：float
        //--------------------------------------------------------------------------------
        float SquareMagnitude(void) const
        {
            return (x_ * x_ + y_ * y_);
        }
        
        //--------------------------------------------------------------------------------
        //  return the normalized vector / 正規化したの値を返す
        //  return：Vector2
        //--------------------------------------------------------------------------------
        Vector2 Normalized(void) const
        {
            float magnitude = Magnitude();
            if (magnitude <= 0.0f) return Vector2::kZero;
            return Vector2(x_ / magnitude, y_ / magnitude);
        }

        //--------------------------------------------------------------------------------
        //  normalize the vector / 正規化
        //--------------------------------------------------------------------------------
        void Normalize(void)
        {
            *this = this->Normalized();
        }

        //--------------------------------------------------------------------------------
        //  return the radian between x, y / xy間の角度の算出
        //  return：float
        //--------------------------------------------------------------------------------
        float ToRadian(void) const
        {
            return atan2f(y_, x_);
        }

        //--------------------------------------------------------------------------------
        //  return the radian between two vector2 / Vector2間の角度を返す
        //  from：開始ベクトル
        //  to：終了ベクトル
        //  return：float
        //--------------------------------------------------------------------------------
        static float RadianBetween(const Vector2& from, const Vector2& to);
    };

    //--------------------------------------------------------------------------------
    //  ベクトル3
    //--------------------------------------------------------------------------------
    class Vector3
    {
    public:
        Vector3() : x_(0.0f), y_(0.0f), z_(0.0f) {}
        ~Vector3() {}

        //--------------------------------------------------------------------------------
        //  set xyz with given value
        //  xyzにvalueを入れる
        //--------------------------------------------------------------------------------
        Vector3(const float& value) : x_(value), y_(value), z_(value) {}
        
        //--------------------------------------------------------------------------------
        //  set xyz with given xyz
        //  xyzに与えられた値を入れるを入れる
        //--------------------------------------------------------------------------------
        Vector3(const float& x, const float& y, const float& z) : x_(x), y_(y), z_(z) {}

        //--------------------------------------------------------------------------------
        //  set xyz with given Vector4's xyz
        //  xyzに与えられたVector4のxyz値を入れるを入れる
        //--------------------------------------------------------------------------------
        Vector3(const Vector4& value);
        
        union
        {
            float m_[3];
            struct
            {
                float x_, y_, z_;
            };
        };

        static const Vector3 kZero; // vector3(0, 0, 0)
        static const Vector3 kOne; // vector3(1, 1, 1)
        static const Vector3 kUp; // vector3(0, 1, 0)
        static const Vector3 kDown; // vector3(0, -1, 0)
        static const Vector3 kLeft; // vector3(-1, 0, 0)
        static const Vector3 kRight; // vector3(1, 0, 0)
        static const Vector3 kForward; // vector3(0, 0, 1)
        static const Vector3 kBack; // vector3(0, 0, -1)
        static const Vector3 kAxisX; // vector3(1, 0, 0)
        static const Vector3 kAxisY; // vector3(0, 1, 0)
        static const Vector3 kAxisZ; // vector3(0, 0, 1)

        //--------------------------------------------------------------------------------
        //  cast to Vector2, return the x,y component / Vector2にキャストする
        //  return：Vector2
        //--------------------------------------------------------------------------------
        operator Vector2() const
        {
            return Vector2(x_, y_);
        }

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
        //--------------------------------------------------------------------------------
        //  cast to D3DXVECTOR3 / D3DXVECTOR3にキャストする
        //  return：D3DXVECTOR3
        //--------------------------------------------------------------------------------
        operator D3DXVECTOR3() const
        {
            return D3DXVECTOR3(x_, y_, z_);
        }
#endif

        //--------------------------------------------------------------------------------
        //  代入処理
        //  value : 値
        //  return：Vector3&
        //--------------------------------------------------------------------------------
        Vector3& operator=(const Vector3& value)
        {
            x_ = value.x_;
            y_ = value.y_;
            z_ = value.z_;
            return *this;
        }

        //--------------------------------------------------------------------------------
        //  check if this equal value / 同値判定処理
        //  value : 値
        //  return：bool
        //--------------------------------------------------------------------------------
        bool operator==(const Vector3& value) const
        {
            return (x_ == value.x_ && y_ == value.y_ && z_ == value.z_);
        }

        //--------------------------------------------------------------------------------
        //  check if this not equal value / 異なる値判定処理
        //  value : 値
        //  return：bool
        //--------------------------------------------------------------------------------
        bool operator!=(const Vector3& value) const
        {
            return (x_ != value.x_ && y_ != value.y_ && z_ != value.z_);
        }

        //--------------------------------------------------------------------------------
        //  add / 足し算
        //  value : 値
        //  return：Vector3
        //--------------------------------------------------------------------------------
        Vector3 operator+(const Vector3& value) const
        {
            return Vector3(x_ + value.x_, y_ + value.y_, z_ + value.z_);
        }

        //--------------------------------------------------------------------------------
        //  add to this / 自分に足す
        //  value : 値
        //--------------------------------------------------------------------------------
        void operator+=(const Vector3& value)
        {
            x_ += value.x_;
            y_ += value.y_;
            z_ += value.z_;
        }

        //--------------------------------------------------------------------------------
        //  minus / 引き算
        //  value : 値
        //  return：Vector3
        //--------------------------------------------------------------------------------
        Vector3 operator-(const Vector3& value) const
        {
            return Vector3(x_ - value.x_, y_ - value.y_, z_ - value.z_);
        }

        //--------------------------------------------------------------------------------
        //  minus to this / 自分に引く
        //  value : 値
        //--------------------------------------------------------------------------------
        void operator-=(const Vector3& value)
        {
            x_ -= value.x_;
            y_ -= value.y_;
            z_ -= value.z_;
        }

        //--------------------------------------------------------------------------------
        //  scale xyz / スケール
        //  value : scale value / スケール値
        //--------------------------------------------------------------------------------
        Vector3 operator*(const float& value) const
        {
            return Vector3(x_ * value, y_ * value, z_ * value);
        }

        //--------------------------------------------------------------------------------
        //  scale xyz / スケール
        //  value : scale value / スケール値
        //--------------------------------------------------------------------------------
        void operator*=(const float& value)
        {
            x_ *= value;
            y_ *= value;
            z_ *= value;
        }

        //--------------------------------------------------------------------------------
        //  cross / 外積
        //  value : 値
        //  return：Vector3
        //--------------------------------------------------------------------------------
        Vector3 operator*(const Vector3& value) const
        {
            return Vector3(y_ * value.z_ - value.y_ * z_, z_ * value.x_ - value.z_ * x_, x_ * value.y_ - value.x_ * y_);
        }

        //--------------------------------------------------------------------------------
        //  cross / 外積
        //  value : 値
        //  return：float
        //--------------------------------------------------------------------------------
        void operator*=(const Vector3& value)
        {
            *this = *this * value;
        }

        //--------------------------------------------------------------------------------
        //  scale xyz / スケール
        //  value : scale value / スケール値
        //--------------------------------------------------------------------------------
        Vector3 operator/(const float& value) const
        {
            assert(value != 0.0f);
            return Vector3(x_ / value, y_ / value, z_ / value);
        }

        //--------------------------------------------------------------------------------
        //  scale xyz / スケール
        //  value : scale value / スケール値
        //--------------------------------------------------------------------------------
        void operator/=(const float& value)
        {
            assert(value != 0.0f);
            x_ /= value;
            y_ /= value;
            z_ /= value;
        }

        //--------------------------------------------------------------------------------
        //  dot / 内積
        //  value : 値
        //  return：float
        //--------------------------------------------------------------------------------
        float Dot(const Vector3& value) const
        {
            return (x_ * value.x_ + y_ * value.y_ + z_ * value.z_);
        }

        //--------------------------------------------------------------------------------
        //  compute the magnitude / 長さの計算
        //  return：float
        //--------------------------------------------------------------------------------
        float Magnitude(void) const
        {
            return sqrtf(x_ * x_ + y_ * y_ + z_ * z_);
        }

        //--------------------------------------------------------------------------------
        //  compute the square magnitude / 長さ平方の計算
        //  return：float
        //--------------------------------------------------------------------------------
        float SquareMagnitude(void) const
        {
            return (x_ * x_ + y_ * y_ + z_ * z_);
        }

        //--------------------------------------------------------------------------------
        //  return the normalized vector / 正規化したの値を返す
        //  return：Vector3
        //--------------------------------------------------------------------------------
        Vector3 Normalized(void) const
        {
            float magnitude = this->Magnitude();
            if (magnitude <= 0.0f) return Vector3::kZero;
            return *this / magnitude;
        }

        //--------------------------------------------------------------------------------
        //  normalize the vector / 正規化
        //--------------------------------------------------------------------------------
        void Normalize(void)
        {
            *this = this->Normalized();
        }
        
        //--------------------------------------------------------------------------------
        //  change the euler to quaternion / Euler角をQuaternionに変換
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion ToQuaternion(void) const;
        
        //--------------------------------------------------------------------------------
        //  両点の間の距離の算出
        //  from：始点
        //  to : 終点
        //  return：float
        //--------------------------------------------------------------------------------
        static float DistanceBetween(const Vector3& from, const Vector3& to)
        {
            return (from - to).Magnitude();
        }
        
        //--------------------------------------------------------------------------------
        //  両点の間の距離の平方を返す
        //  from：始点
        //  to : 終点
        //  return：float
        //--------------------------------------------------------------------------------
        static float SquareDistanceBetween(const Vector3& from, const Vector3& to)
        {
            return (from - to).SquareMagnitude();
        }
        
        //--------------------------------------------------------------------------------
        //  ポイントを回転する
        //  point：回転相手
        //  transform：変換行列
        //  return：Vector3
        //--------------------------------------------------------------------------------
        static Vector3 TransformCoord(const Vector3& point, const Matrix44& transform);

        //--------------------------------------------------------------------------------
        //  法線ベクトルを回転する
        //  normal：回転相手
        //  transform：変換行列
        //  return：Vector3
        //--------------------------------------------------------------------------------
        static Vector3 TransformNormal(const Vector3& normal, const Matrix44& transform);
        
        //--------------------------------------------------------------------------------
        //  ポイントを逆行列により変換する
        //  point：回転相手
        //  transform：変換行列
        //  return：Vector3
        //--------------------------------------------------------------------------------
        static Vector3 TransformInverse(const Vector3& point, const Matrix44& transform);

        //--------------------------------------------------------------------------------
        //  scale x,y,z by scale's x,y,z / XYZ別々でスケールする
        //  value：スケールしたい値
        //  scale：スケール値
        //  return：Vector3
        //--------------------------------------------------------------------------------
        static Vector3 Scale(const Vector3& value, const Vector3& scale)
        {
            return Vector3(value.x_ * scale.x_, value.y_ * scale.y_, value.z_ * scale.z_);
        }

        //--------------------------------------------------------------------------------
        //  ベクトル間のradian角の算出
        //  from : 開始ベクトル
        //  to：終点ベクトル
        //  return：float
        //--------------------------------------------------------------------------------
        static float RadianBetween(const Vector3& from, const Vector3& to);
        
        //--------------------------------------------------------------------------------
        //  ベクトル間のeuler角の算出
        //  from : 開始ベクトル
        //  to：終点ベクトル
        //  return：Vector3
        //--------------------------------------------------------------------------------
        static Vector3 EulerBetween(const Vector3& from, const Vector3& to)
        {
            Vector3 euler;
            euler.x_ = Vector2::RadianBetween(Vector2(from.y_, from.z_), Vector2(to.y_, to.z_));
            euler.y_ = Vector2::RadianBetween(Vector2(from.z_, from.x_), Vector2(to.z_, to.x_));
            euler.z_ = Vector2::RadianBetween(Vector2(from.x_, from.y_), Vector2(to.x_, to.y_));
            return euler;
        }
        
        //--------------------------------------------------------------------------------
        //  project the direction on plane：方向を平面と平行の方向に回転する
        //  direction：回転したい方向
        //  plane_normal：平面の上方向
        //  current_normal：今の上方向
        //  return：Vector3
        //--------------------------------------------------------------------------------
        static Vector3 ProjectOnPlane(const Vector3& direction, const Vector3& plane_normal, const Vector3& current_normal = Vector3::kUp)
        {
            return (current_normal * direction) * plane_normal;
        }
    
        //--------------------------------------------------------------------------------
        //  ベクトルを回転する
        //  direction：回転相手
        //  rotation：回転quaternion
        //  return：Vector3
        //--------------------------------------------------------------------------------
        static Vector3 Rotate(const Vector3& direction, const Quaternion& rotation);

        //--------------------------------------------------------------------------------
        //  回転軸と回転角度でオラー角に変換する
        //  axis：回転軸
        //  radian：回転角度で
        //  return：Vector3
        //--------------------------------------------------------------------------------
        static Vector3 AxisRadianToEuler(const Vector3& axis, const float& radian);
    };

    //--------------------------------------------------------------------------------
    //  ベクトル4クラス
    //--------------------------------------------------------------------------------
    class Vector4
    {
    public:
        Vector4() : x_(0.0f), y_(0.0f), z_(0.0f), w_(0.0f) {}
        ~Vector4() {}

        //--------------------------------------------------------------------------------
        //  set xyzw with given value
        //  xyzwにvalueを入れる
        //--------------------------------------------------------------------------------
        Vector4(const float& value) : x_(value), y_(value), z_(value), w_(value) {}

        //--------------------------------------------------------------------------------
        //  set xyzw with given xyzw
        //  xyzwにvalueを入れる
        //--------------------------------------------------------------------------------
        Vector4(const float& x, const float& y, const float& z, const float& w) : x_(x), y_(y), z_(z), w_(w) {}
        
        //--------------------------------------------------------------------------------
        //  set xyz with given vector3 and w with given w
        //  xyzにvector3を入れて、wにwを入れる
        //--------------------------------------------------------------------------------
        Vector4(const Vector3& value, const float& w) : x_(value.x_), y_(value.y_), z_(value.z_), w_(w) {}
        
        float x_; // x component of the Vector4 / ベクトル4のx要素
        float y_; // y component of the Vector4 / ベクトル4のy要素
        float z_; // z component of the Vector4 / ベクトル4のz要素
        float w_; // w component of the Vector4 / ベクトル4のw要素

        static const Vector4 kZero; // vector4(0, 0, 0, 0)
        static const Vector4 kOne; // vector4(1, 1, 1, 1)

        //--------------------------------------------------------------------------------
        //  mult with matrix / マトリクスとの乗算
        //  matrix : 乗算相手
        //  return：Vector4
        //--------------------------------------------------------------------------------
        Vector4 operator*(const Matrix44& matrix) const;

        //--------------------------------------------------------------------------------
        //  mult this with matrix / マトリクスとの乗算
        //  matrix : 乗算相手
        //--------------------------------------------------------------------------------
        void operator*=(const Matrix44& matrix);

        //--------------------------------------------------------------------------------
        //  normalize the vector4 as plane / 平面正規化
        //--------------------------------------------------------------------------------
        void PlaneNormalize(void)
        {
            float inverse_magnitude = Vector3(this->x_, this->y_, this->z_).SquareMagnitude();
            if (inverse_magnitude)
            {
                inverse_magnitude = 1.0f / sqrtf(inverse_magnitude);
            }
            this->x_ *= inverse_magnitude;
            this->y_ *= inverse_magnitude;
            this->z_ *= inverse_magnitude;
            this->w_ *= inverse_magnitude;
        }

        //--------------------------------------------------------------------------------
        //  dot product of a plane and a 3D vector / 平面と 3D ベクトルの内積
        //--------------------------------------------------------------------------------
        float PlaneDotCoord(const Vector3& point) const
        {
            return this->x_ * point.x_ + this->y_ * point.y_ + this->z_ * point.z_ + this->w_;
        }
    };

    //--------------------------------------------------------------------------------
    //  Matrix4*4
    //--------------------------------------------------------------------------------
    class Matrix44
    {
    public:
        Matrix44();
        ~Matrix44() {}

        //--------------------------------------------------------------------------------
        //  set all elements by given value
        //  全てのメンバーにvalueを入れる
        //--------------------------------------------------------------------------------
        Matrix44(const float& value);

        //--------------------------------------------------------------------------------
        //  set each elements by given value
        //  全てのメンバーに対応の値を入れる
        //--------------------------------------------------------------------------------
        Matrix44(const float& m00, const float& m01, const float& m02, const float& m03,
                 const float& m10, const float& m11, const float& m12, const float& m13,
                 const float& m20, const float& m21, const float& m22, const float& m23,
                 const float& m30, const float& m31, const float& m32, const float& m33);
        
        union 
        {
            Vector4 rows_[4];
            struct
            {
                float m00_, m01_, m02_, m03_;
                float m10_, m11_, m12_, m13_;
                float m20_, m21_, m22_, m23_;
                float m30_, m31_, m32_, m33_;
            };
            float m_[4][4];
        };

        static const Matrix44 kZero; // matrix with all elements equal 0 / 要素が全部0の行列
        static const Matrix44 kIdentity; // identity matrix / 単位行列

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
        //--------------------------------------------------------------------------------
        //  cast to D3DXMATRIX / D3DXMATRIXにキャストする
        //  return：D3DXMATRIX
        //--------------------------------------------------------------------------------
        operator D3DXMATRIX() const
        {
            D3DXMATRIX result;
            memcpy_s(&result, sizeof(result), this, sizeof(*this));
            return result;
        }
#endif

        //--------------------------------------------------------------------------------
        //  代入処理
        //  value : 値
        //  return：Matrix44&
        //--------------------------------------------------------------------------------
        Matrix44& operator=(const Matrix44& value)
        {
            memcpy_s(this, sizeof(*this), &value, sizeof(value));
            return *this;
        }

        //--------------------------------------------------------------------------------
        //  mult value with each element / floatと乗算処理
        //  value : 相手
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        Matrix44 operator*(const float& value) const
        {
            Matrix44 result;
            for (int count_y = 0; count_y < 4; ++count_y)
            {
                for (int count_x = 0; count_x < 4; ++count_x)
                {
                    result.m_[count_y][count_x] = m_[count_y][count_x] * value;
                }
            }
            return result;
        }

        //--------------------------------------------------------------------------------
        //  add element with value / 加算処理
        //  value : 相手
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        Matrix44 operator+(const Matrix44& value) const
        {
            Matrix44 result;
            for (int count_y = 0; count_y < 4; ++count_y)
            {
                for (int count_x = 0; count_x < 4; ++count_x)
                {
                    result.m_[count_y][count_x] = m_[count_y][count_x] + value.m_[count_y][count_x];
                }
            }
            return result;
        }
        
        //--------------------------------------------------------------------------------
        //  mult matrices / 乗算処理
        //  value : 相手
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        Matrix44 operator*(const Matrix44& value) const
        {
            Matrix44 result;
            for (int count_y = 0; count_y < 4; ++count_y)
            {
                for (int count_x = 0; count_x < 4; ++count_x)
                {
                    result.m_[count_y][count_x] =
                        m_[count_y][0] * value.m_[0][count_x] +
                        m_[count_y][1] * value.m_[1][count_x] +
                        m_[count_y][2] * value.m_[2][count_x] +
                        m_[count_y][3] * value.m_[3][count_x];
                }
            }
            return result;
        }

        //--------------------------------------------------------------------------------
        //  mult matrices / 乗算処理
        //  value : 相手
        //--------------------------------------------------------------------------------
        void operator*=(const Matrix44& value)
        {
            Matrix44 copy = *this;
            for (int count_y = 0; count_y < 4; ++count_y)
            {
                for (int count_x = 0; count_x < 4; ++count_x)
                {
                    m_[count_y][count_x] =
                        copy.m_[count_y][0] * value.m_[0][count_x] +
                        copy.m_[count_y][1] * value.m_[1][count_x] +
                        copy.m_[count_y][2] * value.m_[2][count_x] +
                        copy.m_[count_y][3] * value.m_[3][count_x];
                }
            }
        }

        //--------------------------------------------------------------------------------
        //  change to euler / オーラー角に変換
        //  return : Vector3
        //--------------------------------------------------------------------------------
        Vector3 ToEuler(void) const;

        //--------------------------------------------------------------------------------
        //  change to quaternion / Quaternionに変換
        //  return : Quaternion
        //--------------------------------------------------------------------------------
        Quaternion ToQuaternion(void) const;

        //--------------------------------------------------------------------------------
        //  get the transpose matrix / transpose行列に変換して返す
        //  return : Matrix44
        //--------------------------------------------------------------------------------
        Matrix44 Transpose(void) const
        {
            Matrix44 result;
            for (int count_y = 0; count_y < 4; ++count_y)
            {
                for (int count_x = 0; count_x < 4; ++count_x)
                {
                    result.m_[count_y][count_x] = m_[count_x][count_y];
                }
            }
            return result;
        }

        //--------------------------------------------------------------------------------
        //  remove the scale value / 行列のスケール要素を無くす
        //--------------------------------------------------------------------------------
        void RemoveScale(void)
        {
            Vector3& right = Vector3(m00_, m01_, m02_).Normalized();
            m00_ = right.x_;
            m01_ = right.y_;
            m02_ = right.z_;
            Vector3& up = Vector3(m10_, m11_, m12_).Normalized();
            m10_ = up.x_;
            m11_ = up.y_;
            m12_ = up.z_;
            Vector3& forward = Vector3(m20_, m21_, m22_).Normalized();
            m20_ = forward.x_;
            m21_ = forward.y_;
            m22_ = forward.z_;
        }

        //--------------------------------------------------------------------------------
        //  get the inverse matrix / inverse行列に変換して返す
        //  return : Matrix44
        //--------------------------------------------------------------------------------
        Matrix44 Inverse(void) const
        {
            Matrix44 result;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
            D3DXMatrixInverse((D3DXMATRIX*)&result, NULL, (D3DXMATRIX*)this);
#endif
            return result;
        }

        //--------------------------------------------------------------------------------
        //  create scale matrix with scale value / 与えられた値でスケール行列の作成
        //  scale：スケール値
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        static Matrix44 Scale(const Vector3& scale)
        {
            return Matrix44(scale.x_, 0.0f, 0.0f, 0.0f,
                            0.0f, scale.y_, 0.0f, 0.0f,
                            0.0f, 0.0f, scale.z_, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f);
        }

        //--------------------------------------------------------------------------------
        //  create rotation matrix with right,up,forward / 与えられた回転で行列の作成
        //  right：右方向
        //  up：上方向
        //  forward：前方向
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        static Matrix44 Rotation(const Vector3& right, const Vector3& up, const Vector3& forward)
        {
            return Matrix44(right.x_, right.y_, right.z_, 0.0f,
                            up.x_, up.y_, up.z_, 0.0f,
                            forward.x_, forward.y_, forward.z_, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f);
        }

        //--------------------------------------------------------------------------------
        //  create rotation matrix with axis and radian / 与えられた軸と角で回転行列の作成
        //  axis：軸
        //  radian：角度
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        static Matrix44 RotationAxis(const Vector3& axis, const float& radian);

        //--------------------------------------------------------------------------------
        //  create rotation matrix with euler / 与えられたeuler角で回転行列の作成
        //  euler：オーラー角
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        static Matrix44 RotationYawPitchRoll(const Vector3& euler);

        //--------------------------------------------------------------------------------
        //  create traslation matrix with translation value
        //  与えられた移動量で平行移動行列の作成
        //  translation：移動量
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        static Matrix44 Translation(const Vector3& translation)
        {
            Matrix44 result;
            result.m_[3][0] = translation.x_;
            result.m_[3][1] = translation.y_;
            result.m_[3][2] = translation.z_;
            return result;
        }

        //--------------------------------------------------------------------------------
        //  create transform matrix with given right, up, forward, scale, translation
        //  与えられた回転、移動量、スケールで行列の作成
        //  right：右方向
        //  up：上方向
        //  forward：前方向
        //  translation：移動量
        //  scale：スケール量
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        static Matrix44 Transform(const Vector3& right, const Vector3& up, const Vector3& forward, const Vector3& translation, const Vector3& scale = Vector3::kOne);
        
        //--------------------------------------------------------------------------------
        //  create transform matrix with given rotation, scale, translation
        //  与えられた回転、移動量、スケールで行列の作成
        //  rotation：回転量(vector3)
        //  translation：移動量
        //  scale：スケール量
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        static Matrix44 Transform(const Vector3& rotation, const Vector3& translation, const Vector3& scale = Vector3::kOne)
        {
            auto& result = Scale(scale);
            result *= RotationYawPitchRoll(rotation);
            result *= Translation(translation);
            return result;
        }

        //--------------------------------------------------------------------------------
        //  create transform matrix with given rotation, scale, translation
        //  与えられた回転、移動量、スケールで行列の作成
        //  rotation：回転量(quaternion)
        //  translation：移動量
        //  scale：スケール量
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        static Matrix44 Transform(const Quaternion& rotation, const Vector3& translation, const Vector3& scale = Vector3::kOne);

        //--------------------------------------------------------------------------------
        //  create perspective matrix / パースペクティブ行列の作成(左手座標系)
        //  fov_radian_y：視野角度
        //  aspect_ratio：アスペクト比
        //  near_z：一番近い距離
        //  far_z：一番遠い距離
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        static Matrix44 PerspectiveLeftHand(const float& fov_radian_y, const float& aspect_ratio, const float& near_z, const float& far_z);

        //--------------------------------------------------------------------------------
        //  create orthographic matrix / オルソ行列の作成(左手座標系)
        //  width：幅
        //  height：高さ
        //  near_z：一番近い距離
        //  far_z：一番遠い距離
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        static Matrix44 OrthographicLeftHand(const float& left, const float& right, const float& top, const float& bottom, const float& near_z, const float& far_z)
        {
            //float range = 1.0f / (far_z - near_z);
            Matrix44 result;
            //result.m_[0][0] = 2.0f / width;
            //result.m_[1][1] = 2.0f / height;
            //result.m_[2][2] = range;
            //result.m_[3][2] = -range * near_z;
            result.m_[0][0] = 2.0f / (right - left);
            result.m_[1][1] = 2.0f / (top - bottom);
            result.m_[2][2] = 1.0f / (far_z - near_z);
            result.m_[3][0] = -(right + left) / (right - left);
            result.m_[3][1] = -(top + bottom) / (top - bottom);
            result.m_[3][2] = -near_z / (far_z - near_z);
            return result;
        }

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
        //--------------------------------------------------------------------------------
        //  D3DXMATRIX To Matrix44
        //  D3DXMATRIXをMatrix44に変換
        //  value：D3DXMATRIX
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        static Matrix44 ToMatrix44(const D3DXMATRIX& value)
        {
            Matrix44 result;
            memcpy_s(&result, sizeof(result), &value, sizeof(value));
            return result;
        }
#endif
    };

    //--------------------------------------------------------------------------------
    //  クォータニオン
    //--------------------------------------------------------------------------------
    class Quaternion
    {
    public:
        Quaternion() : x_(0.0f), y_(0.0f), z_(0.0f), w_(1.0f) {}
        ~Quaternion() {}

        //--------------------------------------------------------------------------------
        //  set the xyz with value and w with 1
        //  xyzメンバーにvalueを入れて、wに1を入れる
        //--------------------------------------------------------------------------------
        Quaternion(const float& value) : x_(value), y_(value), z_(value), w_(1.0f) {}
        
        //--------------------------------------------------------------------------------
        //  set each component with the given value
        //  xyzwメンバーにvalueを入れる
        //--------------------------------------------------------------------------------
        Quaternion(const float& x, const float& y, const float& z, const float& w) : x_(x), y_(y), z_(z), w_(w) {}
        
        float x_; // x component of the Quaternion / クォータニオンのx要素
        float y_; // y component of the Quaternion / クォータニオンのy要素
        float z_; // z component of the Quaternion / クォータニオンのz要素
        float w_; // w component of the Quaternion / クォータニオンのw要素

        static const Quaternion kIdentity; // Quaternion(0,0,0,1)

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
        //--------------------------------------------------------------------------------
        //  cast to D3DXQUATERNION / D3DXQUATERNIONにキャストする
        //  return：D3DXQUATERNION
        //--------------------------------------------------------------------------------
        operator D3DXQUATERNION () const
        {
            return D3DXQUATERNION(x_, y_, z_, w_);
        }
#endif

        //--------------------------------------------------------------------------------
        //  足し算
        //  value : 相手
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion operator+(const Quaternion& value) const
        {
            return Quaternion(x_ + value.x_, y_ + value.y_, z_ + value.z_, w_ + value.w_);
        }

        //--------------------------------------------------------------------------------
        //  足し算
        //  value : 相手
        //--------------------------------------------------------------------------------
        void operator+=(const Quaternion& value)
        {
            x_ += value.x_;
            y_ += value.y_;
            z_ += value.z_;
            w_ += value.w_;
        }

        //--------------------------------------------------------------------------------
        //  引き算
        //  value : 相手
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion operator-(const Quaternion& value) const
        {
            return Quaternion(x_ - value.x_, y_ - value.y_, z_ - value.z_, w_ - value.w_);
        }

        //--------------------------------------------------------------------------------
        //  引き算
        //  value : 相手
        //--------------------------------------------------------------------------------
        void operator-=(const Quaternion& value)
        {
            x_ -= value.x_;
            y_ -= value.y_;
            z_ -= value.z_;
            w_ -= value.w_;
        }

        //--------------------------------------------------------------------------------
        //  乗算(float)
        //  value : 相手
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion operator*(const float& value) const
        {
            return Quaternion(x_ * value, y_ * value, z_ * value, w_ * value);
        }

        //--------------------------------------------------------------------------------
        //  乗算(float)
        //  value : 相手
        //--------------------------------------------------------------------------------
        void operator*=(const float& value)
        {
            x_ *= value;
            y_ *= value;
            z_ *= value;
            w_ *= value;
        }

        //--------------------------------------------------------------------------------
        //  除算(float)
        //  value : 相手
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion operator/(const float& value) const
        {
            assert(value != 0.0f);
            return Quaternion(x_ / value, y_ / value, z_ / value, w_ / value);
        }

        //--------------------------------------------------------------------------------
        //  除算(float)
        //  value : 相手
        //--------------------------------------------------------------------------------
        void operator/=(const float& value)
        {
            x_ /= value;
            y_ /= value;
            z_ /= value;
            w_ /= value;
        }

        //--------------------------------------------------------------------------------
        //  乗算(Quaternion)
        //  value : 相手
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion operator*(const Quaternion& value) const
        {
            return Quaternion((value.w_ * this->x_) + (value.x_ * this->w_) + (value.y_ * this->z_) - (value.z_ * this->y_),
                              (value.w_ * this->y_) - (value.x_ * this->z_) + (value.y_ * this->w_) + (value.z_ * this->x_),
                              (value.w_ * this->z_) + (value.x_ * this->y_) - (value.y_ * this->x_) + (value.z_ * this->w_),
                              (value.w_ * this->w_) - (value.x_ * this->x_) - (value.y_ * this->y_) - (value.z_ * this->z_));
        }
        
        //--------------------------------------------------------------------------------
        //  乗算(Quaternion)
        //  value : 相手
        //--------------------------------------------------------------------------------
        void operator*=(const Quaternion& value)
        {
            auto copy = *this;
            this->x_ = (value.w_ * copy.x_) + (value.x_ * copy.w_) + (value.y_ * copy.z_) - (value.z_ * copy.y_);
            this->y_ = (value.w_ * copy.y_) - (value.x_ * copy.z_) + (value.y_ * copy.w_) + (value.z_ * copy.x_);
            this->z_ = (value.w_ * copy.z_) + (value.x_ * copy.y_) - (value.y_ * copy.x_) + (value.z_ * copy.w_);
            this->w_ = (value.w_ * copy.w_) - (value.x_ * copy.x_) - (value.y_ * copy.y_) - (value.z_ * copy.z_);
        }

        //--------------------------------------------------------------------------------
        //  Dot乗算
        //    value : 相手
        //    return：float
        //--------------------------------------------------------------------------------
        float Dot(const Quaternion& value) const
        {
            return (x_ * value.x_ + y_ * value.y_ + z_ * value.z_ + w_ * value.w_);
        }
        
        //--------------------------------------------------------------------------------
        //  長さの平方の算出
        //  return：float
        //--------------------------------------------------------------------------------
        float SquareMagnitude(void) const
        {
            return this->Dot(*this);
        }

        //--------------------------------------------------------------------------------
        //  長さの算出
        //  return：float
        //--------------------------------------------------------------------------------
        float Magnitude(void) const
        {
            return sqrtf(this->SquareMagnitude());
        }

        //--------------------------------------------------------------------------------
        //  return the normalized quaternion / 正規化したの値を返す
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion Normalized(void) const
        {
            float magnitude = this->Magnitude();
            if (magnitude <= 0.0f) return Quaternion::kIdentity;
            return *this / magnitude;
        }

        //--------------------------------------------------------------------------------
        //  normalize quaternion / 正規化
        //--------------------------------------------------------------------------------
        void Normalize(void)
        {
            *this = this->Normalized();
        }

        //--------------------------------------------------------------------------------
        //  return the conjugate quaternion
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion Conjugate(void) const
        {
            return Quaternion(-x_, -y_, -z_, w_);
        }
        
        //--------------------------------------------------------------------------------
        //  mult each component with value別々で乗算
        //  value：相手
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion MultiplySeparately(const Quaternion& value) const
        {
            return Quaternion(x_ * value.x_, y_ * value.y_, z_ * value.z_, w_ * value.w_);
        }
        
        //--------------------------------------------------------------------------------
        //    euler角に変換
        //    return：Vector3
        //--------------------------------------------------------------------------------
        Vector3 ToEuler(void) const;
        
        //--------------------------------------------------------------------------------
        //    行列に変換
        //    return：Matrix44
        //--------------------------------------------------------------------------------
        Matrix44 ToMatrix(void) const;

        //--------------------------------------------------------------------------------
        //    create a quaternion rotate by axis
        //    return：quaternion
        //--------------------------------------------------------------------------------
        static Quaternion RotateAxis(const Vector3& axis, const float& radian)
        {
            Quaternion& axis_quaternion = Quaternion(axis.x_, axis.y_, axis.z_, 1.0f);
            float sin = sinf(radian * 0.5f);
            float cos = cosf(radian * 0.5f);
            Quaternion& scale = Quaternion(sin, sin, sin, cos);
            return axis_quaternion.MultiplySeparately(scale);
        }

        //--------------------------------------------------------------------------------
        //    create a quaternion rotate by axis
        //    return：quaternion
        //--------------------------------------------------------------------------------
        static Quaternion FromToRotation(const Vector3& from, const Vector3& to)
        {
            Vector3& euler = Vector3::EulerBetween(from, to);
            return euler.ToQuaternion();
        }
    };

    //--------------------------------------------------------------------------------
    //  色
    //--------------------------------------------------------------------------------
    class Color
    {
    public:
        Color() : r_(0.0f), g_(0.0f), b_(0.0f), a_(0.0f) {}
        ~Color() {}

        //--------------------------------------------------------------------------------
        //  set the rgba with value
        //  rgbaにvalueを入れる
        //--------------------------------------------------------------------------------
        Color(const float& value) : r_(value), g_(value), b_(value), a_(value) {}
        
        //--------------------------------------------------------------------------------
        //  set each component with the given value
        //  rgbaにvalueを入れる
        //--------------------------------------------------------------------------------
        Color(const float& r, const float& g, const float& b, const float& a) : r_(r), g_(g), b_(b), a_(a) {}
        
        float r_; // r component of the color / 色のr要素
        float g_; // g component of the color / 色のg要素
        float b_; // b component of the color / 色のb要素
        float a_; // a component of the color / 色のa要素

        static const Color kWhite; // Color(1,1,1,1)
        static const Color kBlack; // Color(0,0,0,1)
        static const Color kGray; // Color(0.2,0.2,0.2,1)
        static const Color kRed; // Color(1,0,0,1)
        static const Color kBlue; // Color(0,0,1,1)
        static const Color kGreen; // Color(0,1,0,1)

        //キャスト
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
        //--------------------------------------------------------------------------------
        //  cast to D3DCOLORVALUE / D3DCOLORVALUEにキャストする
        //  return：D3DCOLORVALUE
        //--------------------------------------------------------------------------------
        operator D3DCOLORVALUE () const
        {
            D3DCOLORVALUE result;
            result.r = r_;
            result.g = g_;
            result.b = b_;
            result.a = a_;
            return result;
        }

        //--------------------------------------------------------------------------------
        //  cast to unsigned long / unsigned longにキャストする
        //  return：unsigned long
        //--------------------------------------------------------------------------------
        operator ul() const
        {
            return static_cast<ul>(((static_cast<ul>(a_ * 255.0f) & 0xff) << 24)
                | ((static_cast<ul>(r_ * 255.0f) & 0xff) << 16)
                | ((static_cast<ul>(g_ * 255.0f) & 0xff) << 8)
                | (static_cast<ul>(b_ * 255.0f) & 0xff));
        }
#endif

        //--------------------------------------------------------------------------------
        //  代入処理
        //  value : 値
        //  return：Color
        //--------------------------------------------------------------------------------
        Color& operator=(const Color& value)
        {
            r_ = value.r_;
            g_ = value.g_;
            b_ = value.b_;
            a_ = value.a_;
            return *this;
        }
        
        //--------------------------------------------------------------------------------
        //  return true if the colors' rgba are the same / 同値判定処理
        //  value : 相手
        //  return：bool
        //--------------------------------------------------------------------------------
        bool operator==(const Color& value)
        {
            return (r_ == value.r_ && g_ == value.g_ && b_ == value.b_ && a_ == value.a_);
        }

        //--------------------------------------------------------------------------------
        //  add the rgba / 足し算処理
        //  value : 値
        //  return：Color
        //--------------------------------------------------------------------------------
        Color operator+(const Color& value) const
        {
            return Color(r_ + value.r_, g_ + value.g_, b_ + value.b_, a_ + value.a_);
        }

        //--------------------------------------------------------------------------------
        //  add the rgba / 足し算処理
        //  value : 値
        //--------------------------------------------------------------------------------
        void operator+=(const Color& value)
        {
            r_ += value.r_;
            g_ += value.g_;
            b_ += value.b_;
            a_ += value.a_;
        }

        //--------------------------------------------------------------------------------
        //  minus the rgba / 引き算処理
        //  value : 値
        //  return：Color
        //--------------------------------------------------------------------------------
        Color operator-(const Color& value) const
        {
            return Color(r_ - value.r_, g_ - value.g_, b_ - value.b_, a_ - value.a_);
        }
        
        //--------------------------------------------------------------------------------
        //  minus the rgba / 引き算処理
        //  value : 値
        //--------------------------------------------------------------------------------
        void operator-=(const Color& value)
        {
            r_ -= value.r_;
            g_ -= value.g_;
            b_ -= value.b_;
            a_ -= value.a_;
        }

        //--------------------------------------------------------------------------------
        //  mult the rgba / 掛け算処理
        //  value : 値
        //  return：Color
        //--------------------------------------------------------------------------------
        Color operator*(const float& value) const
        {
            return Color(r_ * value, g_ * value, b_ * value, a_ * value);
        }

        //--------------------------------------------------------------------------------
        //  mult the rgba / 掛け算処理
        //  value : 値
        //--------------------------------------------------------------------------------
        void operator*=(const float& value)
        {
            r_ *= value;
            g_ *= value;
            b_ *= value;
            a_ *= value;
        }
    };

    //--------------------------------------------------------------------------------
    //  レイ
    //--------------------------------------------------------------------------------
    class Ray
    {
    public:
        Ray() : origin_(Vector3::kZero) , direction_(Vector3::kZero) {}
        Ray(const Vector3& origin, const Vector3& direction)
            : origin_(origin), direction_(direction) {}
        ~Ray() {}

        Vector3 origin_; // レイの始点
        Vector3 direction_; // レイの方向

        //--------------------------------------------------------------------------------
        //  レイの変換
        //  transform : 変換行列
        //--------------------------------------------------------------------------------
        void Transform(const Matrix44& transform)
        {
            origin_ = Vector3::TransformCoord(origin_, transform);
            direction_ = Vector3::TransformNormal(direction_, transform);
        }
    };

    //--------------------------------------------------------------------------------
    //    ランダム
    //--------------------------------------------------------------------------------
    class Random
    {
    public:
        //--------------------------------------------------------------------------------
        //  ランダムシステム初期化
        //--------------------------------------------------------------------------------
        static void Init(void)
        {
            srand((unsigned)time(NULL));
        }

        //--------------------------------------------------------------------------------
        //  random int between min and max / ランダムのInt値を取得
        //  min：最小値
        //  max：最大値
        //  return：int
        //--------------------------------------------------------------------------------
        static int Range(const int& min, const int& max)
        {
            if (min >= max) return min;
            return rand() % (max - min) + min;
        }

        //--------------------------------------------------------------------------------
        //  random float between min and max / ランダムのfloat値を取得
        //  min：最小値
        //  max：最大値
        //  return：float
        //--------------------------------------------------------------------------------
        static float Range(const float& min, const float& max)
        {
            if (min >= max) { return min; }
            return (rand() % 10000) * 0.0001f * (max - min) + min;
        }

        //--------------------------------------------------------------------------------
        //  random vector3 between min and max / ランダムのvector3値を取得
        //  min：最小値
        //  max：最大値
        //  return：vector3
        //--------------------------------------------------------------------------------
        static Vector3 Range(const Vector3& min, const Vector3& max)
        {
            return Vector3(Range(min.x_, max.x_), Range(min.y_, max.y_), Range(min.z_, max.z_));
        }
    };

    //--------------------------------------------------------------------------------
    //    計算式
    //--------------------------------------------------------------------------------
    class Math
    {
    public:
        //--------------------------------------------------------------------------------
        //  Vector3を線形補間方式で補間する
        //  from：始点
        //  to：終点
        //  time：時間(0 - 1)
        //  return：Vector3
        //--------------------------------------------------------------------------------
        static Vector3 Lerp(const Vector3& from, const Vector3& to, const float& time)
        {
            if (time <= 0.0f) return from;
            if (time >= 1.0f) return to;
            return (from + (to - from) * time);
        }

        //--------------------------------------------------------------------------------
        //  floatを線形補間方式で補間する
        //  from：始点
        //  to：終点
        //  time：時間(0 - 1)
        //  return：float
        //--------------------------------------------------------------------------------
        static float Lerp(const float& from, const float& to, const float& time)
        {
            if (time <= 0.0f) return from;
            if (time >= 1.0f) return to;
            return (from + (to - from) * time);
        }

        //--------------------------------------------------------------------------------
        //  Colorを線形補間方式で補間する
        //  from：始点
        //  to：終点
        //  time：時間(0 - 1)
        //  return：Color
        //--------------------------------------------------------------------------------
        static Color Lerp(const Color& from, const Color& to, const float& time)
        {
            if (time <= 0.0f) return from;
            if (time >= 1.0f) return to;
            return (from + (to - from) * time);
        }

        //--------------------------------------------------------------------------------
        //  Vector3を球形補間方式で補間する
        //  from：始点
        //  to：終点
        //  time：時間(0 - 1)
        //  return：Vector3
        //--------------------------------------------------------------------------------
        static Vector3 Slerp(const Vector3& from, const Vector3& to, const float& time)
        {
            if (time <= 0.0f) return from;
            if (time >= 1.0f) return to;
            return (from + (to - from) * time).Normalized();
        }

        //--------------------------------------------------------------------------------
        //  Quaternionを球形補間方式で補間する
        //  from：始点
        //  to：終点
        //  time：時間(0 - 1)
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        static Quaternion Slerp(const Quaternion& from, const Quaternion& to, const float& time)
        {
            if (time <= 0.0f) { return from; }
            if (time >= 1.0f) { return to; }
            Quaternion result;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
            D3DXQuaternionSlerp((D3DXQUATERNION*)&result, (D3DXQUATERNION*)&from, (D3DXQUATERNION*)&to, time);
#else
            //Quaternion qFromCpy = qFrom;
            //Quaternion qToCpy = qTo;

            //// Only unit quaternions are valid rotations.
            //// Normalize to avoid undefined behavior.

            //// Compute the cosine of the angle between the two vectors.
            //float fDot = QuaternionDot(qFromCpy, qToCpy);

            //if (fabs(fDot) > 0.9995f)
            //{
            //    // If the inputs are too close for comfort, linearly interpolate
            //    // and normalize the result.
            //    Quaternion qResult = qFromCpy + (qToCpy - qFromCpy) * fTime;
            //    QuaternionNormalize(qResult);
            //    return qResult;
            //}

            //// If the dot product is negative, the quaternions
            //// have opposite handed-ness and slerp won't take
            //// the shorter path. Fix by reversing one quaternion.
            //if (fDot < 0.0f) 
            //{
            //    qToCpy *= -1.0f;
            //    fDot = -fDot;
            //}

            //ClampFloat(fDot, -1.0f, 1.0f);        // Robustness: Stay within domain of acos()
            //float fTheta = acosf(fDot) * fTime;    // theta = angle between v0 and result 

            //Quaternion qWork = qToCpy - qFromCpy * fDot;
            //QuaternionNormalize(qWork);            // { v0, v2 } is now an orthonormal basis
            //Quaternion qResult = qFromCpy * cosf(fTheta) + qWork * sinf(fTheta);
#endif
            return result;
        }

        //--------------------------------------------------------------------------------
        //  floatをminとmaxの間にする
        //  value：現在値
        //  min：最小値
        //  max：最大値
        //  return：float
        //--------------------------------------------------------------------------------
        static float Clamp(const float& value, const float& min, const float& max)
        {
            return value < min ? min : value > max ? max : value;
        }

        //--------------------------------------------------------------------------------
        //  Vector3をminとmaxの間にする
        //  value：現在値
        //  min：最小値
        //  max：最大値
        //  return：Vector3
        //--------------------------------------------------------------------------------
        static Vector3 Clamp(const Vector3& value, const Vector3& min, const Vector3& max)
        {
            return Vector3(Clamp(value.x_, min.x_, max.x_),
                           Clamp(value.y_, min.y_, max.y_),
                           Clamp(value.z_, min.z_, max.z_));
        }

        //--------------------------------------------------------------------------------
        //  return the abs's max value / 絶対値が大きい方を返す
        //    value_l、value_r：比較値
        //    return：float
        //--------------------------------------------------------------------------------
        static float AbsMax(const float& value_l, const float& value_r)
        {
            return fabsf(value_l) >= fabsf(value_r) ? value_l : value_r;
        }

        //--------------------------------------------------------------------------------
        //  swap the value of left and right / 左値と右値を交換する
        //--------------------------------------------------------------------------------
        static void Swap(float& left, float& right)
        {
            float work = left;
            left = right;
            right = work;
        }
    };
} // namespace KodFreedom
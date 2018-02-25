//--------------------------------------------------------------------------------
//　kf_math.h
//  classes and methods for math
//  演算用のクラスとメソッド
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include <time.h>
#include <math.h>
#include "../common_setting.h"
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
        Short2(const short x, const short y) : x_(x), y_(y) {}
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
        //--------------------------------------------------------------------------------
        //  コンストラクタとデストラクタ
        //--------------------------------------------------------------------------------
        Vector2() : x_(0.0f), y_(0.0f) {}
        ~Vector2() {}
        Vector2(const float& value) : x_(value), y_(value) {} // xとyにvalueを入れる
        Vector2(const float& x, const float& y) : x_(x), y_(y) {} // xとyに与えられた値を入れるを入れる
        
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
        Vector2& operator=(const Vector2& value);

        //--------------------------------------------------------------------------------
        //  check if this equal value / 同値判定処理
        //  value : 値
        //  return：bool
        //--------------------------------------------------------------------------------
        bool operator==(const Vector2& value) const;

        //--------------------------------------------------------------------------------
        //  add / 足し算
        //  value : 値
        //  return：Vector2
        //--------------------------------------------------------------------------------
        Vector2 operator+(const Vector2& value) const;
        
        //--------------------------------------------------------------------------------
        //  add to this / 自分に足す
        //  value : 値
        //--------------------------------------------------------------------------------
        void operator+=(const Vector2& value);

        //--------------------------------------------------------------------------------
        //  minus / 引き算
        //  value : 値
        //  return：Vector2
        //--------------------------------------------------------------------------------
        Vector2 operator-(const Vector2& value) const;
        
        //--------------------------------------------------------------------------------
        //  minus to this / 自分に引く
        //  value : 値
        //--------------------------------------------------------------------------------
        void operator-=(const Vector2& value);

        //--------------------------------------------------------------------------------
        //  scale x and y / スケール
        //  value : scale value / スケール値
        //  return：Vector2
        //--------------------------------------------------------------------------------
        Vector2 operator*(const float& value) const;

        //--------------------------------------------------------------------------------
        //  scale x and y / スケール
        //  value : scale value / スケール値
        //--------------------------------------------------------------------------------
        void operator*=(const float& value);

        //--------------------------------------------------------------------------------
        //  cross / 外積
        //  value : 値
        //  return：float
        //--------------------------------------------------------------------------------
        float operator*(const Vector2& value) const;

        //--------------------------------------------------------------------------------
        //  dot / 内積
        //  value : 値
        //  return：float
        //--------------------------------------------------------------------------------
        float Dot(const Vector2& value) const;

        //--------------------------------------------------------------------------------
        //  compute the magnitude / 長さの計算
        //  return：float
        //--------------------------------------------------------------------------------
        float Magnitude(void) const;

        //--------------------------------------------------------------------------------
        //  compute the square magnitude / 長さ平方の計算
        //  return：float
        //--------------------------------------------------------------------------------
        float SquareMagnitude(void) const;
        
        //--------------------------------------------------------------------------------
        //  return the normalized vector / 正規化したの値を返す
        //  return：Vector2
        //--------------------------------------------------------------------------------
        Vector2 Normalized(void) const;

        //--------------------------------------------------------------------------------
        //  normalize the vector / 正規化
        //--------------------------------------------------------------------------------
        void Normalize(void);

        //--------------------------------------------------------------------------------
        //  return the radian between x, y / xy間の角度の算出
        //  return：float
        //--------------------------------------------------------------------------------
        float ToRadian(void) const;

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
        //--------------------------------------------------------------------------------
        //  コンストラクタとデストラクタ
        //--------------------------------------------------------------------------------
        Vector3() : x_(0.0f), y_(0.0f), z_(0.0f) {}
        ~Vector3() {}
        Vector3(const float& value) : x_(value), y_(value), z_(value) {} // xyzにvalueを入れる
        Vector3(const float& x, const float& y, const float& z) : x_(x), y_(y), z_(z) {} // xyzに与えられた値を入れるを入れる
        Vector3(const Vector4& value); // xyzに与えられたVector4のxyz値を入れるを入れる
        
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
        operator Vector2() const;

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
        Vector3& operator=(const Vector3& value);

        //--------------------------------------------------------------------------------
        //  check if this equal value / 同値判定処理
        //  value : 値
        //  return：bool
        //--------------------------------------------------------------------------------
        bool operator==(const Vector3& value) const;

        //--------------------------------------------------------------------------------
        //  check if this not equal value / 異なる値判定処理
        //  value : 値
        //  return：bool
        //--------------------------------------------------------------------------------
        bool operator!=(const Vector3& value) const;

        //--------------------------------------------------------------------------------
        //  add / 足し算
        //  value : 値
        //  return：Vector3
        //--------------------------------------------------------------------------------
        Vector3 operator+(const Vector3& value) const;

        //--------------------------------------------------------------------------------
        //  add to this / 自分に足す
        //  value : 値
        //--------------------------------------------------------------------------------
        void operator+=(const Vector3& value);

        //--------------------------------------------------------------------------------
        //  minus / 引き算
        //  value : 値
        //  return：Vector3
        //--------------------------------------------------------------------------------
        Vector3 operator-(const Vector3& value) const;

        //--------------------------------------------------------------------------------
        //  minus to this / 自分に引く
        //  value : 値
        //--------------------------------------------------------------------------------
        void operator-=(const Vector3& value);

        //--------------------------------------------------------------------------------
        //  scale xyz / スケール
        //  value : scale value / スケール値
        //--------------------------------------------------------------------------------
        Vector3 operator*(const float& value) const;

        //--------------------------------------------------------------------------------
        //  scale xyz / スケール
        //  value : scale value / スケール値
        //--------------------------------------------------------------------------------
        void operator*=(const float& value);

        //--------------------------------------------------------------------------------
        //  cross / 外積
        //  value : 値
        //  return：Vector3
        //--------------------------------------------------------------------------------
        Vector3 operator*(const Vector3& value) const;

        //--------------------------------------------------------------------------------
        //  cross / 外積
        //  value : 値
        //  return：float
        //--------------------------------------------------------------------------------
        void operator*=(const Vector3& value);

        //--------------------------------------------------------------------------------
        //  scale xyz / スケール
        //  value : scale value / スケール値
        //--------------------------------------------------------------------------------
        Vector3 operator/(const float& value) const;

        //--------------------------------------------------------------------------------
        //  scale xyz / スケール
        //  value : scale value / スケール値
        //--------------------------------------------------------------------------------
        void operator/=(const float& value);

        //--------------------------------------------------------------------------------
        //  dot / 内積
        //  value : 値
        //  return：float
        //--------------------------------------------------------------------------------
        float Dot(const Vector3& value) const;

        //--------------------------------------------------------------------------------
        //  compute the magnitude / 長さの計算
        //  return：float
        //--------------------------------------------------------------------------------
        float Magnitude(void) const;

        //--------------------------------------------------------------------------------
        //  compute the square magnitude / 長さ平方の計算
        //  return：float
        //--------------------------------------------------------------------------------
        float SquareMagnitude(void) const;

        //--------------------------------------------------------------------------------
        //  return the normalized vector / 正規化したの値を返す
        //  return：Vector3
        //--------------------------------------------------------------------------------
        Vector3 Normalized(void) const;

        //--------------------------------------------------------------------------------
        //  normalize the vector / 正規化
        //--------------------------------------------------------------------------------
        void Normalize(void);
        
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
        static float DistanceBetween(const Vector3& from, const Vector3& to);

        //--------------------------------------------------------------------------------
        //  両点の間の距離の平方を返す
        //  from：始点
        //  to : 終点
        //  return：float
        //--------------------------------------------------------------------------------
        static float SquareDistanceBetween(const Vector3& from, const Vector3& to);

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
        static Vector3 Scale(const Vector3& value, const Vector3& scale);

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
        static Vector3 EulerBetween(const Vector3& from, const Vector3& to);

        //--------------------------------------------------------------------------------
        //  project the direction on plane：方向を平面と平行の方向に回転する
        //  direction：回転したい方向
        //  plane_normal：平面の上方向
        //  current_normal：今の上方向
        //  return：Vector3
        //--------------------------------------------------------------------------------
        static Vector3 ProjectOnPlane(const Vector3& direction, const Vector3& plane_normal, const Vector3& current_normal = Vector3::kUp);

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
    //  ベクトル4
    //--------------------------------------------------------------------------------
    class Vector4
    {
    public:
        //--------------------------------------------------------------------------------
        //  コンストラクタとデストラクタ
        //--------------------------------------------------------------------------------
        Vector4() : x_(0.0f), y_(0.0f), z_(0.0f), w_(0.0f) {}
        ~Vector4() {}
        Vector4(const float& value) : x_(value), y_(value), z_(value), w_(value) {} // xyzwにvalueを入れる
        Vector4(const float& x, const float& y, const float& z, const float& w) : x_(x), y_(y), z_(z), w_(w) {} // xyzwにvalueを入れる
        Vector4(const Vector3& value, const float& w) : x_(value.x_), y_(value.y_), z_(value.z_), w_(w) {} // xyzにvector3を入れて、wにwを入れる
        
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
        void PlaneNormalize(void);

        //--------------------------------------------------------------------------------
        //  dot product of a plane and a 3D vector / 平面と 3D ベクトルの内積
        //--------------------------------------------------------------------------------
        float PlaneDotCoord(const Vector3& point) const;
    };

    //--------------------------------------------------------------------------------
    //  Matrix4*4
    //--------------------------------------------------------------------------------
    class Matrix44
    {
    public:
        //--------------------------------------------------------------------------------
        //  コンストラクタとデストラクタ
        //--------------------------------------------------------------------------------
        Matrix44()
            : m00_(1.0f), m01_(0.0f), m02_(0.0f), m03_(0.0f)
            , m10_(0.0f), m11_(1.0f), m12_(0.0f), m13_(0.0f)
            , m20_(0.0f), m21_(0.0f), m22_(1.0f), m23_(0.0f)
            , m30_(0.0f), m31_(0.0f), m32_(0.0f), m33_(1.0f) {}
        ~Matrix44() {}
        Matrix44(const float& value) // 全てのメンバーにvalueを入れる
            : m00_(value), m01_(value), m02_(value), m03_(value)
            , m10_(value), m11_(value), m12_(value), m13_(value)
            , m20_(value), m21_(value), m22_(value), m23_(value)
            , m30_(value), m31_(value), m32_(value), m33_(value) {}
        Matrix44(const float& m00, const float& m01, const float& m02, const float& m03,
                 const float& m10, const float& m11, const float& m12, const float& m13,
                 const float& m20, const float& m21, const float& m22, const float& m23,
                 const float& m30, const float& m31, const float& m32, const float& m33)
            : m00_(m00), m01_(m01), m02_(m02), m03_(m03)
            , m10_(m10), m11_(m11), m12_(m12), m13_(m13)
            , m20_(m20), m21_(m21), m22_(m22), m23_(m23)
            , m30_(m30), m31_(m31), m32_(m32), m33_(m33) {} // 全てのメンバーに対応の値を入れる
        
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
        Matrix44& operator=(const Matrix44& value);

        //--------------------------------------------------------------------------------
        //  mult value with each element / floatと乗算処理
        //  value : 相手
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        Matrix44 operator*(const float& value) const;

        //--------------------------------------------------------------------------------
        //  add element with value / 加算処理
        //  value : 相手
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        Matrix44 operator+(const Matrix44& value) const;
        
        //--------------------------------------------------------------------------------
        //  mult matrices / 乗算処理
        //  value : 相手
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        Matrix44 operator*(const Matrix44& value) const;

        //--------------------------------------------------------------------------------
        //  mult matrices / 乗算処理
        //  value : 相手
        //--------------------------------------------------------------------------------
        void operator*=(const Matrix44& value);

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
        Matrix44 Transpose(void) const;

        //--------------------------------------------------------------------------------
        //  remove the scale value / 行列のスケール要素を無くす
        //--------------------------------------------------------------------------------
        void RemoveScale(void);

        //--------------------------------------------------------------------------------
        //  get the inverse matrix / inverse行列に変換して返す
        //  return : Matrix44
        //--------------------------------------------------------------------------------
        Matrix44 Inverse(void) const;

        //--------------------------------------------------------------------------------
        //  create scale matrix with scale value / 与えられた値でスケール行列の作成
        //  scale：スケール値
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        static Matrix44 Scale(const Vector3& scale);

        //--------------------------------------------------------------------------------
        //  create rotation matrix with right,up,forward / 与えられた回転で行列の作成
        //  right：右方向
        //  up：上方向
        //  forward：前方向
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        static Matrix44 Rotation(const Vector3& right, const Vector3& up, const Vector3& forward);

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
        static Matrix44 Translation(const Vector3& translation);

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
        static Matrix44 Transform(const Vector3& rotation, const Vector3& translation, const Vector3& scale = Vector3::kOne);

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
        static Matrix44 OrthographicLeftHand(const float& left, const float& right, const float& top, const float& bottom, const float& near_z, const float& far_z);

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
        //--------------------------------------------------------------------------------
        //  コンストラクタとデストラクタ
        //--------------------------------------------------------------------------------
        Quaternion() : x_(0.0f), y_(0.0f), z_(0.0f), w_(1.0f) {}
        ~Quaternion() {}
        Quaternion(const float& value) : x_(value), y_(value), z_(value), w_(1.0f) {} // xyzメンバーにvalueを入れて、wに1を入れる
        Quaternion(const float& x, const float& y, const float& z, const float& w) : x_(x), y_(y), z_(z), w_(w) {} // xyzwメンバーにvalueを入れる
        
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
        Quaternion operator+(const Quaternion& value) const;

        //--------------------------------------------------------------------------------
        //  足し算
        //  value : 相手
        //--------------------------------------------------------------------------------
        void operator+=(const Quaternion& value);

        //--------------------------------------------------------------------------------
        //  引き算
        //  value : 相手
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion operator-(const Quaternion& value) const;

        //--------------------------------------------------------------------------------
        //  引き算
        //  value : 相手
        //--------------------------------------------------------------------------------
        void operator-=(const Quaternion& value);

        //--------------------------------------------------------------------------------
        //  乗算(float)
        //  value : 相手
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion operator*(const float& value) const;

        //--------------------------------------------------------------------------------
        //  乗算(float)
        //  value : 相手
        //--------------------------------------------------------------------------------
        void operator*=(const float& value);

        //--------------------------------------------------------------------------------
        //  除算(float)
        //  value : 相手
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion operator/(const float& value) const;

        //--------------------------------------------------------------------------------
        //  除算(float)
        //  value : 相手
        //--------------------------------------------------------------------------------
        void operator/=(const float& value);

        //--------------------------------------------------------------------------------
        //  乗算(Quaternion)
        //  value : 相手
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion operator*(const Quaternion& value) const;
        
        //--------------------------------------------------------------------------------
        //  乗算(Quaternion)
        //  value : 相手
        //--------------------------------------------------------------------------------
        void operator*=(const Quaternion& value);

        //--------------------------------------------------------------------------------
        //  Dot乗算
        //  value : 相手
        //  return：float
        //--------------------------------------------------------------------------------
        float Dot(const Quaternion& value) const;
        
        //--------------------------------------------------------------------------------
        //  長さの平方の算出
        //  return：float
        //--------------------------------------------------------------------------------
        float SquareMagnitude(void) const;

        //--------------------------------------------------------------------------------
        //  長さの算出
        //  return：float
        //--------------------------------------------------------------------------------
        float Magnitude(void) const;

        //--------------------------------------------------------------------------------
        //  return the normalized quaternion / 正規化したの値を返す
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion Normalized(void) const;

        //--------------------------------------------------------------------------------
        //  normalize quaternion / 正規化
        //--------------------------------------------------------------------------------
        void Normalize(void);

        //--------------------------------------------------------------------------------
        //  return the conjugate quaternion
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion Conjugate(void) const;
        
        //--------------------------------------------------------------------------------
        //  mult each component with value別々で乗算
        //  value：相手
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        Quaternion MultiplySeparately(const Quaternion& value) const;
        
        //--------------------------------------------------------------------------------
        //  euler角に変換
        //  return：Vector3
        //--------------------------------------------------------------------------------
        Vector3 ToEuler(void) const;
        
        //--------------------------------------------------------------------------------
        //  行列に変換
        //  return：Matrix44
        //--------------------------------------------------------------------------------
        Matrix44 ToMatrix(void) const;

        //--------------------------------------------------------------------------------
        //  create a quaternion rotate by axis
        //  return：quaternion
        //--------------------------------------------------------------------------------
        static Quaternion RotateAxis(const Vector3& axis, const float& radian);

        //--------------------------------------------------------------------------------
        //  create a quaternion rotate by axis
        //  return：quaternion
        //--------------------------------------------------------------------------------
        static Quaternion FromToRotation(const Vector3& from, const Vector3& to, const float& weight = 1.0f);
    };

    //--------------------------------------------------------------------------------
    //  色
    //--------------------------------------------------------------------------------
    class Color
    {
    public:
        //--------------------------------------------------------------------------------
        //  コンストラクタとデストラクタ
        //--------------------------------------------------------------------------------
        Color() : r_(0.0f), g_(0.0f), b_(0.0f), a_(0.0f) {}
        ~Color() {}
        Color(const float& value) : r_(value), g_(value), b_(value), a_(value) {} // rgbaにvalueを入れる
        Color(const float& r, const float& g, const float& b, const float& a) : r_(r), g_(g), b_(b), a_(a) {} // rgbaにvalueを入れる
        
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
        Color& operator=(const Color& value);
        
        //--------------------------------------------------------------------------------
        //  return true if the colors' rgba are the same / 同値判定処理
        //  value : 相手
        //  return：bool
        //--------------------------------------------------------------------------------
        bool operator==(const Color& value);

        //--------------------------------------------------------------------------------
        //  add the rgba / 足し算処理
        //  value : 値
        //  return：Color
        //--------------------------------------------------------------------------------
        Color operator+(const Color& value) const;

        //--------------------------------------------------------------------------------
        //  add the rgba / 足し算処理
        //  value : 値
        //--------------------------------------------------------------------------------
        void operator+=(const Color& value);

        //--------------------------------------------------------------------------------
        //  minus the rgba / 引き算処理
        //  value : 値
        //  return：Color
        //--------------------------------------------------------------------------------
        Color operator-(const Color& value) const;
        
        //--------------------------------------------------------------------------------
        //  minus the rgba / 引き算処理
        //  value : 値
        //--------------------------------------------------------------------------------
        void operator-=(const Color& value);

        //--------------------------------------------------------------------------------
        //  mult the rgba / 掛け算処理
        //  value : 値
        //  return：Color
        //--------------------------------------------------------------------------------
        Color operator*(const float& value) const;

        //--------------------------------------------------------------------------------
        //  mult the rgba / 掛け算処理
        //  value : 値
        //--------------------------------------------------------------------------------
        void operator*=(const float& value);
    };

    //--------------------------------------------------------------------------------
    //  レイ
    //--------------------------------------------------------------------------------
    class Ray
    {
    public:
        //--------------------------------------------------------------------------------
        //  コンストラクタとデストラクタ
        //--------------------------------------------------------------------------------
        Ray() : origin_(Vector3::kZero) , direction_(Vector3::kZero) {}
        Ray(const Vector3& origin, const Vector3& direction) : origin_(origin), direction_(direction) {}
        ~Ray() {}

        Vector3 origin_; // レイの始点
        Vector3 direction_; // レイの方向

        //--------------------------------------------------------------------------------
        //  レイの変換
        //  transform : 変換行列
        //--------------------------------------------------------------------------------
        void Transform(const Matrix44& transform);
    };

    //--------------------------------------------------------------------------------
    //  ランダム
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
    //  計算式
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
        static Vector3 Lerp(const Vector3& from, const Vector3& to, const float& time);

        //--------------------------------------------------------------------------------
        //  floatを線形補間方式で補間する
        //  from：始点
        //  to：終点
        //  time：時間(0 - 1)
        //  return：float
        //--------------------------------------------------------------------------------
        static float Lerp(const float& from, const float& to, const float& time);

        //--------------------------------------------------------------------------------
        //  Colorを線形補間方式で補間する
        //  from：始点
        //  to：終点
        //  time：時間(0 - 1)
        //  return：Color
        //--------------------------------------------------------------------------------
        static Color Lerp(const Color& from, const Color& to, const float& time);

        //--------------------------------------------------------------------------------
        //  Vector3を球形補間方式で補間する
        //  from：始点
        //  to：終点
        //  time：時間(0 - 1)
        //  return：Vector3
        //--------------------------------------------------------------------------------
        static Vector3 Slerp(const Vector3& from, const Vector3& to, const float& time);

        //--------------------------------------------------------------------------------
        //  Quaternionを球形補間方式で補間する
        //  from：始点
        //  to：終点
        //  time：時間(0 - 1)
        //  return：Quaternion
        //--------------------------------------------------------------------------------
        static Quaternion Slerp(const Quaternion& from, const Quaternion& to, const float& time);

        //--------------------------------------------------------------------------------
        //  floatをminとmaxの間にする
        //  value：現在値
        //  min：最小値
        //  max：最大値
        //  return：float
        //--------------------------------------------------------------------------------
        static float Clamp(const float& value, const float& min, const float& max);

        //--------------------------------------------------------------------------------
        //  Vector3をminとmaxの間にする
        //  value：現在値
        //  min：最小値
        //  max：最大値
        //  return：Vector3
        //--------------------------------------------------------------------------------
        static Vector3 Clamp(const Vector3& value, const Vector3& min, const Vector3& max);

        //--------------------------------------------------------------------------------
        //  return the abs's max value / 絶対値が大きい方を返す
        //    value_l、value_r：比較値
        //    return：float
        //--------------------------------------------------------------------------------
        static float AbsMax(const float& value_l, const float& value_r);

        //--------------------------------------------------------------------------------
        //  swap the value of left and right / 左値と右値を交換する
        //--------------------------------------------------------------------------------
        static void Swap(float& left, float& right);
    };

    // インライン関数
    #include "kf_math.inl"
} // namespace KodFreedom
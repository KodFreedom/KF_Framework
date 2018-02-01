//　kf_math.inl
//  classes and methods for math
//  演算用のクラスとメソッド
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//
//  Vector2
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  代入処理
//--------------------------------------------------------------------------------
inline Vector2& Vector2::operator=(const Vector2& value)
{
    x_ = value.x_;
    y_ = value.y_;
    return *this;
}

//--------------------------------------------------------------------------------
//  同値判定処理
//--------------------------------------------------------------------------------
inline bool Vector2::operator==(const Vector2& value) const
{
    return (x_ == value.x_ && y_ == value.y_);
}

//--------------------------------------------------------------------------------
//  足し算
//--------------------------------------------------------------------------------
inline Vector2 Vector2::operator+(const Vector2& value) const
{
    return Vector2(x_ + value.x_, y_ + value.y_);
}

//--------------------------------------------------------------------------------
//  自分に足す
//--------------------------------------------------------------------------------
inline void Vector2::operator+=(const Vector2& value)
{
    x_ += value.x_;
    y_ += value.y_;
}

//--------------------------------------------------------------------------------
//  引き算
//--------------------------------------------------------------------------------
inline Vector2 Vector2::operator-(const Vector2& value) const
{
    return Vector2(x_ - value.x_, y_ - value.y_);
}

//--------------------------------------------------------------------------------
//  自分に引く
//--------------------------------------------------------------------------------
inline void Vector2::operator-=(const Vector2& value)
{
    x_ -= value.x_;
    y_ -= value.y_;
}

//--------------------------------------------------------------------------------
//  スケール
//--------------------------------------------------------------------------------
inline Vector2 Vector2::operator*(const float& value) const
{
    return Vector2(x_ * value, y_ * value);
}

//--------------------------------------------------------------------------------
//  スケール
//--------------------------------------------------------------------------------
inline void Vector2::operator*=(const float& value)
{
    x_ *= value;
    y_ *= value;
}

//--------------------------------------------------------------------------------
//  外積
//--------------------------------------------------------------------------------
inline float Vector2::operator*(const Vector2& value) const
{
    return (x_ * value.y_ - y_ * value.x_);
}

//--------------------------------------------------------------------------------
//  内積
//--------------------------------------------------------------------------------
inline float Vector2::Dot(const Vector2& value) const
{
    return (x_ * value.x_ + y_ * value.y_);
}

//--------------------------------------------------------------------------------
//  長さの計算
//--------------------------------------------------------------------------------
inline float Vector2::Magnitude(void) const
{
    return sqrtf(x_ * x_ + y_ * y_);
}

//--------------------------------------------------------------------------------
//  長さ平方の計算
//--------------------------------------------------------------------------------
inline float Vector2::SquareMagnitude(void) const
{
    return (x_ * x_ + y_ * y_);
}

//--------------------------------------------------------------------------------
//  正規化したの値を返す
//--------------------------------------------------------------------------------
inline Vector2 Vector2::Normalized(void) const
{
    float magnitude = Magnitude();
    if (magnitude <= 0.0f) return Vector2::kZero;
    return Vector2(x_ / magnitude, y_ / magnitude);
}

//--------------------------------------------------------------------------------
//  正規化
//--------------------------------------------------------------------------------
inline void Vector2::Normalize(void)
{
    *this = this->Normalized();
}

//--------------------------------------------------------------------------------
//  xy間の角度の算出
//--------------------------------------------------------------------------------
inline float Vector2::ToRadian(void) const
{
    return atan2f(y_, x_);
}

//--------------------------------------------------------------------------------
//  Vector2間の角度を返す
//--------------------------------------------------------------------------------
inline float Vector2::RadianBetween(const Vector2& from, const Vector2& to)
{
    float square_magnitude_from = from.SquareMagnitude();
    float square_magnitude_to = to.SquareMagnitude();
    if (square_magnitude_from <= kDotMin || square_magnitude_to <= kDotMin) return 0.0f;
    float dot = (from * (1.0f / sqrtf(square_magnitude_from))).Dot(to * (1.0f / sqrtf(square_magnitude_to)));
    dot = Math::Clamp(dot, -1.0f, 1.0f);
    return acosf(dot);
}

//--------------------------------------------------------------------------------
//
//  Vector3
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  xyzに与えられたVector4のxyz値を入れるを入れる
//--------------------------------------------------------------------------------
inline Vector3::Vector3(const Vector4& value) : x_(value.x_), y_(value.y_), z_(value.z_) {};

//--------------------------------------------------------------------------------
//  Vector2にキャストする
//--------------------------------------------------------------------------------
inline Vector3::operator Vector2() const
{
    return Vector2(x_, y_);
}

//--------------------------------------------------------------------------------
//  代入処理
//--------------------------------------------------------------------------------
inline Vector3& Vector3::operator=(const Vector3& value)
{
    x_ = value.x_;
    y_ = value.y_;
    z_ = value.z_;
    return *this;
}

//--------------------------------------------------------------------------------
//  同値判定処理
//--------------------------------------------------------------------------------
inline bool Vector3::operator==(const Vector3& value) const
{
    return (x_ == value.x_ && y_ == value.y_ && z_ == value.z_);
}

//--------------------------------------------------------------------------------
//  異なる値判定処理
//--------------------------------------------------------------------------------
inline bool Vector3::operator!=(const Vector3& value) const
{
    return (x_ != value.x_ && y_ != value.y_ && z_ != value.z_);
}

//--------------------------------------------------------------------------------
//  足し算
//--------------------------------------------------------------------------------
inline Vector3 Vector3::operator+(const Vector3& value) const
{
    return Vector3(x_ + value.x_, y_ + value.y_, z_ + value.z_);
}

//--------------------------------------------------------------------------------
//  自分に足す
//--------------------------------------------------------------------------------
inline void Vector3::operator+=(const Vector3& value)
{
    x_ += value.x_;
    y_ += value.y_;
    z_ += value.z_;
}

//--------------------------------------------------------------------------------
//  引き算
//--------------------------------------------------------------------------------
inline Vector3 Vector3::operator-(const Vector3& value) const
{
    return Vector3(x_ - value.x_, y_ - value.y_, z_ - value.z_);
}

//--------------------------------------------------------------------------------
//  自分に引く
//--------------------------------------------------------------------------------
inline void Vector3::operator-=(const Vector3& value)
{
    x_ -= value.x_;
    y_ -= value.y_;
    z_ -= value.z_;
}

//--------------------------------------------------------------------------------
//  スケール
//--------------------------------------------------------------------------------
inline Vector3 Vector3::operator*(const float& value) const
{
    return Vector3(x_ * value, y_ * value, z_ * value);
}

//--------------------------------------------------------------------------------
//  スケール
//--------------------------------------------------------------------------------
inline void Vector3::operator*=(const float& value)
{
    x_ *= value;
    y_ *= value;
    z_ *= value;
}

//--------------------------------------------------------------------------------
//  外積
//--------------------------------------------------------------------------------
inline Vector3 Vector3::operator*(const Vector3& value) const
{
    return Vector3(y_ * value.z_ - value.y_ * z_, z_ * value.x_ - value.z_ * x_, x_ * value.y_ - value.x_ * y_);
}

//--------------------------------------------------------------------------------
//  外積
//--------------------------------------------------------------------------------
inline void Vector3::operator*=(const Vector3& value)
{
    *this = *this * value;
}

//--------------------------------------------------------------------------------
//  スケール
//--------------------------------------------------------------------------------
inline Vector3 Vector3::operator/(const float& value) const
{
    assert(value != 0.0f);
    return Vector3(x_ / value, y_ / value, z_ / value);
}

//--------------------------------------------------------------------------------
//  スケール
//--------------------------------------------------------------------------------
inline void Vector3::operator/=(const float& value)
{
    assert(value != 0.0f);
    x_ /= value;
    y_ /= value;
    z_ /= value;
}

//--------------------------------------------------------------------------------
//  内積
//--------------------------------------------------------------------------------
inline float Vector3::Dot(const Vector3& value) const
{
    return (x_ * value.x_ + y_ * value.y_ + z_ * value.z_);
}

//--------------------------------------------------------------------------------
//  長さの計算
//--------------------------------------------------------------------------------
inline float Vector3::Magnitude(void) const
{
    return sqrtf(x_ * x_ + y_ * y_ + z_ * z_);
}

//--------------------------------------------------------------------------------
//  長さ平方の計算
//--------------------------------------------------------------------------------
inline float Vector3::SquareMagnitude(void) const
{
    return (x_ * x_ + y_ * y_ + z_ * z_);
}

//--------------------------------------------------------------------------------
//  正規化したの値を返す
//--------------------------------------------------------------------------------
inline Vector3 Vector3::Normalized(void) const
{
    float magnitude = this->Magnitude();
    if (magnitude <= 0.0f) return Vector3::kZero;
    return *this / magnitude;
}

//--------------------------------------------------------------------------------
//  正規化
//--------------------------------------------------------------------------------
inline void Vector3::Normalize(void)
{
    *this = this->Normalized();
}

//--------------------------------------------------------------------------------
//  Euler角をQuaternionに変換
//--------------------------------------------------------------------------------
inline Quaternion Vector3::ToQuaternion(void) const
{
    // TODO: 計算式を調べる
    Quaternion result;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    D3DXQuaternionRotationYawPitchRoll((D3DXQUATERNION*)&result, y_, x_, z_);
#endif
    return result;
}

//--------------------------------------------------------------------------------
//  両点の間の距離の算出
//--------------------------------------------------------------------------------
inline float Vector3::DistanceBetween(const Vector3& from, const Vector3& to)
{
    return (from - to).Magnitude();
}

//--------------------------------------------------------------------------------
//  両点の間の距離の平方を返す
//--------------------------------------------------------------------------------
inline float Vector3::SquareDistanceBetween(const Vector3& from, const Vector3& to)
{
    return (from - to).SquareMagnitude();
}

//--------------------------------------------------------------------------------
//  ポイントを回転する
//--------------------------------------------------------------------------------
inline Vector3 Vector3::TransformCoord(const Vector3& point, const Matrix44& transform)
{
    Vector4& work = Vector4(point, 1.0f);
    work *= transform;
    if (work.w_ == 0.0f) return Vector3::kZero;
    return Vector3(work.x_ / work.w_, work.y_ / work.w_, work.z_ / work.w_);
}

//--------------------------------------------------------------------------------
//  法線ベクトルを回転する
//--------------------------------------------------------------------------------
inline Vector3 Vector3::TransformNormal(const Vector3& normal, const Matrix44& transform)
{
    Vector4& work = Vector4(normal, 0.0f);
    work *= transform;
    return Vector3(work.x_, work.y_, work.z_);
}

//--------------------------------------------------------------------------------
//  ポイントを逆行列により変換する
//--------------------------------------------------------------------------------
inline Vector3 Vector3::TransformInverse(const Vector3& point, const Matrix44& transform)
{
    Vector3 work = point;
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
//  XYZ別々でスケールする
//--------------------------------------------------------------------------------
inline Vector3 Vector3::Scale(const Vector3& value, const Vector3& scale)
{
    return Vector3(value.x_ * scale.x_, value.y_ * scale.y_, value.z_ * scale.z_);
}

//--------------------------------------------------------------------------------
//  ベクトル間のradian角の算出
//--------------------------------------------------------------------------------
inline float Vector3::RadianBetween(const Vector3& from, const Vector3& to)
{
    float result = acosf(Math::Clamp(from.Dot(to), -1.0f, 1.0f));
    assert(isnan(result));
    return result;
}

//--------------------------------------------------------------------------------
//  ベクトル間のeuler角の算出
//--------------------------------------------------------------------------------
inline Vector3 Vector3::EulerBetween(const Vector3& from, const Vector3& to)
{
    Vector3 euler;
    euler.x_ = Vector2::RadianBetween(Vector2(from.y_, from.z_), Vector2(to.y_, to.z_));
    euler.y_ = Vector2::RadianBetween(Vector2(from.z_, from.x_), Vector2(to.z_, to.x_));
    euler.z_ = Vector2::RadianBetween(Vector2(from.x_, from.y_), Vector2(to.x_, to.y_));
    return euler;
}

//--------------------------------------------------------------------------------
//  方向を平面と平行の方向に回転する
//--------------------------------------------------------------------------------
inline Vector3 Vector3::ProjectOnPlane(const Vector3& direction, const Vector3& plane_normal, const Vector3& current_normal)
{
    return (current_normal * direction) * plane_normal;
}

//--------------------------------------------------------------------------------
//  ベクトルを回転する
//--------------------------------------------------------------------------------
inline Vector3 Vector3::Rotate(const Vector3& direction, const Quaternion& rotation)
{
    Quaternion& direction_quaternion = Quaternion(direction.x_, direction.y_, direction.z_, 0.0f);
    Quaternion& result = rotation.Conjugate() * direction_quaternion;
    result *= rotation;
    return Vector3(result.x_, result.y_, result.z_);
}

//--------------------------------------------------------------------------------
//  回転軸と回転角度でオラー角に変換する
//--------------------------------------------------------------------------------
inline Vector3 Vector3::AxisRadianToEuler(const Vector3& axis, const float& radian)
{
    Vector3 euler;
    float sin = sinf(radian);
    float cos = cosf(radian);

    // heading y
    // bank x
    // attr z

    if ((axis.x_ * axis.y_ * (1.0f - cos) + axis.z_ * sin) > 0.998f)
    { // north pole singularity detected
        euler.y_ = 2.0f * atan2f(axis.x_ * sinf(radian * 0.5f), cosf(radian * 0.5f));
        euler.z_ = kPi * 0.5f;
        euler.x_ = 0.0f;
        return euler;
    }

    if ((axis.x_ * axis.y_ * (1.0f - cos) + axis.z_ * sin) < -0.998f)
    { // south pole singularity detected
        euler.y_ = -2.0f * atan2f(axis.x_ * sinf(radian * 0.5f), cosf(radian * 0.5f));
        euler.z_ = -kPi * 0.5f;
        euler.x_ = 0.0f;
        return euler;
    }

    euler.y_ = atan2f(axis.y_ * sin - axis.x_ * axis.z_ * (1.0f - cos), 1 - (axis.y_ * axis.y_ + axis.z_ * axis.z_) * (1.0f - cos));
    euler.z_ = asinf(axis.x_ * axis.y_ * (1.0f - cos) + axis.z_ * sin);
    euler.x_ = atan2f(axis.x_ * sin - axis.y_ * axis.z_ * (1.0f - cos), 1 - (axis.x_ * axis.x_ + axis.z_ * axis.z_) * (1.0f - cos));

    return euler;
}

//--------------------------------------------------------------------------------
//
//  Vector4
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  マトリクスとの乗算
//--------------------------------------------------------------------------------
inline Vector4 Vector4::operator*(const Matrix44& matrix) const
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
inline void Vector4::operator*=(const Matrix44& matrix)
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
//  normalize the vector4 as plane / 平面正規化
//--------------------------------------------------------------------------------
inline void Vector4::PlaneNormalize(void)
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
inline float Vector4::PlaneDotCoord(const Vector3& point) const
{
    return this->x_ * point.x_ + this->y_ * point.y_ + this->z_ * point.z_ + this->w_;
}

//--------------------------------------------------------------------------------
//
//  Matrix44
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  代入処理
//--------------------------------------------------------------------------------
inline Matrix44& Matrix44::operator=(const Matrix44& value)
{
    memcpy_s(this, sizeof(*this), &value, sizeof(value));
    return *this;
}

//--------------------------------------------------------------------------------
//  floatと乗算処理
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::operator*(const float& value) const
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
//  加算処理
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::operator+(const Matrix44& value) const
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
//  乗算処理
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::operator*(const Matrix44& value) const
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
//  乗算処理
//--------------------------------------------------------------------------------
inline void Matrix44::operator*=(const Matrix44& value)
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
//  オーラー角に変換
//--------------------------------------------------------------------------------
inline Vector3 Matrix44::ToEuler(void) const
{
    return this->ToQuaternion().ToEuler();
}

//--------------------------------------------------------------------------------
//  Quaternionに変換
//--------------------------------------------------------------------------------
inline Quaternion Matrix44::ToQuaternion(void) const
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
//  transpose行列に変換して返す
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::Transpose(void) const
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
//  行列のスケール要素を無くす
//--------------------------------------------------------------------------------
inline void Matrix44::RemoveScale(void)
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
//  inverse行列に変換して返す
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::Inverse(void) const
{
    Matrix44 result;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
    D3DXMatrixInverse((D3DXMATRIX*)&result, NULL, (D3DXMATRIX*)this);
#endif
    return result;
}

//--------------------------------------------------------------------------------
//  与えられた値でスケール行列の作成
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::Scale(const Vector3& scale)
{
    return Matrix44(scale.x_, 0.0f    , 0.0f    , 0.0f,
                    0.0f    , scale.y_, 0.0f    , 0.0f,
                    0.0f    , 0.0f    , scale.z_, 0.0f,
                    0.0f    , 0.0f    , 0.0f    , 1.0f);
}

//--------------------------------------------------------------------------------
//  与えられた回転で行列の作成
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::Rotation(const Vector3& right, const Vector3& up, const Vector3& forward)
{
    return Matrix44(right.x_  , right.y_  , right.z_  , 0.0f,
                    up.x_     , up.y_     , up.z_     , 0.0f,
                    forward.x_, forward.y_, forward.z_, 0.0f,
                    0.0f      , 0.0f      , 0.0f      , 1.0f);
}

//--------------------------------------------------------------------------------
//  与えられた軸と角で回転行列の作成
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::RotationAxis(const Vector3& axis, const float& radian)
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
//  与えられたeuler角で回転行列の作成
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::RotationYawPitchRoll(const Vector3& euler)
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
//  与えられた移動量で平行移動行列の作成
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::Translation(const Vector3& translation)
{
    Matrix44 result;
    result.m_[3][0] = translation.x_;
    result.m_[3][1] = translation.y_;
    result.m_[3][2] = translation.z_;
    return result;
}

//--------------------------------------------------------------------------------
//  与えられた回転と移動量で行列の作成
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::Transform(const Vector3& right, const Vector3& up, const Vector3& forward, const Vector3& translation, const Vector3& scale)
{
    return Matrix44(
        right.x_   * scale.x_, right.y_   * scale.x_, right.z_   * scale.x_, 0.0f,
        up.x_      * scale.y_, up.y_      * scale.y_, up.z_      * scale.y_, 0.0f,
        forward.x_ * scale.z_, forward.y_ * scale.z_, forward.z_ * scale.z_, 0.0f,
        translation.x_       , translation.y_       , translation.z_       , 1.0f);
}

//--------------------------------------------------------------------------------
//  与えられた回転、移動量、スケールで行列の作成
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::Transform(const Vector3& rotation, const Vector3& translation, const Vector3& scale)
{
    Matrix44& result = Scale(scale);
    result *= RotationYawPitchRoll(rotation);
    result *= Translation(translation);
    return result;
}

//--------------------------------------------------------------------------------
//  与えられた回転、移動量、スケールで行列の作成
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::Transform(const Quaternion& rotation, const Vector3& translation, const Vector3& scale)
{
    Matrix44& result = Scale(scale);
    result *= rotation.ToMatrix();
    result *= Translation(translation);
    return result;
}

//--------------------------------------------------------------------------------
//  パースペクティブ行列の作成(左手座標系)
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::PerspectiveLeftHand(const float& fov_radian_y, const float& aspect_ratio, const float& near_z, const float& far_z)
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
//  オルソ行列の作成(左手座標系)
//--------------------------------------------------------------------------------
inline Matrix44 Matrix44::OrthographicLeftHand(const float& left, const float& right, const float& top, const float& bottom, const float& near_z, const float& far_z)
{
    Matrix44 result;
    result.m_[0][0] = 2.0f / (right - left);
    result.m_[1][1] = 2.0f / (top - bottom);
    result.m_[2][2] = 1.0f / (far_z - near_z);
    result.m_[3][0] = -(right + left) / (right - left);
    result.m_[3][1] = -(top + bottom) / (top - bottom);
    result.m_[3][2] = -near_z / (far_z - near_z);
    return result;
}

//--------------------------------------------------------------------------------
//
//  Quaternion
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  足し算
//--------------------------------------------------------------------------------
inline Quaternion Quaternion::operator+(const Quaternion& value) const
{
    return Quaternion(x_ + value.x_, y_ + value.y_, z_ + value.z_, w_ + value.w_);
}

//--------------------------------------------------------------------------------
//  足し算
//--------------------------------------------------------------------------------
inline void Quaternion::operator+=(const Quaternion& value)
{
    x_ += value.x_;
    y_ += value.y_;
    z_ += value.z_;
    w_ += value.w_;
}

//--------------------------------------------------------------------------------
//  引き算
//--------------------------------------------------------------------------------
inline Quaternion Quaternion::operator-(const Quaternion& value) const
{
    return Quaternion(x_ - value.x_, y_ - value.y_, z_ - value.z_, w_ - value.w_);
}

//--------------------------------------------------------------------------------
//  引き算
//--------------------------------------------------------------------------------
inline void Quaternion::operator-=(const Quaternion& value)
{
    x_ -= value.x_;
    y_ -= value.y_;
    z_ -= value.z_;
    w_ -= value.w_;
}

//--------------------------------------------------------------------------------
//  乗算(float)
//--------------------------------------------------------------------------------
inline Quaternion Quaternion::operator*(const float& value) const
{
    return Quaternion(x_ * value, y_ * value, z_ * value, w_ * value);
}

//--------------------------------------------------------------------------------
//  乗算(float)
//--------------------------------------------------------------------------------
inline void Quaternion::operator*=(const float& value)
{
    x_ *= value;
    y_ *= value;
    z_ *= value;
    w_ *= value;
}

//--------------------------------------------------------------------------------
//  除算(float)
//--------------------------------------------------------------------------------
inline Quaternion Quaternion::operator/(const float& value) const
{
    assert(value != 0.0f);
    return Quaternion(x_ / value, y_ / value, z_ / value, w_ / value);
}

//--------------------------------------------------------------------------------
//  除算(float)
//--------------------------------------------------------------------------------
inline void Quaternion::operator/=(const float& value)
{
    x_ /= value;
    y_ /= value;
    z_ /= value;
    w_ /= value;
}

//--------------------------------------------------------------------------------
//  乗算(Quaternion)
//--------------------------------------------------------------------------------
inline Quaternion Quaternion::operator*(const Quaternion& value) const
{
    return Quaternion((value.w_ * this->x_) + (value.x_ * this->w_) + (value.y_ * this->z_) - (value.z_ * this->y_),
                      (value.w_ * this->y_) - (value.x_ * this->z_) + (value.y_ * this->w_) + (value.z_ * this->x_),
                      (value.w_ * this->z_) + (value.x_ * this->y_) - (value.y_ * this->x_) + (value.z_ * this->w_),
                      (value.w_ * this->w_) - (value.x_ * this->x_) - (value.y_ * this->y_) - (value.z_ * this->z_));
}

//--------------------------------------------------------------------------------
//  乗算(Quaternion)
//--------------------------------------------------------------------------------
inline void Quaternion::operator*=(const Quaternion& value)
{
    Quaternion copy = *this;
    this->x_ = (value.w_ * copy.x_) + (value.x_ * copy.w_) + (value.y_ * copy.z_) - (value.z_ * copy.y_);
    this->y_ = (value.w_ * copy.y_) - (value.x_ * copy.z_) + (value.y_ * copy.w_) + (value.z_ * copy.x_);
    this->z_ = (value.w_ * copy.z_) + (value.x_ * copy.y_) - (value.y_ * copy.x_) + (value.z_ * copy.w_);
    this->w_ = (value.w_ * copy.w_) - (value.x_ * copy.x_) - (value.y_ * copy.y_) - (value.z_ * copy.z_);
}

//--------------------------------------------------------------------------------
//  Dot乗算
//--------------------------------------------------------------------------------
inline float Quaternion::Dot(const Quaternion& value) const
{
    return (x_ * value.x_ + y_ * value.y_ + z_ * value.z_ + w_ * value.w_);
}

//--------------------------------------------------------------------------------
//  長さの平方の算出
//--------------------------------------------------------------------------------
inline float Quaternion::SquareMagnitude(void) const
{
    return this->Dot(*this);
}

//--------------------------------------------------------------------------------
//  長さの算出
//--------------------------------------------------------------------------------
inline float Quaternion::Magnitude(void) const
{
    return sqrtf(this->SquareMagnitude());
}

//--------------------------------------------------------------------------------
//  正規化したの値を返す
//--------------------------------------------------------------------------------
inline Quaternion Quaternion::Normalized(void) const
{
    float magnitude = this->Magnitude();
    if (magnitude <= 0.0f) return Quaternion::kIdentity;
    return *this / magnitude;
}

//--------------------------------------------------------------------------------
//  正規化
//--------------------------------------------------------------------------------
inline void Quaternion::Normalize(void)
{
    *this = this->Normalized();
}

//--------------------------------------------------------------------------------
//  return the conjugate quaternion
//--------------------------------------------------------------------------------
inline Quaternion Quaternion::Conjugate(void) const
{
    return Quaternion(-x_, -y_, -z_, w_);
}

//--------------------------------------------------------------------------------
// value別々で乗算
//--------------------------------------------------------------------------------
inline Quaternion Quaternion::MultiplySeparately(const Quaternion& value) const
{
    return Quaternion(x_ * value.x_, y_ * value.y_, z_ * value.z_, w_ * value.w_);
}

//--------------------------------------------------------------------------------
//    euler角に変換
//--------------------------------------------------------------------------------
inline Vector3 Quaternion::ToEuler(void) const
{
    Vector3 result;
    float suare_x = x_ * x_;
    float suare_y = y_ * y_;
    float suare_z = z_ * z_;
    float suare_w = w_ * w_;
    float unit = suare_x + suare_y + suare_z + suare_w;

    float test = x_ * y_ + z_ * w_;
    if (test > 0.499f * unit)
    { // singularity at north pole
        result.y_ = 2.0f * atan2f(x_, w_);
        result.x_ = kPi * 0.5f;
        result.z_ = 0.0f;
        return result;
    }
    if (test < -0.499f * unit)
    { // singularity at south pole
        result.y_ = -2.0f * atan2f(x_, w_);
        result.x_ = -kPi * 0.5f;
        result.z_ = 0.0f;
        return result;
    }

    result.y_ = atan2f(2.0f * y_ * w_ - 2.0f * x_ * z_, suare_x - suare_y - suare_z + suare_w);
    result.x_ = asinf(2.0f * test / unit);
    result.z_ = atan2f(2.0f * x_ * w_ - 2.0f * y_ * z_, -suare_x + suare_y - suare_z + suare_w);

    //Vector3 vtest;
    //// roll (x-axis rotation)
    //float sin_x = 2.0f * (w_ * x_ + y_ * z_);
    //float cos_x = 1.0f - 2.0f * (x_ * x_ + y_ * y_);
    //vtest.x_ = atan2f(sin_x, cos_x);
    //
    //// pitch (y-axis rotation)
    //float sin_y = 2.0f * (w_ * y_ - z_ * x_);
    //if (fabsf(sin_y) >= 1.0f)
    //{
    //    vtest.y_ = copysignf(kPi * 0.5f, sin_y); // use 90 degrees if out of range
    //}
    //else
    //{
    //    vtest.y_ = asinf(sin_y);
    //}
    //
    //// yaw (z-axis rotation)
    //float sin_z_ = 2.0f * (w_ * z_ + x_ * y_);
    //float cos_z_ = 1.0f - 2.0f * (y_ * y_ + z_ * z_);
    //vtest.z_ = atan2f(sin_z_, cos_z_);

    return result;
}

//--------------------------------------------------------------------------------
//    行列に変換
//--------------------------------------------------------------------------------
inline Matrix44 Quaternion::ToMatrix(void) const
{
    Matrix44 result;
    static const Quaternion& quaternion1110 = Quaternion(1.0f, 1.0f, 1.0f, 0.0f);
    Quaternion& this_x2 = *this + *this;
    Quaternion& this_x_this_x2 = this->MultiplySeparately(this_x2);
    Quaternion& work0 = quaternion1110
        - Quaternion(this_x_this_x2.y_, this_x_this_x2.x_, this_x_this_x2.x_, quaternion1110.w_)
        - Quaternion(this_x_this_x2.z_, this_x_this_x2.z_, this_x_this_x2.y_, quaternion1110.w_);
    Quaternion& work1 = Quaternion(x_, x_, y_, w_).MultiplySeparately(Quaternion(this_x2.z_, this_x2.y_, this_x2.z_, this_x2.w_));
    Quaternion& work2 = Quaternion(w_, w_, w_, w_).MultiplySeparately(Quaternion(this_x2.y_, this_x2.z_, this_x2.x_, this_x2.w_));
    Quaternion& one_plus_two = work1 + work2;
    Quaternion& one_minus_two = work1 - work2;
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
//  create a quaternion rotate by axis
//--------------------------------------------------------------------------------
inline Quaternion Quaternion::RotateAxis(const Vector3& axis, const float& radian)
{
    Quaternion& axis_quaternion = Quaternion(axis.x_, axis.y_, axis.z_, 1.0f);
    float sin = sinf(radian * 0.5f);
    float cos = cosf(radian * 0.5f);
    Quaternion& scale = Quaternion(sin, sin, sin, cos);
    return axis_quaternion.MultiplySeparately(scale);
}

//--------------------------------------------------------------------------------
//  create a quaternion rotate by axis
//--------------------------------------------------------------------------------
inline Quaternion Quaternion::FromToRotation(const Vector3& from, const Vector3& to, const float& weight)
{
    float radian = acosf(from.Dot(to)) * weight;
    Vector3& axis = (from * to).Normalized();
    return Quaternion::RotateAxis(axis, radian);
}

//--------------------------------------------------------------------------------
//
//  Color
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  代入処理
//--------------------------------------------------------------------------------
inline Color& Color::operator=(const Color& value)
{
    r_ = value.r_;
    g_ = value.g_;
    b_ = value.b_;
    a_ = value.a_;
    return *this;
}

//--------------------------------------------------------------------------------
//  同値判定処理
//--------------------------------------------------------------------------------
inline bool Color::operator==(const Color& value)
{
    return (r_ == value.r_ && g_ == value.g_ && b_ == value.b_ && a_ == value.a_);
}

//--------------------------------------------------------------------------------
//  足し算処理
//--------------------------------------------------------------------------------
inline Color Color::operator+(const Color& value) const
{
    return Color(r_ + value.r_, g_ + value.g_, b_ + value.b_, a_ + value.a_);
}

//--------------------------------------------------------------------------------
//  足し算処理
//--------------------------------------------------------------------------------
inline void Color::operator+=(const Color& value)
{
    r_ += value.r_;
    g_ += value.g_;
    b_ += value.b_;
    a_ += value.a_;
}

//--------------------------------------------------------------------------------
//  引き算処理
//--------------------------------------------------------------------------------
inline Color Color::operator-(const Color& value) const
{
    return Color(r_ - value.r_, g_ - value.g_, b_ - value.b_, a_ - value.a_);
}

//--------------------------------------------------------------------------------
//  引き算処理
//--------------------------------------------------------------------------------
inline void Color::operator-=(const Color& value)
{
    r_ -= value.r_;
    g_ -= value.g_;
    b_ -= value.b_;
    a_ -= value.a_;
}

//--------------------------------------------------------------------------------
//  掛け算処理
//--------------------------------------------------------------------------------
inline Color Color::operator*(const float& value) const
{
    return Color(r_ * value, g_ * value, b_ * value, a_ * value);
}

//--------------------------------------------------------------------------------
//  掛け算処理
//--------------------------------------------------------------------------------
inline void Color::operator*=(const float& value)
{
    r_ *= value;
    g_ *= value;
    b_ *= value;
    a_ *= value;
}

//--------------------------------------------------------------------------------
//
//  Ray
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  レイの変換
//--------------------------------------------------------------------------------
inline void Ray::Transform(const Matrix44& transform)
{
    origin_ = Vector3::TransformCoord(origin_, transform);
    direction_ = Vector3::TransformNormal(direction_, transform);
}

//--------------------------------------------------------------------------------
//
//  Math
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Vector3を線形補間方式で補間する
//--------------------------------------------------------------------------------
inline Vector3 Math::Lerp(const Vector3& from, const Vector3& to, const float& time)
{
    if (time <= 0.0f) return from;
    if (time >= 1.0f) return to;
    return (from + (to - from) * time);
}

//--------------------------------------------------------------------------------
//  floatを線形補間方式で補間する
//--------------------------------------------------------------------------------
inline float Math::Lerp(const float& from, const float& to, const float& time)
{
    if (time <= 0.0f) return from;
    if (time >= 1.0f) return to;
    return (from + (to - from) * time);
}

//--------------------------------------------------------------------------------
//  Colorを線形補間方式で補間する
//--------------------------------------------------------------------------------
inline Color Math::Lerp(const Color& from, const Color& to, const float& time)
{
    if (time <= 0.0f) return from;
    if (time >= 1.0f) return to;
    return (from + (to - from) * time);
}

//--------------------------------------------------------------------------------
//  Vector3を球形補間方式で補間する
//--------------------------------------------------------------------------------
inline Vector3 Math::Slerp(const Vector3& from, const Vector3& to, const float& time)
{
    if (time <= 0.0f) return from;
    if (time >= 1.0f) return to;
    return (from + (to - from) * time).Normalized();
}

//--------------------------------------------------------------------------------
//  Quaternionを球形補間方式で補間する
//--------------------------------------------------------------------------------
inline Quaternion Math::Slerp(const Quaternion& from, const Quaternion& to, const float& time)
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
//--------------------------------------------------------------------------------
inline float Math::Clamp(const float& value, const float& min, const float& max)
{
    return value < min ? min : value > max ? max : value;
}

//--------------------------------------------------------------------------------
//  Vector3をminとmaxの間にする
//--------------------------------------------------------------------------------
inline Vector3 Math::Clamp(const Vector3& value, const Vector3& min, const Vector3& max)
{
    return Vector3(Clamp(value.x_, min.x_, max.x_),
                   Clamp(value.y_, min.y_, max.y_),
                   Clamp(value.z_, min.z_, max.z_));
}

//--------------------------------------------------------------------------------
//  絶対値が大きい方を返す
//--------------------------------------------------------------------------------
inline float Math::AbsMax(const float& value_l, const float& value_r)
{
    return fabsf(value_l) >= fabsf(value_r) ? value_l : value_r;
}

//--------------------------------------------------------------------------------
//  左値と右値を交換する
//--------------------------------------------------------------------------------
inline void Math::Swap(float& left, float& right)
{
    float work = left;
    left = right;
    right = work;
}

////--------------------------------------------------------------------------------
////    関数名：ToPickingRay
////  関数説明：スクリーン座標からレイの算出
////    引数：    viewportSize：viewportの副幅
////            projectMatrix00：射影行列の00番
////            projectMatrix11：射影行列の11番
////            viewInverse：view行列の逆行列
////    戻り値：Ray
////--------------------------------------------------------------------------------
//Ray Vector2::ToPickingRay(const Vector2& viewportSize, const float& projectMatrix00, const float& projectMatrix11, const Matrix44& viewInverse)
//{
//    auto& position3D = Vector2(
//        (((2.0f * X) / viewportSize.X) - 1.0f) / projectMatrix00,
//        (((-2.0f * Y) / viewportSize.Y) + 1.0f) / projectMatrix11);
//    Ray result;
//    result.Origin = Vector3::Zero;
//    result.Direction = Vector3(position3D.X, position3D.Y, 1.0f);
//    result.Transform(viewInverse);
//    return result;
//}


/*
//--------------------------------------------------------------------------------
//  Others
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  NormalizeRotInTwoPi(float)
//    回転を-2Piから2Piの間にする
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
//    回転を-2Piから2Piの間にする
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInTwoPi(Vector3& vRot)
{
NormalizeRotInTwoPi(vRot.X);
NormalizeRotInTwoPi(vRot.Y);
NormalizeRotInTwoPi(vRot.Z);
}

//--------------------------------------------------------------------------------
//  NormalizeRotInPi(float)
//    回転を-PiからPiの間にする
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
//    回転を-PiからPiの間にする
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInPi(Vector3& vRot)
{
NormalizeRotInPi(vRot.X);
NormalizeRotInPi(vRot.Y);
NormalizeRotInPi(vRot.Z);
}

//--------------------------------------------------------------------------------
//  NormalizeRotInPi(Vec3)
//    回転を-PiからPiの間にする
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
//    回転を-PiからPiの間にする
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInZeroToTwoPi(Vector3& vRot)
{
NormalizeRotInZeroToTwoPi(vRot.X);
NormalizeRotInZeroToTwoPi(vRot.Y);
NormalizeRotInZeroToTwoPi(vRot.Z);
}

//--------------------------------------------------------------------------------
//  CalculateZDepth
//    Z深度情報算出
//--------------------------------------------------------------------------------
float CKFMath::CalculateZDepth(const Vector3& Position, const Vector3& vCameraEye, const Vector3& vCameraAt)
{
Vector3 vCameraForward = vCameraAt - vCameraEye;
Vector3 PositionToCamera = Position - vCameraEye;
float fDepth = Vec3Dot(vCameraForward, PositionToCamera);
return fDepth;
}
*/
//--------------------------------------------------------------------------------
//	演算用関数
//	KF_Math.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "KF_Math.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const Vector3		CKFMath::sc_vZero = Vector3(0.0f);
const Vector3		CKFMath::sc_vOne = Vector3(1.0f);
const Vector3		CKFMath::sc_vUp = Vector3(0.0f, 1.0f, 0.0f);
const Vector3		CKFMath::sc_vDown = Vector3(0.0f, -1.0f, 0.0f);
const Vector3		CKFMath::sc_vLeft = Vector3(-1.0f, 0.0f, 0.0f);
const Vector3		CKFMath::sc_vRight = Vector3(1.0f, 0.0f, 0.0f);
const Vector3		CKFMath::sc_vForward = Vector3(0.0f, 0.0f, 1.0f);
const Vector3		CKFMath::sc_vBack = Vector3(0.0f, 0.0f, -1.0f);
const Color		CKFMath::sc_cWhite = Color(1.0f, 1.0f, 1.0f, 1.0f);
const Color		CKFMath::sc_cBlack = Color(0.0f, 0.0f, 0.0f, 1.0f);
const Color		CKFMath::sc_cRed = Color(1.0f, 0.0f, 0.0f, 1.0f);
const Color		CKFMath::sc_cBlue = Color(0.0f, 0.0f, 1.0f, 1.0f);
const Color		CKFMath::sc_cGreen = Color(0.0f, 1.0f, 0.0f, 1.0f);
const Quaternion	CKFMath::sc_qRotZero = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

//--------------------------------------------------------------------------------
//  Vector2
//--------------------------------------------------------------------------------
#ifdef USING_DIRECTX
//--------------------------------------------------------------------------------
//  キャスト(D3DXVECTOR2)
//	DXの環境のためオーバーロードする
//--------------------------------------------------------------------------------
Vector2::operator D3DXVECTOR2() const
{
	D3DXVECTOR2 vValue;
	vValue.x = X;
	vValue.y = Y;
	return vValue;
}
#endif

//--------------------------------------------------------------------------------
//  operator=
//--------------------------------------------------------------------------------
Vector2 &Vector2::operator=(const Vector2 &vValue)
{
	X = vValue.X;
	Y = vValue.Y;
	return *this;
}

//--------------------------------------------------------------------------------
//  operator==
//--------------------------------------------------------------------------------
bool Vector2::operator==(const Vector2 &vValue) const
{
	if (X == vValue.X && Y == vValue.Y) { return true; }
	return false;
}

//--------------------------------------------------------------------------------
//  operator+
//--------------------------------------------------------------------------------
Vector2 Vector2::operator+(const Vector2 &vValue) const
{
	Vector2 vAnswer;
	vAnswer.X = X + vValue.X;
	vAnswer.Y = Y + vValue.Y;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator+=
//--------------------------------------------------------------------------------
void Vector2::operator+=(const Vector2 &vValue)
{
	X += vValue.X;
	Y += vValue.Y;
}

//--------------------------------------------------------------------------------
//  operator-
//--------------------------------------------------------------------------------
Vector2 Vector2::operator-(const Vector2 &vValue) const
{
	Vector2 vAnswer;
	vAnswer.X = X - vValue.X;
	vAnswer.Y = Y - vValue.Y;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator-=
//--------------------------------------------------------------------------------
void Vector2::operator-=(const Vector2 &vValue)
{
	X -= vValue.X;
	Y -= vValue.Y;
}

//--------------------------------------------------------------------------------
//  operator*(外積)
//--------------------------------------------------------------------------------
float Vector2::operator*(const Vector2& vValue) const
{
	float fAnswer = X * vValue.Y - Y * vValue.X;
	return fAnswer;
}

//--------------------------------------------------------------------------------
//  operator*(float)
//--------------------------------------------------------------------------------
Vector2 Vector2::operator*(const float &fValue) const
{
	Vector2 vAnswer;
	vAnswer.X = X * fValue;
	vAnswer.Y = Y * fValue;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=(float)
//--------------------------------------------------------------------------------
void Vector2::operator*=(const float &fValue)
{
	X *= fValue;
	Y *= fValue;
}

//--------------------------------------------------------------------------------
//  Vector3
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  キャスト(Vector2)
//--------------------------------------------------------------------------------
Vector3::operator Vector2() const
{
	Vector2 vValue;
	vValue.X = X;
	vValue.Y = Y;
	return vValue;
}

#ifdef USING_DIRECTX
//--------------------------------------------------------------------------------
//  キャスト(D3DXVECTOR3)
//	DXの環境のためオーバーロードする
//--------------------------------------------------------------------------------
Vector3::operator D3DXVECTOR3() const
{
	D3DXVECTOR3 vValue;
	vValue.x = X;
	vValue.y = Y;
	vValue.z = Z;
	return vValue;
}
#endif

//--------------------------------------------------------------------------------
//  operator=
//--------------------------------------------------------------------------------
Vector3 &Vector3::operator=(const Vector3 &vValue)
{
	X = vValue.X;
	Y = vValue.Y;
	Z = vValue.Z;
	return *this;
}

//--------------------------------------------------------------------------------
//  operator==
//--------------------------------------------------------------------------------
bool Vector3::operator==(const Vector3 &vValue) const
{
	if (X == vValue.X && Y == vValue.Y && Z == vValue.Z) { return true; }
	return false;
}

//--------------------------------------------------------------------------------
//  operator!=
//--------------------------------------------------------------------------------
bool Vector3::operator!=(const Vector3 &vValue) const
{
	if (X != vValue.X || Y != vValue.Y || Z != vValue.Z) { return true; }
	return false;
}

//--------------------------------------------------------------------------------
//  operator+
//--------------------------------------------------------------------------------
Vector3 Vector3::operator+(const Vector3 &vValue) const
{
	Vector3 vAnswer;
	vAnswer.X = X + vValue.X;
	vAnswer.Y = Y + vValue.Y;
	vAnswer.Z = Z + vValue.Z;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator+=
//--------------------------------------------------------------------------------
void Vector3::operator+=(const Vector3 &vValue)
{
	X += vValue.X;
	Y += vValue.Y;
	Z += vValue.Z;
}

//--------------------------------------------------------------------------------
//  operator-
//--------------------------------------------------------------------------------
Vector3 Vector3::operator-(const Vector3 &vValue) const
{
	Vector3 vAnswer;
	vAnswer.X = X - vValue.X;
	vAnswer.Y = Y - vValue.Y;
	vAnswer.Z = Z - vValue.Z;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator-=
//--------------------------------------------------------------------------------
void Vector3::operator-=(const Vector3 &vValue)
{
	X -= vValue.X;
	Y -= vValue.Y;
	Z -= vValue.Z;
}

//--------------------------------------------------------------------------------
//  operator*(float)
//--------------------------------------------------------------------------------
Vector3 Vector3::operator*(const float &fValue) const
{
	Vector3 vAnswer;
	vAnswer.X = X * fValue;
	vAnswer.Y = Y * fValue;
	vAnswer.Z = Z * fValue;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=(float)
//--------------------------------------------------------------------------------
void Vector3::operator*=(const float &fValue)
{
	X *= fValue;
	Y *= fValue;
	Z *= fValue;
}

//--------------------------------------------------------------------------------
//  operator*(Vec3 外積(Cross))
//--------------------------------------------------------------------------------
Vector3 Vector3::operator*(const Vector3 &vValue) const
{
	Vector3 vAnswer;
	vAnswer.X = Y * vValue.Z - vValue.Y * Z;
	vAnswer.Y = Z * vValue.X - vValue.Z * X;
	vAnswer.Z = X * vValue.Y - vValue.X * Y;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=(Vec3 外積(Cross))
//--------------------------------------------------------------------------------
void Vector3::operator*=(const Vector3 &vValue)
{
	Vector3 vThis = *this;
	X = vThis.Y * vValue.Z - vValue.Y * vThis.Z;
	Y = vThis.Z * vValue.X - vValue.Z * vThis.X;
	Z = vThis.X * vValue.Y - vValue.X * vThis.Y;
}

//--------------------------------------------------------------------------------
//  operator/(float)
//--------------------------------------------------------------------------------
Vector3 Vector3::operator/(const float &fValue) const
{
	Vector3 vAnswer = Vector3(0.0f);
	if (fValue != 0.0f)
	{
		vAnswer.X = X / fValue;
		vAnswer.Y = Y / fValue;
		vAnswer.Z = Z / fValue;
	}
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator/=(float)
//--------------------------------------------------------------------------------
void Vector3::operator/=(const float &fValue)
{
	Vector3 vAnswer = Vector3(0.0f);
	if (fValue != 0.0f)
	{
		vAnswer.X = X / fValue;
		vAnswer.Y = Y / fValue;
		vAnswer.Z = Z / fValue;
	}
	*this = vAnswer;
}

//--------------------------------------------------------------------------------
//  Matrix44
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
Matrix44::Matrix44()
{
	float afMtx[4][4] = {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f } };
	memcpy(Elements, afMtx, sizeof(Elements));
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
Matrix44::Matrix44(
	const float &f11, const float &f12, const float &f13, const float &f14,
	const float &f21, const float &f22, const float &f23, const float &f24,
	const float &f31, const float &f32, const float &f33, const float &f34,
	const float &f41, const float &f42, const float &f43, const float &f44)
{
	Elements[0][0] = f11;
	Elements[0][1] = f12;
	Elements[0][2] = f13;
	Elements[0][3] = f14;
	Elements[1][0] = f21;
	Elements[1][1] = f22;
	Elements[1][2] = f23;
	Elements[1][3] = f24;
	Elements[2][0] = f31;
	Elements[2][1] = f32;
	Elements[2][2] = f33;
	Elements[2][3] = f34;
	Elements[3][0] = f41;
	Elements[3][1] = f42;
	Elements[3][2] = f43;
	Elements[3][3] = f44;
}

#ifdef USING_DIRECTX
//--------------------------------------------------------------------------------
//  キャスト(D3DXVECTOR3)
//	DXの環境のためオーバーロードする
//--------------------------------------------------------------------------------
Matrix44::operator D3DXMATRIX() const
{
	D3DXMATRIX mtxValue;
	
	for (int nCntY = 0; nCntY < 4; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			mtxValue(nCntY, nCntX) = Elements[nCntY][nCntX];
		}
	}

	return mtxValue;
}
#endif

//--------------------------------------------------------------------------------
//  operator*
//--------------------------------------------------------------------------------
Matrix44 &Matrix44::operator=(const Matrix44 &mtxValue)
{
	for (int nCntY = 0; nCntY < 4; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			Elements[nCntY][nCntX] = mtxValue.Elements[nCntY][nCntX];
		}
	}

	return *this;
}

//--------------------------------------------------------------------------------
//  operator*
//--------------------------------------------------------------------------------
Matrix44 Matrix44::operator*(const Matrix44 &mtxValue) const
{
	Matrix44 mtxAnswer;

	for (int nCntY = 0; nCntY < 4; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			mtxAnswer.Elements[nCntY][nCntX] = 
				Elements[nCntY][0] * mtxValue.Elements[0][nCntX] + 
				Elements[nCntY][1] * mtxValue.Elements[1][nCntX] + 
				Elements[nCntY][2] * mtxValue.Elements[2][nCntX] + 
				Elements[nCntY][3] * mtxValue.Elements[3][nCntX];
		}								    
	}									   
										   
	return mtxAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=
//--------------------------------------------------------------------------------
void Matrix44::operator*=(const Matrix44 &mtxValue)
{
	Matrix44 mtxCpy = *this;

	for (int nCntY = 0; nCntY < 4; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			Elements[nCntY][nCntX] =
				mtxCpy.Elements[nCntY][0] * mtxValue.Elements[0][nCntX] +
				mtxCpy.Elements[nCntY][1] * mtxValue.Elements[1][nCntX] +
				mtxCpy.Elements[nCntY][2] * mtxValue.Elements[2][nCntX] +
				mtxCpy.Elements[nCntY][3] * mtxValue.Elements[3][nCntX];
		}
	}
}

//--------------------------------------------------------------------------------
//  Vector4
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  operator*(Matrix44)
//--------------------------------------------------------------------------------
Vector4 Vector4::operator*(const Matrix44 &mtxValue) const
{
	Vector4 vAnswer;

	vAnswer.X =
		X * mtxValue.Elements[0][0] +
		Y * mtxValue.Elements[1][0] +
		Z * mtxValue.Elements[2][0] +
		W * mtxValue.Elements[3][0];
	vAnswer.Y =
		X * mtxValue.Elements[0][1] +
		Y * mtxValue.Elements[1][1] +
		Z * mtxValue.Elements[2][1] +
		W * mtxValue.Elements[3][1];
	vAnswer.Z =
		X * mtxValue.Elements[0][2] +
		Y * mtxValue.Elements[1][2] +
		Z * mtxValue.Elements[2][2] +
		W * mtxValue.Elements[3][2];
	vAnswer.W =
		X * mtxValue.Elements[0][3] +
		Y * mtxValue.Elements[1][3] +
		Z * mtxValue.Elements[2][3] +
		W * mtxValue.Elements[3][3];

	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=(Matrix44)
//--------------------------------------------------------------------------------
void Vector4::operator*=(const Matrix44 &mtxValue)
{
	Vector4 vThis = *this;

	X =
		vThis.X * mtxValue.Elements[0][0] +
		vThis.Y * mtxValue.Elements[1][0] +
		vThis.Z * mtxValue.Elements[2][0] +
		vThis.W * mtxValue.Elements[3][0];
	Y =
		vThis.X * mtxValue.Elements[0][1] +
		vThis.Y * mtxValue.Elements[1][1] +
		vThis.Z * mtxValue.Elements[2][1] +
		vThis.W * mtxValue.Elements[3][1];
	Z =
		vThis.X * mtxValue.Elements[0][2] +
		vThis.Y * mtxValue.Elements[1][2] +
		vThis.Z * mtxValue.Elements[2][2] +
		vThis.W * mtxValue.Elements[3][2];
	W =
		vThis.X * mtxValue.Elements[0][3] +
		vThis.Y * mtxValue.Elements[1][3] +
		vThis.Z * mtxValue.Elements[2][3] +
		vThis.W * mtxValue.Elements[3][3];
}

//--------------------------------------------------------------------------------
//  Quaternion
//--------------------------------------------------------------------------------
#ifdef USING_DIRECTX
//--------------------------------------------------------------------------------
//  キャスト(D3DXQUATERNION)
//	DXの環境のためオーバーロードする
//--------------------------------------------------------------------------------
Quaternion::operator D3DXQUATERNION () const
{
	D3DXQUATERNION qDx;
	qDx.x = X;
	qDx.y = Y;
	qDx.z = Z;
	qDx.w = W;
	return qDx;
}
#endif

//--------------------------------------------------------------------------------
//  operator+(Quaternion)
//	quaternionの加算
//--------------------------------------------------------------------------------
Quaternion Quaternion::operator+(const Quaternion& qValue) const
{
	Quaternion qAnswer;
	qAnswer.X = X + qValue.X;
	qAnswer.Y = Y + qValue.Y;
	qAnswer.Z = Z + qValue.Z;
	qAnswer.W = W + qValue.W;
	return qAnswer;
}

//--------------------------------------------------------------------------------
//  operator+=(Quaternion)
//	quaternionの加算
//--------------------------------------------------------------------------------
void Quaternion::operator+=(const Quaternion& qValue)
{
	X += qValue.X;
	Y += qValue.Y;
	Z += qValue.Z;
	W += qValue.W;
}

//--------------------------------------------------------------------------------
//  operator-(Quaternion)
//	quaternionの減算
//--------------------------------------------------------------------------------
Quaternion Quaternion::operator-(const Quaternion& qValue) const
{
	Quaternion qAnswer;
	qAnswer.X = X - qValue.X;
	qAnswer.Y = Y - qValue.Y;
	qAnswer.Z = Z - qValue.Z;
	qAnswer.W = W - qValue.W;
	return qAnswer;
}

//--------------------------------------------------------------------------------
//  operator-=(Quaternion)
//	quaternionの減算
//--------------------------------------------------------------------------------
void Quaternion::operator-=(const Quaternion& qValue)
{
	X -= qValue.X;
	Y -= qValue.Y;
	Z -= qValue.Z;
	W -= qValue.W;
}

//--------------------------------------------------------------------------------
//  operator*(float)
//	quaternionの乗算
//--------------------------------------------------------------------------------
Quaternion Quaternion::operator*(const float& fValue) const
{
	Quaternion qAnswer;
	qAnswer.X = X * fValue;
	qAnswer.Y = Y * fValue;
	qAnswer.Z = Z * fValue;
	qAnswer.W = W * fValue;
	return qAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=(float)
//	quaternionの乗算
//--------------------------------------------------------------------------------
void Quaternion::operator*=(const float& fValue)
{
	X *= fValue;
	Y *= fValue;
	Z *= fValue;
	W *= fValue;
}

//--------------------------------------------------------------------------------
//  operator/=
//--------------------------------------------------------------------------------
void Quaternion::operator/=(const float& fValue)
{
	X /= fValue;
	Y /= fValue;
	Z /= fValue;
	W /= fValue;
}

//--------------------------------------------------------------------------------
//  operator*
//--------------------------------------------------------------------------------
Quaternion Quaternion::operator*(const Quaternion& qValue) const
{
	Quaternion qAnswer;
	qAnswer.X = (qValue.W * this->X) + (qValue.X * this->W) + (qValue.Y * this->Z) - (qValue.Z * this->Y);
	qAnswer.Y = (qValue.W * this->Y) - (qValue.X * this->Z) + (qValue.Y * this->W) + (qValue.Z * this->X);
	qAnswer.Z = (qValue.W * this->Z) + (qValue.X * this->Y) - (qValue.Y * this->X) + (qValue.Z * this->W);
	qAnswer.W = (qValue.W * this->W) - (qValue.X * this->X) - (qValue.Y * this->Y) - (qValue.Z * this->Z);
	return qAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=
//--------------------------------------------------------------------------------
void Quaternion::operator*=(const Quaternion& qValue)
{
	auto qCopy = *this;
	this->X = (qValue.W * qCopy.X) + (qValue.X * qCopy.W) + (qValue.Y * qCopy.Z) - (qValue.Z * qCopy.Y);
	this->Y = (qValue.W * qCopy.Y) - (qValue.X * qCopy.Z) + (qValue.Y * qCopy.W) + (qValue.Z * qCopy.X);
	this->Z = (qValue.W * qCopy.Z) + (qValue.X * qCopy.Y) - (qValue.Y * qCopy.X) + (qValue.Z * qCopy.W);
	this->W = (qValue.W * qCopy.W) - (qValue.X * qCopy.X) - (qValue.Y * qCopy.Y) - (qValue.Z * qCopy.Z);
}

//--------------------------------------------------------------------------------
//  Color
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  operator=
//--------------------------------------------------------------------------------
Color &Color::operator=(const Color &cValue)
{
	R = cValue.R;
	G = cValue.G;
	B = cValue.B;
	A = cValue.A;

	return *this;
}

//--------------------------------------------------------------------------------
//  operator==
//--------------------------------------------------------------------------------
bool Color::operator==(const Color &cValue)
{
	if (R == cValue.R && G == cValue.G && B == cValue.B && A == cValue.A) { return true; }
	return false;
}

//--------------------------------------------------------------------------------
//  operator+
//--------------------------------------------------------------------------------
Color Color::operator+(const Color &cValue) const
{
	Color cAnswer;
	cAnswer.R = R + cValue.R;
	cAnswer.G = G + cValue.G;
	cAnswer.B = B + cValue.B;
	cAnswer.A = A + cValue.A;
	return cAnswer;
}

//--------------------------------------------------------------------------------
//  operator+=
//--------------------------------------------------------------------------------
void Color::operator+=(const Color &cValue)
{
	R += cValue.R;
	G += cValue.G;
	B += cValue.B;
	A += cValue.A;
}

//--------------------------------------------------------------------------------
//  operator-
//--------------------------------------------------------------------------------
Color Color::operator-(const Color &cValue) const
{
	Color cAnswer;
	cAnswer.R = R - cValue.R;
	cAnswer.G = G - cValue.G;
	cAnswer.B = B - cValue.B;
	cAnswer.A = A - cValue.A;
	return cAnswer;
}

//--------------------------------------------------------------------------------
//  operator-=
//--------------------------------------------------------------------------------
void Color::operator-=(const Color &cValue)
{
	R -= cValue.R;
	G -= cValue.G;
	B -= cValue.B;
	A -= cValue.A;
}

//--------------------------------------------------------------------------------
//  operator*(float)
//--------------------------------------------------------------------------------
Color Color::operator*(const float &fValue) const
{
	Color cAnswer;
	cAnswer.R = R * fValue;
	cAnswer.G = G * fValue;
	cAnswer.B = B * fValue;
	cAnswer.A = A * fValue;
	return cAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=(float)
//--------------------------------------------------------------------------------
void Color::operator*=(const float &fValue)
{
	R *= fValue;
	G *= fValue;
	B *= fValue;
	A *= fValue;
}

#ifdef USING_DIRECTX
//--------------------------------------------------------------------------------
//  キャスト(D3DCOLORVALUE)
//	DXの環境のため(Light)オーバーロードする
//--------------------------------------------------------------------------------
Color::operator D3DCOLORVALUE() const
{
	D3DCOLORVALUE vAnswer;
	vAnswer.r = R;
	vAnswer.g = G;
	vAnswer.b = B;
	vAnswer.a = A;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  キャスト(unsigned long)
//	DXの環境で頂点情報のカラーはDWORD(unsigned long)のためオーバーロードする
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
//  CKFMath
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Random
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ランダムシステム初期化
//--------------------------------------------------------------------------------
void CKFMath::InitRandom(void)
{
	srand((unsigned)time(NULL));
}

//--------------------------------------------------------------------------------
//  GetRandomInt
//--------------------------------------------------------------------------------
int	CKFMath::GetRandomInt(const int &nMin, const int &nMax)
{
	if (nMin >= nMax) { return nMin; }
	int nWork = rand() % (nMax - nMin);
	nWork = nWork + nMin;
	return nWork;
}

//--------------------------------------------------------------------------------
//  GetRandomFloat
//--------------------------------------------------------------------------------
float CKFMath::GetRandomFloat(const float &fMin, const float &fMax)
{
	if (fMin >= fMax) { return fMin; }
	float fWork = (rand() % 10000) * 0.0001f;
	fWork = fWork * (fMax - fMin) + fMin;
	return fWork;
}

//--------------------------------------------------------------------------------
//  GetRandomVec3
//--------------------------------------------------------------------------------
Vector3 CKFMath::GetRandomVec3(const Vector3 &vMin, const Vector3 &vMax)
{
	Vector3 vWork;
	vWork.X = GetRandomFloat(vMin.X, vMax.X);
	vWork.Y = GetRandomFloat(vMin.Y, vMax.Y);
	vWork.Z = GetRandomFloat(vMin.Z, vMax.Z);
	return vWork;
}

//--------------------------------------------------------------------------------
//  Vector
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Magnitude(Vector2)
//--------------------------------------------------------------------------------
float CKFMath::VecMagnitude(const Vector2 &vValue)
{
	float fMagnitude = sqrtf(vValue.X * vValue.X + vValue.Y * vValue.Y);
	return fMagnitude;
}

//--------------------------------------------------------------------------------
//  Magnitude(Vector3)
//--------------------------------------------------------------------------------
float CKFMath::VecMagnitude(const Vector3 &vValue)
{
	float fMagnitude = sqrtf(vValue.X * vValue.X + vValue.Y * vValue.Y + vValue.Z * vValue.Z);
	return fMagnitude;
}

//--------------------------------------------------------------------------------
//  MagnitudeSquare(Vector2)
//--------------------------------------------------------------------------------
float CKFMath::VecMagnitudeSquare(const Vector2 &vValue)
{
	float fMagnitudeSquare = vValue.X * vValue.X + vValue.Y * vValue.Y;
	return fMagnitudeSquare;
}

//--------------------------------------------------------------------------------
//  MagnitudeSquare(Vector3)
//--------------------------------------------------------------------------------
float CKFMath::VecMagnitudeSquare(const Vector3 &vValue)
{
	float fMagnitudeSquare = vValue.X * vValue.X + vValue.Y * vValue.Y + vValue.Z * vValue.Z;
	return fMagnitudeSquare;
}

//--------------------------------------------------------------------------------
//  Normalize(Vector2)
//--------------------------------------------------------------------------------
void CKFMath::VecNormalize(Vector2& vVec)
{
	float fMagnitude = VecMagnitude(vVec);
	if (fMagnitude == 0.0f) { return; }
	vVec.X /= fMagnitude;
	vVec.Y /= fMagnitude;
}

//--------------------------------------------------------------------------------
//  Normalize(Vector3)
//--------------------------------------------------------------------------------
void CKFMath::VecNormalize(Vector3& vVec)
{
	float fMagnitude = VecMagnitude(vVec);
	if (fMagnitude == 0.0f) { return; }
	vVec /= fMagnitude;
}

//--------------------------------------------------------------------------------
//  Normalize(Vector3)
//--------------------------------------------------------------------------------
Vector3 CKFMath::VecNormalize(const Vector3& vVec)
{
	float fMagnitude = VecMagnitude(vVec);
	if (fMagnitude == 0.0f) { return Vector3(0.0f); }
	auto vAnswer = vVec;
	vAnswer /= fMagnitude;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  乗算(Dot)(Vector2)
//--------------------------------------------------------------------------------
float CKFMath::Vec2Dot(const Vector2& vVecL, const Vector2& vVecR)
{
	float fDot = vVecL.X * vVecR.X + vVecL.Y * vVecR.Y;
	return fDot;
}

//--------------------------------------------------------------------------------
//  乗算(Dot)(Vector3)
//--------------------------------------------------------------------------------
float CKFMath::Vec3Dot(const Vector3 &vVecL, const Vector3 &vVecR)
{
	float fDot = vVecL.X * vVecR.X
		+ vVecL.Y * vVecR.Y
		+ vVecL.Z * vVecR.Z;
	return fDot;
}

//--------------------------------------------------------------------------------
//  VecDistance(Vector3)
//--------------------------------------------------------------------------------
float CKFMath::VecDistance(const Vector3 &vVecL, const Vector3 &vVecR)
{
	Vector3 vDis = vVecL - vVecR;
	float fDis = VecMagnitude(vDis);
	return fDis;
}

//--------------------------------------------------------------------------------
//  VecDistanceSquare(Vector3)
//--------------------------------------------------------------------------------
float CKFMath::VecDistanceSquare(const Vector3 &vVecL, const Vector3 &vVecR)
{
	Vector3 vDis = vVecL - vVecR;
	float fDisSquare = VecMagnitudeSquare(vDis);
	return fDisSquare;
}

//--------------------------------------------------------------------------------
//  Vec3TransformCoord
//	回転行列にとってVec3を回転する
//--------------------------------------------------------------------------------
Vector3 CKFMath::Vec3TransformCoord(const Vector3& vVec, const Matrix44 &mtxRot)
{
	Vector4 vVec4 = Vector4(vVec, 1.0f);
	Vector3 vAnswer;
	vVec4 *= mtxRot;

	if (vVec4.W != 0.0f)
	{
		vAnswer.X = vVec4.X / vVec4.W;
		vAnswer.Y = vVec4.Y / vVec4.W;
		vAnswer.Z = vVec4.Z / vVec4.W;
	}
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  Vec3TransformNormal
//	回転行列にとってVec3(ベクトル)を回転する
//--------------------------------------------------------------------------------
Vector3 CKFMath::Vec3TransformNormal(const Vector3& vVec, const Matrix44 &mtxRot)
{
	Vector4 vVec4 = Vector4(vVec, 0.0f);
	Vector3 vAnswer;
	vVec4 *= mtxRot;
	vAnswer.X = vVec4.X;
	vAnswer.Y = vVec4.Y;
	vAnswer.Z = vVec4.Z;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  Vec3TransformNormal
//	回転行列にとってVec3(ベクトル)を回転する
//--------------------------------------------------------------------------------
Vector3 CKFMath::Vec3Scale(const Vector3& vValue, const Vector3& vScale)
{
	auto vAnswer = Vector3(vValue.X * vScale.X, vValue.Y * vScale.Y, vValue.Z * vScale.Z);
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  Vector2の値から角度を計算する
//--------------------------------------------------------------------------------
float CKFMath::Vec2Radian(const Vector2& vValue)
{
	float fValue = atan2f(vValue.Y, vValue.X);
	return fValue;
}

//--------------------------------------------------------------------------------
//  RadianBetweenVec
//	Vec2(ベクトル)間の角度の算出
//--------------------------------------------------------------------------------
float CKFMath::RadianBetweenVec(const Vector2& vVecL, const Vector2& vVecR)
{
	float fLengthL = VecMagnitude(vVecL);
	float fLengthR = VecMagnitude(vVecR);
	if (vVecL == vVecR || fLengthL == 0.0f || fLengthR == 0.0f) { return 0.0f; }
	float fDot = Vec2Dot(vVecL, vVecR);
	float fCross = vVecL * vVecR;
	float fSign = fCross >= 0.0f ? 1.0f : -1.0f;
	float fRot = acosf(fDot / (fLengthL * fLengthR)) * fSign;
	return fRot;
}

//--------------------------------------------------------------------------------
//  RadianBetweenVec3
//	回転行列にとってVec3(ベクトル)を回転する
//--------------------------------------------------------------------------------
float CKFMath::RadianBetweenVec(const Vector3 &vVecL, const Vector3 &vVecR)
{
	float fLengthL = VecMagnitude(vVecL);
	float fLengthR = VecMagnitude(vVecR);
	if (vVecL == vVecR || fLengthL == 0.0f || fLengthR == 0.0f) { return 0.0f; }

	float fRot = 0.0f;
	float fWork;

	//左右判断
	fWork = vVecR.Z * vVecL.X - vVecR.X * vVecL.Z;
	if (fWork >= 0.0f)
	{
		fWork = 1.0f;
	}
	else
	{
		fWork = -1.0f;
	}

	//ベクトルの成す角度の算出
	fRot = acosf(Vec3Dot(vVecL, vVecR) / (fLengthL * fLengthR));
	fRot *= fWork;

	//-nan(ind)防止
	if (abs(fRot) >= 0.0f)
	{
		fRot = fRot;
	}
	else
	{
		fRot = 0.0f;
	}

	return fRot;
}

//--------------------------------------------------------------------------------
//  EulerBetweenVec3
//	Vector3間の角度をx,y,z回転式で算出
//--------------------------------------------------------------------------------
Vector3	CKFMath::EulerBetweenVec3(const Vector3& vVecFrom, const Vector3& vVecTo)
{
	Vector3 vRot;

	//X軸回転
	vRot.X = RadianBetweenVec(Vector2(vVecFrom.Y, vVecFrom.Z), Vector2(vVecTo.Y, vVecTo.Z));

	//Y軸回転
	vRot.Y = RadianBetweenVec(Vector2(vVecFrom.Z, vVecFrom.X), Vector2(vVecTo.Z, vVecTo.X));

	//Z軸回転
	vRot.Z = RadianBetweenVec(Vector2(vVecFrom.X, vVecFrom.Y), Vector2(vVecTo.X, vVecTo.Y));

	return vRot;
}

//--------------------------------------------------------------------------------
//  EulerBetweenVec3
//	Vector3間の角度をx,y,z回転式で算出
//--------------------------------------------------------------------------------
Vector3	CKFMath::ProjectOnPlane(const Vector3& vVec, const Vector3& vUpPlane, const Vector3& vUpNow)
{
	auto vAnswer = vVec;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  TransformInverse
//	Vector3間の角度をx,y,z回転式で算出
//--------------------------------------------------------------------------------
Vector3	CKFMath::TransformInverse(const Matrix44& mtx, const Vector3& vVec)
{
	Vector3 vWork = vVec;
	vWork.X -= mtx.Elements[3][0];
	vWork.Y -= mtx.Elements[3][1];
	vWork.Z -= mtx.Elements[3][2];
	Vector3 vAnswer;
	vAnswer.X = vWork.X * mtx.Elements[0][0] +
		vWork.Y * mtx.Elements[0][1] +
		vWork.Z * mtx.Elements[0][2];
	vAnswer.Y = vWork.X * mtx.Elements[1][0] +
		vWork.Y * mtx.Elements[1][1] +
		vWork.Z * mtx.Elements[1][2];
	vAnswer.Z = vWork.X * mtx.Elements[2][0] +
		vWork.Y * mtx.Elements[2][1] +
		vWork.Z * mtx.Elements[2][2];

	//D3DXVECTOR3 vTest = vVec;
	//D3DXMATRIX mtxTest = mtx;
	//D3DXMatrixInverse(&mtxTest, nullptr, &mtxTest);
	//D3DXVec3TransformCoord(&vTest, &vTest, &mtxTest);

	return vAnswer;
}

//--------------------------------------------------------------------------------
//  Matrix
//--------------------------------------------------------------------------------
#ifdef USING_DIRECTX
//--------------------------------------------------------------------------------
//  ChangeDXMtxToMtx44
//--------------------------------------------------------------------------------
Matrix44 CKFMath::ChangeDXMtxToMtx44(const D3DXMATRIX &mtx)
{
	Matrix44 mtxOut;
	for (int nCntY = 0; nCntY < 4; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			mtxOut.Elements[nCntY][nCntX] = mtx(nCntY, nCntX);
		}
	}

	return mtxOut;
}
#endif

//--------------------------------------------------------------------------------
//  MtxIdentity
//	単位行列の作成
//--------------------------------------------------------------------------------
void CKFMath::MtxIdentity(Matrix44& mtx)
{
	mtx.Elements[0][0] = 1.0f;
	mtx.Elements[0][1] = 0.0f;
	mtx.Elements[0][2] = 0.0f;
	mtx.Elements[0][3] = 0.0f;
	mtx.Elements[1][0] = 0.0f;
	mtx.Elements[1][1] = 1.0f;
	mtx.Elements[1][2] = 0.0f;
	mtx.Elements[1][3] = 0.0f;
	mtx.Elements[2][0] = 0.0f;
	mtx.Elements[2][1] = 0.0f;
	mtx.Elements[2][2] = 1.0f;
	mtx.Elements[2][3] = 0.0f;
	mtx.Elements[3][0] = 0.0f;
	mtx.Elements[3][1] = 0.0f;
	mtx.Elements[3][2] = 0.0f;
	mtx.Elements[3][3] = 1.0f;
}

//--------------------------------------------------------------------------------
//  MtxRotAxis
//	回転行列の作成
//--------------------------------------------------------------------------------
void CKFMath::MtxRotAxis(Matrix44& mtxRot, const Vector3 &vAxis, const float &fAngle)
{
	//軸をノーマライズする
	Vector3 vAxisCpy = vAxis;
	VecNormalize(vAxisCpy);

	//計算用変数
	float fX = vAxisCpy.X;
	float fY = vAxisCpy.Y;
	float fZ = vAxisCpy.Z;
	float fSin = sinf(fAngle);
	float fCos = cosf(fAngle);

	mtxRot.Elements[0][0] = fCos + fX * fX * (1 - fCos);
	mtxRot.Elements[0][1] = fX * fY * (1 - fCos) + fZ * fSin;
	mtxRot.Elements[0][2] = fX * fZ * (1 - fCos) - fY * fSin;
	mtxRot.Elements[0][3] = 0.0f;
	mtxRot.Elements[1][0] = fX * fY * (1 - fCos) - fZ * fSin;
	mtxRot.Elements[1][1] = fCos + fY * fY * (1 - fCos);
	mtxRot.Elements[1][2] = fZ * fY * (1 - fCos) + fX * fSin;
	mtxRot.Elements[1][3] = 0.0f;
	mtxRot.Elements[2][0] = fX * fZ * (1 - fCos) + fY * fSin;
	mtxRot.Elements[2][1] = fY * fZ * (1 - fCos) - fX * fSin;
	mtxRot.Elements[2][2] = fCos + fZ * fZ * (1 - fCos);
	mtxRot.Elements[2][3] = 0.0f;
	mtxRot.Elements[3][0] = 0.0f;
	mtxRot.Elements[3][1] = 0.0f;
	mtxRot.Elements[3][2] = 0.0f;
	mtxRot.Elements[3][3] = 1.0f;
}

//--------------------------------------------------------------------------------
//  MtxRotationYawPitchRoll
//	回転行列の作成(mtxY*mtxX*mtxZ)
//--------------------------------------------------------------------------------
void CKFMath::MtxRotationYawPitchRoll(Matrix44& mtxRot, const Vector3 &vRot)
{
	float fSinX = sinf(vRot.X); 
	float fCosX = cosf(vRot.X); 
	float fSinY = sinf(vRot.Y);
	float fCosY = cosf(vRot.Y);
	float fSinZ = sinf(vRot.Z);
	float fCosZ = cosf(vRot.Z);

	mtxRot.Elements[0][0] = fCosY * fCosZ + fSinX * fSinY * fSinZ;
	mtxRot.Elements[0][1] = fCosX * fSinZ;
	mtxRot.Elements[0][2] = -fSinY * fCosZ + fSinX * fCosY * fSinZ;
	mtxRot.Elements[0][3] = 0.0f;
	mtxRot.Elements[1][0] = -fCosY * fSinZ + fSinX * fSinY * fCosZ;
	mtxRot.Elements[1][1] = fCosX * fCosZ;
	mtxRot.Elements[1][2] = fSinY * fSinZ + fSinX * fCosY * fCosZ;
	mtxRot.Elements[1][3] = 0.0f;
	mtxRot.Elements[2][0] = fCosX * fSinY;
	mtxRot.Elements[2][1] = -fSinX;
	mtxRot.Elements[2][2] = fCosX * fCosY;
	mtxRot.Elements[2][3] = 0.0f;
	mtxRot.Elements[3][0] = 0.0f;
	mtxRot.Elements[3][1] = 0.0f;
	mtxRot.Elements[3][2] = 0.0f;
	mtxRot.Elements[3][3] = 1.0f;
}

//--------------------------------------------------------------------------------
//	MtxTranslation
//	平行移動行列の作成
//--------------------------------------------------------------------------------
void CKFMath::MtxTranslation(Matrix44& mtxTrans, const Vector3 &vPos)
{
	MtxIdentity(mtxTrans);
	mtxTrans.Elements[3][0] = vPos.X;
	mtxTrans.Elements[3][1] = vPos.Y;
	mtxTrans.Elements[3][2] = vPos.Z;
}

//--------------------------------------------------------------------------------
//	MtxTranspose
//	行列のトランスポース
//--------------------------------------------------------------------------------
Matrix44 CKFMath::MtxTranspose(const Matrix44& mtx)
{
	Matrix44 mtxOut;
	for (int nCntY = 0; nCntY < 4; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			mtxOut.Elements[nCntY][nCntX] = mtx.Elements[nCntX][nCntY];
		}
	}
	return mtxOut;
}

//--------------------------------------------------------------------------------
//	MtxTranspose
//	行列のトランスポース
//--------------------------------------------------------------------------------
Vector3 CKFMath::MtxToEular(const Matrix44& mtx)
{
	return QuaternionToEuler(MtxToQuaternion(mtx));
}

//--------------------------------------------------------------------------------
//  Quaternion
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	QuaternionIdentity
//	平行移動行列の作成
//--------------------------------------------------------------------------------
void CKFMath::QuaternionIdentity(Quaternion& qValue)
{
	qValue.X = qValue.Y = qValue.Z = 0.0f;
	qValue.W = 1.0f;
}

//--------------------------------------------------------------------------------
//	QuaternionMagnitudeSquare
//	平行移動行列の作成
//--------------------------------------------------------------------------------
float CKFMath::QuaternionMagnitudeSquare(const Quaternion& qValue)
{
	float fSquare = QuaternionDot(qValue, qValue);
	return fSquare;
}

//--------------------------------------------------------------------------------
//	QuaternionMagnitude
//	平行移動行列の作成
//--------------------------------------------------------------------------------
float CKFMath::QuaternionMagnitude(const Quaternion& qValue)
{
	float fValue = QuaternionMagnitudeSquare(qValue);
	fValue = sqrtf(fValue);
	return fValue;
}

//--------------------------------------------------------------------------------
//	QuaternionNormalize
//	平行移動行列の作成
//--------------------------------------------------------------------------------
void CKFMath::QuaternionNormalize(Quaternion& qValue)
{
	float fMag = QuaternionMagnitude(qValue);
	if (fMag == 0.0f)
	{
		QuaternionIdentity(qValue);
		return;
	}
	qValue /= fMag;
}

//--------------------------------------------------------------------------------
//	QuaternionDot
//	平行移動行列の作成
//--------------------------------------------------------------------------------
float CKFMath::QuaternionDot(const Quaternion& qL, const Quaternion& qR)
{
	float fValue = qL.X * qR.X + qL.Y * qR.Y + qL.Z * qR.Z + qL.W * qR.W;
	return fValue;
}

//--------------------------------------------------------------------------------
//	QuaternionMultiplyXYZW
//	平行移動行列の作成
//--------------------------------------------------------------------------------
Quaternion CKFMath::QuaternionMultiplyXYZW(const Quaternion& qL, const Quaternion& qR)
{
	return Quaternion(
		qL.X * qR.X,
		qL.Y * qR.Y,
		qL.Z * qR.Z,
		qL.W * qR.W);
}

//--------------------------------------------------------------------------------
//	MtxToQuaternion
//	平行移動行列の作成
//--------------------------------------------------------------------------------
Quaternion CKFMath::EulerToQuaternion(const Vector3& vVec)
{
	Quaternion qValue;
#ifdef USING_DIRECTX
	D3DXQUATERNION qDx;
	D3DXQuaternionRotationYawPitchRoll(&qDx, vVec.Y, vVec.X, vVec.Z);
	qValue.X = qDx.x;
	qValue.Y = qDx.y;
	qValue.Z = qDx.z;
	qValue.W = qDx.w;
#endif // USING_DIRECTX
	return qValue;
}

//--------------------------------------------------------------------------------
//	MtxToQuaternion
//	平行移動行列の作成
//--------------------------------------------------------------------------------
Quaternion CKFMath::MtxToQuaternion(const Matrix44& mtxRot)
{
	Quaternion qValue;
	float fR22 = mtxRot.Elements[2][2];
	if (fR22 <= 0.0f)  // x^2 + y^2 >= z^2 + w^2
	{
		float fDiff10 = mtxRot.Elements[1][1] - mtxRot.Elements[0][0];
		float fOmr22 = 1.0f - fR22;
		if (fDiff10 <= 0.0f)  // x^2 >= y^2
		{
			float f4XSqr = fOmr22 - fDiff10;
			float fInv4X = 0.5f / sqrtf(f4XSqr);
			qValue.X = f4XSqr * fInv4X;
			qValue.Y = (mtxRot.Elements[0][1] + mtxRot.Elements[1][0]) * fInv4X;
			qValue.Z = (mtxRot.Elements[0][2] + mtxRot.Elements[2][0]) * fInv4X;
			qValue.W = (mtxRot.Elements[1][2] - mtxRot.Elements[2][1]) * fInv4X;
		}
		else  // y^2 >= x^2
		{
			float f4YSqr = fOmr22 + fDiff10;
			float fInv4Y = 0.5f / sqrtf(f4YSqr);
			qValue.X = (mtxRot.Elements[0][1] + mtxRot.Elements[1][0]) * fInv4Y;
			qValue.Y = f4YSqr * fInv4Y;
			qValue.Z = (mtxRot.Elements[1][2] + mtxRot.Elements[2][1]) * fInv4Y;
			qValue.W = (mtxRot.Elements[2][0] - mtxRot.Elements[0][2]) * fInv4Y;
		}
	}
	else  // z^2 + w^2 >= x^2 + y^2
	{
		float fSum10 = mtxRot.Elements[1][1] + mtxRot.Elements[0][0];
		float fOpr22 = 1.0f + fR22;
		if (fSum10 <= 0.0f)  // z^2 >= w^2
		{
			float f4ZSqr = fOpr22 - fSum10;
			float fInv4Z = 0.5f / sqrtf(f4ZSqr);
			qValue.X = (mtxRot.Elements[0][2] + mtxRot.Elements[2][0]) * fInv4Z;
			qValue.Y = (mtxRot.Elements[1][2] + mtxRot.Elements[2][1]) * fInv4Z;
			qValue.Z = f4ZSqr * fInv4Z;
			qValue.W = (mtxRot.Elements[0][1] - mtxRot.Elements[1][0]) * fInv4Z;
		}
		else  // w^2 >= z^2
		{
			float f4WSqr = fOpr22 + fSum10;
			float fInv4W = 0.5f / sqrtf(f4WSqr);
			qValue.X = (mtxRot.Elements[1][2] - mtxRot.Elements[2][1]) * fInv4W;
			qValue.Y = (mtxRot.Elements[2][0] - mtxRot.Elements[0][2]) * fInv4W;
			qValue.Z = (mtxRot.Elements[0][1] - mtxRot.Elements[1][0]) * fInv4W;
			qValue.W = f4WSqr * fInv4W;
		}
	}
	return qValue;
}

//--------------------------------------------------------------------------------
//  QuaternionToMtx
//	
//--------------------------------------------------------------------------------
Matrix44 CKFMath::QuaternionToMtx(const Quaternion& qValue)
{
	Matrix44 mtx;

	static const auto qAnt1110 = Quaternion(1.0f, 1.0f, 1.0f, 0.0f);
	auto q0 = qValue + qValue;
	auto q1 = QuaternionMultiplyXYZW(qValue, q0);
	auto q3 = Quaternion(q1.Y, q1.X, q1.X, qAnt1110.W);
	auto q4 = Quaternion(q1.Z, q1.Z, q1.Y, qAnt1110.W);
	auto qR0 = qAnt1110 - q3;
	qR0 -= q4;
	q3 = Quaternion(qValue.X, qValue.X, qValue.Y, qValue.W);
	q4 = Quaternion(q0.Z, q0.Y, q0.Z, q0.W);
	q3 = QuaternionMultiplyXYZW(q3, q4);
	q4 = Quaternion(qValue.W, qValue.W, qValue.W, qValue.W);
	auto q5 = Quaternion(q0.Y, q0.Z, q0.X, q0.W);
	q4 = QuaternionMultiplyXYZW(q4, q5);
	auto qR1 = q3 + q4;
	auto qR2 = q3 - q4;
	q3 = Quaternion(qR1.Y, qR2.X, qR2.Y, qR1.Z);
	q4 = Quaternion(qR1.X, qR2.Z, qR1.X, qR2.Z);

	mtx.Elements[0][0] = qR0.X;
	mtx.Elements[0][1] = q3.X;
	mtx.Elements[0][2] = q3.Y;
	mtx.Elements[0][3] = qR0.W;
	mtx.Elements[1][0] = q3.Z;
	mtx.Elements[1][1] = qR0.Y;
	mtx.Elements[1][2] = q3.W;
	mtx.Elements[1][3] = qR0.W;
	mtx.Elements[2][0] = q4.X;
	mtx.Elements[2][1] = q4.Y;
	mtx.Elements[2][2] = qR0.Z;
	mtx.Elements[2][3] = qR0.W;
	mtx.Elements[3][0] = 0.0f;
	mtx.Elements[3][1] = 0.0f;
	mtx.Elements[3][2] = 0.0f;
	mtx.Elements[3][3] = 1.0f;
	
	return mtx;
}

//--------------------------------------------------------------------------------
//  QuaternionToEuler
//	スクリーン上のポインタから3D空間の射線を求める関数
//--------------------------------------------------------------------------------
Vector3	CKFMath::QuaternionToEuler(const Quaternion& quaternion)
{
	Vector3 vRot;
#ifdef USING_DIRECTX
	D3DXQUATERNION q = quaternion;
	D3DXQuaternionToAxisAngle(&q, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), &vRot.X);
	D3DXQuaternionToAxisAngle(&q, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &vRot.Y);
	D3DXQuaternionToAxisAngle(&q, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &vRot.Z);
#else
	float fX = quaternion.X;
	float fY = quaternion.Y;
	float fZ = quaternion.Z;
	float fW = quaternion.W;
	vRot.X = atan2f(2.0f * (fW * fZ + fX * fY), 1.0f - 2.0f * (fZ * fZ + fX * fX));
	vRot.Y = asinf(2.0f * (fW * fX - fY * fZ));
	vRot.Z = atan2f(2.0f * (fW * fY + fZ * fX), 1.0f - 2.0f * (fX * fX + fY * fY));
#endif // USING_DIRECTX
	return vRot;
}

//--------------------------------------------------------------------------------
//  Ray
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  CalculatePickingRay
//	スクリーン上のポインタから3D空間の射線を求める関数
//--------------------------------------------------------------------------------
Ray CKFMath::CalculatePickingRay(const Vector2 &vScreenPos, const float &fViewportWidth, const float &fViewportHeight, const float &fProjMtx00, const float &fProjMtx11, const Matrix44 &mtxViewInverse)
{
	Ray ray = ChangePosToRay(vScreenPos, fViewportWidth, fViewportHeight, fProjMtx00, fProjMtx11);
	TransformRay(ray, mtxViewInverse);
	return ray;
}

//--------------------------------------------------------------------------------
//  ChangePosToRay
//	スクリーン上のポインタから3D空間の射線を求める関数
//--------------------------------------------------------------------------------
Ray CKFMath::ChangePosToRay(const Vector2& vScreenPos, const float& fViewportWidth, const float &fViewportHeight, const float &fProjMtx00, const float &fProjMtx11)
{
	Vector2 vPos3D;

	vPos3D.X = (((2.0f * vScreenPos.X) / fViewportWidth) - 1.0f) / fProjMtx00;
	vPos3D.Y = (((-2.0f * vScreenPos.Y) / fViewportHeight) + 1.0f) / fProjMtx11;

	Ray ray;
	ray.Origin = Vector3(0.0f);
	ray.Direction = Vector3(vPos3D.X, vPos3D.Y, 1.0f);

	return ray;
}

//--------------------------------------------------------------------------------
//  TransformRay
//	レイをカメラ向きに変換関数
//--------------------------------------------------------------------------------
void CKFMath::TransformRay(Ray& ray, const Matrix44& mtxTrans)
{
	ray.Origin = Vec3TransformCoord(ray.Origin, mtxTrans);
	ray.Direction = Vec3TransformNormal(ray.Direction, mtxTrans);
	VecNormalize(ray.Direction);
}

//--------------------------------------------------------------------------------
//  Lerp
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  LerpVec3
//	Vector3を線形補間方式で移動する
//--------------------------------------------------------------------------------
Vector3 CKFMath::LerpVec3(const Vector3& vVecFrom, const Vector3& vVecTo, const float& fTime)
{
	float fPersent = fTime;
	fPersent = fPersent < 0.0f ? 0.0f : fPersent > 1.0f ? 1.0f : fPersent;
	Vector3 vDelta = vVecTo - vVecFrom;
	Vector3 vVec3 = vVecFrom + vDelta * fPersent;
	return vVec3;
}

//--------------------------------------------------------------------------------
//  LerpNormal
//	法線を線形補間方式で回転する
//--------------------------------------------------------------------------------
Vector3 CKFMath::LerpNormal(const Vector3& vNormalFrom, const Vector3& vNormalTo, const float& fTime)
{
	Vector3 vNormal = LerpVec3(vNormalFrom, vNormalTo, fTime);
	VecNormalize(vNormal);
	return vNormal;
}

//--------------------------------------------------------------------------------
//  LerpFloat
//	Floatを線形補間方式で回転する
//--------------------------------------------------------------------------------
float CKFMath::LerpFloat(const float& fFrom, const float& fTo, const float& fTime)
{
	float fPersent = fTime;
	fPersent = fPersent < 0.0f ? 0.0f : fPersent > 1.0f ? 1.0f : fPersent;
	float fDelta = fTo - fFrom;
	float fAnswer = fFrom + fDelta * fPersent;
	return fAnswer;
}

//--------------------------------------------------------------------------------
//  LerpColor
//	Colorを線形補間方式で回転する
//--------------------------------------------------------------------------------
Color CKFMath::LerpColor(const Color& cFrom, const Color& cTo, const float& fTime)
{
	float fPersent = fTime;
	fPersent = fPersent < 0.0f ? 0.0f : fPersent > 1.0f ? 1.0f : fPersent;
	Color cDelta = cTo - cFrom;
	Color cAnswer = cFrom + cDelta * fPersent;
	return cAnswer;
}

//--------------------------------------------------------------------------------
//  SlerpQuaternion
//	Quaternionを球形補間方式で回転する
//--------------------------------------------------------------------------------
Quaternion CKFMath::SlerpQuaternion(const Quaternion& qFrom, const Quaternion& qTo, const float& fTime)
{
	//if (fTime <= 0.0f) { return qFrom; }
	//if (fTime >= 1.0f) { return qTo; }

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
	
	D3DXQUATERNION qFromDX = qFrom;
	D3DXQUATERNION qToDX = qTo;
	D3DXQUATERNION qOut;
	D3DXQuaternionSlerp(&qOut, &qFromDX, &qToDX, fTime);
	Quaternion qResult;
	qResult.X = qOut.x;
	qResult.Y = qOut.y;
	qResult.Z = qOut.z;
	qResult.W = qOut.w;
	return qResult;
}

//--------------------------------------------------------------------------------
//  Others
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  NormalizeRotInTwoPi(float)
//	回転を-2Piから2Piの間にする
//--------------------------------------------------------------------------------
void CKFMath::ClampFloat(float& fValue, const float& fMin, const float& fMax)
{
	if (fValue < fMin) { fValue = fMin; }
	else if (fValue > fMax) { fValue = fMax; }
}

//--------------------------------------------------------------------------------
//  Others
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  NormalizeColor
//	色を0.0fから1.0fの間にする
//--------------------------------------------------------------------------------
void CKFMath::NormalizeColor(Color& cColor)
{
	cColor.R = cColor.R < 0.0f ? 0.0f : cColor.R > 1.0f ? 1.0f : cColor.R;
	cColor.G = cColor.G < 0.0f ? 0.0f : cColor.G > 1.0f ? 1.0f : cColor.G;
	cColor.B = cColor.B < 0.0f ? 0.0f : cColor.B > 1.0f ? 1.0f : cColor.B;
	cColor.A = cColor.A < 0.0f ? 0.0f : cColor.A > 1.0f ? 1.0f : cColor.A;
}

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
float CKFMath::CalculateZDepth(const Vector3& vPos, const Vector3& vCameraEye, const Vector3& vCameraAt)
{
	Vector3 vCameraForward = vCameraAt - vCameraEye;
	Vector3 vPosToCamera = vPos - vCameraEye;
	float fDepth = Vec3Dot(vCameraForward, vPosToCamera);
	return fDepth;
}
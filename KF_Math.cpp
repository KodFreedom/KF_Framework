//--------------------------------------------------------------------------------
//	演算用関数
//	KF_Math.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "KF_Math.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  CKFVec2
//--------------------------------------------------------------------------------
#ifdef USING_DIRECTX9
//--------------------------------------------------------------------------------
//  キャスト(D3DXVECTOR2)
//	DXの環境のためオーバーロードする
//--------------------------------------------------------------------------------
CKFVec2::operator D3DXVECTOR2() const
{
	D3DXVECTOR2 vValue;
	vValue.x = m_fX;
	vValue.y = m_fY;

	return vValue;
}
#endif

//--------------------------------------------------------------------------------
//  operator=
//--------------------------------------------------------------------------------
CKFVec2 &CKFVec2::operator=(const CKFVec2 &vValue)
{
	m_fX = vValue.m_fX;
	m_fY = vValue.m_fY;

	return *this;
}

//--------------------------------------------------------------------------------
//  operator==
//--------------------------------------------------------------------------------
bool CKFVec2::operator==(const CKFVec2 &vValue) const
{
	if (m_fX == vValue.m_fX && m_fY == vValue.m_fY)
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------
//  operator+
//--------------------------------------------------------------------------------
CKFVec2 CKFVec2::operator+(const CKFVec2 &vValue) const
{
	CKFVec2 vAnswer;

	vAnswer.m_fX = m_fX + vValue.m_fX;
	vAnswer.m_fY = m_fY + vValue.m_fY;

	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator+=
//--------------------------------------------------------------------------------
void CKFVec2::operator+=(const CKFVec2 &vValue)
{
	m_fX += vValue.m_fX;
	m_fY += vValue.m_fY;
}

//--------------------------------------------------------------------------------
//  operator*
//--------------------------------------------------------------------------------
float CKFVec2::operator*(const CKFVec2& vValue) const
{
	float fAnswer = m_fX * vValue.m_fY - m_fY * vValue.m_fX;
	return fAnswer;
}

//--------------------------------------------------------------------------------
//  CKFVec3
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  キャスト(CKFVec2)
//--------------------------------------------------------------------------------
CKFVec3::operator CKFVec2() const
{
	CKFVec2 vValue;
	vValue.m_fX = m_fX;
	vValue.m_fY = m_fY;

	return vValue;
}

#ifdef USING_DIRECTX9
//--------------------------------------------------------------------------------
//  キャスト(D3DXVECTOR3)
//	DXの環境のためオーバーロードする
//--------------------------------------------------------------------------------
CKFVec3::operator D3DXVECTOR3() const
{
	D3DXVECTOR3 vValue;
	vValue.x = m_fX;
	vValue.y = m_fY;
	vValue.z = m_fZ;

	return vValue;
}
#endif

//--------------------------------------------------------------------------------
//  operator=
//--------------------------------------------------------------------------------
CKFVec3 &CKFVec3::operator=(const CKFVec3 &vValue)
{
	m_fX = vValue.m_fX;
	m_fY = vValue.m_fY;
	m_fZ = vValue.m_fZ;

	return *this;
}

//--------------------------------------------------------------------------------
//  operator==
//--------------------------------------------------------------------------------
bool CKFVec3::operator==(const CKFVec3 &vValue) const
{
	if (m_fX == vValue.m_fX && m_fY == vValue.m_fY && m_fZ == vValue.m_fZ)
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------
//  operator!=
//--------------------------------------------------------------------------------
bool CKFVec3::operator!=(const CKFVec3 &vValue) const
{
	if (m_fX != vValue.m_fX || m_fY != vValue.m_fY || m_fZ != vValue.m_fZ)
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------
//  operator+
//--------------------------------------------------------------------------------
CKFVec3 CKFVec3::operator+(const CKFVec3 &vValue) const
{
	CKFVec3 vAnswer;

	vAnswer.m_fX = m_fX + vValue.m_fX;
	vAnswer.m_fY = m_fY + vValue.m_fY;
	vAnswer.m_fZ = m_fZ + vValue.m_fZ;

	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator+=
//--------------------------------------------------------------------------------
void CKFVec3::operator+=(const CKFVec3 &vValue)
{
	m_fX += vValue.m_fX;
	m_fY += vValue.m_fY;
	m_fZ += vValue.m_fZ;
}

//--------------------------------------------------------------------------------
//  operator-
//--------------------------------------------------------------------------------
CKFVec3 CKFVec3::operator-(const CKFVec3 &vValue) const
{
	CKFVec3 vAnswer;

	vAnswer.m_fX = m_fX - vValue.m_fX;
	vAnswer.m_fY = m_fY - vValue.m_fY;
	vAnswer.m_fZ = m_fZ - vValue.m_fZ;

	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator-=
//--------------------------------------------------------------------------------
void CKFVec3::operator-=(const CKFVec3 &vValue)
{
	m_fX -= vValue.m_fX;
	m_fY -= vValue.m_fY;
	m_fZ -= vValue.m_fZ;
}

//--------------------------------------------------------------------------------
//  operator*(float)
//--------------------------------------------------------------------------------
CKFVec3 CKFVec3::operator*(const float &fValue) const
{
	CKFVec3 vAnswer;

	vAnswer.m_fX = m_fX * fValue;
	vAnswer.m_fY = m_fY * fValue;
	vAnswer.m_fZ = m_fZ * fValue;

	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=(float)
//--------------------------------------------------------------------------------
void CKFVec3::operator*=(const float &fValue)
{
	m_fX *= fValue;
	m_fY *= fValue;
	m_fZ *= fValue;
}

//--------------------------------------------------------------------------------
//  operator*(Vec3 外積(Cross))
//--------------------------------------------------------------------------------
CKFVec3 CKFVec3::operator*(const CKFVec3 &vValue) const
{
	CKFVec3 vAnswer;

	vAnswer.m_fX = m_fY * vValue.m_fZ - vValue.m_fY * m_fZ;
	vAnswer.m_fY = m_fZ * vValue.m_fX - vValue.m_fZ * m_fX;
	vAnswer.m_fZ = m_fX * vValue.m_fY - vValue.m_fX * m_fY;

	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=(Vec3 外積(Cross))
//--------------------------------------------------------------------------------
void CKFVec3::operator*=(const CKFVec3 &vValue)
{
	CKFVec3 vThis = *this;

	m_fX = vThis.m_fY * vValue.m_fZ - vValue.m_fY * vThis.m_fZ;
	m_fY = vThis.m_fZ * vValue.m_fX - vValue.m_fZ * vThis.m_fX;
	m_fZ = vThis.m_fX * vValue.m_fY - vValue.m_fX * vThis.m_fY;
}

//--------------------------------------------------------------------------------
//  operator/(float)
//--------------------------------------------------------------------------------
CKFVec3 CKFVec3::operator/(const float &fValue) const
{
	CKFVec3 vAnswer = CKFVec3(0.0f);

	if (fValue != 0.0f)
	{
		vAnswer.m_fX = m_fX / fValue;
		vAnswer.m_fY = m_fY / fValue;
		vAnswer.m_fZ = m_fZ / fValue;
	}

	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator/=(float)
//--------------------------------------------------------------------------------
void CKFVec3::operator/=(const float &fValue)
{
	CKFVec3 vAnswer = CKFVec3(0.0f);

	if (fValue != 0.0f)
	{
		vAnswer.m_fX = m_fX / fValue;
		vAnswer.m_fY = m_fY / fValue;
		vAnswer.m_fZ = m_fZ / fValue;
	}

	*this = vAnswer;
}

//--------------------------------------------------------------------------------
//  CKFMtx44
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CKFMtx44::CKFMtx44()
{
	float afMtx[4][4] = {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f } };
	memcpy(m_af, afMtx, sizeof(m_af));
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CKFMtx44::CKFMtx44(
	const float &f11, const float &f12, const float &f13, const float &f14,
	const float &f21, const float &f22, const float &f23, const float &f24,
	const float &f31, const float &f32, const float &f33, const float &f34,
	const float &f41, const float &f42, const float &f43, const float &f44)
{
	m_af[0][0] = f11;
	m_af[0][1] = f12;
	m_af[0][2] = f13;
	m_af[0][3] = f14;
	m_af[1][0] = f21;
	m_af[1][1] = f22;
	m_af[1][2] = f23;
	m_af[1][3] = f24;
	m_af[2][0] = f31;
	m_af[2][1] = f32;
	m_af[2][2] = f33;
	m_af[2][3] = f34;
	m_af[3][0] = f41;
	m_af[3][1] = f42;
	m_af[3][2] = f43;
	m_af[3][3] = f44;
}

#ifdef USING_DIRECTX9
//--------------------------------------------------------------------------------
//  キャスト(D3DXVECTOR3)
//	DXの環境のためオーバーロードする
//--------------------------------------------------------------------------------
CKFMtx44::operator D3DXMATRIX() const
{
	D3DXMATRIX mtxValue;
	
	for (int nCntY = 0; nCntY < 4; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			mtxValue(nCntY, nCntX) = m_af[nCntY][nCntX];
		}
	}

	return mtxValue;
}
#endif

//--------------------------------------------------------------------------------
//  operator*
//--------------------------------------------------------------------------------
CKFMtx44 &CKFMtx44::operator=(const CKFMtx44 &mtxValue)
{
	for (int nCntY = 0; nCntY < 4; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			m_af[nCntY][nCntX] = mtxValue.m_af[nCntY][nCntX];
		}
	}

	return *this;
}

//--------------------------------------------------------------------------------
//  operator*
//--------------------------------------------------------------------------------
CKFMtx44 CKFMtx44::operator*(const CKFMtx44 &mtxValue) const
{
	CKFMtx44 mtxAnswer;

	for (int nCntY = 0; nCntY < 4; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			mtxAnswer.m_af[nCntY][nCntX] = 
				m_af[nCntY][0] * mtxValue.m_af[0][nCntX] + 
				m_af[nCntY][1] * mtxValue.m_af[1][nCntX] + 
				m_af[nCntY][2] * mtxValue.m_af[2][nCntX] + 
				m_af[nCntY][3] * mtxValue.m_af[3][nCntX];
		}								    
	}									   
										   
	return mtxAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=
//--------------------------------------------------------------------------------
void CKFMtx44::operator*=(const CKFMtx44 &mtxValue)
{
	CKFMtx44 mtxCpy = *this;

	for (int nCntY = 0; nCntY < 4; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			m_af[nCntY][nCntX] =
				mtxCpy.m_af[nCntY][0] * mtxValue.m_af[0][nCntX] +
				mtxCpy.m_af[nCntY][1] * mtxValue.m_af[1][nCntX] +
				mtxCpy.m_af[nCntY][2] * mtxValue.m_af[2][nCntX] +
				mtxCpy.m_af[nCntY][3] * mtxValue.m_af[3][nCntX];
		}
	}
}

//--------------------------------------------------------------------------------
//  CKFVec4
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  operator*(Matrix44)
//--------------------------------------------------------------------------------
CKFVec4 CKFVec4::operator*(const CKFMtx44 &mtxValue) const
{
	CKFVec4 vAnswer;

	vAnswer.m_fX =
		m_fX * mtxValue.m_af[0][0] +
		m_fY * mtxValue.m_af[1][0] +
		m_fZ * mtxValue.m_af[2][0] +
		m_fW * mtxValue.m_af[3][0];
	vAnswer.m_fY =
		m_fX * mtxValue.m_af[0][1] +
		m_fY * mtxValue.m_af[1][1] +
		m_fZ * mtxValue.m_af[2][1] +
		m_fW * mtxValue.m_af[3][1];
	vAnswer.m_fZ =
		m_fX * mtxValue.m_af[0][2] +
		m_fY * mtxValue.m_af[1][2] +
		m_fZ * mtxValue.m_af[2][2] +
		m_fW * mtxValue.m_af[3][2];
	vAnswer.m_fW =
		m_fX * mtxValue.m_af[0][3] +
		m_fY * mtxValue.m_af[1][3] +
		m_fZ * mtxValue.m_af[2][3] +
		m_fW * mtxValue.m_af[3][3];

	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=(Matrix44)
//--------------------------------------------------------------------------------
void CKFVec4::operator*=(const CKFMtx44 &mtxValue)
{
	CKFVec4 vThis = *this;

	m_fX =
		vThis.m_fX * mtxValue.m_af[0][0] +
		vThis.m_fY * mtxValue.m_af[1][0] +
		vThis.m_fZ * mtxValue.m_af[2][0] +
		vThis.m_fW * mtxValue.m_af[3][0];
	m_fY =
		vThis.m_fX * mtxValue.m_af[0][1] +
		vThis.m_fY * mtxValue.m_af[1][1] +
		vThis.m_fZ * mtxValue.m_af[2][1] +
		vThis.m_fW * mtxValue.m_af[3][1];
	m_fZ =
		vThis.m_fX * mtxValue.m_af[0][2] +
		vThis.m_fY * mtxValue.m_af[1][2] +
		vThis.m_fZ * mtxValue.m_af[2][2] +
		vThis.m_fW * mtxValue.m_af[3][2];
	m_fW =
		vThis.m_fX * mtxValue.m_af[0][3] +
		vThis.m_fY * mtxValue.m_af[1][3] +
		vThis.m_fZ * mtxValue.m_af[2][3] +
		vThis.m_fW * mtxValue.m_af[3][3];
}

//--------------------------------------------------------------------------------
//  CKFColor
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  operator=
//--------------------------------------------------------------------------------
CKFColor &CKFColor::operator=(const CKFColor &vValue)
{
	m_fR = vValue.m_fR;
	m_fG = vValue.m_fG;
	m_fB = vValue.m_fB;
	m_fA = vValue.m_fA;

	return *this;
}

//--------------------------------------------------------------------------------
//  operator==
//--------------------------------------------------------------------------------
bool CKFColor::operator==(const CKFColor &vValue)
{
	if (m_fR == vValue.m_fR && m_fG == vValue.m_fG && m_fB == vValue.m_fB && m_fA == vValue.m_fA)
	{
		return true;
	}

	return false;
}

#ifdef USING_DIRECTX9
//--------------------------------------------------------------------------------
//  キャスト(D3DCOLORVALUE)
//	DXの環境のため(Light)オーバーロードする
//--------------------------------------------------------------------------------
CKFColor::operator D3DCOLORVALUE() const
{
	D3DCOLORVALUE vAnswer;
	vAnswer.r = m_fR;
	vAnswer.g = m_fG;
	vAnswer.b = m_fB;
	vAnswer.a = m_fA;

	return vAnswer;
}

//--------------------------------------------------------------------------------
//  キャスト(unsigned long)
//	DXの環境で頂点情報のカラーはDWORD(unsigned long)のためオーバーロードする
//--------------------------------------------------------------------------------
CKFColor::operator unsigned long() const
{
	return (unsigned long)((((unsigned long)(m_fA * 255.0f) & 0xff) << 24) 
		| (((unsigned long)(m_fR * 255.0f) & 0xff) << 16) 
		| (((unsigned long)(m_fG * 255.0f) & 0xff) << 8) 
		| ((unsigned long)(m_fB * 255.0f) & 0xff));
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
	if (nMin >= nMax)
	{
		return nMin;
	}

	int nWork = rand() % (nMax - nMin);

	nWork = nWork + nMin;

	return nWork;
}

//--------------------------------------------------------------------------------
//  GetRandomFloat
//--------------------------------------------------------------------------------
float CKFMath::GetRandomFloat(const float &fMin, const float &fMax)
{
	if (fMin >= fMax)
	{
		return fMin;
	}

	float fWork = (rand() % 10000) * 0.0001f;

	fWork = fWork * (fMax - fMin) + fMin;

	return fWork;
}

//--------------------------------------------------------------------------------
//  GetRandomVec3
//--------------------------------------------------------------------------------
CKFVec3 CKFMath::GetRandomVec3(const CKFVec3 &vMin, const CKFVec3 &vMax)
{
	CKFVec3 vWork;

	vWork.m_fX = GetRandomFloat(vMin.m_fX, vMax.m_fX);
	vWork.m_fY = GetRandomFloat(vMin.m_fY, vMax.m_fY);
	vWork.m_fZ = GetRandomFloat(vMin.m_fZ, vMax.m_fZ);

	return vWork;
}

//--------------------------------------------------------------------------------
//  Vector
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Magnitude(Vector2)
//--------------------------------------------------------------------------------
float CKFMath::VecMagnitude(const CKFVec2 &vValue)
{
	float fMagnitude;

	fMagnitude = sqrtf(vValue.m_fX * vValue.m_fX + vValue.m_fY * vValue.m_fY);

	return fMagnitude;
}

//--------------------------------------------------------------------------------
//  Magnitude(Vector3)
//--------------------------------------------------------------------------------
float CKFMath::VecMagnitude(const CKFVec3 &vValue)
{
	float fMagnitude;

	fMagnitude = sqrtf(vValue.m_fX * vValue.m_fX + vValue.m_fY * vValue.m_fY + vValue.m_fZ * vValue.m_fZ);

	return fMagnitude;
}

//--------------------------------------------------------------------------------
//  Normalize(Vector2)
//--------------------------------------------------------------------------------
void CKFMath::VecNormalize(CKFVec2 *pVec)
{
	if (pVec == NULL) { return; }

	float fMagnitude = VecMagnitude(*pVec);
	if (fMagnitude == 0.0f) { return; }

	pVec->m_fX /= fMagnitude;
	pVec->m_fY /= fMagnitude;
}

//--------------------------------------------------------------------------------
//  Normalize(Vector3)
//--------------------------------------------------------------------------------
void CKFMath::VecNormalize(CKFVec3 *pVec)
{
	if (pVec == NULL) { return; }

	float fMagnitude = VecMagnitude(*pVec);
	if (fMagnitude == 0.0f) { return; }

	pVec->m_fX /= fMagnitude;
	pVec->m_fY /= fMagnitude;
	pVec->m_fZ /= fMagnitude;
}

//--------------------------------------------------------------------------------
//  乗算(Dot)(Vector3)
//--------------------------------------------------------------------------------
float CKFMath::Vec3Dot(const CKFVec3 &vVecL, const CKFVec3 &vVecR)
{
	float fDot = vVecL.m_fX * vVecR.m_fX
		+ vVecL.m_fY * vVecR.m_fY
		+ vVecL.m_fZ * vVecR.m_fZ;

	return fDot;
}

//--------------------------------------------------------------------------------
//  VecDistance(Vector3)
//--------------------------------------------------------------------------------
float CKFMath::VecDistance(const CKFVec3 &vVecL, const CKFVec3 &vVecR)
{
	CKFVec3 vDis = vVecL - vVecR;
	float fDis = VecMagnitude(vDis);
	return fDis;
}

//--------------------------------------------------------------------------------
//  Vec3TransformCoord
//	回転行列にとってVec3を回転する
//--------------------------------------------------------------------------------
void CKFMath::Vec3TransformCoord(CKFVec3 *pVec, const CKFMtx44 &mtxRot)
{
	CKFVec4 vVec = CKFVec4(*pVec, 1.0f);
	vVec *= mtxRot;

	if (vVec.m_fW != 0.0f)
	{
		pVec->m_fX = vVec.m_fX / vVec.m_fW;
		pVec->m_fY = vVec.m_fY / vVec.m_fW;
		pVec->m_fZ = vVec.m_fZ / vVec.m_fW;
	}
}

//--------------------------------------------------------------------------------
//  Vec3TransformNormal
//	回転行列にとってVec3(ベクトル)を回転する
//--------------------------------------------------------------------------------
void CKFMath::Vec3TransformNormal(CKFVec3 *pVec, const CKFMtx44 &mtxRot)
{
	CKFVec4 vVec = CKFVec4(*pVec, 0.0f);
	vVec *= mtxRot;

	pVec->m_fX = vVec.m_fX;
	pVec->m_fY = vVec.m_fY;
	pVec->m_fZ = vVec.m_fZ;
}

//--------------------------------------------------------------------------------
//  RadianBetweenVec
//	Vec2(ベクトル)間の角度の算出
//--------------------------------------------------------------------------------
float CKFMath::RadianBetweenVec(const CKFVec2& vVecL, const CKFVec2& vVecR)
{
	float fLengthL = VecMagnitude(vVecL);
	float fLengthR = VecMagnitude(vVecR);
	if (vVecL == vVecR || fLengthL == 0.0f || fLengthR == 0.0f) { return 0.0f; }
	float fCross = vVecL * vVecR;
	float fRot = asinf(fCross / (fLengthL * fLengthR));
	return fRot;
}

//--------------------------------------------------------------------------------
//  RadianBetweenVec3
//	回転行列にとってVec3(ベクトル)を回転する
//--------------------------------------------------------------------------------
float CKFMath::RadianBetweenVec(const CKFVec3 &vVecL, const CKFVec3 &vVecR)
{
	float fLengthL = VecMagnitude(vVecL);
	float fLengthR = VecMagnitude(vVecR);
	if (vVecL == vVecR || fLengthL == 0.0f || fLengthR == 0.0f) { return 0.0f; }

	float fRot = 0.0f;
	float fWork;

	//左右判断
	fWork = vVecR.m_fZ * vVecL.m_fX - vVecR.m_fX * vVecL.m_fZ;
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
//  LerpVec3
//	Vector3を線形補間方式で移動する
//--------------------------------------------------------------------------------
CKFVec3	CKFMath::EulerBetweenVec3(const CKFVec3& vVecFrom, const CKFVec3& vVecTo)
{
	CKFVec3 vRot;

	//X軸回転
	vRot.m_fX = RadianBetweenVec(CKFVec2(vVecFrom.m_fY, vVecFrom.m_fZ), CKFVec2(vVecTo.m_fY, vVecTo.m_fZ));

	//Y軸回転
	vRot.m_fY = RadianBetweenVec(CKFVec2(vVecFrom.m_fZ, vVecFrom.m_fX), CKFVec2(vVecTo.m_fZ, vVecTo.m_fX));

	//Z軸回転
	vRot.m_fZ = RadianBetweenVec(CKFVec2(vVecFrom.m_fX, vVecFrom.m_fY), CKFVec2(vVecTo.m_fX, vVecTo.m_fY));

	return vRot;
}

//--------------------------------------------------------------------------------
//  LerpVec3
//	Vector3を線形補間方式で移動する
//--------------------------------------------------------------------------------
CKFVec3 CKFMath::LerpVec3(const CKFVec3& vVecFrom, const CKFVec3& vVecTo, const float& fTime)
{
	float fPersent = fTime / 1.0f;
	fPersent = fPersent < 0.0f ? 0.0f : fPersent > 1.0f ? 1.0f : fPersent;
	CKFVec3 vDelta = vVecTo - vVecFrom;
	CKFVec3 vVec3 = vVecFrom + vDelta * fPersent;
	return vVec3;
}

//--------------------------------------------------------------------------------
//  LerpNormal
//	法線を線形補間方式で回転する
//--------------------------------------------------------------------------------
CKFVec3 CKFMath::LerpNormal(const CKFVec3& vNormalFrom, const CKFVec3& vNormalTo, const float& fTime)
{
	CKFVec3 vNormal = LerpVec3(vNormalFrom, vNormalTo, fTime);
	VecNormalize(&vNormal);
	return vNormal;
}

//--------------------------------------------------------------------------------
//  Matrix
//--------------------------------------------------------------------------------
#ifdef USING_DIRECTX9
//--------------------------------------------------------------------------------
//  ChangeDXMtxToMtx44
//--------------------------------------------------------------------------------
CKFMtx44 CKFMath::ChangeDXMtxToMtx44(const D3DXMATRIX &mtx)
{
	CKFMtx44 mtxOut;
	for (int nCntY = 0; nCntY < 4; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			mtxOut.m_af[nCntY][nCntX] = mtx(nCntY, nCntX);
		}
	}

	return mtxOut;
}
#endif

//--------------------------------------------------------------------------------
//  MtxIdentity
//	単位行列の作成
//--------------------------------------------------------------------------------
void CKFMath::MtxIdentity(CKFMtx44 *pMtx)
{
	float afMtx[4][4] = {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f } };
	memcpy(pMtx->m_af, afMtx, sizeof(afMtx));
}

//--------------------------------------------------------------------------------
//  MtxRotAxis
//	回転行列の作成
//--------------------------------------------------------------------------------
void CKFMath::MtxRotAxis(CKFMtx44 *pMtxRot, const CKFVec3 &vAxis, const float &fAngle)
{
	//軸をノーマライズする
	CKFVec3 vAxisCpy = vAxis;
	VecNormalize(&vAxisCpy);

	//計算用変数
	float fX = vAxisCpy.m_fX;
	float fY = vAxisCpy.m_fY;
	float fZ = vAxisCpy.m_fZ;
	float fSin = sinf(fAngle);
	float fCos = cosf(fAngle);

	pMtxRot->m_af[0][0] = fCos + fX * fX * (1 - fCos);
	pMtxRot->m_af[0][1] = fX * fY * (1 - fCos) + fZ * fSin;
	pMtxRot->m_af[0][2] = fX * fZ * (1 - fCos) - fY * fSin;
	pMtxRot->m_af[0][3] = 0.0f;
	pMtxRot->m_af[1][0] = fX * fY * (1 - fCos) - fZ * fSin;
	pMtxRot->m_af[1][1] = fCos + fY * fY * (1 - fCos);
	pMtxRot->m_af[1][2] = fZ * fY * (1 - fCos) + fX * fSin;
	pMtxRot->m_af[1][3] = 0.0f;
	pMtxRot->m_af[2][0] = fX * fZ * (1 - fCos) + fY * fSin;
	pMtxRot->m_af[2][1] = fY * fZ * (1 - fCos) - fX * fSin;
	pMtxRot->m_af[2][2] = fCos + fZ * fZ * (1 - fCos);
	pMtxRot->m_af[2][3] = 0.0f;
	pMtxRot->m_af[3][0] = 0.0f;
	pMtxRot->m_af[3][1] = 0.0f;
	pMtxRot->m_af[3][2] = 0.0f;
	pMtxRot->m_af[3][3] = 1.0f;

	////回転の距離算出
	//float fLength = sinf(fAngle);   

	////軸に距離をかける
	//vAxisCpy *= fLength;

	//計算用変数
	//float fA = vAxisCpy.m_fX;
	//float fB = vAxisCpy.m_fY;  
	//float fC = vAxisCpy.m_fZ;     
	//float fD = cosf(fAngle);  
	//float fT = fLength == 0 ? 0 : (1 - fD) / (fLength * fLength);
	//
	//*pMtxRot = CKFMtx44(
	//	fA * fT * fA + fD, fB * fT * fA + fC, fC * fT * fA - fB, 0.0f,
	//	fA * fT * fB - fC, fB * fT * fB + fD, fC * fT * fB + fA, 0.0f,
	//	fA * fT * fC + fB, fB * fT * fC - fA, fC * fT * fC + fD, 0.0f,
	//	0.0f, 0.0f, 0.0f, 1.0f);
}

//--------------------------------------------------------------------------------
//  MtxRotationYawPitchRoll
//	回転行列の作成(mtxY*mtxX*mtxZ)
//--------------------------------------------------------------------------------
void CKFMath::MtxRotationYawPitchRoll(CKFMtx44 *pMtxRot, const CKFVec3 &vRot)
{
	float fSinX = sinf(vRot.m_fX); 
	float fCosX = cosf(vRot.m_fX); 
	float fSinY = sinf(vRot.m_fY);
	float fCosY = cosf(vRot.m_fY);
	float fSinZ = sinf(vRot.m_fZ);
	float fCosZ = cosf(vRot.m_fZ);

	pMtxRot->m_af[0][0] = fCosY * fCosZ + fSinX * fSinY * fSinZ;
	pMtxRot->m_af[0][1] = fCosX * fSinZ;
	pMtxRot->m_af[0][2] = -fSinY * fCosZ + fSinX * fCosY * fSinZ;
	pMtxRot->m_af[0][3] = 0.0f;
	pMtxRot->m_af[1][0] = -fCosY * fSinZ + fSinX * fSinY * fCosZ;
	pMtxRot->m_af[1][1] = fCosX * fCosZ;
	pMtxRot->m_af[1][2] = fSinY * fSinZ + fSinX * fCosY * fCosZ;
	pMtxRot->m_af[1][3] = 0.0f;
	pMtxRot->m_af[2][0] = fCosX * fSinY;
	pMtxRot->m_af[2][1] = -fSinX;
	pMtxRot->m_af[2][2] = fCosX * fCosY;
	pMtxRot->m_af[2][3] = 0.0f;
	pMtxRot->m_af[3][0] = 0.0f;
	pMtxRot->m_af[3][1] = 0.0f;
	pMtxRot->m_af[3][2] = 0.0f;
	pMtxRot->m_af[3][3] = 1.0f;
}

//--------------------------------------------------------------------------------
//	MtxTranslation
//	平行移動行列の作成
//--------------------------------------------------------------------------------
void CKFMath::MtxTranslation(CKFMtx44 *pMtxTrans, const CKFVec3 &vPos)
{
	MtxIdentity(pMtxTrans);
	pMtxTrans->m_af[3][0] = vPos.m_fX;
	pMtxTrans->m_af[3][1] = vPos.m_fY;
	pMtxTrans->m_af[3][2] = vPos.m_fZ;
}

//--------------------------------------------------------------------------------
//  Ray
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  CalculatePickingRay
//	スクリーン上のポインタから3D空間の射線を求める関数
//--------------------------------------------------------------------------------
CKFRay CKFMath::CalculatePickingRay(const CKFVec2 &vScreenPos, const float &fViewportWidth, const float &fViewportHeight, const float &fProjMtx00, const float &fProjMtx11, const CKFMtx44 &mtxViewInverse)
{
	CKFRay ray = ChangePosToRay(vScreenPos, fViewportWidth, fViewportHeight, fProjMtx00, fProjMtx11);
	TransformRay(&ray, mtxViewInverse);
	return ray;
}

//--------------------------------------------------------------------------------
//  ChangePosToRay
//	スクリーン上のポインタから3D空間の射線を求める関数
//--------------------------------------------------------------------------------
CKFRay CKFMath::ChangePosToRay(const CKFVec2 &vScreenPos, const float &fViewportWidth, const float &fViewportHeight, const float &fProjMtx00, const float &fProjMtx11)
{
	CKFVec2 vPos3D;

	vPos3D.m_fX = (((2.0f * vScreenPos.m_fX) / fViewportWidth) - 1.0f) / fProjMtx00;
	vPos3D.m_fY = (((-2.0f * vScreenPos.m_fY) / fViewportHeight) + 1.0f) / fProjMtx11;

	CKFRay ray;
	ray.m_vOrigin = CKFVec3(0.0f);
	ray.m_vDirection = CKFVec3(vPos3D.m_fX, vPos3D.m_fY, 1.0f);

	return ray;
}

//--------------------------------------------------------------------------------
//  TransformRay
//	レイをカメラ向きに変換関数
//--------------------------------------------------------------------------------
void CKFMath::TransformRay(CKFRay *pRay, const CKFMtx44 &mtxTrans)
{
	Vec3TransformCoord(&pRay->m_vOrigin, mtxTrans);
	Vec3TransformNormal(&pRay->m_vDirection, mtxTrans);
	VecNormalize(&pRay->m_vDirection);
}

//--------------------------------------------------------------------------------
//  ContactRaytoSphere
//	レイとスフィアの当たり判定
//--------------------------------------------------------------------------------
CKFMath::RTS_INFO CKFMath::ContactRayToSphere(const CKFRay &ray, const CKFVec3 &vSpherePos, const float &fRadius)
{
	RTS_INFO info;
	CKFVec3 vOriginToSphere;
	float fWorkA, fWorkB, fTimeA, fTimeB;
	float fDiscriminant;

	info.bIsContact = false;
	info.fTimingMin = 0.0f;

	vOriginToSphere = ray.m_vOrigin - vSpherePos;
	fWorkA = 2.0f * Vec3Dot(ray.m_vDirection, vOriginToSphere);
	fWorkB = Vec3Dot(vOriginToSphere, vOriginToSphere) - fRadius * fRadius;

	fDiscriminant = fWorkA * fWorkA - 4.0f * fWorkB;

	if (fDiscriminant < 0.0f)
	{
		return info;
	}

	fDiscriminant = sqrtf(fDiscriminant);

	fTimeA = (-fWorkA + fDiscriminant) / 2.0f;
	fTimeB = (-fWorkA - fDiscriminant) / 2.0f;

	if (fTimeA >= 0.0f || fTimeB >= 0.0f)
	{
		info.bIsContact = true;

		//最短時間を保存
		fTimeA = fTimeA < 0.0f ? fTimeB : fTimeA;
		fTimeB = fTimeB < 0.0f ? fTimeA : fTimeB;
		info.fTimingMin = fTimeA <= fTimeB ? fTimeA : fTimeB;

		return info;
	}

	return info;
}

//--------------------------------------------------------------------------------
//  Others
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  NormalizeRotInTwoPi(float)
//	回転を-2Piから2Piの間にする
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInTwoPi(float* pRot)
{
	while (*pRot >= KF_PI * 2.0f)
	{
		*pRot -= KF_PI * 2.0f;
	}

	while (*pRot <= -KF_PI * 2.0f)
	{
		*pRot += KF_PI * 2.0f;
	}
}

//--------------------------------------------------------------------------------
//  NormalizeRotInTwoPi(Vec3)
//	回転を-2Piから2Piの間にする
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInTwoPi(CKFVec3* pRot)
{
	NormalizeRotInTwoPi(&pRot->m_fX);
	NormalizeRotInTwoPi(&pRot->m_fY);
	NormalizeRotInTwoPi(&pRot->m_fZ);
}

//--------------------------------------------------------------------------------
//  NormalizeRotInPi(float)
//	回転を-PiからPiの間にする
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInPi(float* pRot)
{
	if (*pRot > KF_PI)
	{
		*pRot -= KF_PI * 2.0f;
	}
	else if (*pRot < -KF_PI)
	{
		*pRot += KF_PI * 2.0f;
	}
}

//--------------------------------------------------------------------------------
//  NormalizeRotInPi(Vec3)
//	回転を-PiからPiの間にする
//--------------------------------------------------------------------------------
void CKFMath::NormalizeRotInPi(CKFVec3* pRot)
{
	NormalizeRotInPi(&pRot->m_fX);
	NormalizeRotInPi(&pRot->m_fY);
	NormalizeRotInPi(&pRot->m_fZ);
}

//--------------------------------------------------------------------------------
//  CalculateZDepth
//	Z深度情報算出
//--------------------------------------------------------------------------------
float CKFMath::CalculateZDepth(const CKFVec3 &vPos, const CKFVec3 &vCameraEye, const CKFVec3 &vCameraAt)
{
	CKFVec3 vCameraForward = vCameraAt - vCameraEye;
	CKFVec3 vPosToCamera = vPos - vCameraEye;
	float fDepth = Vec3Dot(vCameraForward, vPosToCamera);
	return fDepth;
}
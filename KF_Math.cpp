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
const CKFVec3	CKFMath::sc_vZero = CKFVec3(0.0f);
const CKFVec3	CKFMath::sc_vUp = CKFVec3(0.0f, 1.0f, 0.0f);
const CKFVec3	CKFMath::sc_vDown = CKFVec3(0.0f, -1.0f, 0.0f);
const CKFVec3	CKFMath::sc_vLeft = CKFVec3(-1.0f, 0.0f, 0.0f);
const CKFVec3	CKFMath::sc_vRight = CKFVec3(1.0f, 0.0f, 0.0f);
const CKFVec3	CKFMath::sc_vForward = CKFVec3(0.0f, 0.0f, 1.0f);
const CKFVec3	CKFMath::sc_vBack = CKFVec3(0.0f, 0.0f, -1.0f);
const CKFColor	CKFMath::sc_cWhite = CKFColor(1.0f, 1.0f, 1.0f, 1.0f);
const CKFColor	CKFMath::sc_cBlack = CKFColor(0.0f, 0.0f, 0.0f, 1.0f);

//--------------------------------------------------------------------------------
//  CKFVec2
//--------------------------------------------------------------------------------
#ifdef USING_DIRECTX
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
	if (m_fX == vValue.m_fX && m_fY == vValue.m_fY) { return true; }
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
//  operator-
//--------------------------------------------------------------------------------
CKFVec2 CKFVec2::operator-(const CKFVec2 &vValue) const
{
	CKFVec2 vAnswer;
	vAnswer.m_fX = m_fX - vValue.m_fX;
	vAnswer.m_fY = m_fY - vValue.m_fY;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator-=
//--------------------------------------------------------------------------------
void CKFVec2::operator-=(const CKFVec2 &vValue)
{
	m_fX -= vValue.m_fX;
	m_fY -= vValue.m_fY;
}

//--------------------------------------------------------------------------------
//  operator*(外積)
//--------------------------------------------------------------------------------
float CKFVec2::operator*(const CKFVec2& vValue) const
{
	float fAnswer = m_fX * vValue.m_fY - m_fY * vValue.m_fX;
	return fAnswer;
}

//--------------------------------------------------------------------------------
//  operator*(float)
//--------------------------------------------------------------------------------
CKFVec2 CKFVec2::operator*(const float &fValue) const
{
	CKFVec2 vAnswer;
	vAnswer.m_fX = m_fX * fValue;
	vAnswer.m_fY = m_fY * fValue;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=(float)
//--------------------------------------------------------------------------------
void CKFVec2::operator*=(const float &fValue)
{
	m_fX *= fValue;
	m_fY *= fValue;
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

#ifdef USING_DIRECTX
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
	if (m_fX == vValue.m_fX && m_fY == vValue.m_fY && m_fZ == vValue.m_fZ) { return true; }
	return false;
}

//--------------------------------------------------------------------------------
//  operator!=
//--------------------------------------------------------------------------------
bool CKFVec3::operator!=(const CKFVec3 &vValue) const
{
	if (m_fX != vValue.m_fX || m_fY != vValue.m_fY || m_fZ != vValue.m_fZ) { return true; }
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

#ifdef USING_DIRECTX
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
//  Quaternion
//--------------------------------------------------------------------------------
#ifdef USING_DIRECTX
//--------------------------------------------------------------------------------
//  キャスト(D3DXQUATERNION)
//	DXの環境のためオーバーロードする
//--------------------------------------------------------------------------------
CKFQuaternion::operator D3DXQUATERNION () const
{
	D3DXQUATERNION qDx;
	qDx.x = m_fX;
	qDx.y = m_fY;
	qDx.z = m_fZ;
	qDx.w = m_fW;
	return qDx;
}
#endif

//--------------------------------------------------------------------------------
//  operator+(CKFQuaternion)
//	quaternionの加算
//--------------------------------------------------------------------------------
CKFQuaternion CKFQuaternion::operator+(const CKFQuaternion& qValue) const
{
	CKFQuaternion qAnswer;
	qAnswer.m_fX = m_fX + qValue.m_fX;
	qAnswer.m_fY = m_fY + qValue.m_fY;
	qAnswer.m_fZ = m_fZ + qValue.m_fZ;
	qAnswer.m_fW = m_fW + qValue.m_fW;
	return qAnswer;
}

//--------------------------------------------------------------------------------
//  operator+=(CKFQuaternion)
//	quaternionの加算
//--------------------------------------------------------------------------------
void CKFQuaternion::operator+=(const CKFQuaternion& qValue)
{
	m_fX += qValue.m_fX;
	m_fY += qValue.m_fY;
	m_fZ += qValue.m_fZ;
	m_fW += qValue.m_fW;
}

//--------------------------------------------------------------------------------
//  operator-(CKFQuaternion)
//	quaternionの減算
//--------------------------------------------------------------------------------
CKFQuaternion CKFQuaternion::operator-(const CKFQuaternion& qValue) const
{
	CKFQuaternion qAnswer;
	qAnswer.m_fX = m_fX - qValue.m_fX;
	qAnswer.m_fY = m_fY - qValue.m_fY;
	qAnswer.m_fZ = m_fZ - qValue.m_fZ;
	qAnswer.m_fW = m_fW - qValue.m_fW;
	return qAnswer;
}

//--------------------------------------------------------------------------------
//  operator-=(CKFQuaternion)
//	quaternionの減算
//--------------------------------------------------------------------------------
void CKFQuaternion::operator-=(const CKFQuaternion& qValue)
{
	m_fX -= qValue.m_fX;
	m_fY -= qValue.m_fY;
	m_fZ -= qValue.m_fZ;
	m_fW -= qValue.m_fW;
}

//--------------------------------------------------------------------------------
//  operator*(float)
//	quaternionの乗算
//--------------------------------------------------------------------------------
CKFQuaternion CKFQuaternion::operator*(const float& fValue) const
{
	CKFQuaternion qAnswer;
	qAnswer.m_fX = m_fX * fValue;
	qAnswer.m_fY = m_fY * fValue;
	qAnswer.m_fZ = m_fZ * fValue;
	qAnswer.m_fW = m_fW * fValue;
	return qAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=(float)
//	quaternionの乗算
//--------------------------------------------------------------------------------
void CKFQuaternion::operator*=(const float& fValue)
{
	m_fX *= fValue;
	m_fY *= fValue;
	m_fZ *= fValue;
	m_fW *= fValue;
}

//--------------------------------------------------------------------------------
//  operator/=
//--------------------------------------------------------------------------------
void CKFQuaternion::operator/=(const float& fValue)
{
	m_fX /= fValue;
	m_fY /= fValue;
	m_fZ /= fValue;
	m_fW /= fValue;
}

//--------------------------------------------------------------------------------
//  operator*
//--------------------------------------------------------------------------------
CKFQuaternion CKFQuaternion::operator*(const CKFQuaternion& qValue) const
{
	CKFQuaternion qAnswer;
	qAnswer.m_fX = (qValue.m_fW * this->m_fX) + (qValue.m_fX * this->m_fW) + (qValue.m_fY * this->m_fZ) - (qValue.m_fZ * this->m_fY);
	qAnswer.m_fY = (qValue.m_fW * this->m_fY) - (qValue.m_fX * this->m_fZ) + (qValue.m_fY * this->m_fW) + (qValue.m_fZ * this->m_fX);
	qAnswer.m_fZ = (qValue.m_fW * this->m_fZ) + (qValue.m_fX * this->m_fY) - (qValue.m_fY * this->m_fX) + (qValue.m_fZ * this->m_fW);
	qAnswer.m_fW = (qValue.m_fW * this->m_fW) - (qValue.m_fX * this->m_fX) - (qValue.m_fY * this->m_fY) - (qValue.m_fZ * this->m_fZ);
	return qAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=
//--------------------------------------------------------------------------------
void CKFQuaternion::operator*=(const CKFQuaternion& qValue)
{
	auto qCopy = *this;
	this->m_fX = (qValue.m_fW * qCopy.m_fX) + (qValue.m_fX * qCopy.m_fW) + (qValue.m_fY * qCopy.m_fZ) - (qValue.m_fZ * qCopy.m_fY);
	this->m_fY = (qValue.m_fW * qCopy.m_fY) - (qValue.m_fX * qCopy.m_fZ) + (qValue.m_fY * qCopy.m_fW) + (qValue.m_fZ * qCopy.m_fX);
	this->m_fZ = (qValue.m_fW * qCopy.m_fZ) + (qValue.m_fX * qCopy.m_fY) - (qValue.m_fY * qCopy.m_fX) + (qValue.m_fZ * qCopy.m_fW);
	this->m_fW = (qValue.m_fW * qCopy.m_fW) - (qValue.m_fX * qCopy.m_fX) - (qValue.m_fY * qCopy.m_fY) - (qValue.m_fZ * qCopy.m_fZ);
}

//--------------------------------------------------------------------------------
//  CKFColor
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  operator=
//--------------------------------------------------------------------------------
CKFColor &CKFColor::operator=(const CKFColor &cValue)
{
	m_fR = cValue.m_fR;
	m_fG = cValue.m_fG;
	m_fB = cValue.m_fB;
	m_fA = cValue.m_fA;

	return *this;
}

//--------------------------------------------------------------------------------
//  operator==
//--------------------------------------------------------------------------------
bool CKFColor::operator==(const CKFColor &cValue)
{
	if (m_fR == cValue.m_fR && m_fG == cValue.m_fG && m_fB == cValue.m_fB && m_fA == cValue.m_fA) { return true; }
	return false;
}

//--------------------------------------------------------------------------------
//  operator+
//--------------------------------------------------------------------------------
CKFColor CKFColor::operator+(const CKFColor &cValue) const
{
	CKFColor cAnswer;
	cAnswer.m_fR = m_fR + cValue.m_fR;
	cAnswer.m_fG = m_fG + cValue.m_fG;
	cAnswer.m_fB = m_fB + cValue.m_fB;
	cAnswer.m_fA = m_fA + cValue.m_fA;
	return cAnswer;
}

//--------------------------------------------------------------------------------
//  operator+=
//--------------------------------------------------------------------------------
void CKFColor::operator+=(const CKFColor &cValue)
{
	m_fR += cValue.m_fR;
	m_fG += cValue.m_fG;
	m_fB += cValue.m_fB;
	m_fA += cValue.m_fA;
}

//--------------------------------------------------------------------------------
//  operator-
//--------------------------------------------------------------------------------
CKFColor CKFColor::operator-(const CKFColor &cValue) const
{
	CKFColor cAnswer;
	cAnswer.m_fR = m_fR - cValue.m_fR;
	cAnswer.m_fG = m_fG - cValue.m_fG;
	cAnswer.m_fB = m_fB - cValue.m_fB;
	cAnswer.m_fA = m_fA - cValue.m_fA;
	return cAnswer;
}

//--------------------------------------------------------------------------------
//  operator-=
//--------------------------------------------------------------------------------
void CKFColor::operator-=(const CKFColor &cValue)
{
	m_fR -= cValue.m_fR;
	m_fG -= cValue.m_fG;
	m_fB -= cValue.m_fB;
	m_fA -= cValue.m_fA;
}

//--------------------------------------------------------------------------------
//  operator*(float)
//--------------------------------------------------------------------------------
CKFColor CKFColor::operator*(const float &fValue) const
{
	CKFColor cAnswer;
	cAnswer.m_fR = m_fR * fValue;
	cAnswer.m_fG = m_fG * fValue;
	cAnswer.m_fB = m_fB * fValue;
	cAnswer.m_fA = m_fA * fValue;
	return cAnswer;
}

//--------------------------------------------------------------------------------
//  operator*=(float)
//--------------------------------------------------------------------------------
void CKFColor::operator*=(const float &fValue)
{
	m_fR *= fValue;
	m_fG *= fValue;
	m_fB *= fValue;
	m_fA *= fValue;
}

#ifdef USING_DIRECTX
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
	float fMagnitude = sqrtf(vValue.m_fX * vValue.m_fX + vValue.m_fY * vValue.m_fY);
	return fMagnitude;
}

//--------------------------------------------------------------------------------
//  Magnitude(Vector3)
//--------------------------------------------------------------------------------
float CKFMath::VecMagnitude(const CKFVec3 &vValue)
{
	float fMagnitude = sqrtf(vValue.m_fX * vValue.m_fX + vValue.m_fY * vValue.m_fY + vValue.m_fZ * vValue.m_fZ);
	return fMagnitude;
}

//--------------------------------------------------------------------------------
//  MagnitudeSquare(Vector2)
//--------------------------------------------------------------------------------
float CKFMath::VecMagnitudeSquare(const CKFVec2 &vValue)
{
	float fMagnitudeSquare = vValue.m_fX * vValue.m_fX + vValue.m_fY * vValue.m_fY;
	return fMagnitudeSquare;
}

//--------------------------------------------------------------------------------
//  MagnitudeSquare(Vector3)
//--------------------------------------------------------------------------------
float CKFMath::VecMagnitudeSquare(const CKFVec3 &vValue)
{
	float fMagnitudeSquare = vValue.m_fX * vValue.m_fX + vValue.m_fY * vValue.m_fY + vValue.m_fZ * vValue.m_fZ;
	return fMagnitudeSquare;
}

//--------------------------------------------------------------------------------
//  Normalize(Vector2)
//--------------------------------------------------------------------------------
void CKFMath::VecNormalize(CKFVec2& vVec)
{
	float fMagnitude = VecMagnitude(vVec);
	if (fMagnitude == 0.0f) { return; }
	vVec.m_fX /= fMagnitude;
	vVec.m_fY /= fMagnitude;
}

//--------------------------------------------------------------------------------
//  Normalize(Vector3)
//--------------------------------------------------------------------------------
void CKFMath::VecNormalize(CKFVec3& vVec)
{
	float fMagnitude = VecMagnitude(vVec);
	if (fMagnitude == 0.0f) { return; }
	vVec /= fMagnitude;
}

//--------------------------------------------------------------------------------
//  Normalize(Vector3)
//--------------------------------------------------------------------------------
CKFVec3 CKFMath::VecNormalize(const CKFVec3& vVec)
{
	float fMagnitude = VecMagnitude(vVec);
	if (fMagnitude == 0.0f) { return CKFVec3(0.0f); }
	auto vAnswer = vVec;
	vAnswer /= fMagnitude;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  乗算(Dot)(Vector2)
//--------------------------------------------------------------------------------
float CKFMath::Vec2Dot(const CKFVec2& vVecL, const CKFVec2& vVecR)
{
	float fDot = vVecL.m_fX * vVecR.m_fX + vVecL.m_fY * vVecR.m_fY;
	return fDot;
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
//  VecDistanceSquare(Vector3)
//--------------------------------------------------------------------------------
float CKFMath::VecDistanceSquare(const CKFVec3 &vVecL, const CKFVec3 &vVecR)
{
	CKFVec3 vDis = vVecL - vVecR;
	float fDisSquare = VecMagnitudeSquare(vDis);
	return fDisSquare;
}

//--------------------------------------------------------------------------------
//  Vec3TransformCoord
//	回転行列にとってVec3を回転する
//--------------------------------------------------------------------------------
CKFVec3 CKFMath::Vec3TransformCoord(const CKFVec3& vVec, const CKFMtx44 &mtxRot)
{
	CKFVec4 vVec4 = CKFVec4(vVec, 1.0f);
	CKFVec3 vAnswer;
	vVec4 *= mtxRot;

	if (vVec4.m_fW != 0.0f)
	{
		vAnswer.m_fX = vVec4.m_fX / vVec4.m_fW;
		vAnswer.m_fY = vVec4.m_fY / vVec4.m_fW;
		vAnswer.m_fZ = vVec4.m_fZ / vVec4.m_fW;
	}
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  Vec3TransformNormal
//	回転行列にとってVec3(ベクトル)を回転する
//--------------------------------------------------------------------------------
CKFVec3 CKFMath::Vec3TransformNormal(const CKFVec3& vVec, const CKFMtx44 &mtxRot)
{
	CKFVec4 vVec4 = CKFVec4(vVec, 0.0f);
	CKFVec3 vAnswer;
	vVec4 *= mtxRot;
	vAnswer.m_fX = vVec4.m_fX;
	vAnswer.m_fY = vVec4.m_fY;
	vAnswer.m_fZ = vVec4.m_fZ;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  Vec3TransformNormal
//	回転行列にとってVec3(ベクトル)を回転する
//--------------------------------------------------------------------------------
CKFVec3 CKFMath::Vec3Scale(const CKFVec3& vValue, const CKFVec3& vScale)
{
	auto vAnswer = CKFVec3(vValue.m_fX * vScale.m_fX, vValue.m_fY * vScale.m_fY, vValue.m_fZ * vScale.m_fZ);
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  Vector2の値から角度を計算する
//--------------------------------------------------------------------------------
float CKFMath::Vec2Radian(const CKFVec2& vValue)
{
	float fValue = atan2f(vValue.m_fY, vValue.m_fX);
	return fValue;
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
//  EulerBetweenVec3
//	Vector3間の角度をx,y,z回転式で算出
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
//  EulerBetweenVec3
//	Vector3間の角度をx,y,z回転式で算出
//--------------------------------------------------------------------------------
CKFVec3	CKFMath::ProjectOnPlane(const CKFVec3& vVec, const CKFVec3& vUpPlane, const CKFVec3& vUpNow)
{
	auto vAnswer = vVec;
	return vAnswer;
}

//--------------------------------------------------------------------------------
//  TransformInverse
//	Vector3間の角度をx,y,z回転式で算出
//--------------------------------------------------------------------------------
CKFVec3	CKFMath::TransformInverse(const CKFMtx44& mtx, const CKFVec3& vVec)
{
	CKFVec3 vWork = vVec;
	vWork.m_fX -= mtx.m_af[3][0];
	vWork.m_fY -= mtx.m_af[3][1];
	vWork.m_fZ -= mtx.m_af[3][2];
	CKFVec3 vAnswer;
	vAnswer.m_fX = vWork.m_fX * mtx.m_af[0][0] +
		vWork.m_fY * mtx.m_af[0][1] +
		vWork.m_fZ * mtx.m_af[0][2];
	vAnswer.m_fY = vWork.m_fX * mtx.m_af[1][0] +
		vWork.m_fY * mtx.m_af[1][1] +
		vWork.m_fZ * mtx.m_af[1][2];
	vAnswer.m_fZ = vWork.m_fX * mtx.m_af[2][0] +
		vWork.m_fY * mtx.m_af[2][1] +
		vWork.m_fZ * mtx.m_af[2][2];

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
void CKFMath::MtxIdentity(CKFMtx44& mtx)
{
	mtx.m_af[0][0] = 1.0f;
	mtx.m_af[0][1] = 0.0f;
	mtx.m_af[0][2] = 0.0f;
	mtx.m_af[0][3] = 0.0f;
	mtx.m_af[1][0] = 0.0f;
	mtx.m_af[1][1] = 1.0f;
	mtx.m_af[1][2] = 0.0f;
	mtx.m_af[1][3] = 0.0f;
	mtx.m_af[2][0] = 0.0f;
	mtx.m_af[2][1] = 0.0f;
	mtx.m_af[2][2] = 1.0f;
	mtx.m_af[2][3] = 0.0f;
	mtx.m_af[3][0] = 0.0f;
	mtx.m_af[3][1] = 0.0f;
	mtx.m_af[3][2] = 0.0f;
	mtx.m_af[3][3] = 1.0f;
}

//--------------------------------------------------------------------------------
//  MtxRotAxis
//	回転行列の作成
//--------------------------------------------------------------------------------
void CKFMath::MtxRotAxis(CKFMtx44& mtxRot, const CKFVec3 &vAxis, const float &fAngle)
{
	//軸をノーマライズする
	CKFVec3 vAxisCpy = vAxis;
	VecNormalize(vAxisCpy);

	//計算用変数
	float fX = vAxisCpy.m_fX;
	float fY = vAxisCpy.m_fY;
	float fZ = vAxisCpy.m_fZ;
	float fSin = sinf(fAngle);
	float fCos = cosf(fAngle);

	mtxRot.m_af[0][0] = fCos + fX * fX * (1 - fCos);
	mtxRot.m_af[0][1] = fX * fY * (1 - fCos) + fZ * fSin;
	mtxRot.m_af[0][2] = fX * fZ * (1 - fCos) - fY * fSin;
	mtxRot.m_af[0][3] = 0.0f;
	mtxRot.m_af[1][0] = fX * fY * (1 - fCos) - fZ * fSin;
	mtxRot.m_af[1][1] = fCos + fY * fY * (1 - fCos);
	mtxRot.m_af[1][2] = fZ * fY * (1 - fCos) + fX * fSin;
	mtxRot.m_af[1][3] = 0.0f;
	mtxRot.m_af[2][0] = fX * fZ * (1 - fCos) + fY * fSin;
	mtxRot.m_af[2][1] = fY * fZ * (1 - fCos) - fX * fSin;
	mtxRot.m_af[2][2] = fCos + fZ * fZ * (1 - fCos);
	mtxRot.m_af[2][3] = 0.0f;
	mtxRot.m_af[3][0] = 0.0f;
	mtxRot.m_af[3][1] = 0.0f;
	mtxRot.m_af[3][2] = 0.0f;
	mtxRot.m_af[3][3] = 1.0f;
}

//--------------------------------------------------------------------------------
//  MtxRotationYawPitchRoll
//	回転行列の作成(mtxY*mtxX*mtxZ)
//--------------------------------------------------------------------------------
void CKFMath::MtxRotationYawPitchRoll(CKFMtx44& mtxRot, const CKFVec3 &vRot)
{
	float fSinX = sinf(vRot.m_fX); 
	float fCosX = cosf(vRot.m_fX); 
	float fSinY = sinf(vRot.m_fY);
	float fCosY = cosf(vRot.m_fY);
	float fSinZ = sinf(vRot.m_fZ);
	float fCosZ = cosf(vRot.m_fZ);

	mtxRot.m_af[0][0] = fCosY * fCosZ + fSinX * fSinY * fSinZ;
	mtxRot.m_af[0][1] = fCosX * fSinZ;
	mtxRot.m_af[0][2] = -fSinY * fCosZ + fSinX * fCosY * fSinZ;
	mtxRot.m_af[0][3] = 0.0f;
	mtxRot.m_af[1][0] = -fCosY * fSinZ + fSinX * fSinY * fCosZ;
	mtxRot.m_af[1][1] = fCosX * fCosZ;
	mtxRot.m_af[1][2] = fSinY * fSinZ + fSinX * fCosY * fCosZ;
	mtxRot.m_af[1][3] = 0.0f;
	mtxRot.m_af[2][0] = fCosX * fSinY;
	mtxRot.m_af[2][1] = -fSinX;
	mtxRot.m_af[2][2] = fCosX * fCosY;
	mtxRot.m_af[2][3] = 0.0f;
	mtxRot.m_af[3][0] = 0.0f;
	mtxRot.m_af[3][1] = 0.0f;
	mtxRot.m_af[3][2] = 0.0f;
	mtxRot.m_af[3][3] = 1.0f;
}

//--------------------------------------------------------------------------------
//	MtxTranslation
//	平行移動行列の作成
//--------------------------------------------------------------------------------
void CKFMath::MtxTranslation(CKFMtx44& mtxTrans, const CKFVec3 &vPos)
{
	MtxIdentity(mtxTrans);
	mtxTrans.m_af[3][0] = vPos.m_fX;
	mtxTrans.m_af[3][1] = vPos.m_fY;
	mtxTrans.m_af[3][2] = vPos.m_fZ;
}

//--------------------------------------------------------------------------------
//	MtxTranspose
//	行列のトランスポース
//--------------------------------------------------------------------------------
CKFMtx44 CKFMath::MtxTranspose(const CKFMtx44& mtx)
{
	CKFMtx44 mtxOut;
	for (int nCntY = 0; nCntY < 4; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			mtxOut.m_af[nCntY][nCntX] = mtx.m_af[nCntX][nCntY];
		}
	}
	return mtxOut;
}

//--------------------------------------------------------------------------------
//  Quaternion
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	QuaternionIdentity
//	平行移動行列の作成
//--------------------------------------------------------------------------------
void CKFMath::QuaternionIdentity(CKFQuaternion& qValue)
{
	qValue.m_fX = qValue.m_fY = qValue.m_fZ = 0.0f;
	qValue.m_fW = 1.0f;
}

//--------------------------------------------------------------------------------
//	QuaternionMagnitudeSquare
//	平行移動行列の作成
//--------------------------------------------------------------------------------
float CKFMath::QuaternionMagnitudeSquare(const CKFQuaternion& qValue)
{
	float fSquare = QuaternionDot(qValue, qValue);
	return fSquare;
}

//--------------------------------------------------------------------------------
//	QuaternionMagnitude
//	平行移動行列の作成
//--------------------------------------------------------------------------------
float CKFMath::QuaternionMagnitude(const CKFQuaternion& qValue)
{
	float fValue = QuaternionMagnitudeSquare(qValue);
	fValue = sqrtf(fValue);
	return fValue;
}

//--------------------------------------------------------------------------------
//	QuaternionNormalize
//	平行移動行列の作成
//--------------------------------------------------------------------------------
void CKFMath::QuaternionNormalize(CKFQuaternion& qValue)
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
float CKFMath::QuaternionDot(const CKFQuaternion& qL, const CKFQuaternion& qR)
{
	float fValue = qL.m_fX * qR.m_fX + qL.m_fY * qR.m_fY + qL.m_fZ * qR.m_fZ + qL.m_fW * qR.m_fW;
	return fValue;
}

//--------------------------------------------------------------------------------
//	QuaternionMultiplyXYZW
//	平行移動行列の作成
//--------------------------------------------------------------------------------
CKFQuaternion CKFMath::QuaternionMultiplyXYZW(const CKFQuaternion& qL, const CKFQuaternion& qR)
{
	return CKFQuaternion(
		qL.m_fX * qR.m_fX,
		qL.m_fY * qR.m_fY,
		qL.m_fZ * qR.m_fZ,
		qL.m_fW * qR.m_fW);
}

//--------------------------------------------------------------------------------
//	MtxToQuaternion
//	平行移動行列の作成
//--------------------------------------------------------------------------------
CKFQuaternion CKFMath::EulerToQuaternion(const CKFVec3& vVec)
{
	CKFQuaternion qValue;
	D3DXQUATERNION qDx;
	D3DXQuaternionRotationYawPitchRoll(&qDx, vVec.m_fY, vVec.m_fX, vVec.m_fZ);
	qValue.m_fX = qDx.x;
	qValue.m_fY = qDx.y;
	qValue.m_fZ = qDx.z;
	qValue.m_fW = qDx.w;
	return qValue;
}

//--------------------------------------------------------------------------------
//	MtxToQuaternion
//	平行移動行列の作成
//--------------------------------------------------------------------------------
CKFQuaternion CKFMath::MtxToQuaternion(const CKFMtx44& mtxRot)
{
	CKFQuaternion qValue;
	float fR22 = mtxRot.m_af[2][2];
	if (fR22 <= 0.0f)  // x^2 + y^2 >= z^2 + w^2
	{
		float fDiff10 = mtxRot.m_af[1][1] - mtxRot.m_af[0][0];
		float fOmr22 = 1.0f - fR22;
		if (fDiff10 <= 0.0f)  // x^2 >= y^2
		{
			float f4XSqr = fOmr22 - fDiff10;
			float fInv4X = 0.5f / sqrtf(f4XSqr);
			qValue.m_fX = f4XSqr * fInv4X;
			qValue.m_fY = (mtxRot.m_af[0][1] + mtxRot.m_af[1][0]) * fInv4X;
			qValue.m_fZ = (mtxRot.m_af[0][2] + mtxRot.m_af[2][0]) * fInv4X;
			qValue.m_fW = (mtxRot.m_af[1][2] - mtxRot.m_af[2][1]) * fInv4X;
		}
		else  // y^2 >= x^2
		{
			float f4YSqr = fOmr22 + fDiff10;
			float fInv4Y = 0.5f / sqrtf(f4YSqr);
			qValue.m_fX = (mtxRot.m_af[0][1] + mtxRot.m_af[1][0]) * fInv4Y;
			qValue.m_fY = f4YSqr * fInv4Y;
			qValue.m_fZ = (mtxRot.m_af[1][2] + mtxRot.m_af[2][1]) * fInv4Y;
			qValue.m_fW = (mtxRot.m_af[2][0] - mtxRot.m_af[0][2]) * fInv4Y;
		}
	}
	else  // z^2 + w^2 >= x^2 + y^2
	{
		float fSum10 = mtxRot.m_af[1][1] + mtxRot.m_af[0][0];
		float fOpr22 = 1.0f + fR22;
		if (fSum10 <= 0.0f)  // z^2 >= w^2
		{
			float f4ZSqr = fOpr22 - fSum10;
			float fInv4Z = 0.5f / sqrtf(f4ZSqr);
			qValue.m_fX = (mtxRot.m_af[0][2] + mtxRot.m_af[2][0]) * fInv4Z;
			qValue.m_fY = (mtxRot.m_af[1][2] + mtxRot.m_af[2][1]) * fInv4Z;
			qValue.m_fZ = f4ZSqr * fInv4Z;
			qValue.m_fW = (mtxRot.m_af[0][1] - mtxRot.m_af[1][0]) * fInv4Z;
		}
		else  // w^2 >= z^2
		{
			float f4WSqr = fOpr22 + fSum10;
			float fInv4W = 0.5f / sqrtf(f4WSqr);
			qValue.m_fX = (mtxRot.m_af[1][2] - mtxRot.m_af[2][1]) * fInv4W;
			qValue.m_fY = (mtxRot.m_af[2][0] - mtxRot.m_af[0][2]) * fInv4W;
			qValue.m_fZ = (mtxRot.m_af[0][1] - mtxRot.m_af[1][0]) * fInv4W;
			qValue.m_fW = f4WSqr * fInv4W;
		}
	}
	return qValue;
}

//--------------------------------------------------------------------------------
//  QuaternionToMtx
//	
//--------------------------------------------------------------------------------
CKFMtx44 CKFMath::QuaternionToMtx(const CKFQuaternion& qValue)
{
	CKFMtx44 mtx;

	static const auto qAnt1110 = CKFQuaternion(1.0f, 1.0f, 1.0f, 0.0f);
	auto q0 = qValue + qValue;
	auto q1 = QuaternionMultiplyXYZW(qValue, q0);
	auto q3 = CKFQuaternion(q1.m_fY, q1.m_fX, q1.m_fX, qAnt1110.m_fW);
	auto q4 = CKFQuaternion(q1.m_fZ, q1.m_fZ, q1.m_fY, qAnt1110.m_fW);
	auto qR0 = qAnt1110 - q3;
	qR0 -= q4;
	q3 = CKFQuaternion(qValue.m_fX, qValue.m_fX, qValue.m_fY, qValue.m_fW);
	q4 = CKFQuaternion(q0.m_fZ, q0.m_fY, q0.m_fZ, q0.m_fW);
	q3 = QuaternionMultiplyXYZW(q3, q4);
	q4 = CKFQuaternion(qValue.m_fW, qValue.m_fW, qValue.m_fW, qValue.m_fW);
	auto q5 = CKFQuaternion(q0.m_fY, q0.m_fZ, q0.m_fX, q0.m_fW);
	q4 = QuaternionMultiplyXYZW(q4, q5);
	auto qR1 = q3 + q4;
	auto qR2 = q3 - q4;
	q3 = CKFQuaternion(qR1.m_fY, qR2.m_fX, qR2.m_fY, qR1.m_fZ);
	q4 = CKFQuaternion(qR1.m_fX, qR2.m_fZ, qR1.m_fX, qR2.m_fZ);

	mtx.m_af[0][0] = qR0.m_fX;
	mtx.m_af[0][1] = q3.m_fX;
	mtx.m_af[0][2] = q3.m_fY;
	mtx.m_af[0][3] = qR0.m_fW;
	mtx.m_af[1][0] = q3.m_fZ;
	mtx.m_af[1][1] = qR0.m_fY;
	mtx.m_af[1][2] = q3.m_fW;
	mtx.m_af[1][3] = qR0.m_fW;
	mtx.m_af[2][0] = q4.m_fX;
	mtx.m_af[2][1] = q4.m_fY;
	mtx.m_af[2][2] = qR0.m_fZ;
	mtx.m_af[2][3] = qR0.m_fW;
	mtx.m_af[3][0] = 0.0f;
	mtx.m_af[3][1] = 0.0f;
	mtx.m_af[3][2] = 0.0f;
	mtx.m_af[3][3] = 1.0f;
	
	return mtx;
}

//--------------------------------------------------------------------------------
//  QuaternionToEuler
//	スクリーン上のポインタから3D空間の射線を求める関数
//--------------------------------------------------------------------------------
CKFVec3	CKFMath::QuaternionToEuler(const CKFQuaternion& quaternion)
{
	CKFVec3 vRot;
	float fX = quaternion.m_fX;
	float fY = quaternion.m_fY;
	float fZ = quaternion.m_fZ;
	float fW = quaternion.m_fW;
	vRot.m_fX = atan2f(2.0f * (fW * fZ + fX * fY), 1.0f - 2.0f * (fZ * fZ + fX * fX));
	vRot.m_fY = asinf(2.0f * (fW * fX - fY * fZ));
	vRot.m_fZ = atan2f(2.0f * (fW * fY + fZ * fX), 1.0f - 2.0f * (fX * fX + fY * fY));
	return vRot;
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
	TransformRay(ray, mtxViewInverse);
	return ray;
}

//--------------------------------------------------------------------------------
//  ChangePosToRay
//	スクリーン上のポインタから3D空間の射線を求める関数
//--------------------------------------------------------------------------------
CKFRay CKFMath::ChangePosToRay(const CKFVec2& vScreenPos, const float& fViewportWidth, const float &fViewportHeight, const float &fProjMtx00, const float &fProjMtx11)
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
void CKFMath::TransformRay(CKFRay& ray, const CKFMtx44& mtxTrans)
{
	ray.m_vOrigin = Vec3TransformCoord(ray.m_vOrigin, mtxTrans);
	ray.m_vDirection = Vec3TransformNormal(ray.m_vDirection, mtxTrans);
	VecNormalize(ray.m_vDirection);
}

//--------------------------------------------------------------------------------
//  Lerp
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  LerpVec3
//	Vector3を線形補間方式で移動する
//--------------------------------------------------------------------------------
CKFVec3 CKFMath::LerpVec3(const CKFVec3& vVecFrom, const CKFVec3& vVecTo, const float& fTime)
{
	float fPersent = fTime;
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
//	CKFColorを線形補間方式で回転する
//--------------------------------------------------------------------------------
CKFColor CKFMath::LerpColor(const CKFColor& cFrom, const CKFColor& cTo, const float& fTime)
{
	float fPersent = fTime;
	fPersent = fPersent < 0.0f ? 0.0f : fPersent > 1.0f ? 1.0f : fPersent;
	CKFColor cDelta = cTo - cFrom;
	CKFColor cAnswer = cFrom + cDelta * fPersent;
	return cAnswer;
}

//--------------------------------------------------------------------------------
//  SlerpQuaternion
//	Quaternionを球形補間方式で回転する
//--------------------------------------------------------------------------------
CKFQuaternion CKFMath::SlerpQuaternion(const CKFQuaternion& qFrom, const CKFQuaternion& qTo, const float& fTime)
{
	//if (fTime <= 0.0f) { return qFrom; }
	//if (fTime >= 1.0f) { return qTo; }

	//CKFQuaternion qFromCpy = qFrom;
	//CKFQuaternion qToCpy = qTo;

	//// Only unit quaternions are valid rotations.
	//// Normalize to avoid undefined behavior.

	//// Compute the cosine of the angle between the two vectors.
	//float fDot = QuaternionDot(qFromCpy, qToCpy);

	//if (fabs(fDot) > 0.9995f)
	//{
	//	// If the inputs are too close for comfort, linearly interpolate
	//	// and normalize the result.
	//	CKFQuaternion qResult = qFromCpy + (qToCpy - qFromCpy) * fTime;
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

	//CKFQuaternion qWork = qToCpy - qFromCpy * fDot;
	//QuaternionNormalize(qWork);			// { v0, v2 } is now an orthonormal basis
	//CKFQuaternion qResult = qFromCpy * cosf(fTheta) + qWork * sinf(fTheta);
	
	D3DXQUATERNION qFromDX = qFrom;
	D3DXQUATERNION qToDX = qTo;
	D3DXQUATERNION qOut;
	D3DXQuaternionSlerp(&qOut, &qFromDX, &qToDX, fTime);
	CKFQuaternion qResult;
	qResult.m_fX = qOut.x;
	qResult.m_fY = qOut.y;
	qResult.m_fZ = qOut.z;
	qResult.m_fW = qOut.w;
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
void CKFMath::NormalizeColor(CKFColor& cColor)
{
	cColor.m_fR = cColor.m_fR < 0.0f ? 0.0f : cColor.m_fR > 1.0f ? 1.0f : cColor.m_fR;
	cColor.m_fG = cColor.m_fG < 0.0f ? 0.0f : cColor.m_fG > 1.0f ? 1.0f : cColor.m_fG;
	cColor.m_fB = cColor.m_fB < 0.0f ? 0.0f : cColor.m_fB > 1.0f ? 1.0f : cColor.m_fB;
	cColor.m_fA = cColor.m_fA < 0.0f ? 0.0f : cColor.m_fA > 1.0f ? 1.0f : cColor.m_fA;
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
void CKFMath::NormalizeRotInTwoPi(CKFVec3& vRot)
{
	NormalizeRotInTwoPi(vRot.m_fX);
	NormalizeRotInTwoPi(vRot.m_fY);
	NormalizeRotInTwoPi(vRot.m_fZ);
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
void CKFMath::NormalizeRotInPi(CKFVec3& vRot)
{
	NormalizeRotInPi(vRot.m_fX);
	NormalizeRotInPi(vRot.m_fY);
	NormalizeRotInPi(vRot.m_fZ);
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
void CKFMath::NormalizeRotInZeroToTwoPi(CKFVec3& vRot)
{
	NormalizeRotInZeroToTwoPi(vRot.m_fX);
	NormalizeRotInZeroToTwoPi(vRot.m_fY);
	NormalizeRotInZeroToTwoPi(vRot.m_fZ);
}

//--------------------------------------------------------------------------------
//  CalculateZDepth
//	Z深度情報算出
//--------------------------------------------------------------------------------
float CKFMath::CalculateZDepth(const CKFVec3& vPos, const CKFVec3& vCameraEye, const CKFVec3& vCameraAt)
{
	CKFVec3 vCameraForward = vCameraAt - vCameraEye;
	CKFVec3 vPosToCamera = vPos - vCameraEye;
	float fDepth = Vec3Dot(vCameraForward, vPosToCamera);
	return fDepth;
}
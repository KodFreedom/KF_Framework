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
bool CKFVec2::operator==(const CKFVec2 &vValue)
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
//  CKFVec3
//--------------------------------------------------------------------------------
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
bool CKFVec3::operator==(const CKFVec3 &vValue)
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
bool CKFVec3::operator!=(const CKFVec3 &vValue)
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
//  CKFVec4
//--------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------
//  CKFMath
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

	//回転の距離算出
	float fLength = sinf(fAngle);   

	//軸に距離をかける
	vAxisCpy *= fLength;

	//計算用変数
	float fA = vAxisCpy.m_fX;
	float fB = vAxisCpy.m_fY;  
	float fC = vAxisCpy.m_fZ;     
	float fD = cosf(fAngle);  
	float fT =  (1 - fD) / (fLength * fLength);  
	
	*pMtxRot = CKFMtx44(
		fA * fT * fA + fD, fB * fT * fA + fC, fC * fT * fA - fB, 0.0f,
		fA * fT * fB - fC, fB * fT * fB + fD, fC * fT * fB + fA, 0.0f,
		fA * fT * fC + fB, fB * fT * fC - fA, fC * fT * fC + fD, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
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
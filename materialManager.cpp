//--------------------------------------------------------------------------------
//	マテリアルマネージャ
//　materialManager.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	CKFMaterial
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CKFMaterial::CKFMaterial()
	: m_cAmbient(CKFMath::sc_cWhite)
	, m_cDiffuse(CKFMath::sc_cWhite)
	, m_cEmissive(CKFMath::sc_cWhite)
	, m_cSpecular(CKFMath::sc_cWhite)
	, m_fPower(1.0f)
{}

#ifdef USING_DIRECTX
//--------------------------------------------------------------------------------
//  キャスト(D3DMATERIAL9)
//	DXの環境のため(マテリアル)オーバーロードする
//--------------------------------------------------------------------------------
CKFMaterial::operator D3DMATERIAL9() const
{
	D3DMATERIAL9 vAnswer;
	vAnswer.Ambient = m_cAmbient;
	vAnswer.Diffuse = m_cDiffuse;
	vAnswer.Emissive = m_cEmissive;
	vAnswer.Specular = m_cSpecular;
	vAnswer.Power = m_fPower;
	return vAnswer;
}
#endif

//--------------------------------------------------------------------------------
//	operator==
//--------------------------------------------------------------------------------
bool CKFMaterial::operator==(const CKFMaterial& mat) const
{
	if (this->m_cAmbient == mat.m_cAmbient
		&& this->m_cDiffuse == mat.m_cDiffuse
		&& this->m_cEmissive == mat.m_cEmissive
		&& this->m_cSpecular == mat.m_cSpecular
		&& this->m_fPower == mat.m_fPower)
	{ return true; }
	return false;
}

//--------------------------------------------------------------------------------
//	CMaterialManager
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	コンストラクタ
//--------------------------------------------------------------------------------
CMaterialManager::CMaterialManager()
{
	m_umMaterial.clear();

	//Normal
	SaveMaterial(CKFMath::sc_cWhite, CKFMath::sc_cWhite, CKFMath::sc_cBlack, CKFMath::sc_cBlack, 1.0f);
}

//--------------------------------------------------------------------------------
//  SaveMaterial
//--------------------------------------------------------------------------------
const unsigned short CMaterialManager::SaveMaterial(const CKFColor &cAmbient, const CKFColor &cDiffuse, const CKFColor &cSpecular, const CKFColor &cEmissive, const float &fPower)
{
	auto& mat = CKFMaterial(cAmbient, cDiffuse, cSpecular, cEmissive, fPower);

	//今までのマテリアルと比較する
	for (auto& mMat : m_umMaterial)
	{
		if (mMat.second == mat)
		{//すでにあるならIDを返す
			return mMat.first;
		}
	}

	//Mapに追加する
	auto usID = (unsigned short)m_umMaterial.size();
	m_umMaterial.emplace(usID, mat);

	return usID;
}

//--------------------------------------------------------------------------------
//  GetMaterial
//--------------------------------------------------------------------------------
const CKFMaterial& CMaterialManager::GetMaterial(const unsigned short& usID)
{
	return m_umMaterial.at(usID);
}
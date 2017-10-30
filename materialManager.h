//--------------------------------------------------------------------------------
//	マテリアルマネージャ
//　materialManager.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define CMM CMaterialManager	//CMaterialManagerの略称

//--------------------------------------------------------------------------------
//  クラス定義
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	マテリアル
//--------------------------------------------------------------------------------
class CKFMaterial
{
public:
	CKFMaterial();
	CKFMaterial(const Color& cAmbient, const Color& cDiffuse, const Color& cSpecular, const Color& cEmissive, const float& fPower)
		: m_cAmbient(cAmbient)
		, m_cDiffuse(cDiffuse)
		, m_cSpecular(cSpecular)
		, m_cEmissive(cEmissive)
		, m_fPower(fPower)
	{}
	~CKFMaterial() {}

	Color	m_cAmbient;		// 環境光の反射率
	Color	m_cDiffuse;		// 漫射光の反射率
	Color	m_cSpecular;	// 鏡面光の反射率
	Color	m_cEmissive;	// 自発光
	float		m_fPower;		// ハイライトのシャープネス

	//キャスト
#ifdef USING_DIRECTX
	operator D3DMATERIAL9 () const;
#endif

	//operator
	bool operator==(const CKFMaterial& mat) const;
};

//--------------------------------------------------------------------------------
//	マテリアルマネージャ
//--------------------------------------------------------------------------------
class CMaterialManager
{
public:
	//--------------------------------------------------------------------------------
	//  列挙型定義
	//--------------------------------------------------------------------------------
	enum MAT_TYPE
	{
		MT_NORMAL = 0,
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CMaterialManager();
	~CMaterialManager() {}

	void					Release(void) 
	{
		m_umMaterial.clear();
		delete this; 
	}
	const unsigned short	SaveMaterial(const Color &cAmbient, const Color &cDiffuse, const Color &cSpecular, const Color &cEmissive, const float &fPower);
	const CKFMaterial&		GetMaterial(const unsigned short& usID);

private:
	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	unordered_map<unsigned short, CKFMaterial> m_umMaterial;
};
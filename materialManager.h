//--------------------------------------------------------------------------------
//	マテリアルマネージャ
//　materialManager.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MATERIAL_MANAGER_H_
#define _MATERIAL_MANAGER_H_

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
	~CKFMaterial() {}

	CKFColor	m_cAmbient;		// 環境光の反射率
	CKFColor	m_cDiffuse;		// 漫射光の反射率
	CKFColor	m_cSpecular;	// 鏡面光の反射率
	CKFColor	m_cEmissive;	// 自発光
	float		m_fPower;		// ハイライトのシャープネス

								//キャスト
	operator D3DMATERIAL9 () const;
};

//--------------------------------------------------------------------------------
//	マテリアルマネージャ
//--------------------------------------------------------------------------------
class CMaterialManager
{
public:
	enum MATERIAL
	{
		MAT_NORMAL,
		MAT_MAX
	};

	CMaterialManager() {}
	~CMaterialManager() {}

	void Init(void);

	CKFMaterial GetMaterial(const MATERIAL &mat);
private:
	CKFMaterial InitMaterial(const CKFColor &cAmbient, const CKFColor &cDiffuse, const CKFColor &cSpecular, const CKFColor &cEmissive, const float &fPower);

	CKFMaterial m_aMaterial[MAT_MAX];
};

#endif
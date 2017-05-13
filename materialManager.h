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

	D3DMATERIAL9 GetMaterial(const MATERIAL &mat);
private:
	D3DMATERIAL9 InitMaterial(const CKFColor &cAmbient, const CKFColor &cDiffuse, const CKFColor &cSpecular, const CKFColor &cEmissive, const float &fPower);

	D3DMATERIAL9 m_aMaterial[MAT_MAX];
};

#endif
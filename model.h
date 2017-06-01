//--------------------------------------------------------------------------------
//
//　model.h
//	Author : Xu Wenjie
//	Date   : 2017-01-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "modelManager.h"
#include "textureManager.h"
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CModel
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CModel() {}
	~CModel() {}

	virtual KFRESULT	Init(void) { return KF_SUCCEEDED; }
	virtual void		Uninit(void) = 0;
	virtual void		Draw(const CKFMtx44 &mtxWorldParents, const CMM::MATERIAL &matType, const CTM::TEX_NAME &texName) = 0;

	const CMOM::MODEL_TYPE GetModelType(void) const { return m_modelType; }

protected:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct XFILE
	{
		std::vector<LPDIRECT3DTEXTURE9>	vectorTexture;	//textureインターフェース
		LPD3DXMESH						pMesh;			//メッシュ情報インターフェイス
		LPD3DXBUFFER					pBufferMaterial;//マテリアル情報　動的メモリ
		DWORD							dwNumMaterial;	//モデルのマテリアル数
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	static KFRESULT LoadXFile(XFILE* pXFile, const LPCSTR &pXFilePath);
	static void		DrawXFile(const XFILE &XFile, const CKFMtx44 &mtxWorldParents);
	static void		DrawXFile(const XFILE &XFile, const CKFMtx44 &mtxWorldParents, const CMM::MATERIAL &matType);
	static void		DrawXFile(const XFILE &XFile, const CKFMtx44 &mtxWorldParents, const CTM::TEX_NAME &texName);
	static void		DrawXFile(const XFILE &XFile, const CKFMtx44 &mtxWorldParents, const CMM::MATERIAL &matType, const CTM::TEX_NAME &texName);
	static void		ReleaseXFile(XFILE* pXFile);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	CMOM::MODEL_TYPE m_modelType;
};

#endif
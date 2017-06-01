//--------------------------------------------------------------------------------
//
//	XFileモデル
//　modelX.h
//	Author : Xu Wenjie
//	
//--------------------------------------------------------------------------------
#ifndef _MODEL_X_H_
#define _MODEL_X_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "model.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CModelX : public CModel
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CModelX();
	~CModelX() {}

	KFRESULT	Init(const LPCSTR &pXFilePath);
	void		Uninit(void) override;
	void		Draw(const CKFMtx44 &mtxWorldParents, const CMM::MATERIAL &matType, const CTM::TEX_NAME &texName) override;

	static CModelX* Create(const LPCSTR &pXFilePath);

private:

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	XFILE		m_XFileInfo;
};

#endif
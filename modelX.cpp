//--------------------------------------------------------------------------------
//
//	XFileモデル
//　modelX.cpp
//	Author : Xu Wenjie
//	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "modelX.h"

#ifdef USING_DIRECTX9
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CModelX::CModelX() : CModel()
{
	m_XFileInfo.pMesh = NULL;
	m_XFileInfo.pBufferMaterial = NULL;
	m_XFileInfo.dwNumMaterial = 0;
	m_XFileInfo.vectorTexture.clear();
}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
KFRESULT CModelX::Init(const LPCSTR &pXFilePath)
{
	KFRESULT result = LoadXFile(&m_XFileInfo, pXFilePath);
	return result;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CModelX::Uninit(void)
{
	ReleaseXFile(&m_XFileInfo);
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CModelX::Draw(const CKFMtx44 &mtxWorldParents, const CMM::MATERIAL &matType, const CTM::TEX_NAME &texName)
{
	if (texName == CTM::TEX_NONE && matType == CMM::MAT_NORMAL)
	{
		DrawXFile(m_XFileInfo, mtxWorldParents);
	}
	else if (texName == CTM::TEX_NONE)
	{
		DrawXFile(m_XFileInfo, mtxWorldParents, matType);
	}
	else if (matType == CMM::MAT_NORMAL)
	{
		DrawXFile(m_XFileInfo, mtxWorldParents, texName);
	}
	else
	{
		DrawXFile(m_XFileInfo, mtxWorldParents, matType, texName);
	}
}

//--------------------------------------------------------------------------------
//  作成
//--------------------------------------------------------------------------------
CModelX *CModelX::Create(const LPCSTR &pXFilePath)
{
	CModelX *pModelX = NULL;
	pModelX = new CModelX;
	pModelX->Init(pXFilePath);
	pModelX->m_modelType = CMOM::XFILE;
	return pModelX;
}

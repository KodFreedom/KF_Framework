//--------------------------------------------------------------------------------
//
//	XFile���f��
//�@modelX.cpp
//	Author : Xu Wenjie
//	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "modelX.h"

#ifdef USING_DIRECTX9
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CModelX::CModelX() : CModel()
{
	m_XFileInfo.pMesh = NULL;
	m_XFileInfo.pBufferMaterial = NULL;
	m_XFileInfo.dwNumMaterial = 0;
	m_XFileInfo.vectorTexture.clear();
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
KFRESULT CModelX::Init(const LPCSTR &pXFilePath)
{
	KFRESULT result = LoadXFile(&m_XFileInfo, pXFilePath);
	return result;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CModelX::Uninit(void)
{
	ReleaseXFile(&m_XFileInfo);
}

//--------------------------------------------------------------------------------
//  �`�揈��
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
//  �쐬
//--------------------------------------------------------------------------------
CModelX *CModelX::Create(const LPCSTR &pXFilePath)
{
	CModelX *pModelX = NULL;
	pModelX = new CModelX;
	pModelX->Init(pXFilePath);
	pModelX->m_modelType = CMOM::XFILE;
	return pModelX;
}

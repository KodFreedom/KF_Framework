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

#ifdef USING_DIRECTX
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
bool CModelX::Init(const LPCSTR &pXFilePath)
{
	bool result = LoadXFile(&m_XFileInfo, pXFilePath);
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
void CModelX::Draw(const CKFMtx44 &mtxWorldParents, const CMM::MATERIAL &matType, const string& strTexName)
{
	if (strTexName.empty() && matType == CMM::MAT_NORMAL)
	{
		DrawXFile(m_XFileInfo, mtxWorldParents);
	}
	else if (strTexName.empty())
	{
		DrawXFile(m_XFileInfo, mtxWorldParents, matType);
	}
	else if (matType == CMM::MAT_NORMAL)
	{
		DrawXFile(m_XFileInfo, mtxWorldParents, strTexName);
	}
	else
	{
		DrawXFile(m_XFileInfo, mtxWorldParents, matType, strTexName);
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

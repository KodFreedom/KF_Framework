//--------------------------------------------------------------------------------
//
//�@model.cpp
//	Author : Xu Wenjie
//	Date   : 2017-01-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "rendererDX.h"
#include "model.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  XFile�̓ǂݍ���
//--------------------------------------------------------------------------------
KFRESULT CModel::LoadXFile(XFILE* pXFile, const LPCSTR &pXFilePath)
{
	//�`�F�b�N�|�C���^
	if (pXFile == NULL) { return KF_FAILED; }

	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//�n�[�h�f�B�X�N����X�t�@�C���̓ǂݍ���
	HRESULT hr = D3DXLoadMeshFromX(
		pXFilePath,					//�t�@�C����
		D3DXMESH_MANAGED,
		pDevice,
		NULL,						//�אڏ��
		&pXFile->pBufferMaterial,	//���f���̃}�e���A�����
		NULL,
		&pXFile->dwNumMaterial,		//���f���̃}�e���A����
		&pXFile->pMesh);			//���b�V�����

	if (FAILED(hr))
	{
		MessageBox(NULL, "CModel : D3DXLoadMeshFromX ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return KF_FAILED;
	}

	//texture
	D3DXMATERIAL *pMat;//�}�e���A�������󂯎���p�̃|�C���^

	pMat = (D3DXMATERIAL*)pXFile->pBufferMaterial->GetBufferPointer();
	for (int nCnt = 0; nCnt < (int)pXFile->dwNumMaterial; nCnt++)
	{
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		if (pMat[nCnt].pTextureFilename != NULL)
		{
			//�n�[�h�f�B�X�N����Texture�̓ǂݍ���
			//���G���[�`�F�b�N�K�{
			hr = D3DXCreateTextureFromFile(pDevice,
				pMat[nCnt].pTextureFilename,
				&pTexture);

			if (FAILED(hr))
			{
				char aStr[128];
				wsprintf(aStr, "CModel %d�Ԃ̃e�N�X�`�� ERROR!", nCnt);
				MessageBox(NULL, aStr, "�G���[", MB_OK | MB_ICONWARNING);
			}
		}
		pXFile->vectorTexture.push_back(pTexture);
	}

	return KF_SUCCEEDED;
}

//--------------------------------------------------------------------------------
//  XFile�̕`��
//--------------------------------------------------------------------------------
void CModel::DrawXFile(const XFILE &XFile, const CKFMtx44 &mtxWorldParents)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	
	//�}�g���b�N�X�ݒ�
	D3DXMATRIX mtx = mtxWorldParents;
	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	
	//���݃f�o�C�X�ɐݒ肳��Ă�}�e���A�������擾
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	//�}�e���A�������󂯎��
	D3DXMATERIAL *pMat;
	pMat = (D3DXMATERIAL*)XFile.pBufferMaterial->GetBufferPointer();

	for (int nCnt = 0; nCnt < (int)XFile.dwNumMaterial; nCnt++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCnt].MatD3D);

		//Texture
		if (nCnt < (int)XFile.vectorTexture.size())
		{
			pDevice->SetTexture(0, XFile.vectorTexture[nCnt]);
		}

		//���b�V���̕`��
		XFile.pMesh->DrawSubset(nCnt);
	}

	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------------------------------------
//  XFile�̕`��(Material�w��)
//--------------------------------------------------------------------------------
void CModel::DrawXFile(const XFILE &XFile, const CKFMtx44 &mtxWorldParents, const CMM::MATERIAL &matType)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//�}�g���b�N�X�ݒ�
	D3DXMATRIX mtx = mtxWorldParents;
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	//���݃f�o�C�X�ɐݒ肳��Ă�}�e���A�������擾
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// �}�e���A���̐ݒ�
	D3DMATERIAL9 mat = GetManager()->GetMaterialManager()->GetMaterial(matType);
	pDevice->SetMaterial(&mat);

	for (int nCnt = 0; nCnt < (int)XFile.dwNumMaterial; nCnt++)
	{
		//Texture
		if (nCnt < (int)XFile.vectorTexture.size())
		{
			pDevice->SetTexture(0, XFile.vectorTexture[nCnt]);
		}

		//���b�V���̕`��
		XFile.pMesh->DrawSubset(nCnt);
	}

	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------------------------------------
//  XFile�̕`��(Texture�w��)
//--------------------------------------------------------------------------------
void CModel::DrawXFile(const XFILE &XFile, const CKFMtx44 &mtxWorldParents, const CTM::TEX_NAME &texName)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//�}�g���b�N�X�ݒ�
	D3DXMATRIX mtx = mtxWorldParents;
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	//���݃f�o�C�X�ɐݒ肳��Ă�}�e���A�������擾
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	//�}�e���A�������󂯎��
	D3DXMATERIAL *pMat;
	pMat = (D3DXMATERIAL*)XFile.pBufferMaterial->GetBufferPointer();

	// �e�N�X�`���̐ݒ�
	LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture(texName);
	pDevice->SetTexture(0, pTexture);

	for (int nCnt = 0; nCnt < (int)XFile.dwNumMaterial; nCnt++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCnt].MatD3D);

		//���b�V���̕`��
		XFile.pMesh->DrawSubset(nCnt);
	}

	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------------------------------------
//  XFile�̕`��(Material��Texture�w��)
//--------------------------------------------------------------------------------
void CModel::DrawXFile(const XFILE &XFile, const CKFMtx44 &mtxWorldParents, const CMM::MATERIAL &matType, const CTM::TEX_NAME &texName)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//�}�g���b�N�X�ݒ�
	D3DXMATRIX mtx = mtxWorldParents;
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	//���݃f�o�C�X�ɐݒ肳��Ă�}�e���A�������擾
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// �}�e���A���̐ݒ�
	D3DMATERIAL9 mat = GetManager()->GetMaterialManager()->GetMaterial(matType);
	pDevice->SetMaterial(&mat);

	// �e�N�X�`���̐ݒ�
	LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture(texName);
	pDevice->SetTexture(0, pTexture);

	for (int nCnt = 0; nCnt < (int)XFile.dwNumMaterial; nCnt++)
	{
		//���b�V���̕`��
		XFile.pMesh->DrawSubset(nCnt);
	}

	pDevice->SetMaterial(&matDef);
}

//--------------------------------------------------------------------------------
//  XFile�̔j��
//--------------------------------------------------------------------------------
void CModel::ReleaseXFile(XFILE* pXFile)
{
	if (pXFile != NULL)
	{
		if (pXFile->pMesh != NULL)
		{
			pXFile->pMesh->Release();
			pXFile->pMesh = NULL;
		}

		if (pXFile->pBufferMaterial != NULL)
		{
			pXFile->pBufferMaterial->Release();
			pXFile->pBufferMaterial = NULL;
		}

		for (int nCnt = 0; nCnt < (int)pXFile->vectorTexture.size(); nCnt++)
		{
			if (pXFile->vectorTexture[nCnt] != NULL)
			{
				pXFile->vectorTexture[nCnt]->Release();
				pXFile->vectorTexture[nCnt] = NULL;
			}
		}

		pXFile->vectorTexture.clear();
	}
}
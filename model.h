//--------------------------------------------------------------------------------
//
//�@model.h
//	Author : Xu Wenjie
//	Date   : 2017-01-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "modelManager.h"
#include "textureManager.h"
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CModel
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CModel() {}
	~CModel() {}

	virtual KFRESULT	Init(void) { return KF_SUCCEEDED; }
	virtual void		Uninit(void) = 0;
	virtual void		Draw(const CKFMtx44 &mtxWorldParents, const CMM::MATERIAL &matType, const CTM::TEX_NAME &texName) = 0;

	const CMOM::MODEL_TYPE GetModelType(void) const { return m_modelType; }

protected:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct XFILE
	{
		std::vector<LPDIRECT3DTEXTURE9>	vectorTexture;	//texture�C���^�[�t�F�[�X
		LPD3DXMESH						pMesh;			//���b�V�����C���^�[�t�F�C�X
		LPD3DXBUFFER					pBufferMaterial;//�}�e���A�����@���I������
		DWORD							dwNumMaterial;	//���f���̃}�e���A����
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	static KFRESULT LoadXFile(XFILE* pXFile, const LPCSTR &pXFilePath);
	static void		DrawXFile(const XFILE &XFile, const CKFMtx44 &mtxWorldParents);
	static void		DrawXFile(const XFILE &XFile, const CKFMtx44 &mtxWorldParents, const CMM::MATERIAL &matType);
	static void		DrawXFile(const XFILE &XFile, const CKFMtx44 &mtxWorldParents, const CTM::TEX_NAME &texName);
	static void		DrawXFile(const XFILE &XFile, const CKFMtx44 &mtxWorldParents, const CMM::MATERIAL &matType, const CTM::TEX_NAME &texName);
	static void		ReleaseXFile(XFILE* pXFile);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	CMOM::MODEL_TYPE m_modelType;
};

#endif
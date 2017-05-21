//--------------------------------------------------------------------------------
//	3D���b�V���R���|�l���g
//�@3DmeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18
//--------------------------------------------------------------------------------
#ifndef _3D_MESH_COMPONENT_H_
#define _3D_MESH_COMPONENT_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "meshComponent.h"
#include "textureManager.h"
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  3D���b�V���R���|�l���g�N���X
//--------------------------------------------------------------------------------
class C3DMeshComponent : public CMeshComponent
{
public:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct MESH3D
	{
		int						nNumVtx;
		int						nNumIdx;
		int						nNumPolygon;

#ifdef USING_DIRECTX9
		LPDIRECT3DVERTEXBUFFER9 pVtxBuffer;	// ���_�o�b�t�@�ւ̃|�C���^
		LPDIRECT3DINDEXBUFFER9	pIdxBuffer;	// �C���f�b�N�X�ւ̃|�C���^
#endif
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	C3DMeshComponent() : CMeshComponent()
		, m_texName(CTM::TEX_NONE)
		, m_matType(CMM::MAT_NORMAL)
	{
		m_meshInfo.nNumIdx = 0;
		m_meshInfo.nNumPolygon = 0;
		m_meshInfo.nNumVtx = 0;
		m_meshInfo.pIdxBuffer = NULL;
		m_meshInfo.pVtxBuffer = NULL;
	}

	~C3DMeshComponent() {}

	virtual KFRESULT		Init(void) override = 0;
	void					Uninit(void) override;
	void					Update(CGameObject &gameObj) override;

	//Get�֐�
	const CTM::TEX_NAME&	GetTexName(void) const { return m_texName; }
	const CMM::MATERIAL&	GetMatType(void) const { return m_matType; }
	const MESH3D&			GetMeshInfo(void) const { return m_meshInfo; }

	//Set�֐�
	void					SetTexName(const CTM::TEX_NAME& texName) { m_texName = texName; }
	void					SetMatType(const CMM::MATERIAL& matType) { m_matType = matType; }

protected:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	KFRESULT			CreateBuffer(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	CTM::TEX_NAME		m_texName;		//�e�N�X�`��
	CMM::MATERIAL		m_matType;		//�}�e���A��
	MESH3D				m_meshInfo;		//���b�V�����
};

#endif
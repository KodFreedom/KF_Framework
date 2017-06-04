//--------------------------------------------------------------------------------
//	2D���b�V���R���|�l���g
//�@2DmeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18
//--------------------------------------------------------------------------------
#ifndef _2D_MESH_COMPONENT_H_
#define _2D_MESH_COMPONENT_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "meshComponent.h"
#include "textureManager.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ���b�V���R���|�l���g�N���X
//--------------------------------------------------------------------------------
class C2DMeshComponent : public CMeshComponent
{
public:
	C2DMeshComponent(CGameObject* pGameObj) : CMeshComponent(pGameObj, MESH_2D)
		, m_cColor(CKFColor(1.0f))
		, m_texName(CTM::TEX_NONE)
#ifdef USING_DIRECTX9
		, m_pVtxBuffer(NULL)
#endif
	{
	}

	~C2DMeshComponent() {}

	KFRESULT	Init(void) override;
	void		Uninit(void) override;
	void		Update(void) override;

	//Get�֐�
	const CTM::TEX_NAME&	GetTexName(void) const { return m_texName; }
	const int&				GetNumVertex(void) const { return s_nNumVtx; }
	const int&				GetNumPolygon(void) const { return s_nNumPolygon; }
#ifdef USING_DIRECTX9
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuffer(void) const { return m_pVtxBuffer; }
#endif

	//Set�֐�
	void					SetTexName(const CTM::TEX_NAME& texName) { m_texName = texName; }

private:
	CKFColor				m_cColor;		//�J���[
	CTM::TEX_NAME			m_texName;		//�e�N�X�`��
	
#ifdef USING_DIRECTX9
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;
#endif
	
	static const int	s_nNumVtx = 4;
	static const int	s_nNumPolygon = 2;
};

#endif
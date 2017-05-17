//--------------------------------------------------------------------------------
//
//�@gameObject2D.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_2D_H_
#define _GAMEOBJECT_2D_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObject.h"
#include "textureManager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObject2D : public CGameObject
{
public:
	CGameObject2D();
	~CGameObject2D();

	KFRESULT	Init(const CKFVec3 &vPos, const float &fRot, const CKFVec2 &vSize, const CTM::TEX_NAME &texName);
	void		Uninit(void) override;
	void		Update(void) override;
	void		LateUpdate(void) override;
	void		Draw(void) override;

	static CGameObject2D* Create(const CKFVec3 &vPos, const float &fRot, const CKFVec2 &vSize, const CTM::TEX_NAME &texName);
protected:
	virtual void		SetRenderState(void);
	virtual void		ResetRenderState(void);

	static const int	m_nNumVtx2D = 4;		//2D�I�u�W�F�N�g�̒��_��
	static const int	m_nNumPolygon2D = 2;	//2D�I�u�W�F�N�g�̃|���S����
	float				m_fRot;					//��]
	CKFVec2				m_vSize;				//�T�C�Y
	CKFColor			m_cColor;				//�J���[

private:
	KFRESULT			CreateBuffer(void);
	void				UpdateVertex(void);
	void				SetVtxPos(VERTEX_2D *pVtx);
	void				SetVtxColor(VERTEX_2D *pVtx);
	void				SetVtxUV(VERTEX_2D *pVtx);

	CTM::TEX_NAME		m_texName;		//�e�N�X�`��
};

#endif
//--------------------------------------------------------------------------------
//
//�@gameObject3D.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_3D_H_
#define _GAMEOBJECT_3D_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObject.h"
#include "textureManager.h"
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObject3D : public CGameObject
{
public:
	CGameObject3D();
	~CGameObject3D();

	virtual KFRESULT	Init(const int &nVtxNum, const int &nIdxNum, const int &nPolygonNum);
	virtual void		Uninit(void);
	virtual void		Update(void);
	virtual void		LateUpdate(void);
	virtual void		Draw(void);

protected:
	virtual void			SetMatrix(void);
	virtual void			SetRenderState(void);
	virtual void			ResetRenderState(void);

	CKFVec3					m_vRot;			// ��]
	CKFVec3					m_vScale;		// �g�嗦
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuffer;	// �C���f�b�N�X�ւ̃|�C���^
	CTM::TEX_NAME			m_texName;		// �e�N�X�`��
	CMM::MATERIAL			m_matType;		// �}�e���A��

private:
	KFRESULT	CreateBuffer(void);
	
	int			m_nVtxNum;		// ���_��
	int			m_nIdxNum;		// �C���f�b�N�X��
	int			m_nPolygonNum;	// �|���S����
};

#endif
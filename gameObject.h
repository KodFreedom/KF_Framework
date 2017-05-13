//--------------------------------------------------------------------------------
//
//�@gameObject.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObjectManager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObject
{
public:
	CGameObject() : m_vPos(CKFVec3(0.0f)), m_pVtxBuffer(NULL), m_nID(-1), m_pri(GOM::PRI_MAX) {}
	~CGameObject() {}
	
	virtual KFRESULT	Init(void) { return KF_SUCCEEDED; }
	virtual void		Uninit(void) = 0;
	virtual void		Update(void) = 0;
	virtual void		LateUpdate(void) = 0;
	virtual void		Draw(void) = 0;
	void				Release(void);

	//Get�֐�
	CKFVec3				GetPos(void);

	//Set�֐�
	void				SetPos(const CKFVec3 &vPos);

protected:
	CKFVec3						m_vPos;			//�I�u�W�F�N�g�ʒu
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffer;	//���_�o�b�t�@�Ǘ��C���^�[�t�F�[�X�|�C���^
	int							m_nID;
	GOM::PRIORITY				m_pri;
};

#endif
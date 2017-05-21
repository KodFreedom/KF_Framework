//--------------------------------------------------------------------------------
//	�Q�[���I�u�W�F�N�g�X�[�p�[�N���X
//�@gameObject.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObjectManager.h"
#include "inputComponent.h"
#include "drawComponent.h"
#include "physicComponent.h"
#include "meshComponent.h"
#include "collisionComponent.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �Q�[���I�u�W�F�N�g�N���X
//--------------------------------------------------------------------------------
class CGameObject
{
	friend class CGameObjectManager;

public:
	CGameObject(const GOM::PRIORITY &pri = GOM::PRI_3D);
	~CGameObject() {}
	
	virtual KFRESULT	Init(void) 
	{ 
		m_pInput->Init();
		m_pCollision->Init();
		m_pPhysic->Init();
		m_pMesh->Init();
		m_pDraw->Init();
		return KF_SUCCEEDED;
	}

	virtual void		Uninit(void)
	{
		m_pInput->Release();
		m_pCollision->Release();
		m_pPhysic->Release();
		m_pMesh->Release();
		m_pDraw->Release();
	}

	virtual void		Update(void)
	{
		SwitchParam();
		m_pInput->Update(*this);
	}

	virtual void		LateUpdate(void)
	{
		m_pCollision->Update(*this);
		m_pPhysic->Update(*this);
		m_pMesh->Update(*this);
	}

	virtual void		Draw(void)
	{
		m_pDraw->Draw(*this, *m_pMesh);
	}

	void				Release(void);

	//Get�֐�
	CKFVec3				GetPos(void) const { return m_vPos; }
	CKFVec3				GetRot(void) const { return m_vRot; }
	CKFVec3				GetScale(void) const { return m_vScale; }
	CKFVec3				GetPosNext(void) const { return m_vPosNext; }
	CKFVec3				GetRotNext(void) const { return m_vRotNext; }
	CKFVec3				GetScaleNext(void) const { return m_vScaleNext; }
	CKFMtx44			GetMatrix(void) const { return m_mtxThis; }

	//Set�֐�
	void				SetPos(const CKFVec3 &vPos) { m_vPos = vPos; }
	void				SetRot(const CKFVec3 &vRot) { m_vRot = vRot; }
	void				SetScale(const CKFVec3 &vScale) { m_vScale = vScale; }
	void				SetPosNext(const CKFVec3 &vPosNext) { m_vPosNext = vPosNext; }
	void				SetRotNext(const CKFVec3 &vRotNext) { m_vRotNext = vRotNext; }
	void				SetScaleNext(const CKFVec3 &vScaleNext) { m_vScaleNext = vScaleNext; }
	void				SetMatrix(const CKFMtx44 &mtx) { m_mtxThis = mtx; }

protected:
	virtual void		SwitchParam(void);

	//�R���|�l���g
	CInputComponent*		m_pInput;		//���͂ɂ��̏����p�[�c
	CPhysicComponent*		m_pPhysic;		//���������p�[�c
	CCollisionComponent*	m_pCollision;	//�R���W�����p�[�c
	CMeshComponent*			m_pMesh;		//���b�V���p�[�c
	CDrawComponent*			m_pDraw;		//�`�揈���p�[�c

	//�p�����[�^�[
	CKFVec3		m_vPos;			//�I�u�W�F�N�g�ʒu
	CKFVec3		m_vRot;			//�I�u�W�F�N�g��]
	CKFVec3		m_vScale;		//�I�u�W�F�N�g�T�C�Y
	CKFVec3		m_vPosNext;		//���̃I�u�W�F�N�g�ʒu
	CKFVec3		m_vRotNext;		//���̃I�u�W�F�N�g��]
	CKFVec3		m_vScaleNext;	//���̃I�u�W�F�N�g�T�C�Y
	CKFMtx44	m_mtxThis;		//�����̉�]���s�ړ��s��

private:
	//�p�����[�^�[(�Ǘ��p)
	bool				m_bActive;		//�����t���O
	bool				m_bAlive;		//������t���O
	GOM::PRIORITY		m_pri;			//�D��x

	//�k���R���|�l���g
	static CNullInputComponent		s_nullInput;
	static CNullPhysicComponent		s_nullPhysic;
	static CNullCollisionComponent	s_nullCollision;
	static CNullMeshComponent		s_nullMesh;
	static CNullDrawComponent		s_nullDraw;
};

//--------------------------------------------------------------------------------
//  �k���Q�[���I�u�W�F�N�g�N���X
//--------------------------------------------------------------------------------
class CNullGameObject : public CGameObject
{
public:
	CNullGameObject() : CGameObject() {}
	~CNullGameObject() {}

	void	Uninit(void) override {}
	void	Update(void) override {}
	void	LateUpdate(void) override {}
	void	Draw(void) override {}
};

#endif
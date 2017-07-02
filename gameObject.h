//--------------------------------------------------------------------------------
//	�Q�[���I�u�W�F�N�g�X�[�p�[�N���X
//�@gameObject.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObjectManager.h"
#include "inputComponent.h"
#include "drawComponent.h"
#include "rigidbodyComponent.h"
#include "meshComponent.h"
#include "colliderComponent.h"

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
	CGameObject();
	CGameObject(const GOM::PRIORITY &pri);
	~CGameObject() {}
	
	virtual KFRESULT	Init(void) 
	{ 
		m_pInput->Init();
		m_pRigidbody->Init();
		for (auto itr = m_apCollider.begin(); itr != m_apCollider.end(); itr++) { (*itr)->Init(); }
		m_pMesh->Init();
		m_pDraw->Init();
		return KF_SUCCEEDED;
	}

	virtual void		Uninit(void)
	{
		m_pInput->Release();
		m_pRigidbody->Release();
		for (auto itr = m_apCollider.begin(); itr != m_apCollider.end(); itr++) { (*itr)->Release(); }
		m_apCollider.clear();
		m_pMesh->Release();
		m_pDraw->Release();
	}

	virtual void		Update(void)
	{
		SwapParam();
		m_pInput->Update();
		m_pRigidbody->Update();
		for (auto itr = m_apCollider.begin(); itr != m_apCollider.end(); itr++) { (*itr)->Update(); }
	}

	virtual void		LateUpdate(void)
	{
		m_pMesh->Update();
	}

	virtual void		Draw(void)
	{
		m_pDraw->Draw();

#ifdef _DEBUG
		DrawNormal();
#endif
	}

	virtual void		Release(void);

	//Get�֐�
	CKFVec3					GetPos(void) const { return m_vPos; }
	CKFVec3					GetPosNext(void) const { return m_vPosNext; }
	CKFVec3					GetScale(void) const { return m_vScale; }
	CKFVec3					GetScaleNext(void) const { return m_vScaleNext; }
	CKFVec3					GetForward(void) const { return m_vForward; }
	CKFVec3					GetForwardNext(void) const { return m_vForwardNext; }
	CKFVec3					GetUp(void) const { return m_vUp; }
	CKFVec3					GetUpNext(void) const { return m_vUpNext; }
	CKFVec3					GetRight(void) const { return m_vRight; }
	CKFVec3					GetRightNext(void) const { return m_vRightNext; }
	CKFMtx44				GetMatrixRot(void);
	CKFMtx44				GetMatrix(void) const { return m_mtxThis; }
	CMeshComponent*			GetMeshComponent(void) { return m_pMesh; }
	CRigidbodyComponent*	GetRigidbodyComponent(void) { return m_pRigidbody; }

	//Set�֐�
	void					SetPos(const CKFVec3 &vPos) { m_vPos = vPos; }
	void					SetPosNext(const CKFVec3 &vPosNext) { m_vPosNext = vPosNext; }
	void					SetScale(const CKFVec3 &vScale) { m_vScale = vScale; }
	void					SetScaleNext(const CKFVec3 &vScaleNext) { m_vScaleNext = vScaleNext; }
	void					SetForward(const CKFVec3& vForward) { m_vForward = vForward; }
	void					SetForwardNext(const CKFVec3& vForward) { m_vForwardNext = vForward; }
	void					SetUp(const CKFVec3& vUp) { m_vUp = vUp; }
	void					SetUpNext(const CKFVec3& vUp) { m_vUpNext = vUp; }
	void					SetRight(const CKFVec3& vRight) { m_vRight = vRight; }
	void					SetRightNext(const CKFVec3& vRight) { m_vRightNext = vRight; }
	void					SetMatrix(const CKFMtx44 &mtx) { m_mtxThis = mtx; }

	//��]�֐�
	void					RotByEuler(const CKFVec3& vRot);
	void					RotByPitch(const float& fRadian);
	void					RotByYaw(const float& fRadian);
	void					RotByRoll(const float& fRadian);
	void					RotByUp(const CKFVec3& vUp);
	void					RotByForward(const CKFVec3& vForward);
	void					RotByRight(const CKFVec3& vRight);

protected:
	virtual void			SwapParam(void);

	//�R���|�l���g
	CInputComponent*				m_pInput;		//���͂ɂ��̏����p�[�c
	CRigidbodyComponent*			m_pRigidbody;	//���������p�[�c
	std::list<CColliderComponent*>	m_apCollider;	//�R���W�����p�[�c
	CMeshComponent*					m_pMesh;		//���b�V���p�[�c
	CDrawComponent*					m_pDraw;		//�`�揈���p�[�c

	//�p�����[�^�[
	CKFVec3		m_vPos;			//�I�u�W�F�N�g�ʒu
	CKFVec3		m_vPosNext;		//���̃I�u�W�F�N�g�ʒu
	CKFVec3		m_vScale;		//�I�u�W�F�N�g�T�C�Y
	CKFVec3		m_vScaleNext;	//���̃I�u�W�F�N�g�T�C�Y
	CKFVec3		m_vForward;		//�I�u�W�F�N�g�̑O����
	CKFVec3		m_vForwardNext;	//���̃I�u�W�F�N�g�̑O����
	CKFVec3		m_vUp;			//�I�u�W�F�N�g�̏����
	CKFVec3		m_vUpNext;		//���̃I�u�W�F�N�g�̏����
	CKFVec3		m_vRight;		//�I�u�W�F�N�g�̉E����
	CKFVec3		m_vRightNext;	//���̃I�u�W�F�N�g�̉E����
	CKFMtx44	m_mtxRot;		//�����̉�]�s��
	CKFMtx44	m_mtxThis;		//�����̉�]���s�ړ��s��

private:
#ifdef _DEBUG
	void		DrawNormal(void);
#endif

	//�p�����[�^�[(�Ǘ��p)
	bool				m_bActive;		//�����t���O
	bool				m_bAlive;		//������t���O
	GOM::PRIORITY		m_pri;			//�D��x

	//�k���R���|�l���g
	static CNullInputComponent		s_nullInput;
	static CNullRigidbodyComponent	s_nullRigidbody;
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
	void	Release(void) override {}
};
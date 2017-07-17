//--------------------------------------------------------------------------------
//	�g�����X�t�H�[���R���|�l���g
//�@transformComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "component.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CTransformComponent : public CComponent
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CTransformComponent(CGameObject* const pGameObj);
	~CTransformComponent() {}
	
	bool		Init(void) override { return true; }
	void			Uninit(void) override {}
	void			UpdateMatrix(void);	
	void			UpdateMatrix(const CKFMtx44& mtxParent);
	void			SwapParam(void);
	
	//�e�q�֌W
	void			RegisterChild(CTransformComponent* pChild);
	void			DeregisterChild(CTransformComponent* pChild);
	void			RegisterParent(CTransformComponent* pParent, const CKFVec3& vOffsetPos = CKFVec3(0.0f), const CKFVec3& vOffsetRot = CKFVec3(0.0f));

	//Matrix
	void			CalculateMtxThis(void);
	void			CalculateMtxThis(const CKFMtx44& mtxParent);

	//Get�֐�
	CKFVec3			GetPos(void) const { return m_vPos; }
	CKFVec3			GetPosNext(void) const { return m_vPosNext; }
	CKFQuaternion	GetRot(void);
	CKFQuaternion	GetRotNext(void);
	CKFVec3			GetScale(void) const { return m_vScale; }
	CKFVec3			GetScaleNext(void) const { return m_vScaleNext; }
	CKFVec3			GetForward(void) const { return m_vForward; }
	CKFVec3			GetForwardNext(void) const { return m_vForwardNext; }
	CKFVec3			GetUp(void) const { return m_vUp; }
	CKFVec3			GetUpNext(void) const { return m_vUpNext; }
	CKFVec3			GetRight(void) const { return m_vRight; }
	CKFVec3			GetRightNext(void) const { return m_vRightNext; }
	CKFMtx44		GetMatrixRot(void);
	CKFMtx44		GetMatrixRotNext(void);
	CKFMtx44		GetMatrix(void) const { return m_mtxThis; }
	CKFMtx44		GetMatrixNext(void);

	//Set�֐�
	void			SetPos(const CKFVec3& vPos) { m_vPos = vPos; }
	void			SetPosNext(const CKFVec3& vPosNext) { m_vPosNext = vPosNext; }
	void			MovePosNext(const CKFVec3& vMovement) { m_vPosNext += vMovement; }
	void			SetScale(const CKFVec3& vScale) { m_vScale = vScale; }
	void			SetScaleNext(const CKFVec3& vScaleNext) { m_vScaleNext = vScaleNext; }
	void			SetForward(const CKFVec3& vForward) { m_vForward = vForward; }
	void			SetForwardNext(const CKFVec3& vForward) { m_vForwardNext = vForward; }
	void			SetUp(const CKFVec3& vUp) { m_vUp = vUp; }
	void			SetUpNext(const CKFVec3& vUp) { m_vUpNext = vUp; }
	void			SetRight(const CKFVec3& vRight) { m_vRight = vRight; }
	void			SetRightNext(const CKFVec3& vRight) { m_vRightNext = vRight; }
	void			SetMatrix(const CKFMtx44& mtx) { m_mtxThis = mtx; }

	//��]�֐�
	void			RotByEuler(const CKFVec3& vRot);
	void			RotByPitch(const float& fRadian);
	void			RotByYaw(const float& fRadian);
	void			RotByRoll(const float& fRadian);
	void			RotByUp(const CKFVec3& vUp);
	void			RotByForward(const CKFVec3& vForward);
	void			RotByRight(const CKFVec3& vRight);

private:
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
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
	CKFMtx44	m_mtxThis;		//�����̉�]���s�ړ��s��

	//�e�q�֌W
	CTransformComponent*			m_pParent;
	list<CTransformComponent*> m_listChildren;
	CKFVec3							m_vOffsetPos;
	CKFVec3							m_vOffsetRot;
};
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
	
	bool					Init(void) override { return true; }
	void					Uninit(void) override {}
	void					UpdateMatrix(void);	
	void					UpdateMatrix(const CKFMtx44& mtxParent);
	void					SwapParam(void);
	
	//�e�q�֌W
	void					RegisterChild(CTransformComponent* pChild);
	void					DeregisterChild(CTransformComponent* pChild);
	void					RegisterParent(CTransformComponent* pParent, const CKFVec3& vOffsetPos = CKFVec3(0.0f), const CKFVec3& vOffsetRot = CKFVec3(0.0f));

	//Get�֐�
	const auto&				GetPos(void) const { return m_vPos; }
	const auto&				GetPosNext(void) const { return m_vPosNext; }
	const CKFQuaternion		GetRot(void) const;
	const CKFQuaternion		GetRotNext(void) const;
	const auto&				GetScale(void) const { return m_vScale; }
	const auto&				GetScaleNext(void) const { return m_vScaleNext; }
	const auto&				GetForward(void) const { return m_vForward; }
	const auto&				GetForwardNext(void) const { return m_vForwardNext; }
	const auto&				GetUp(void) const { return m_vUp; }
	const auto&				GetUpNext(void) const { return m_vUpNext; }
	const auto&				GetRight(void) const { return m_vRight; }
	const auto&				GetRightNext(void) const { return m_vRightNext; }
	const CKFMtx44			GetMatrixRot(void) const;
	const CKFMtx44			GetMatrixRotNext(void) const;
	const CKFMtx44&			GetMatrix(void) const { return m_mtxThis; }
	const CKFMtx44			GetMatrixWorldNext(void) const;
	const auto				GetParent(void) const { return m_pParent; }
	auto&					GetChildren(void) { return m_listChildren; }

	//Set�֐�
	void					SetPos(const CKFVec3& vPos) { m_vPos = vPos; }
	void					SetPosNext(const CKFVec3& vPosNext) { m_vPosNext = vPosNext; }
	void					SetRot(const CKFQuaternion& qRot);
	void					SetRotNext(const CKFQuaternion& qRotNext);
	void					MovePosNext(const CKFVec3& vMovement) { m_vPosNext += vMovement; }
	void					SetScale(const CKFVec3& vScale) { m_vScale = vScale; }
	void					SetScaleNext(const CKFVec3& vScaleNext) { m_vScaleNext = vScaleNext; }
	void					SetForward(const CKFVec3& vForward) { m_vForward = vForward; }
	void					SetForwardNext(const CKFVec3& vForward) { m_vForwardNext = vForward; }
	void					SetUp(const CKFVec3& vUp) { m_vUp = vUp; }
	void					SetUpNext(const CKFVec3& vUp) { m_vUpNext = vUp; }
	void					SetRight(const CKFVec3& vRight) { m_vRight = vRight; }
	void					SetRightNext(const CKFVec3& vRight) { m_vRightNext = vRight; }
	void					SetMatrix(const CKFMtx44& mtx) { m_mtxThis = mtx; }
	void					SetOffset(const CKFVec3& vPos, const CKFVec3& vRot)
	{
		m_vOffsetPos = vPos;
		m_vOffsetRot = vRot;
	}

	//��]�֐�
	void					RotByEuler(const CKFVec3& vRot);
	void					RotByPitch(const float& fRadian);
	void					RotByYaw(const float& fRadian);
	void					RotByRoll(const float& fRadian);
	void					RotByUp(const CKFVec3& vUp);
	void					RotByForward(const CKFVec3& vForward);
	void					RotByRight(const CKFVec3& vRight);

	//���̊֐�
	CKFVec3					TransformDirectionToWorld(const CKFVec3& vDirection);
	CKFVec3					TransformDirectionToLocal(const CKFVec3& vDirection);

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void		calculateMtxThis(void);
	void		calculateMtxThis(const CKFMtx44& mtxParent);

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
	list<CTransformComponent*>		m_listChildren;
	CKFVec3							m_vOffsetPos;
	CKFVec3							m_vOffsetRot;
};
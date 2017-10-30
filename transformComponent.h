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
#ifdef _DEBUG
	friend class CDebugManager;
#endif // _DEBUG

public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CTransformComponent(CGameObject* const pGameObj);
	~CTransformComponent() {}
	
	bool					Init(void) override { return true; }
	void					Uninit(void) override {}
	void					UpdateMatrix(void);	
	void					UpdateMatrix(const Matrix44& mtxParent);
	void					SwapParam(void);
	
	//�e�q�֌W
	void					RegisterChild(CTransformComponent* pChild);
	void					DeregisterChild(CTransformComponent* pChild);
	void					RegisterParent(CTransformComponent* pParent, const Vector3& vOffsetPos = Vector3(0.0f), const Vector3& vOffsetRot = Vector3(0.0f));

	//Get�֐�
	const auto&				GetPos(void) const { return m_vPos; }
	const auto&				GetPosNext(void) const { return m_vPosNext; }
	const Quaternion		GetRot(void) const;
	const Quaternion		GetRotNext(void) const;
	const Vector3			GetEulerRot(void) const;
	const auto&				GetScale(void) const { return m_vScale; }
	const auto&				GetScaleNext(void) const { return m_vScaleNext; }
	const auto&				GetForward(void) const { return m_vForward; }
	const auto&				GetForwardNext(void) const { return m_vForwardNext; }
	const auto&				GetUp(void) const { return m_vUp; }
	const auto&				GetUpNext(void) const { return m_vUpNext; }
	const auto&				GetRight(void) const { return m_vRight; }
	const auto&				GetRightNext(void) const { return m_vRightNext; }
	const Matrix44			GetMatrixRot(void) const;
	const Matrix44			GetMatrixRotNext(void) const;
	const Matrix44&			GetMatrix(void) const { return m_mtxThis; }
	const Matrix44			GetMatrixWorldNext(void) const;
	const auto				GetParent(void) const { return m_pParent; }
	auto&					GetChildren(void) { return m_listChildren; }

	//Set�֐�
	void					SetPos(const Vector3& vPos) { m_vPos = vPos; }
	void					SetPosNext(const Vector3& vPosNext) { m_vPosNext = vPosNext; }
	void					SetRot(const Quaternion& qRot);
	void					SetRotNext(const Quaternion& qRotNext);
	void					SetRotNext(const Vector3& vRot);
	void					MovePosNext(const Vector3& vMovement) { m_vPosNext += vMovement; }
	void					SetScale(const Vector3& vScale) { m_vScale = vScale; }
	void					SetScaleNext(const Vector3& vScaleNext) { m_vScaleNext = vScaleNext; }
	void					SetForward(const Vector3& vForward) { m_vForward = vForward; }
	void					SetForwardNext(const Vector3& vForward) { m_vForwardNext = vForward; }
	void					SetUp(const Vector3& vUp) { m_vUp = vUp; }
	void					SetUpNext(const Vector3& vUp) { m_vUpNext = vUp; }
	void					SetRight(const Vector3& vRight) { m_vRight = vRight; }
	void					SetRightNext(const Vector3& vRight) { m_vRightNext = vRight; }
	void					SetMatrix(const Matrix44& mtx) { m_mtxThis = mtx; }
	void					SetOffset(const Vector3& vPos, const Vector3& vRot)
	{
		m_vOffsetPos = vPos;
		m_vOffsetRot = vRot;
	}

	//��]�֐�
	void					RotByEuler(const Vector3& vRot);
	void					RotByPitch(const float& fRadian);
	void					RotByYaw(const float& fRadian);
	void					RotByRoll(const float& fRadian);
	void					RotByUp(const Vector3& vUp);
	void					RotByForward(const Vector3& vForward);
	void					RotByRight(const Vector3& vRight);

	//���̊֐�
	Vector3					TransformDirectionToWorld(const Vector3& vDirection);
	Vector3					TransformDirectionToLocal(const Vector3& vDirection);

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void		calculateMtxThis(void);
	void		calculateMtxThis(const Matrix44& mtxParent);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	//�p�����[�^�[
	Vector3		m_vPos;			//�I�u�W�F�N�g�ʒu
	Vector3		m_vPosNext;		//���̃I�u�W�F�N�g�ʒu
	Vector3		m_vScale;		//�I�u�W�F�N�g�T�C�Y
	Vector3		m_vScaleNext;	//���̃I�u�W�F�N�g�T�C�Y
	Vector3		m_vForward;		//�I�u�W�F�N�g�̑O����
	Vector3		m_vForwardNext;	//���̃I�u�W�F�N�g�̑O����
	Vector3		m_vUp;			//�I�u�W�F�N�g�̏����
	Vector3		m_vUpNext;		//���̃I�u�W�F�N�g�̏����
	Vector3		m_vRight;		//�I�u�W�F�N�g�̉E����
	Vector3		m_vRightNext;	//���̃I�u�W�F�N�g�̉E����
	Matrix44	m_mtxThis;		//�����̉�]���s�ړ��s��
	
	//�e�q�֌W
	CTransformComponent*			m_pParent;
	list<CTransformComponent*>		m_listChildren;
	Vector3							m_vOffsetPos;
	Vector3							m_vOffsetRot;
};
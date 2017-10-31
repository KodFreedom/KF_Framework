//--------------------------------------------------------------------------------
//	collider�R���|�l���g
//�@colliderComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "component.h"
#include "KF_CollisionSystem.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Collider�|�l���g�N���X
//--------------------------------------------------------------------------------
class CColliderComponent : public CComponent
{
public:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CColliderComponent(CGameObject* const pGameObj, const CS::COL_TYPE& type, const CS::COL_MODE& mode);
	~CColliderComponent();

	virtual bool		Init(void) override { return true; }
	virtual void		Uninit(void) override;
	virtual void		Update(void);
	
	//Set�֐�
	void				SetTrigger(const bool& bTrigger) { m_bTrigger = bTrigger; }
	void				SetTag(const string& strTag) { m_strTag = strTag; }
	void				SetOffset(const Vector3& Position, const Vector3& vRot = Vector3(0.0f));

	//Get�֐�
	Vector3				GetLocalPos(void) const { return Vector3(m_mtxOffset.Elements[3][0], m_mtxOffset.Elements[3][1], m_mtxOffset.Elements[3][2]); }
	Vector3				GetWorldPosNext(void) const { return Vector3(m_mtxWorldNext.Elements[3][0], m_mtxWorldNext.Elements[3][1], m_mtxWorldNext.Elements[3][2]); }
	Matrix44			GetMatrixWorldNext(void) const { return m_mtxWorldNext; }
	Matrix44			GetMatrixOffset(void) const { return m_mtxOffset; }
	const CS::COL_TYPE	GetType(void) const { return m_type; }
	const bool			IsTrigger(void) const { return m_bTrigger; }
	const string&		GetTag(void) const { return m_strTag; }

	//��
	void				Sleep(void);
	void				Awake(void);

protected:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CColliderComponent() : CComponent() , m_type(CS::COL_MAX), m_mode(CS::MODE_MAX) {}

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	CS::COL_TYPE	m_type;			//collider�̃^�C�v
	CS::COL_MODE	m_mode;			//Static/Dynamic mode
	bool			m_bTrigger;		//Trigger Flag
	Matrix44		m_mtxOffset;	//���΍s��
	Matrix44		m_mtxWorldNext;	//���[���h�s��(�������x����̂���)
	string			m_strTag;		//���ʗp�^�O
};
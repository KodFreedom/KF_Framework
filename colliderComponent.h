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
	~CColliderComponent() {}

	virtual bool		Init(void) override { return true; }
	virtual void		Uninit(void) override;
	virtual void		Update(void);
	
	//Set�֐�
	void				SetTrigger(const bool& bTrigger) { m_bTrigger = bTrigger; }
	void				SetTag(const string& strTag) { m_strTag = strTag; }
	void				SetOffset(const CKFVec3& vPos, const CKFVec3& vRot = CKFVec3(0.0f));

	//Get�֐�
	CKFVec3				GetLocalPos(void) const { return CKFVec3(m_mtxOffset.m_af[3][0], m_mtxOffset.m_af[3][1], m_mtxOffset.m_af[3][2]); }
	CKFVec3				GetWorldPos(void) const { return CKFVec3(m_mtxWorld.m_af[3][0], m_mtxWorld.m_af[3][1], m_mtxWorld.m_af[3][2]); }
	CKFMtx44			GetMatrixWorld(void) const { return m_mtxWorld; }
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
	CS::COL_TYPE	m_type;		//collider�̃^�C�v
	CS::COL_MODE	m_mode;		//Static/Dynamic mode
	bool			m_bTrigger;	//Trigger Flag
	CKFMtx44		m_mtxOffset;//���΍s��
	CKFMtx44		m_mtxWorld;	//���[���h�s��(�������x����̂���)
	string			m_strTag;	//���ʗp�^�O
};
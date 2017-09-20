//--------------------------------------------------------------------------------
//
//�@animatorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "component.h"
#include "motionStatus.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CMotion;
class CMotionInfo;
class CMotionKey;
class CColliderComponent;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �A�j���[�^�[�R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CAnimatorComponent : public CComponent
{
	//--------------------------------------------------------------------------------
	//  �t�����h�N���X
	//--------------------------------------------------------------------------------
	friend class CNormalMotionStatus;
	friend class CAwaitMotionStatus;
	friend class CBlendMotionStatus;

public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CAnimatorComponent(CGameObject* const pGameObj, const string& strPath);
	~CAnimatorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void);

	//Set�֐�
	void	SetAttack(const bool& bAttack);
	void	SetGrounded(const bool& bGrounded);
	void	SetJump(const bool& bJump);
	void	SetMove(const float& fMovement);

	//Get�֐�
	bool	CanAct(void);

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void			analyzeFile(const string& strPath);
	//void			updateAttack(void);
	void			changeMotionStatus(CMotionStatus* pMotionStatus);
	MOTION_PATTERN	getMotionNext(const MOTION_PATTERN& motion);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	CMotionInfo*				m_apMotionData[MP_MAX];
	vector<CGameObject*>		m_vecBorns;
	MOTION_PATTERN				m_motionNow;
	MOTION_PATTERN				m_motionNext;
	CMotionStatus*				m_pMotionStatus;
	bool						m_bIsGrounded;
};

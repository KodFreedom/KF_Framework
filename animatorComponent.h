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
	//  �񋓌^��`
	//--------------------------------------------------------------------------------
	enum MOTION_PATTERN
	{
		MP_NEUTAL,
		MP_RUN,
		MP_ATTACK,
		MP_JUMP,
		MP_LAND,
		MP_MAX
	};
	
	enum MOTION_STATUS
	{
		MS_NORMAL,
		MS_CHANGE,
		MS_WAIT,
	};

	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const int	sc_nChangeFrame = 10;

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void analyzeFile(const string& strPath);
	void changeMotion(const MOTION_PATTERN& motion);
	//void changeMotionImmediately(const MOTION_PATTERN& motion);
	bool checkCanChange(const MOTION_PATTERN& motion);
	void updateAttack(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	CMotionInfo*				m_apMotionData[MP_MAX];
	CMotionKey*					m_pMotionKeyLast;
	list<CColliderComponent*>	m_listAttackCollider;
	vector<CGameObject*>		m_vecBorns;
	MOTION_PATTERN				m_motionNow;
	MOTION_PATTERN				m_motionNext;
	MOTION_STATUS				m_status;
	int							m_nKeyNow;	
	int							m_nCntFrame;
	int							m_nCntChangeFrame;
	bool						m_bIsGrounded;
};

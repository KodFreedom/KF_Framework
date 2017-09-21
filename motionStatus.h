//--------------------------------------------------------------------------------
//	
//�@motionStatus.h
//	Author : Xu Wenjie
//	Date   : 2017-09-20
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CAnimatorComponent;

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

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�[�p�[�N���X
//--------------------------------------------------------------------------------
class CMotionStatus
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CMotionStatus(const int nKeyNow, const int nFrameNow)
		: m_nKeyNow(nKeyNow), m_nFrameNow(nFrameNow) {}
	~CMotionStatus() {};

	virtual void	Update(CAnimatorComponent& animator) = 0;
	virtual void	ChangeMotion(CAnimatorComponent& animator, const MOTION_PATTERN& motion) = 0;

	int				GetKeyNow(void) const { return m_nKeyNow; }
	int				GetFrameNow(void) const { return m_nFrameNow; }

protected:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	virtual bool	checkCanChange(CAnimatorComponent& animator, const MOTION_PATTERN& motion) = 0;

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	int	m_nKeyNow;
	int	m_nFrameNow;
};

//--------------------------------------------------------------------------------
//  �m�[�}�����[�V����
//--------------------------------------------------------------------------------
class CNormalMotionStatus : public CMotionStatus
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CNormalMotionStatus(const int nKeyNow = 0, const int nFrameNow = 0)
		: CMotionStatus(nKeyNow, nFrameNow) {}
	~CNormalMotionStatus() {}

	void Update(CAnimatorComponent& animator) override;
	void ChangeMotion(CAnimatorComponent& animator, const MOTION_PATTERN& motion) override;

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	bool checkCanChange(CAnimatorComponent& animator, const MOTION_PATTERN& motion) override;
};

//--------------------------------------------------------------------------------
//  �ҋ@���[�V����
//--------------------------------------------------------------------------------
class CAwaitMotionStatus : public CMotionStatus
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CAwaitMotionStatus(const int nKeyNow = 0, const int nFrameNow = 0)
		: CMotionStatus(nKeyNow, nFrameNow) {}
	~CAwaitMotionStatus() {}

	void Update(CAnimatorComponent& animator) override;
	void ChangeMotion(CAnimatorComponent& animator, const MOTION_PATTERN& motion) override;

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	bool checkCanChange(CAnimatorComponent& animator, const MOTION_PATTERN& motion) override;
};

//--------------------------------------------------------------------------------
//  �u�����h���[�V����
//--------------------------------------------------------------------------------
class CBlendMotionStatus : public CMotionStatus
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CBlendMotionStatus(const int nKeyNow = 0, const int nFrameNow = 0)
		: CMotionStatus(nKeyNow, nFrameNow)
		, m_nBlendKeyNow(0)
		, m_nBlendFrameNow(0)
		, m_nBlendFrameCnt(0) {}
	~CBlendMotionStatus() {}

	void Update(CAnimatorComponent& animator) override;
	void ChangeMotion(CAnimatorComponent& animator, const MOTION_PATTERN& motion) override;

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	bool checkCanChange(CAnimatorComponent& animator, const MOTION_PATTERN& motion) override;

	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const int	sc_nBlendFrame = 10;

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------
	int	m_nBlendKeyNow;
	int	m_nBlendFrameNow;
	int m_nBlendFrameCnt;
};
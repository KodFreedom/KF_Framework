//--------------------------------------------------------------------------------
//	
//　motionStatus.h
//	Author : Xu Wenjie
//	Date   : 2017-09-20
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CAnimatorComponent;

//--------------------------------------------------------------------------------
//  列挙型定義
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
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  スーパークラス
//--------------------------------------------------------------------------------
class CMotionStatus
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
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
	//  関数定義
	//--------------------------------------------------------------------------------
	virtual bool	checkCanChange(CAnimatorComponent& animator, const MOTION_PATTERN& motion) = 0;

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	int	m_nKeyNow;
	int	m_nFrameNow;
};

//--------------------------------------------------------------------------------
//  ノーマルモーション
//--------------------------------------------------------------------------------
class CNormalMotionStatus : public CMotionStatus
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CNormalMotionStatus(const int nKeyNow = 0, const int nFrameNow = 0)
		: CMotionStatus(nKeyNow, nFrameNow) {}
	~CNormalMotionStatus() {}

	void Update(CAnimatorComponent& animator) override;
	void ChangeMotion(CAnimatorComponent& animator, const MOTION_PATTERN& motion) override;

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	bool checkCanChange(CAnimatorComponent& animator, const MOTION_PATTERN& motion) override;
};

//--------------------------------------------------------------------------------
//  待機モーション
//--------------------------------------------------------------------------------
class CAwaitMotionStatus : public CMotionStatus
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CAwaitMotionStatus(const int nKeyNow = 0, const int nFrameNow = 0)
		: CMotionStatus(nKeyNow, nFrameNow) {}
	~CAwaitMotionStatus() {}

	void Update(CAnimatorComponent& animator) override;
	void ChangeMotion(CAnimatorComponent& animator, const MOTION_PATTERN& motion) override;

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	bool checkCanChange(CAnimatorComponent& animator, const MOTION_PATTERN& motion) override;
};

//--------------------------------------------------------------------------------
//  ブレンドモーション
//--------------------------------------------------------------------------------
class CBlendMotionStatus : public CMotionStatus
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
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
	//  関数定義
	//--------------------------------------------------------------------------------
	bool checkCanChange(CAnimatorComponent& animator, const MOTION_PATTERN& motion) override;

	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------
	static const int	sc_nBlendFrame = 10;

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------
	int	m_nBlendKeyNow;
	int	m_nBlendFrameNow;
	int m_nBlendFrameCnt;
};
////--------------------------------------------------------------------------------
////	�A�N�^�[���b�V���R���|�l���g
////�@actorMeshComponent.h
////	Author : Xu Wenjie
////	Date   : 2017-05-22
////--------------------------------------------------------------------------------
//#pragma once
//
////--------------------------------------------------------------------------------
////  �C���N���[�h�t�@�C��
////--------------------------------------------------------------------------------
//#include "modelMeshComponent.h"
//#include "modelActorX.h"
//
////--------------------------------------------------------------------------------
////  �O���錾
////--------------------------------------------------------------------------------
//
////--------------------------------------------------------------------------------
////  �N���X�錾
////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
//// �A�N�^�[���b�V���R���|�l���g�N���X
////--------------------------------------------------------------------------------
//class CActorMeshComponent : public CModelMeshComponent
//{
//public:
//	//--------------------------------------------------------------------------------
//	//  �\���̒�`
//	//--------------------------------------------------------------------------------
//	enum MOTION
//	{
//		MOTION_NEUTAL = 0,
//		MOTION_MOVE,
//		MOTION_ATTACK,
//		MOTION_JUMP,
//		MOTION_LANDING,
//		MOTION_MAX
//	};
//
//	struct ACTOR_MOTION_INFO
//	{
//		MOTION	motionNow;		//���̃��[�V����
//		MOTION	motionNext;		//���̃��[�V����
//		int		nKeyNow;		//���̃L�[�t���[��
//		int		nCntFrame;		//���̃t���[��
//		vector<CModelActorX::PARTS_INFO>	vectorPartsInfo;	// �p�[�c���
//	};
//
//	//--------------------------------------------------------------------------------
//	//  �֐���`
//	//--------------------------------------------------------------------------------
//	CActorMeshComponent(CGameObject* const pGameObj) : CModelMeshComponent(pGameObj)
//		, m_status(MS_NORMAL)
//		, m_nCntChangeFrame(0)
//	{
//		m_motionInfo.nCntFrame = 0;
//		m_motionInfo.nKeyNow = 0;
//		m_motionInfo.motionNow = MOTION_NEUTAL;
//		m_motionInfo.vectorPartsInfo.clear();
//	}
//
//	~CActorMeshComponent() {}
//
//	virtual bool		Init(void) override;
//	virtual void		Uninit(void) override;
//	virtual void		Update(void) override;
//
//	//Get�֐�
//	ACTOR_MOTION_INFO&	GetMotionInfo(void) { return m_motionInfo; }
//	MOTION				GetMotionNow(void) const { return m_motionInfo.motionNow; }
//
//	//Set�֐�
//	void SetMotion(const MOTION& motion);
//	void SetMotionAtNow(const MOTION& motion);
//
//private:
//	//--------------------------------------------------------------------------------
//	//  �񋓌^��`
//	//--------------------------------------------------------------------------------
//	enum MOTION_STATUS
//	{
//		MS_NORMAL,
//		MS_CHANGE
//	};
//
//	//--------------------------------------------------------------------------------
//	//  �萔��`
//	//--------------------------------------------------------------------------------
//	static const int	sc_nChangeFrame = 10;
//
//	//--------------------------------------------------------------------------------
//	//  �֐���`
//	//--------------------------------------------------------------------------------
//	void ChangeMotion(const MOTION& motion);
//	bool CanChangeMotion(const MOTION& motion);
//
//	//--------------------------------------------------------------------------------
//	//  �ϐ���`
//	//--------------------------------------------------------------------------------
//	MOTION_STATUS		m_status;
//	int					m_nCntChangeFrame;
//	ACTOR_MOTION_INFO	m_motionInfo;
//};
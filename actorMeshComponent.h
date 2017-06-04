//--------------------------------------------------------------------------------
//	�A�N�^�[���b�V���R���|�l���g
//�@actorMeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#ifndef _ACTOR_MESH_COMPONENT_H_
#define _ACTOR_MESH_COMPONENT_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "modelMeshComponent.h"
#include "modelActorX.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// �A�N�^�[���b�V���R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CActorMeshComponent : public CModelMeshComponent
{
public:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	enum MOTION
	{
		MOTION_NEUTAL = 0,
		MOTION_MOVE,
		MOTION_ATTACK,
		MOTION_JUMP,
		MOTION_LANDING,
		MOTION_MAX
	};

	struct ACTOR_MOTION_INFO
	{
		MOTION	motionNow;		//���̃��[�V����
		MOTION	motionNext;		//���̃��[�V����
		int		nKeyNow;		//���̃L�[�t���[��
		int		nCntFrame;		//���̃t���[��
		std::vector<CModelActorX::PARTS_INFO>	vectorPartsInfo;	// �p�[�c���
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CActorMeshComponent(CGameObject* const pGameObj) : CModelMeshComponent(pGameObj, MESH_ACTOR)
	{
		m_motionInfo.nCntFrame = 0;
		m_motionInfo.nKeyNow = 0;
		m_motionInfo.motionNow = MOTION_NEUTAL;
		m_motionInfo.vectorPartsInfo.clear();
	}

	~CActorMeshComponent() {}

	virtual KFRESULT		Init(void) override;
	virtual void			Uninit(void) override;
	virtual void			Update(void) override;

	//Get�֐�
	ACTOR_MOTION_INFO& GetMotionInfo(void) { return m_motionInfo; }
	MOTION				GetMotionNow(void) const { return m_motionInfo.motionNow; }

	//Set�֐�
	void SetMotionNext(const MOTION& motion);
	void SetMotionAtNow(const MOTION& motion);

private:
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	ACTOR_MOTION_INFO m_motionInfo;
};

#endif
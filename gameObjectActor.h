//--------------------------------------------------------------------------------
//
//�@gameObjectActor.h
//	Author : Xu Wenjie
//	Date   : 2017-05-15
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_ACTOR_H_
#define _GAMEOBJECT_ACTOR_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObjectModel.h"
#include "modelManager.h"
#include "modelActorX.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObjectActor : public CGameObjectModel
{
public:
	CGameObjectActor();
	~CGameObjectActor() {};

	void	Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName);
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override;
	void	Draw(void) override;

	static CGameObjectActor*	Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName);

private:
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

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void	UpdateMotion(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	MOTION	m_motionNow;	//���̃��[�V����
	int		m_nKeyNow;		//���̃L�[�t���[��
	int		m_nCntFrame;	//���̃t���[��
	std::vector<CModelActorX::PARTS_INFO>	m_vectorPartsInfo;	// �p�[�c���
};

#endif
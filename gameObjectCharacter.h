//--------------------------------------------------------------------------------
//
//�@gameObjectCharacter.h
//	Author : Xu Wenjie
//	Date   : 2017-05-15
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_CHARACTER_H_
#define _GAMEOBJECT_CHARACTER_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObjectModel.h"
#include "modelManager.h"
#include "modelCharacterX.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObjectCharacter : public CGameObjectModel
{
public:
	CGameObjectCharacter();
	~CGameObjectCharacter() {};

	void	Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName);
	void	Uninit(void);
	void	Update(void);
	void	LateUpdate(void);
	void	Draw(void);

	static CGameObjectCharacter*	Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName);

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

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	MOTION	m_motionNow;	//���̃��[�V����
	int		m_nKeyNow;		//���̃L�[�t���[��
	int		m_nCntFrame;	//���̃t���[��
	std::vector<CModelCharacterX::PARTS_INFO>	m_vectorPartsInfo;	// �p�[�c���
};

#endif
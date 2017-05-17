//--------------------------------------------------------------------------------
//
//�@modelCharacter.h
//	Author : Xu Wenjie
//	Date   : 2017-01-23
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MODEL_CHARACTOR_H_
#define _MODEL_CHARACTOR_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "model.h"
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CModelActorX : public CModel
{
public:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct KEY_FRAME
	{
		int		nFrame;
		CKFVec3 vPos;
		CKFVec3 vRot;
	};

	struct MOTION
	{
		bool					bLoop;			//���[�v�t���b�O
		std::vector<KEY_FRAME>	vectorKeyFrame;	//Key Frame���
	};

	typedef std::vector<MOTION>	VEC_MOTION;		//���[�V�����̓��I�z��

	struct PARTS_INFO
	{
		CKFVec3				vPos;			//�ʒu
		CKFVec3				vRot;			//��]
		CKFMtx44			mtxWorld;		//�����̃��[���h�}�g���N�X
		int					nParentID;		//�e��ID
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CModelActorX();
	~CModelActorX() {}

	KFRESULT	Init(const LPCSTR &pTxtPath);
	void		Uninit(void) override;
	void		Draw(const CKFMtx44 &mtxWorldParents, std::vector<PARTS_INFO> &vectorParts);
	void		Draw(const CKFMtx44 &mtxWorldParents, std::vector<PARTS_INFO> &vectorParts, const CMM::MATERIAL &matType);
	
	//Get�֐�
	std::vector<PARTS_INFO>			GetDefaultPartsInfo(void) const;
	const std::vector<VEC_MOTION>	&GetPartsMotionInfo(void) const;

	static CModelActorX *Create(const LPCSTR &pTxtPath);
private:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct ACTOR
	{
		std::vector<PARTS_INFO> vectorPartsInfoDefault;
		std::vector<VEC_MOTION>	vectorPartsMotionInfo;
		std::vector<XFILE>		vectorPartsXFileInfo;
	};

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	ACTOR		m_actorInfo;	//������Ԃ̃p�[�c
	float		m_fMoveSpeed;
	float		m_fJumpSpeed;
	float		m_fRadius;
};

#endif
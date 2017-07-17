//--------------------------------------------------------------------------------
//
//�@modelCharacter.h
//	Author : Xu Wenjie
//	Date   : 2017-01-23
//--------------------------------------------------------------------------------
#pragma once

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
		bool				bLoop;			//���[�v�t���b�O
		vector<KEY_FRAME>	vectorKeyFrame;	//Key Frame���
	};

	typedef vector<MOTION>	VEC_MOTION;		//���[�V�����̓��I�z��

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

	bool	Init(const LPCSTR &pTxtPath);
	void	Uninit(void) override;
	void	Draw(const CKFMtx44 &mtxWorldParents, const CMM::MATERIAL &matType, const string& strTexName) override;
	void	Draw(const CKFMtx44 &mtxWorldParents, vector<PARTS_INFO> &vectorParts, const CMM::MATERIAL &matType, const string& strTexName);
	
	//Get�֐�
	vector<PARTS_INFO>			GetDefaultPartsInfo(void) const;
	const vector<VEC_MOTION>	&GetPartsMotionInfo(void) const;

	static CModelActorX *Create(const LPCSTR &pTxtPath);
private:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct ACTOR
	{
		vector<PARTS_INFO>	vectorPartsInfoDefault;
		vector<VEC_MOTION>	vectorPartsMotionInfo;
		vector<XFILE>		vectorPartsXFileInfo;
	};

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	ACTOR		m_actorInfo;	//������Ԃ̃p�[�c
	float		m_fMoveSpeed;
	float		m_fJumpSpeed;
	float		m_fRadius;
};
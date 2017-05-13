//--------------------------------------------------------------------------------
//
//�@gameObjectModel.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_MODEL_H_
#define _GAMEOBJECT_MODEL_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObject.h"
#include "modelManager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObjectModel : public CGameObject
{
public:
	CGameObjectModel();
	~CGameObjectModel();

	void	Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName);
	void	Uninit(void);
	void	Update(void);
	void	LateUpdate(void);
	void	Draw(void);

	static CGameObjectModel*	Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName);

protected:
	void				SetMatrix(void);
	void				SetRenderState(void);
	void				ResetRenderState(void);

	CKFVec3				m_vRot;			// ��]
	CMOM::MODEL_NAME	m_modelName;	// ���f��
	CMM::MATERIAL		m_matType;		// �}�e���A��
};

#endif
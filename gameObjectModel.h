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
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObjectModel : public CGameObject
{
public:
	CGameObjectModel();
	~CGameObjectModel();

	virtual void	Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName);
	virtual void	Uninit(void) override;
	virtual void	Update(void) override;
	virtual void	LateUpdate(void) override;
	virtual void	Draw(void) override;

	static CGameObjectModel*	Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName);

protected:
	virtual void		SetMatrix(void);
	virtual void		SetRenderState(void);
	virtual void		ResetRenderState(void);

	CKFVec3				m_vRot;			// ��]
	CMOM::MODEL_NAME	m_modelName;	// ���f��
	CMM::MATERIAL		m_matType;		// �}�e���A��
};

#endif
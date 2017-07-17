//--------------------------------------------------------------------------------
//	���f�����b�V���R���|�l���g
//�@modelMeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "meshComponent.h"
#include "modelManager.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// ���f�����b�V���R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CModelMeshComponent : public CMeshComponent
{
public:
	CModelMeshComponent(CGameObject* const pGameObj) : CMeshComponent(pGameObj)
		, m_modelName(CMOM::MODEL_NONE)
	{
		m_strModelPath.clear();
	}

	~CModelMeshComponent() {}

	virtual bool			Init(void) override { return true; }
	virtual void			Uninit(void) override {}
	virtual void			Update(void) override {}

	//Get�֐�
	const string&			GetModelPath(void) const { return m_strModelPath; }
	const CMOM::MODEL_NAME& GetModelName(void) const { return m_modelName; }

	//Set�֐�
	void					SetModelPath(const string& strPath) { m_strModelPath = strPath; }
	void					SetModelName(const CMOM::MODEL_NAME& modelName) { m_modelName = modelName; }

protected:
	string					m_strModelPath;
	CMOM::MODEL_NAME		m_modelName;	//���f��
};
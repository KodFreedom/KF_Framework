//--------------------------------------------------------------------------------
//	モデルメッシュコンポネント
//　modelMeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "meshComponent.h"
#include "modelManager.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// モデルメッシュコンポネントクラス
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

	//Get関数
	const string&			GetModelPath(void) const { return m_strModelPath; }
	const CMOM::MODEL_NAME& GetModelName(void) const { return m_modelName; }

	//Set関数
	void					SetModelPath(const string& strPath) { m_strModelPath = strPath; }
	void					SetModelName(const CMOM::MODEL_NAME& modelName) { m_modelName = modelName; }

protected:
	string					m_strModelPath;
	CMOM::MODEL_NAME		m_modelName;	//モデル
};
//--------------------------------------------------------------------------------
//	モデルメッシュコンポネント
//　modelMeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#ifndef _MODEL_MESH_COMPONENT_H_
#define _MODEL_MESH_COMPONENT_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "meshComponent.h"
#include "textureManager.h"
#include "materialManager.h"
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
	CModelMeshComponent(CGameObject* const pGameObj, const MESH_TYPE& type = MESH_MODEL) : CMeshComponent(pGameObj, type)
		, m_modelName(CMOM::MODEL_NONE)
		, m_texName(CTM::TEX_NONE)
		, m_matType(CMM::MAT_NORMAL)
	{
	}

	~CModelMeshComponent() {}

	virtual KFRESULT		Init(void) override { return KF_SUCCEEDED; }
	virtual void			Uninit(void) override {}
	virtual void			Update(void) override {}

	//Get関数
	const CMOM::MODEL_NAME& GetModelName(void) const { return m_modelName; }
	const CTM::TEX_NAME&	GetTexName(void) const { return m_texName; }
	const CMM::MATERIAL&	GetMatType(void) const { return m_matType; }

	//Set関数
	void					SetModelName(const CMOM::MODEL_NAME& modelName) { m_modelName = modelName; }
	void					SetTexName(const CTM::TEX_NAME& texName) { m_texName = texName; }
	void					SetMatType(const CMM::MATERIAL& matType) { m_matType = matType; }

protected:
	CMOM::MODEL_NAME		m_modelName;	//モデル
	CTM::TEX_NAME			m_texName;		//テクスチャ
	CMM::MATERIAL			m_matType;		//マテリアル
};

#endif
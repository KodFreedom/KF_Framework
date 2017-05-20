//--------------------------------------------------------------------------------
//	3Dメッシュコンポネント
//　3DmeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18
//--------------------------------------------------------------------------------
#ifndef _3D_MESH_COMPONENT_H_
#define _3D_MESH_COMPONENT_H_

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
//  3Dメッシュコンポネントクラス
//--------------------------------------------------------------------------------
class C3DMeshComponent : public CMeshComponent
{
public:
	C3DMeshComponent() : CMeshComponent()
		, m_texName(CTM::TEX_NONE)
		, m_matType(CMM::MAT_NORMAL)
		, m_modelName(CMOM::MODEL_NONE)
	{}

	~C3DMeshComponent() {}

	KFRESULT	Init(void) override {}
	void		Uninit(void) override {}
	void		Update(CGameObject &gameObj) override {}

	//Get関数
	const CTM::TEX_NAME&	GetTexName(void) const { return m_texName; }
	const CMM::MATERIAL&	GetMatType(void) const { return m_matType; }
	const CMOM::MODEL_NAME& GetModelName(void) const { return m_modelName; }

	//Set関数
	void					SetTexName(const CTM::TEX_NAME& texName) { m_texName = texName; }
	void					SetMatType(const CMM::MATERIAL& matType) { m_matType = matType; }
	void					SetModel(const CMOM::MODEL_NAME &modelName) { m_modelName = modelName; }

private:
	CTM::TEX_NAME		m_texName;		//テクスチャ
	CMM::MATERIAL		m_matType;		//マテリアル
	CMOM::MODEL_NAME	m_modelName;	//モデル
};

#endif
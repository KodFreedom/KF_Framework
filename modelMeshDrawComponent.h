//--------------------------------------------------------------------------------
//	モデルメッシュ描画コンポネント
//　modelMeshDrawComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#ifndef _MODEL_MESH_DRAW_COMPONENT_H_
#define _MODEL_MESH_DRAW_COMPONENT_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "drawComponent.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  モデル描画コンポネントクラス
//--------------------------------------------------------------------------------
class CModelMeshDrawComponent : public CDrawComponent
{
public:
	CModelMeshDrawComponent(CGameObject* const pGameObj) : CDrawComponent(pGameObj) {}
	~CModelMeshDrawComponent() {}

	void	Draw(void) override;
};

#endif

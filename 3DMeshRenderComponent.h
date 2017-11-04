//--------------------------------------------------------------------------------
//	3Dメッシュ描画コンポネント
//　3DMeshRenderComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-21	
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "meshRenderer.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class C3DMeshComponent;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  3D描画コンポネントクラス
//--------------------------------------------------------------------------------
class C3DMeshRenderComponent : public MeshRenderer
{
public:
	C3DMeshRenderComponent(GameObject* const pGameObj)
		: MeshRenderer(pGameObj){}
	~C3DMeshRenderComponent() {}

	void	Render(void) override;
};
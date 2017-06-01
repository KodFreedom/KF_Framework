//--------------------------------------------------------------------------------
//	2D描画コンポネント
//　2DDrawComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-21	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "2DDrawComponent.h"
#include "2DMeshComponent.h"
#include "gameObject.h"
#include "manager.h"
#include "textureManager.h"

#ifdef USING_DIRECTX9
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void C2DDrawComponent::Draw(CGameObject &gameObj, const CMeshComponent &meshComponent)
{
	C2DMeshComponent *pMesh = (C2DMeshComponent*)&meshComponent;

#ifdef USING_DIRECTX9
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	// レンダーステート設定
	m_pRenderState->SetRenderState();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,						//ストリーム番号
		pMesh->GetVtxBuffer(),	//頂点バッファ
		0,						//オフセット
		sizeof(VERTEX_2D));		//ストライド量

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture(pMesh->GetTexName());
	pDevice->SetTexture(0, pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,							//オフセット
		pMesh->GetNumPolygon());	//ポリゴン数

	// レンダーステートリセット
	m_pRenderState->ResetRenderState();
#endif
}
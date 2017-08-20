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

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void C2DDrawComponent::Draw(void)
{
#ifdef USING_DIRECTX
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();

	// レンダーステート設定
	m_pRenderState->SetRenderState();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,							//ストリーム番号
		c_pMesh->GetVtxBuffer(),	//頂点バッファ
		0,							//オフセット
		sizeof(VERTEX_2D));			//ストライド量

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	LPDIRECT3DTEXTURE9 pTexture = CMain::GetManager()->GetTextureManager()->GetTexture(m_strTexName);
	pDevice->SetTexture(0, pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,							//オフセット
		c_pMesh->GetNumPolygon());	//ポリゴン数

	// レンダーステートリセット
	m_pRenderState->ResetRenderState();
#endif
}
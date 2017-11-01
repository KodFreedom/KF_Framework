////--------------------------------------------------------------------------------
////	2D描画コンポネント
////　2DRenderComponent.h
////	Author : Xu Wenjie
////	Date   : 2017-05-21	
////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
////  インクルードファイル
////--------------------------------------------------------------------------------
//#include "2DRenderComponent.h"
//#include "2DMeshComponent.h"
//#include "gameObject.h"
//#include "manager.h"
//#include "textureManager.h"
//
//#ifdef USING_DIRECTX
//#include "rendererDX.h"
//#endif
//
////--------------------------------------------------------------------------------
////  クラス
////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
////  描画処理
////--------------------------------------------------------------------------------
//void C2DRenderComponent::Render(void)
//{
//#ifdef USING_DIRECTX
//	auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
//
//	// 頂点バッファをデータストリームに設定
//	pDevice->SetStreamSource(
//		0,							//ストリーム番号
//		c_mesh->GetVtxBuffer(),	//頂点バッファ
//		0,							//オフセット
//		sizeof(VERTEX_2D));			//ストライド量
//
//	// 頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	// テクスチャの設定
//	LPDIRECT3DTEXTURE9 pTexture = Main::GetManager()->GetTextureManager()->GetTexture(m_strTexName);
//	pDevice->SetTexture(0, pTexture);
//
//	// ポリゴンの描画
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
//		0,							//オフセット
//		c_mesh->GetNumPolygon());	//ポリゴン数
//#endif
//}
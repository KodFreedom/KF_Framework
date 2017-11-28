//--------------------------------------------------------------------------------
//　ui_object_2d.cpp
//	2duiオブジェクト
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "ui_object_2d.h"
#include "main_system.h"
#include "texture_manager.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UIObject2D::Uninit(void)
{
	auto texture_manager = MainSystem::Instance()->GetTextureManager();
	for (auto iterator = sprites_.begin(); iterator != sprites_.end();)
	{
		texture_manager->Disuse(iterator->texture_name);
		iterator = sprites_.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void UIObject2D::Render(void)
{
	//for (auto itr = sprites_.begin(); itr != sprites_.end(); ++itr)
	//{
	//	/*
	//	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	//
	//	// 頂点バッファをデータストリームに設定
	//	pDevice->SetStreamSource(
	//		0,							//ストリーム番号
	//		itr->pVtxBuffer,			//頂点バッファ
	//		0,							//オフセット
	//		sizeof(VERTEX_2D));			//ストライド量
	//
	//	// 頂点フォーマットの設定
	//	pDevice->SetFVF(FVF_VERTEX_2D);
	//
	//	// テクスチャの設定
	//	auto texture = TextureManager::Instance()->GetTexture(itr->texture);
	//	pDevice->SetTexture(0, texture);
	//
	//	// ポリゴンの描画
	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
	//		0,							//オフセット
	//		itr->polygonNumber);			//ポリゴン数
	//	*/
	//}
}
//--------------------------------------------------------------------------------
//
//　UIObject2D.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "UIObject2D.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
UIObject2D::UIObject2D(const us order)
	: UIObject(order)
{
	sprites.clear();
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void UIObject2D::Uninit(void)
{
	for (auto itr = sprites.begin(); itr != sprites.end();)
	{
		TextureManager::Instance()->Disuse(itr->texture);
#ifdef USING_DIRECTX
		SAFE_RELEASE(itr->pVtxBuffer);
#endif
		itr = sprites.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void UIObject2D::Draw(void)
{
	for (auto itr = sprites.begin(); itr != sprites.end(); ++itr)
	{
		/*
		LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(
			0,							//ストリーム番号
			itr->pVtxBuffer,			//頂点バッファ
			0,							//オフセット
			sizeof(VERTEX_2D));			//ストライド量

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		auto texture = TextureManager::Instance()->GetTexture(itr->texture);
		pDevice->SetTexture(0, texture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,							//オフセット
			itr->polygonNumber);			//ポリゴン数
		*/
	}
}
//--------------------------------------------------------------------------------
//
//　2DUIObject.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "textureManager.h"
#include "2DUIObject.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
C2DUIObject::C2DUIObject(const unsigned short& usPriority)
	: CUIObject(usPriority)
{
	m_listSprite.clear();
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void C2DUIObject::Uninit(void)
{
	for (auto itr = m_listSprite.begin(); itr != m_listSprite.end();)
	{
		Main::GetManager()->GetTextureManager()->DisuseTexture(itr->strTexName);
		SAFE_RELEASE(itr->pVtxBuffer);
		itr = m_listSprite.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void C2DUIObject::Draw(void)
{
	for (auto itr = m_listSprite.begin(); itr != m_listSprite.end(); ++itr)
	{
#ifdef USING_DIRECTX
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
		LPDIRECT3DTEXTURE9 texture = Main::GetManager()->GetTextureManager()->GetTexture(itr->strTexName);
		pDevice->SetTexture(0, texture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,							//オフセット
			itr->usNumPolygon);			//ポリゴン数
#endif
	}
}
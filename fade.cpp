//--------------------------------------------------------------------------------
//
//　fade.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-09
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "fade.h"
#include "manager.h"
#include "textureManager.h"
#include "mode.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const float CFade::sc_fFadeRate = 1.0f / (120.0f * 0.5f);

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CFade::Init(void)
{
#ifdef USING_DIRECTX
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	CMain::GetManager()->GetTextureManager()->UseTexture("polygon.jpg");

	//頂点バッファ
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,				//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,					//頂点バッファの使用方法
		FVF_VERTEX_2D,						//書かなくても大丈夫
		D3DPOOL_MANAGED,					//メモリ管理方法(managed：デバイスにお任せ)
		&m_pVtxBuffer,						//頂点バッファのアドレス
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CFade : CreateVertexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
	}

	// 頂点情報を設定
	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点位置設定
	//頂点座標の設定（2D座標、右回り）
	pVtx[0].vPos = CKFVec3(0.0f, 0.0f, 0.0f);
	pVtx[1].vPos = CKFVec3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].vPos = CKFVec3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].vPos = CKFVec3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//頂点カラー設定
	pVtx[0].ulColor = m_cColor;
	pVtx[1].ulColor = m_cColor;
	pVtx[2].ulColor = m_cColor;
	pVtx[3].ulColor = m_cColor;

	//頂点UV設定
	pVtx[0].vUV = CKFVec2(0.0f, 0.0f);
	pVtx[1].vUV = CKFVec2(1.0f, 0.0f);
	pVtx[2].vUV = CKFVec2(0.0f, 1.0f);
	pVtx[3].vUV = CKFVec2(1.0f, 1.0f);

	//rhwの設定(必ず1.0f)
	pVtx[0].fRhw = 1.0f;
	pVtx[1].fRhw = 1.0f;
	pVtx[2].fRhw = 1.0f;
	pVtx[3].fRhw = 1.0f;

	//仮想アドレス解放
	m_pVtxBuffer->Unlock();
#endif
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CFade::Uninit(void)
{
#ifdef USING_DIRECTX
	CMain::GetManager()->GetTextureManager()->DisuseTexture("polygon.jpg");
	SAFE_RELEASE(m_pVtxBuffer);
#endif
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CFade::Update(void)
{
	if (m_fade == FADE_NONE) { return; }

	//fade時間カウント
	if (m_nCnt != 0)
	{
		m_nCnt -= 1;
		return;
	}

	if (m_fade == FADE_IN)
	{
		m_cColor.m_fA -= sc_fFadeRate;//alphaを減算して後ろの画面を浮き上がらせる

		if (m_cColor.m_fA <= 0.0f)
		{
			m_cColor.m_fA = 0.0f;
			m_fade = FADE_NONE;
		}
	}
	else if (m_fade == FADE_OUT)
	{
		m_cColor.m_fA += sc_fFadeRate;

		if (m_cColor.m_fA >= 1.0f)
		{
			m_cColor.m_fA = 1.0f;

			//Check SE & BGM(fade out effect)
			m_fade = FADE_IN;
			CMain::GetManager()->SetMode(m_pModeNext);
		}
	}

	SetColorFade(m_cColor);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CFade::Draw(void)
{
#ifdef USING_DIRECTX
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	LPDIRECT3DTEXTURE9 pTexture = CMain::GetManager()->GetTextureManager()->GetTexture("polygon.jpg");
	pDevice->SetTexture(0, pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
#endif
}

//--------------------------------------------------------------------------------
//  FadeToMode
//--------------------------------------------------------------------------------
void CFade::FadeToMode(CMode* pModeNext)
{
	if (m_fade == FADE_OUT) 
	{
		delete pModeNext;
		return;
	}

	m_fade = FADE_OUT;
	m_pModeNext = pModeNext;
	m_nCnt = 10;
}

//--------------------------------------------------------------------------------
//  SetColorFade
//--------------------------------------------------------------------------------
void CFade::SetColorFade(const CKFColor &cColor)
{
#ifdef USING_DIRECTX
	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx = NULL;

	//頂点バッファをロックして、仮想アドレスを取得する
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定(0～255の整数値)
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//仮想アドレス解放
	m_pVtxBuffer->Unlock();
#endif
}

//--------------------------------------------------------------------------------
//  作成
//--------------------------------------------------------------------------------
CFade *CFade::Create(void)
{
	CFade *pFade = new CFade;
	pFade->Init();
	return pFade;
}
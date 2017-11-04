//--------------------------------------------------------------------------------
//
//　fade.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-09
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "fadeSystem.h"
#include "manager.h"
#include "textureManager.h"
#include "mode.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void FadeSystem::Update(void)
{
	switch (currentState)
	{
	case State::None:
		break;
	case State::FadeIn:
		fadeIn();
		break;
	case State::FadeOut:
		fadeOut();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void FadeSystem::Draw(void)
{
	//#ifdef USING_DIRECTX
	//	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	//
	//	// 頂点バッファをデータストリームに設定
	//	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));
	//
	//	// 頂点フォーマットの設定
	//	pDevice->SetFVF(FVF_VERTEX_2D);
	//
	//	// テクスチャの設定
	//	LPDIRECT3DTEXTURE9 texture = Main::GetManager()->GetTextureManager()->GetTexture("polygon.jpg");
	//	pDevice->SetTexture(0, texture);
	//
	//	// ポリゴンの描画
	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	//#endif
}

//--------------------------------------------------------------------------------
//	関数名：FadeTo
//  関数説明：次のモードにフェードする
//	引数：	nextMode：次のモード
//			fadeTime：フェードの時間（秒数）
//	戻り値：なし
//--------------------------------------------------------------------------------
void FadeSystem::FadeTo(Mode* nextMode, const float fadeTime)
{
	if (currentState == State::FadeOut)
	{
		delete nextMode;
		return;
	}
	currentState = State::FadeOut;
	timeCounter = 0.0f;
	FadeSystem::nextMode = nextMode;
	FadeSystem::fadeTime = fadeTime;
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void FadeSystem::init(void)
{
//#ifdef USING_DIRECTX
//	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();
//	HRESULT hr;
//
//	Main::GetManager()->GetTextureManager()->UseTexture("polygon.jpg");
//
//	//頂点バッファ
//	hr = pDevice->CreateVertexBuffer(
//		sizeof(VERTEX_2D) * 4,				//作成したい頂点バッファのサイズ
//		D3DUSAGE_WRITEONLY,					//頂点バッファの使用方法
//		FVF_VERTEX_2D,						//書かなくても大丈夫
//		D3DPOOL_MANAGED,					//メモリ管理方法(managed：デバイスにお任せ)
//		&m_pVtxBuffer,						//頂点バッファのアドレス
//		NULL);
//
//	if (FAILED(hr))
//	{
//		MessageBox(NULL, "FadeSystem : CreateVertexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
//	}
//
//	// 頂点情報を設定
//	//仮想アドレスを取得するためのポインタ
//	VERTEX_2D *pVtx;
//
//	//頂点バッファをロックして、仮想アドレスを取得する
//	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
//
//	//頂点位置設定
//	//頂点座標の設定（2D座標、右回り）
//	pVtx[0].Position = Vector3(0.0f, 0.0f, 0.0f);
//	pVtx[1].Position = Vector3(SCREEN_WIDTH, 0.0f, 0.0f);
//	pVtx[2].Position = Vector3(0.0f, SCREEN_HEIGHT, 0.0f);
//	pVtx[3].Position = Vector3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
//
//	//頂点カラー設定
//	pVtx[0].Color = color;
//	pVtx[1].Color = color;
//	pVtx[2].Color = color;
//	pVtx[3].Color = color;
//
//	//頂点UV設定
//	pVtx[0].UV = Vector2(0.0f, 0.0f);
//	pVtx[1].UV = Vector2(1.0f, 0.0f);
//	pVtx[2].UV = Vector2(0.0f, 1.0f);
//	pVtx[3].UV = Vector2(1.0f, 1.0f);
//
//	//rhwの設定(必ず1.0f)
//	pVtx[0].Rhw = 1.0f;
//	pVtx[1].Rhw = 1.0f;
//	pVtx[2].Rhw = 1.0f;
//	pVtx[3].Rhw = 1.0f;
//
//	//仮想アドレス解放
//	m_pVtxBuffer->Unlock();
//#endif
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void FadeSystem::uninit(void)
{
//#ifdef USING_DIRECTX
//	Main::GetManager()->GetTextureManager()->DisuseTexture("polygon.jpg");
//	SAFE_RELEASE(m_pVtxBuffer);
//#endif
}

//--------------------------------------------------------------------------------
//  fadeIn
//--------------------------------------------------------------------------------
void FadeSystem::fadeIn(void)
{
	timeCounter -= DELTA_TIME;
	if (timeCounter <= 0.0f)
	{
		timeCounter = 0.0f;
		currentState = State::None;
	}
	setColor();
}

//--------------------------------------------------------------------------------
//  fadeOut
//--------------------------------------------------------------------------------
void FadeSystem::fadeOut(void)
{
	timeCounter += DELTA_TIME;
	if (timeCounter >= fadeTime)
	{
		timeCounter = fadeTime;

		// Todo : Check SE & BGM(fade out effect)
		currentState = State::FadeIn;
		Manager::Instance()->Change(nextMode);
	}
	setColor();
}

//--------------------------------------------------------------------------------
//	色の設定
//--------------------------------------------------------------------------------
void FadeSystem::setColor(void)
{
	color.A = timeCounter / fadeTime;
//#ifdef USING_DIRECTX
//	//仮想アドレスを取得するためのポインタ
//	VERTEX_2D *pVtx = NULL;
//
//	//頂点バッファをロックして、仮想アドレスを取得する
//	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
//
//	//頂点カラーの設定(0～255の整数値)
//	pVtx[0].Color = cColor;
//	pVtx[1].Color = cColor;
//	pVtx[2].Color = cColor;
//	pVtx[3].Color = cColor;
//
//	//仮想アドレス解放
//	m_pVtxBuffer->Unlock();
//#endif
}
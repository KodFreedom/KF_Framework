//--------------------------------------------------------------------------------
//　fade_system.h
//  manage the materials' save, load
//	マテリアル管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "fade_system.h"
#include "main_system.h"
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
	switch (current_state_)
	{
	case FadeState::kFadeNone:
		break;
	case FadeState::kFadeIn:
		FadeIn();
		break;
	case FadeState::kFadeOut:
		FadeOut();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void FadeSystem::Render(void)
{
	//#ifdef USING_DIRECTX
	//	LPDIRECT3DDEVICE9 pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	//
	//	// 頂点バッファをデータストリームに設定
	//	pDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(VERTEX_2D));
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
//  次のモードにフェードする
//--------------------------------------------------------------------------------
void FadeSystem::FadeTo(Mode* next_mode, const float fade_time)
{
	if (current_state_ == FadeState::kFadeOut)
	{
		delete next_mode;
		return;
	}
	current_state_ = FadeState::kFadeOut;
	time_counter_ = 0.0f;
	next_mode_ = next_mode;
	fade_time_ = fade_time;
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void FadeSystem::Init(void)
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
//		&vertexBuffer,						//頂点バッファのアドレス
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
//	vertexBuffer->Lock(0, 0, (void**)&pVtx, 0);
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
//	vertexBuffer->Unlock();
//#endif
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void FadeSystem::Uninit(void)
{
//#ifdef USING_DIRECTX
//	Main::GetManager()->GetTextureManager()->DisuseTexture("polygon.jpg");
//	SAFE_RELEASE(vertexBuffer);
//#endif
}

//--------------------------------------------------------------------------------
//  fadeIn
//--------------------------------------------------------------------------------
void FadeSystem::FadeIn(void)
{
	time_counter_ -= DELTA_TIME;
	if (time_counter_ <= 0.0f)
	{
		time_counter_ = 0.0f;
		current_state_ = kFadeNone;
	}
	color_.a_ = time_counter_ / fade_time_;
}

//--------------------------------------------------------------------------------
//  fadeOut
//--------------------------------------------------------------------------------
void FadeSystem::FadeOut(void)
{
	time_counter_ += DELTA_TIME;
	if (time_counter_ >= fade_time_)
	{
		time_counter_ = fade_time_;

		// Todo : Check SE & BGM(fade out effect)
		current_state_ = kFadeIn;
		MainSystem::Instance()->Change(next_mode_);
	}
	color_.a_ = time_counter_ / fade_time_;
}
//--------------------------------------------------------------------------------
//	DX用便利関数
//　KF_UtilityDX.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "KF_UtilityDX.h"
#include "manager.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  頂点生成
//--------------------------------------------------------------------------------
bool UtilityDX::MakeVertex(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& PositionCenter, const Vector2& vSize, const Vector2& UVBegin, const Vector2& UVSize, const float& fRot, const Color& cColor)
{
	auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//頂点バッファ
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,		//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,			//頂点バッファの使用方法
		FVF_VERTEX_2D,				//書かなくても大丈夫
		D3DPOOL_MANAGED,			//メモリ管理方法(managed：デバイスにお任せ)
		&pVtxBuffer,				//頂点バッファのアドレス
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "UtilityDX : CreateVertexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}

	// 頂点情報を設定
	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点位置設定
	float fLength = CKFMath::VecMagnitude(vSize) * 0.5f;
	float fAngle = atan2f(vSize.Y, vSize.X);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].Position = Vector3(PositionCenter.X + cosf(fRot + KF_PI + fAngle) * fLength,
		PositionCenter.Y + sinf(fRot + KF_PI + fAngle) * fLength, 0.0f);
	pVtx[1].Position = Vector3(PositionCenter.X + cosf(fRot - fAngle) * fLength,
		PositionCenter.Y + sinf(fRot - fAngle) * fLength, 0.0f);
	pVtx[2].Position = Vector3(PositionCenter.X + cosf(fRot - KF_PI - fAngle) * fLength,
		PositionCenter.Y + sinf(fRot - KF_PI - fAngle) * fLength, 0.0f);
	pVtx[3].Position = Vector3(PositionCenter.X + cosf(fRot + fAngle) * fLength,
		PositionCenter.Y + sinf(fRot + fAngle) * fLength, 0.0f);

	//頂点カラー設定
	pVtx[0].Color = cColor;
	pVtx[1].Color = cColor;
	pVtx[2].Color = cColor;
	pVtx[3].Color = cColor;

	//頂点UV設定
	pVtx[0].UV = UVBegin;
	pVtx[1].UV = UVBegin + Vector2(UVSize.X, 0.0f);
	pVtx[2].UV = UVBegin + Vector2(0.0f, UVSize.Y);
	pVtx[3].UV = UVBegin + UVSize;

	//rhwの設定(必ず1.0f)
	pVtx[0].Rhw = 1.0f;
	pVtx[1].Rhw = 1.0f;
	pVtx[2].Rhw = 1.0f;
	pVtx[3].Rhw = 1.0f;

	//仮想アドレス解放
	pVtxBuffer->Unlock();

	return true;
}

//--------------------------------------------------------------------------------
//  頂点生成(ゲージ)
//--------------------------------------------------------------------------------
bool UtilityDX::MakeVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& PositionLeftTop, const Vector2& vSize, const Color& cColor, const Vector2& UVBegin, const Vector2& UVSize)
{
	auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//頂点バッファ
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,		//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,			//頂点バッファの使用方法
		FVF_VERTEX_2D,				//書かなくても大丈夫
		D3DPOOL_MANAGED,			//メモリ管理方法(managed：デバイスにお任せ)
		&pVtxBuffer,				//頂点バッファのアドレス
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "UtilityDX : CreateVertexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}

	// 頂点情報を設定
	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f);
	pVtx[1].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f) + Vector3(vSize.X, 0.0f, 0.0f);
	pVtx[2].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f) + Vector3(0.0f, vSize.Y, 0.0f);
	pVtx[3].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f) + Vector3(vSize.X, vSize.Y, 0.0f);

	//頂点カラー設定
	pVtx[0].Color = cColor;
	pVtx[1].Color = cColor;
	pVtx[2].Color = cColor;
	pVtx[3].Color = cColor;

	//頂点UV設定
	pVtx[0].UV = UVBegin;
	pVtx[1].UV = UVBegin + Vector2(UVSize.X, 0.0f);
	pVtx[2].UV = UVBegin + Vector2(0.0f, UVSize.Y);
	pVtx[3].UV = UVBegin + UVSize;

	//rhwの設定(必ず1.0f)
	pVtx[0].Rhw = 1.0f;
	pVtx[1].Rhw = 1.0f;
	pVtx[2].Rhw = 1.0f;
	pVtx[3].Rhw = 1.0f;

	//仮想アドレス解放
	pVtxBuffer->Unlock();

	return true;
}

//--------------------------------------------------------------------------------
//  頂点更新(ゲージ)
//--------------------------------------------------------------------------------
void UtilityDX::UpdateVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& PositionLeftTop, const Vector2& vSize, const float& fRate, const Color& cColor, const Vector2& UVBegin, const Vector2& UVSize)
{
	// 頂点情報を設定
	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f);
	pVtx[1].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f) + Vector3(vSize.X * fRate, 0.0f, 0.0f);
	pVtx[2].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f) + Vector3(0.0f, vSize.Y, 0.0f);
	pVtx[3].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f) + Vector3(vSize.X * fRate, vSize.Y, 0.0f);

	//頂点カラー設定
	pVtx[0].Color = cColor;
	pVtx[1].Color = cColor;
	pVtx[2].Color = cColor;
	pVtx[3].Color = cColor;

	//頂点UV設定
	pVtx[0].UV = UVBegin;
	pVtx[1].UV = UVBegin + Vector2(UVSize.X * fRate, 0.0f);
	pVtx[2].UV = UVBegin + Vector2(0.0f, UVSize.Y);
	pVtx[3].UV = UVBegin + Vector2(UVSize.X * fRate, UVSize.Y);

	//仮想アドレス解放
	pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
//  頂点更新(UV)
//--------------------------------------------------------------------------------
void UtilityDX::UpdateUV(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& UVBegin, const Vector2& UVSize)
{
	// 頂点情報を設定
	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点UV設定
	pVtx[0].UV = UVBegin;
	pVtx[1].UV = UVBegin + Vector2(UVSize.X, 0.0f);
	pVtx[2].UV = UVBegin + Vector2(0.0f, UVSize.Y);
	pVtx[3].UV = UVBegin + UVSize;

	//仮想アドレス解放
	pVtxBuffer->Unlock();
}
#endif
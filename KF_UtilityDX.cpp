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
bool CKFUtilityDX::MakeVertex(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& vPosCenter, const Vector2& vSize, const Vector2& vUVBegin, const Vector2& vUVSize, const float& fRot, const Color& cColor)
{
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
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
		MessageBox(NULL, "CKFUtilityDX : CreateVertexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
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
	pVtx[0].vPos = Vector3(vPosCenter.X + cosf(fRot + KF_PI + fAngle) * fLength,
		vPosCenter.Y + sinf(fRot + KF_PI + fAngle) * fLength, 0.0f);
	pVtx[1].vPos = Vector3(vPosCenter.X + cosf(fRot - fAngle) * fLength,
		vPosCenter.Y + sinf(fRot - fAngle) * fLength, 0.0f);
	pVtx[2].vPos = Vector3(vPosCenter.X + cosf(fRot - KF_PI - fAngle) * fLength,
		vPosCenter.Y + sinf(fRot - KF_PI - fAngle) * fLength, 0.0f);
	pVtx[3].vPos = Vector3(vPosCenter.X + cosf(fRot + fAngle) * fLength,
		vPosCenter.Y + sinf(fRot + fAngle) * fLength, 0.0f);

	//頂点カラー設定
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//頂点UV設定
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + Vector2(vUVSize.X, 0.0f);
	pVtx[2].vUV = vUVBegin + Vector2(0.0f, vUVSize.Y);
	pVtx[3].vUV = vUVBegin + vUVSize;

	//rhwの設定(必ず1.0f)
	pVtx[0].fRhw = 1.0f;
	pVtx[1].fRhw = 1.0f;
	pVtx[2].fRhw = 1.0f;
	pVtx[3].fRhw = 1.0f;

	//仮想アドレス解放
	pVtxBuffer->Unlock();

	return true;
}

//--------------------------------------------------------------------------------
//  頂点生成(ゲージ)
//--------------------------------------------------------------------------------
bool CKFUtilityDX::MakeVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& vPosLeftTop, const Vector2& vSize, const Color& cColor, const Vector2& vUVBegin, const Vector2& vUVSize)
{
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
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
		MessageBox(NULL, "CKFUtilityDX : CreateVertexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}

	// 頂点情報を設定
	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f);
	pVtx[1].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f) + Vector3(vSize.X, 0.0f, 0.0f);
	pVtx[2].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f) + Vector3(0.0f, vSize.Y, 0.0f);
	pVtx[3].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f) + Vector3(vSize.X, vSize.Y, 0.0f);

	//頂点カラー設定
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//頂点UV設定
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + Vector2(vUVSize.X, 0.0f);
	pVtx[2].vUV = vUVBegin + Vector2(0.0f, vUVSize.Y);
	pVtx[3].vUV = vUVBegin + vUVSize;

	//rhwの設定(必ず1.0f)
	pVtx[0].fRhw = 1.0f;
	pVtx[1].fRhw = 1.0f;
	pVtx[2].fRhw = 1.0f;
	pVtx[3].fRhw = 1.0f;

	//仮想アドレス解放
	pVtxBuffer->Unlock();

	return true;
}

//--------------------------------------------------------------------------------
//  頂点更新(ゲージ)
//--------------------------------------------------------------------------------
void CKFUtilityDX::UpdateVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& vPosLeftTop, const Vector2& vSize, const float& fRate, const Color& cColor, const Vector2& vUVBegin, const Vector2& vUVSize)
{
	// 頂点情報を設定
	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f);
	pVtx[1].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f) + Vector3(vSize.X * fRate, 0.0f, 0.0f);
	pVtx[2].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f) + Vector3(0.0f, vSize.Y, 0.0f);
	pVtx[3].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f) + Vector3(vSize.X * fRate, vSize.Y, 0.0f);

	//頂点カラー設定
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//頂点UV設定
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + Vector2(vUVSize.X * fRate, 0.0f);
	pVtx[2].vUV = vUVBegin + Vector2(0.0f, vUVSize.Y);
	pVtx[3].vUV = vUVBegin + Vector2(vUVSize.X * fRate, vUVSize.Y);

	//仮想アドレス解放
	pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
//  頂点更新(UV)
//--------------------------------------------------------------------------------
void CKFUtilityDX::UpdateUV(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& vUVBegin, const Vector2& vUVSize)
{
	// 頂点情報を設定
	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点UV設定
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + Vector2(vUVSize.X, 0.0f);
	pVtx[2].vUV = vUVBegin + Vector2(0.0f, vUVSize.Y);
	pVtx[3].vUV = vUVBegin + vUVSize;

	//仮想アドレス解放
	pVtxBuffer->Unlock();
}
#endif
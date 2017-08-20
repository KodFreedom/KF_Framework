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
bool CKFUtilityDX::MakeVertex(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const CKFVec2& vPosCenter, const CKFVec2& vSize, const CKFVec2& vUVBegin, const CKFVec2& vUVSize, const float& fRot, const CKFColor& cColor)
{
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
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
	float fAngle = atan2f(vSize.m_fY, vSize.m_fX);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].vPos = CKFVec3(vPosCenter.m_fX + cosf(fRot + KF_PI + fAngle) * fLength,
		vPosCenter.m_fY + sinf(fRot + KF_PI + fAngle) * fLength, 0.0f);
	pVtx[1].vPos = CKFVec3(vPosCenter.m_fX + cosf(fRot - fAngle) * fLength,
		vPosCenter.m_fY + sinf(fRot - fAngle) * fLength, 0.0f);
	pVtx[2].vPos = CKFVec3(vPosCenter.m_fX + cosf(fRot - KF_PI - fAngle) * fLength,
		vPosCenter.m_fY + sinf(fRot - KF_PI - fAngle) * fLength, 0.0f);
	pVtx[3].vPos = CKFVec3(vPosCenter.m_fX + cosf(fRot + fAngle) * fLength,
		vPosCenter.m_fY + sinf(fRot + fAngle) * fLength, 0.0f);

	//頂点カラー設定
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//頂点UV設定
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + CKFVec2(vUVSize.m_fX, 0.0f);
	pVtx[2].vUV = vUVBegin + CKFVec2(0.0f, vUVSize.m_fY);
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
bool CKFUtilityDX::MakeVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const CKFVec2& vPosLeftTop, const CKFVec2& vSize, const CKFColor& cColor, const CKFVec2& vUVBegin, const CKFVec2& vUVSize)
{
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
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
	pVtx[0].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f);
	pVtx[1].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f) + CKFVec3(vSize.m_fX, 0.0f, 0.0f);
	pVtx[2].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f) + CKFVec3(0.0f, vSize.m_fY, 0.0f);
	pVtx[3].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f) + CKFVec3(vSize.m_fX, vSize.m_fY, 0.0f);

	//頂点カラー設定
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//頂点UV設定
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + CKFVec2(vUVSize.m_fX, 0.0f);
	pVtx[2].vUV = vUVBegin + CKFVec2(0.0f, vUVSize.m_fY);
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
void CKFUtilityDX::UpdateVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const CKFVec2& vPosLeftTop, const CKFVec2& vSize, const float& fRate, const CKFColor& cColor, const CKFVec2& vUVBegin, const CKFVec2& vUVSize)
{
	// 頂点情報を設定
	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f);
	pVtx[1].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f) + CKFVec3(vSize.m_fX * fRate, 0.0f, 0.0f);
	pVtx[2].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f) + CKFVec3(0.0f, vSize.m_fY, 0.0f);
	pVtx[3].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f) + CKFVec3(vSize.m_fX * fRate, vSize.m_fY, 0.0f);

	//頂点カラー設定
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//頂点UV設定
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + CKFVec2(vUVSize.m_fX * fRate, 0.0f);
	pVtx[2].vUV = vUVBegin + CKFVec2(0.0f, vUVSize.m_fY);
	pVtx[3].vUV = vUVBegin + CKFVec2(vUVSize.m_fX * fRate, vUVSize.m_fY);

	//仮想アドレス解放
	pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
//  頂点更新(UV)
//--------------------------------------------------------------------------------
void CKFUtilityDX::UpdateUV(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const CKFVec2& vUVBegin, const CKFVec2& vUVSize)
{
	// 頂点情報を設定
	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点UV設定
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + CKFVec2(vUVSize.m_fX, 0.0f);
	pVtx[2].vUV = vUVBegin + CKFVec2(0.0f, vUVSize.m_fY);
	pVtx[3].vUV = vUVBegin + vUVSize;

	//仮想アドレス解放
	pVtxBuffer->Unlock();
}
#endif
//--------------------------------------------------------------------------------
//	メッシュコンポネント
//　meshComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-21
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "2DmeshComponent.h"
#include "gameObject.h"

#ifdef USING_DIRECTX
#include "manager.h"
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool C2DMeshComponent::Init(void)
{
#ifdef USING_DIRECTX
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//頂点バッファ
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * s_nNumVtx,		//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,					//頂点バッファの使用方法
		FVF_VERTEX_2D,						//書かなくても大丈夫
		D3DPOOL_MANAGED,					//メモリ管理方法(managed：デバイスにお任せ)
		&m_pVtxBuffer,						//頂点バッファのアドレス
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "C2DMeshComponent : CreateVertexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return false;
	}
#endif
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void C2DMeshComponent::Uninit(void)
{
#ifdef USING_DIRECTX
	if (m_pVtxBuffer != NULL)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = NULL;
	}
#endif
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void C2DMeshComponent::Update(void)
{
#ifdef USING_DIRECTX
	// 頂点情報を設定
	//仮想アドレスを取得するためのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点位置設定
	CKFVec2 vSize = m_pGameObj->GetTransformComponent()->GetScale();
	CKFVec3 vPos = m_pGameObj->GetTransformComponent()->GetPos();
	CKFVec3 vRot = CKFVec3(0.0f);
	float fLength = CKFMath::VecMagnitude(vSize) * 0.5f;
	float fAngle = atan2f(vSize.m_fY, vSize.m_fX);

	//頂点座標の設定（2D座標、右回り）
	pVtx[0].vPos = CKFVec3(vPos.m_fX + cosf(vRot.m_fZ + KF_PI + fAngle) * fLength, vPos.m_fY + sinf(vRot.m_fZ + KF_PI + fAngle) * fLength, 0.0f);
	pVtx[1].vPos = CKFVec3(vPos.m_fX + cosf(vRot.m_fZ - fAngle) * fLength, vPos.m_fY + sinf(vRot.m_fZ - fAngle) * fLength, 0.0f);
	pVtx[2].vPos = CKFVec3(vPos.m_fX + cosf(vRot.m_fZ - KF_PI - fAngle) * fLength, vPos.m_fY + sinf(vRot.m_fZ - KF_PI - fAngle) * fLength, 0.0f);
	pVtx[3].vPos = CKFVec3(vPos.m_fX + cosf(vRot.m_fZ + fAngle) * fLength, vPos.m_fY + sinf(vRot.m_fZ + fAngle) * fLength, 0.0f);

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
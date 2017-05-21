//--------------------------------------------------------------------------------
//	3Dメッシュコンポネント
//　3DmeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "3DMeshComponent.h"
#include "gameObject.h"
#include "manager.h"

#ifdef USING_DIRECTX9
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void C3DMeshComponent::Uninit(void)
{
#ifdef USING_DIRECTX9
	if (m_meshInfo.pIdxBuffer != NULL)
	{
		m_meshInfo.pIdxBuffer->Release();
		m_meshInfo.pIdxBuffer = NULL;
	}

	if (m_meshInfo.pVtxBuffer != NULL)
	{
		m_meshInfo.pVtxBuffer->Release();
		m_meshInfo.pVtxBuffer = NULL;
	}
#endif
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void C3DMeshComponent::Update(CGameObject &gameObj)
{
	//マトリクス算出
	CKFMtx44 mtxWorld;
	CKFMtx44 mtxRot;
	CKFMtx44 mtxPos;

	//単位行列に初期化
	CKFMath::MtxIdentity(&mtxWorld);

	//回転(Y->X->Z)
	CKFMath::MtxRotationYawPitchRoll(&mtxRot, gameObj.GetRot());
	mtxWorld *= mtxRot;

	//平行移動
	CKFMath::MtxTranslation(&mtxPos, gameObj.GetPos());
	mtxWorld *= mtxPos;

	gameObj.SetMatrix(mtxWorld);
}

//--------------------------------------------------------------------------------
//  バッファ作成
//--------------------------------------------------------------------------------
KFRESULT C3DMeshComponent::CreateBuffer(void)
{
#ifdef USING_DIRECTX9
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//頂点バッファ
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_meshInfo.nNumVtx,	//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,						//頂点バッファの使用方法
		FVF_VERTEX_3D,							//書かなくても大丈夫
		D3DPOOL_MANAGED,						//メモリ管理方法(managed：デバイスにお任せ)
		&m_meshInfo.pVtxBuffer,					//頂点バッファのポインタ
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "C3DMeshComponent : CreateVertexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return KF_FAILED;
	}

	//インデックスバッファの作成
	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_meshInfo.nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_meshInfo.pIdxBuffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "C3DMeshComponent : CreateIndexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return KF_FAILED;
	}
#endif

	return KF_SUCCEEDED;
}

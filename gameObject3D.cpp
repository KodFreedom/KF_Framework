//--------------------------------------------------------------------------------
//
//　gameObject3D.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-19
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "rendererDX.h"
#include "gameObject3D.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObject3D::CGameObject3D() :CGameObject()
	, m_pIdxBuffer(NULL)
	, m_texName(CTM::TEX_MAX)
	, m_matType(CMM::MAT_NORMAL)
	, m_nIdxNum(0)
	, m_nPolygonNum(0)
	, m_nVtxNum(0)
	, m_vRot(CKFVec3(0.0f))
	, m_vScale(CKFVec3(1.0f))
{
}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CGameObject3D::~CGameObject3D()
{
}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
KFRESULT CGameObject3D::Init(const int &nVtxNum, const int &nIdxNum, const int &nPolygonNum)
{
	m_nVtxNum = nVtxNum;
	m_nIdxNum = nIdxNum;
	m_nPolygonNum = nPolygonNum;

	KFRESULT result = CreateBuffer();
	
	return result;
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void CGameObject3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuffer != NULL)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = NULL;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuffer != NULL)
	{
		m_pIdxBuffer->Release();
		m_pIdxBuffer = NULL;
	}
}

//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void CGameObject3D::Update(void)
{

}

//--------------------------------------------------------------------------------
// 更新処理(描画直前)
//--------------------------------------------------------------------------------
void CGameObject3D::LateUpdate(void)
{

}

//--------------------------------------------------------------------------------
// 描画処理
//--------------------------------------------------------------------------------
void CGameObject3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//RenderState設定
	SetRenderState();

	//マトリックス設定
	SetMatrix();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,						//ストリーム番号
		m_pVtxBuffer,			//頂点バッファ
		0,						//オフセット（開始位置）
		sizeof(VERTEX_3D));		//ストライド量

	// 頂点インデックスの設定
	pDevice->SetIndices(m_pIdxBuffer);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture(m_texName);
	pDevice->SetTexture(0, pTexture);

	// マテリアルの設定
	D3DMATERIAL9 mat = GetManager()->GetMaterialManager()->GetMaterial(m_matType);
	pDevice->SetMaterial(&mat);

	//プリミティブ描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nVtxNum,
		0,
		m_nPolygonNum);

	//RenderState戻す
	ResetRenderState();
}

//--------------------------------------------------------------------------------
// バッファ生成
//--------------------------------------------------------------------------------
KFRESULT CGameObject3D::CreateBuffer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//頂点バッファ
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nVtxNum,	//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,				//頂点バッファの使用方法
		FVF_VERTEX_3D,					//書かなくても大丈夫
		D3DPOOL_MANAGED,				//メモリ管理方法(managed：デバイスにお任せ)
		&m_pVtxBuffer,					//頂点バッファのポインタ
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "GameObject3D : CreateVertexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return KF_FAILED;
	}

	//インデックスバッファの作成
	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nIdxNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "GameObject3D : CreateIndexBuffer ERROR!!", "エラー", MB_OK | MB_ICONWARNING);
		return KF_FAILED;
	}

	return KF_SUCCEEDED;
}

//--------------------------------------------------------------------------------
// SetWorldMatrix
//--------------------------------------------------------------------------------
void CGameObject3D::SetMatrix(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//ワールド相対座標
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxPos;

	//単位行列に初期化
	D3DXMatrixIdentity(&mtxWorld);

	//回転(Y->X->Z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vRot.m_fY, m_vRot.m_fX, m_vRot.m_fZ);
	mtxWorld *= mtxRot;

	//平行移動
	D3DXMatrixTranslation(&mtxPos, m_vPos.m_fX, m_vPos.m_fY, m_vPos.m_fZ);
	mtxWorld *= mtxPos;

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
}

//--------------------------------------------------------------------------------
// SetRenderState
//--------------------------------------------------------------------------------
void CGameObject3D::SetRenderState(void)
{

}

//--------------------------------------------------------------------------------
// ResetRenderState
//--------------------------------------------------------------------------------
void CGameObject3D::ResetRenderState(void)
{

}
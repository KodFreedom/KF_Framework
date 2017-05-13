//--------------------------------------------------------------------------------
//
//　gameObjectModel.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
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
#include "model.h"
#include "modelX.h"
#include "gameObject.h"
#include "gameObjectModel.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CGameObjectModel::CGameObjectModel() : CGameObject()
	, m_vRot(CKFVec3(0.0f))
	, m_modelName(CMOM::MODEL_NONE)
	, m_matType(CMM::MAT_MAX)
{

}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CGameObjectModel::~CGameObjectModel()
{

}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CGameObjectModel::Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName)
{
	m_vPos = vPos;
	m_vRot = vRot;
	m_modelName = modelName;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CGameObjectModel::Uninit(void)
{

}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CGameObjectModel::Update(void)
{

}

//--------------------------------------------------------------------------------
//  更新処理(描画直前)
//--------------------------------------------------------------------------------
void CGameObjectModel::LateUpdate(void)
{

}

//--------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------
void CGameObjectModel::Draw(void)
{
	CModel* pModel = GetManager()->GetModelManager()->GetModel(m_modelName);

	if (pModel != NULL)
	{
		//チェックモデルタイプ
		if (pModel->GetModelType() != CMOM::XFILE) { return; }

		//キャスト
		CModelX* pModelX = (CModelX*)pModel;

		//RenderState設定
		SetRenderState();

		//マトリックス設定
		SetMatrix();

		//描画
		if (m_matType != CMM::MAT_MAX)
		{
			pModelX->Draw(m_matType);
		}
		else
		{
			pModelX->Draw();
		}

		//RenderState戻す
		ResetRenderState();
	}
}

//--------------------------------------------------------------------------------
// SetWorldMatrix
//--------------------------------------------------------------------------------
void CGameObjectModel::SetMatrix(void)
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
void CGameObjectModel::SetRenderState(void)
{

}

//--------------------------------------------------------------------------------
// ResetRenderState
//--------------------------------------------------------------------------------
void CGameObjectModel::ResetRenderState(void)
{

}

//--------------------------------------------------------------------------------
// 生成
//--------------------------------------------------------------------------------
CGameObjectModel* CGameObjectModel::Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CMOM::MODEL_NAME &modelName)
{
	CGameObjectModel *pObjModel = NULL;
	pObjModel = new CGameObjectModel;
	pObjModel->Init(vPos, vRot, modelName);
	pObjModel->m_pri = GOM::PRI_3D;
	pObjModel->m_nID = GetManager()->GetGameObjectManager()->SaveGameObj(GOM::PRI_3D, pObjModel);
	return pObjModel;
}
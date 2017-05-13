//--------------------------------------------------------------------------------
//
//　meshField.h
//	Author : Xu Wenjie
//	Date   : 2016-12-13
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "meshField.h"

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CMeshField::CMeshField() : CGameObject3D()
	, m_nNumBlockX(0)
	, m_nNumBlockZ(0)
	, m_vBlockSize(CKFVec2(0.0f, 0.0f))
{

}

//--------------------------------------------------------------------------------
//  デストラクタ
//--------------------------------------------------------------------------------
CMeshField::~CMeshField()
{

}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
KFRESULT CMeshField::Init(const int &nNumBlockX, const int &nNumBlockZ, const CKFVec2 &vBlockSize, const CKFVec3 &vPos, const CKFVec3 &vRot)
{
	m_vPos = vPos;
	m_vRot = vRot;
	m_nNumBlockX = nNumBlockX;
	m_nNumBlockZ = nNumBlockZ;
	m_vBlockSize = vBlockSize;
	m_texName = CTM::TEX_DEMO_ROAD;

	int nVtxNum = (m_nNumBlockX + 1) * (m_nNumBlockZ + 1);
	int nIdxNum = ((m_nNumBlockX + 1) * 2 + 2) * m_nNumBlockZ - 1;
	int nPolygonNum = (m_nNumBlockX + 2) * 2 * m_nNumBlockZ - 4;

	if (CGameObject3D::Init(nVtxNum, nIdxNum, nPolygonNum) == KF_FAILED)
	{
		return KF_FAILED;
	}

	MakeVertex();

	return KF_SUCCEEDED;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CMeshField::Update(void)
{
	
}


//--------------------------------------------------------------------------------
//  頂点作成
//--------------------------------------------------------------------------------
void CMeshField::MakeVertex(void)
{
	//仮想アドレスを取得するためのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックして、仮想アドレスを取得する
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	CKFVec3 vStartPos = CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);

	for (int nCntZ = 0; nCntZ < m_nNumBlockZ + 1; nCntZ++)
	{
		for (int nCntPos = 0; nCntPos < m_nNumBlockX + 1; nCntPos++)
		{
			pVtx[nCntZ * (m_nNumBlockX + 1) + nCntPos].vPos = vStartPos
				+ CKFVec3(nCntPos * m_vBlockSize.m_fX, 0.0f, -nCntZ * m_vBlockSize.m_fY);
			pVtx[nCntZ * (m_nNumBlockX + 1) + nCntPos].vUV = CKFVec2(nCntPos * 1.0f, nCntZ * 1.0f);
			pVtx[nCntZ * (m_nNumBlockX + 1) + nCntPos].ulColor = CKFColor(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[nCntZ * (m_nNumBlockX + 1) + nCntPos].vNormal = CKFVec3(0.0f, 1.0f, 0.0f);
		}
	}

	//仮想アドレス解放
	m_pVtxBuffer->Unlock();

	//インデックス
	WORD *pIdx;
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntZ = 0; nCntZ < m_nNumBlockZ; nCntZ++)
	{
		for (int nCntPos = 0; nCntPos < (m_nNumBlockX + 1) * 2 + 2; nCntPos++)
		{
			if (nCntPos < (m_nNumBlockX + 1) * 2)
			{
				pIdx[nCntZ * ((m_nNumBlockX + 1) * 2 + 2) + nCntPos] = nCntPos / 2 + (nCntPos + 1) % 2 * (m_nNumBlockX + 1) + nCntZ * (m_nNumBlockX + 1);
			}
			else if (nCntZ * ((m_nNumBlockX + 1) * 2 + 2) + nCntPos < (((m_nNumBlockX + 1) * 2 + 2) * m_nNumBlockZ - 1))//縮退ポリゴン
			{
				pIdx[nCntZ * ((m_nNumBlockX + 1) * 2 + 2) + nCntPos] = (nCntPos - 1) / 2 % (m_nNumBlockX + 1) + nCntPos % 2 * 2 * (m_nNumBlockX + 1) + nCntZ * (m_nNumBlockX + 1);
			}
		}
	}

	m_pIdxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
//  生成
//--------------------------------------------------------------------------------
CMeshField* CMeshField::Create(const int &nNumBlockX, const int &nNumBlockZ, const CKFVec2 &vBlockSize, const CKFVec3 &vPos, const CKFVec3 &vRot)
{
	CMeshField* pField = NULL;
	pField = new CMeshField;
	pField->Init(nNumBlockX, nNumBlockZ, vBlockSize, vPos, vRot);
	pField->m_pri = GOM::PRI_3D;
	pField->m_nID = GetManager()->GetGameObjectManager()->SaveGameObj(GOM::PRI_3D, pField);
	return pField;
}
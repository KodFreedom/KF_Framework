//--------------------------------------------------------------------------------
//
//�@meshCube.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "meshCube.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CMeshCube::CMeshCube() : CGameObject3D()
	, m_vSize(CKFVec3(1.0f))
	, m_cColor(CKFColor(1.0f))
{

}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
KFRESULT CMeshCube::Init(const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vSize, const CKFColor &cColor)
{
	m_vPos = vPos;
	m_vRot = vRot;
	m_vSize = vSize;
	m_cColor = cColor;
	m_texName = CTM::TEX_DEMO_POLYGON;

	int nNumVtx = 6 * 4;
	int nNumIdx = 6 * 4 + 5 * 2;
	int nNumPolygon = 6 * 2 + 5 * 4;

	if (CGameObject3D::Init(nNumVtx, nNumIdx, nNumPolygon) == KF_FAILED)
	{
		return KF_FAILED;
	}

	MakeVertex();

	return KF_SUCCEEDED;
}

//--------------------------------------------------------------------------------
//  ���_����
//--------------------------------------------------------------------------------
void CMeshCube::MakeVertex(void)
{
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	CKFVec3 vHalfSize = m_vSize * 0.5f;
	int nCntVtx = 0;

	//����
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX + (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			-vHalfSize.m_fZ);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = m_cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 0.0f, -1.0f);
		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX + (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			vHalfSize.m_fY,
			vHalfSize.m_fZ - (nCnt / 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = m_cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 1.0f, 0.0f);
		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			vHalfSize.m_fZ - (nCnt % 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = m_cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(-1.0f, 0.0f, 0.0f);
		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX + (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			-vHalfSize.m_fY,
			-vHalfSize.m_fZ + (nCnt / 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = m_cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, -1.0f, 0.0f);
		nCntVtx++;
	}

	//�E
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			vHalfSize.m_fX,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			-vHalfSize.m_fZ + (nCnt % 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = m_cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(1.0f, 0.0f, 0.0f);
		nCntVtx++;
	}

	//���
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			vHalfSize.m_fX - (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			vHalfSize.m_fZ);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = m_cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 0.0f, 1.0f);
		nCntVtx++;
	}

	//���z�A�h���X���
	m_pVtxBuffer->Unlock();

	//�C���f�b�N�X
	WORD *pIdx;
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < 6 * 4 + 5 * 2; nCnt++)
	{
		if (nCnt % 6 < 4)
		{
			pIdx[nCnt] = (nCnt / 6) * 4 + (nCnt % 6) % 4;
		}
		else
		{//�k��
			pIdx[nCnt] = (nCnt / 6) * 4 + (nCnt % 2) + 3;
		}
	}

	m_pIdxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
//  ����
//--------------------------------------------------------------------------------
CMeshCube *CMeshCube::Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vSize, const CKFColor &cColor)
{
	CMeshCube *pCube = NULL;
	pCube = new CMeshCube;
	pCube->Init(vPos, vRot, vSize, cColor);
	pCube->m_pri = GOM::PRI_3D;
	pCube->m_nID = GetManager()->GetGameObjectManager()->SaveGameObj(GOM::PRI_3D, pCube);
	return pCube;
}
//--------------------------------------------------------------------------------
//	fieldCollider�R���|�l���g
//�@fieldColliderComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "fieldColliderComponent.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  
//	Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CFieldColliderComponent::Uninit(void)
{
	m_vectorVtx.clear();
	CColliderComponent::Uninit();
}

//--------------------------------------------------------------------------------
//	�֐����FGetPointInfo
//  �֐������F�|�C���g�̃f�[�^���擾����
//	�����F	vPos�F�|�C���g�ʒu
//	�߂�l�FINFO
//--------------------------------------------------------------------------------
//CFieldColliderComponent::INFO CFieldColliderComponent::GetPointInfo(const CKFVec3& vPos)
//{
//	INFO info;
//	info.bInTheField = false;
//	info.fHeight = 0.0f;
//	info.vFaceNormal = CKFVec3(0.0f);
//	CKFVec3 vPosCenter = CKFVec3(m_mtxOffset.m_af[3][0], m_mtxOffset.m_af[3][1], m_mtxOffset.m_af[3][2]);
//	CKFVec3 vStartPos = vPosCenter + CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
//	int nXLeftUp = (int)(((vPos.m_fX - vStartPos.m_fX) / (m_vBlockSize.m_fX * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
//	int nZLeftUp = -(int)(((vPos.m_fZ - vStartPos.m_fZ) / (m_vBlockSize.m_fY * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);
//
//	//�t�B�[���h�͈̔͊O�������珈���I��
//	if (nXLeftUp < 0 || nXLeftUp >= m_nNumBlockX || nZLeftUp < 0 || nZLeftUp >= m_nNumBlockZ)
//	{
//		return info;
//	}
//
//	int nXRightDown = nXLeftUp + 1;
//	int nZRightDown = nZLeftUp + 1;
//
//	CKFVec3 vPosTarget = CKFVec3(vPos.m_fX, 0.0f, vPos.m_fZ);
//	CKFVec3 vPLeftUp = m_vectorVtx[nZLeftUp * (m_nNumBlockX + 1) + nXLeftUp];
//	CKFVec3 vPRightDown = m_vectorVtx[nZRightDown * (m_nNumBlockX + 1) + nXRightDown];
//
//	//Check Side
//	CKFVec3 vMid = vPRightDown - vPLeftUp;
//	CKFVec3 vTL = vPosTarget - vPLeftUp;
//	CKFVec3 vCross = vTL * vMid;
//	int nXSide, nZSide;
//	int nSign = 0;
//	if (vCross.m_fY >= 0.0f)
//	{//LeftSide
//		nXSide = nXLeftUp + 1;
//		nZSide = nZLeftUp;
//		nSign = -1;
//	}
//	else
//	{//RightSide
//		nXSide = nXLeftUp;
//		nZSide = nZLeftUp + 1;
//		nSign = 1;
//	}
//	CKFVec3 vPSide = m_vectorVtx[nZSide * (m_nNumBlockX + 1) + nXSide];
//	CKFVec3 vLS = vPLeftUp - vPSide;
//	CKFVec3 vRS = vPRightDown - vPSide;
//	CKFVec3 vNormal = (vLS * vRS) * (float)nSign;
//	CKFMath::VecNormalize(vNormal);
//
//	//���S�_
//	auto vOffsetPos = (vPLeftUp + vPRightDown) * 0.5f;
//
//	//��]�s��̎Z�o
//	auto vRight = vNormal * CKFVec3(0.0f, 0.0f, 1.0f);
//	auto vForward = vRight * vNormal;
//	CKFMtx44 mtxThis;
//	mtxThis.m_af[0][0] = vRight.m_fX;
//	mtxThis.m_af[0][1] = vRight.m_fY;
//	mtxThis.m_af[0][2] = vRight.m_fZ;
//	mtxThis.m_af[1][0] = vNormal.m_fX;
//	mtxThis.m_af[1][1] = vNormal.m_fY;
//	mtxThis.m_af[1][2] = vNormal.m_fZ;
//	mtxThis.m_af[2][0] = vForward.m_fX;
//	mtxThis.m_af[2][1] = vForward.m_fY;
//	mtxThis.m_af[2][2] = vForward.m_fZ;
//	mtxThis.m_af[3][0] = vOffsetPos.m_fX;
//	mtxThis.m_af[3][1] = vOffsetPos.m_fY;
//	mtxThis.m_af[3][2] = vOffsetPos.m_fZ;
//	auto vRealPos = CKFMath::TransformInverse(mtxThis, vPos);
//	
//	info.fPenetration = -vRealPos.m_fY;
//
//	info.bInTheField = true;
//	info.fHeight = vPSide.m_fY - ((vPos.m_fX - vPSide.m_fX) * vNormal.m_fX + (vPos.m_fZ - vPSide.m_fZ) * vNormal.m_fZ) / vNormal.m_fY;
//	info.vFaceNormal = vNormal;
//
//	return info;
//}

//--------------------------------------------------------------------------------
//	�֐����FGetProjectionInfo
//  �֐������F�|�C���g���t�B�[���h��ɓ��e���ď����v�Z����
//	�����F	vPos�F�|�C���g�ʒu
//	�߂�l�FFINFO
//--------------------------------------------------------------------------------
CFieldColliderComponent::FINFO CFieldColliderComponent::GetProjectionInfo(const CKFVec3& vPos)
{
	FINFO info;
	info.bInFieldRange = false;
	info.fPenetration = 0.0f;
	info.vFaceNormal = CKFVec3(0.0f);
	CKFVec3 vPLeftUp, vPRightDown, vPSide;
	if (!getPointInfo(vPos, vPLeftUp, vPRightDown, vPSide, info.vFaceNormal)) { return info; }
	
	info.bInFieldRange = true;

	//�n�ʖ@���̊p�x��60�x�ȏ�Ȃ�n�ʖ@����Ԃ�
	//��������Ȃ��Ȃ�������Ԃ�
	auto fDot = CKFMath::Vec3Dot(CKFMath::sc_vUp, info.vFaceNormal);
	if (fDot > 0.5f) 
	{
		float fY = vPSide.m_fY - ((vPos.m_fX - vPSide.m_fX) * info.vFaceNormal.m_fX + (vPos.m_fZ - vPSide.m_fZ) * info.vFaceNormal.m_fZ) / info.vFaceNormal.m_fY;
		info.fPenetration = fY - vPos.m_fY;
		info.vFaceNormal = CKFMath::sc_vUp;
		return info;
	}

	//���S�_
	auto vOffsetPos = (vPLeftUp + vPRightDown) * 0.5f;

	//��]�s��̎Z�o
	auto vRight = vPRightDown - vPLeftUp;
	CKFMath::VecNormalize(vRight);
	auto vForward = vRight * info.vFaceNormal;
	CKFMath::VecNormalize(vForward);
	CKFMtx44 mtxThis;
	mtxThis.m_af[0][0] = vRight.m_fX;
	mtxThis.m_af[0][1] = vRight.m_fY;
	mtxThis.m_af[0][2] = vRight.m_fZ;
	mtxThis.m_af[1][0] = info.vFaceNormal.m_fX;
	mtxThis.m_af[1][1] = info.vFaceNormal.m_fY;
	mtxThis.m_af[1][2] = info.vFaceNormal.m_fZ;
	mtxThis.m_af[2][0] = vForward.m_fX;
	mtxThis.m_af[2][1] = vForward.m_fY;
	mtxThis.m_af[2][2] = vForward.m_fZ;
	mtxThis.m_af[3][0] = vOffsetPos.m_fX;
	mtxThis.m_af[3][1] = vOffsetPos.m_fY;
	mtxThis.m_af[3][2] = vOffsetPos.m_fZ;
	auto vRealPos = CKFMath::TransformInverse(mtxThis, vPos);

	if (vRealPos.m_fY < 0.0f)
	{
 		int n = 0;
	}

	//�o���Ȃ����ߖ@����������Ɛ��������ɂ���
	vRight = CKFMath::sc_vUp * info.vFaceNormal;
	info.vFaceNormal = vRight * CKFMath::sc_vUp;
	CKFMath::VecNormalize(info.vFaceNormal);
	info.fPenetration = -vRealPos.m_fY;
	return info;
}

//--------------------------------------------------------------------------------
//	�֐����FGetVtxByRange
//  �֐������F�͈͓��̒��_�f�[�^���擾����
//	�����F	vBegin�F�J�n�ʒu
//			vEnd�F�I�_�ʒu
//			nNumVtxXOut�FX�����̒��_��(�o��)
//			nNumVtxZOut�FZ�����̒��_��(�o��)
//			vecOut�F�͈͓��̒��_�f�[�^(�o��)
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool CFieldColliderComponent::GetVtxByRange(const CKFVec3& vBegin, const CKFVec3& vEnd, int& nNumVtxXOut, int& nNumVtxZOut, vector<CKFVec3>& vecOut)
{
	CKFVec3 vMin = CKFVec3(min(vBegin.m_fX, vEnd.m_fX), min(vBegin.m_fY, vEnd.m_fY), min(vBegin.m_fZ, vEnd.m_fZ));
	CKFVec3 vMax = CKFVec3(max(vBegin.m_fX, vEnd.m_fX), max(vBegin.m_fY, vEnd.m_fY), max(vBegin.m_fZ, vEnd.m_fZ));

	//�͈͂��Z�o
	CKFVec3 vPosCenter = CKFVec3(m_mtxOffset.m_af[3][0], m_mtxOffset.m_af[3][1], m_mtxOffset.m_af[3][2]);
	CKFVec3 vStartPos = vPosCenter + CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
	int nXMin = (int)(((vMin.m_fX - vStartPos.m_fX) / (m_vBlockSize.m_fX * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZMin = -(int)(((vMin.m_fZ - vStartPos.m_fZ) / (m_vBlockSize.m_fY * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);
	int nXMax = 1 + (int)(((vMax.m_fX - vStartPos.m_fX) / (m_vBlockSize.m_fX * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZMax = 1 - (int)(((vMax.m_fZ - vStartPos.m_fZ) / (m_vBlockSize.m_fY * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);
	nXMin = nXMin < 0 ? 0 : nXMin;
	nZMin = nZMin < 0 ? 0 : nZMin;
	nXMax = nXMax > m_nNumBlockX + 1 ? m_nNumBlockX + 1 : nXMax;
	nZMax = nZMax > m_nNumBlockZ + 1 ? m_nNumBlockZ + 1 : nZMax;

	//�t�B�[���h�͈̔͊O�������珈���I��
	if (nXMin >= m_nNumBlockX + 1 || nZMin >= m_nNumBlockZ + 1
		|| nXMax <= 0 || nZMax <= 0)
	{
		return false;
	}

	//�͈͓��̒��_��ۑ�����
	nNumVtxXOut = nXMax - nXMin;
	nNumVtxZOut = nZMax - nZMin;
	vecOut.resize(nNumVtxXOut * nNumVtxZOut);
	int nCnt = 0;
	for (int nCntZ = nZMin; nCntZ <= nZMax; nCntZ++)
	{
		for (int nCntX = nXMin; nCntX <= nXMax; nCntX++)
		{
			vecOut[nCnt] = m_vectorVtx[nCntZ * (m_nNumBlockX + 1) + nCntX];
			nCnt++;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------
//  
//	Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����Fload
//  �֐������F�t�@�C�����璸�_�f�[�^��ǂݍ���
//	�����F	strFieldName�F�t�B�[���h���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CFieldColliderComponent::load(const string& strFieldName)
{
	//file open
	string strName = "data/FIELD/" + strFieldName + ".field";
	FILE *pFile;
	fopen_s(&pFile, strName.c_str(), "rb");
	if (!pFile) 
	{
		MessageBox(NULL, "CFieldColliderComponent : load ERROR!! �t�@�C����������Ȃ�!!", "�G���[", MB_OK | MB_ICONWARNING);
		return;
	}

	//�u���b�N���̓Ǎ�
	fread(&m_nNumBlockX, sizeof(int), 1, pFile);
	fread(&m_nNumBlockZ, sizeof(int), 1, pFile);

	//�u���b�NSize�̓Ǎ�
	fread(&m_vBlockSize, sizeof(CKFVec2), 1, pFile);

	//���_�f�[�^���̓Ǎ�
	int nNumVtx;
	fread(&nNumVtx, sizeof(int), 1, pFile);

	//���_�f�[�^�̓Ǎ�
	m_vectorVtx.resize(nNumVtx);
	fread(&m_vectorVtx[0], sizeof(CKFVec3), nNumVtx, pFile);

	fclose(pFile);
	return;
}

//--------------------------------------------------------------------------------
//	�֐����FgetPointInfo
//  �֐������F�t�@�C�����璸�_�f�[�^��ǂݍ���
//	�����F	strFieldName�F�t�B�[���h���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
bool CFieldColliderComponent::getPointInfo(const CKFVec3& vPos, CKFVec3& vPLeftUp, CKFVec3& vPRightDown, CKFVec3& vPSide, CKFVec3& vFaceNormal)
{
	auto vPosCenter = CKFVec3(m_mtxOffset.m_af[3][0], m_mtxOffset.m_af[3][1], m_mtxOffset.m_af[3][2]);
	auto vStartPos = vPosCenter + CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
	int nXLeftUp = (int)(((vPos.m_fX - vStartPos.m_fX) / (m_vBlockSize.m_fX * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZLeftUp = -(int)(((vPos.m_fZ - vStartPos.m_fZ) / (m_vBlockSize.m_fY * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);

	//�t�B�[���h�͈̔͊O�������珈���I��
	if (nXLeftUp < 0 || nXLeftUp >= m_nNumBlockX || nZLeftUp < 0 || nZLeftUp >= m_nNumBlockZ)
	{
		return false;
	}

	int nXRightDown = nXLeftUp + 1;
	int nZRightDown = nZLeftUp + 1;

	auto vPosTarget = CKFVec3(vPos.m_fX, 0.0f, vPos.m_fZ);
	vPLeftUp = m_vectorVtx[nZLeftUp * (m_nNumBlockX + 1) + nXLeftUp];
	vPRightDown = m_vectorVtx[nZRightDown * (m_nNumBlockX + 1) + nXRightDown];

	//Check Side
	auto vMid = vPRightDown - vPLeftUp;
	auto vTL = vPosTarget - vPLeftUp;
	auto vCross = vTL * vMid;
	int nXSide, nZSide;
	int nSign = 0;
	if (vCross.m_fY >= 0.0f)
	{//LeftSide
		nXSide = nXLeftUp + 1;
		nZSide = nZLeftUp;
		nSign = -1;
	}
	else
	{//RightSide
		nXSide = nXLeftUp;
		nZSide = nZLeftUp + 1;
		nSign = 1;
	}
	vPSide = m_vectorVtx[nZSide * (m_nNumBlockX + 1) + nXSide];
	auto vLS = vPLeftUp - vPSide;
	auto vRS = vPRightDown - vPSide;
	vFaceNormal = (vLS * vRS) * (float)nSign;
	CKFMath::VecNormalize(vFaceNormal);
	return true;
}
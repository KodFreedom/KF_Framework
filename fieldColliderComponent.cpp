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
//	�����F	Position�F�|�C���g�ʒu
//	�߂�l�FINFO
//--------------------------------------------------------------------------------
//CFieldColliderComponent::INFO CFieldColliderComponent::GetPointInfo(const Vector3& Position)
//{
//	INFO info;
//	info.bInTheField = false;
//	info.fHeight = 0.0f;
//	info.vFaceNormal = Vector3(0.0f);
//	Vector3 PositionCenter = Vector3(m_mtxOffset.Elements[3][0], m_mtxOffset.Elements[3][1], m_mtxOffset.Elements[3][2]);
//	Vector3 vStartPos = PositionCenter + Vector3(-m_nNumBlockX * 0.5f * m_vBlockSize.X, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.Y);
//	int nXLeftUp = (int)(((Position.X - vStartPos.X) / (m_vBlockSize.X * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
//	int nZLeftUp = -(int)(((Position.Z - vStartPos.Z) / (m_vBlockSize.Y * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);
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
//	Vector3 PositionTarget = Vector3(Position.X, 0.0f, Position.Z);
//	Vector3 vPLeftUp = m_vectorVtx[nZLeftUp * (m_nNumBlockX + 1) + nXLeftUp];
//	Vector3 vPRightDown = m_vectorVtx[nZRightDown * (m_nNumBlockX + 1) + nXRightDown];
//
//	//Check Side
//	Vector3 vMid = vPRightDown - vPLeftUp;
//	Vector3 vTL = PositionTarget - vPLeftUp;
//	Vector3 vCross = vTL * vMid;
//	int nXSide, nZSide;
//	int nSign = 0;
//	if (vCross.Y >= 0.0f)
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
//	Vector3 vPSide = m_vectorVtx[nZSide * (m_nNumBlockX + 1) + nXSide];
//	Vector3 vLS = vPLeftUp - vPSide;
//	Vector3 vRS = vPRightDown - vPSide;
//	Vector3 Normal = (vLS * vRS) * (float)nSign;
//	CKFMath::VecNormalize(Normal);
//
//	//���S�_
//	auto vOffsetPos = (vPLeftUp + vPRightDown) * 0.5f;
//
//	//��]�s��̎Z�o
//	auto vRight = Normal * Vector3(0.0f, 0.0f, 1.0f);
//	auto vForward = vRight * Normal;
//	Matrix44 mtxThis;
//	mtxThis.Elements[0][0] = vRight.X;
//	mtxThis.Elements[0][1] = vRight.Y;
//	mtxThis.Elements[0][2] = vRight.Z;
//	mtxThis.Elements[1][0] = Normal.X;
//	mtxThis.Elements[1][1] = Normal.Y;
//	mtxThis.Elements[1][2] = Normal.Z;
//	mtxThis.Elements[2][0] = vForward.X;
//	mtxThis.Elements[2][1] = vForward.Y;
//	mtxThis.Elements[2][2] = vForward.Z;
//	mtxThis.Elements[3][0] = vOffsetPos.X;
//	mtxThis.Elements[3][1] = vOffsetPos.Y;
//	mtxThis.Elements[3][2] = vOffsetPos.Z;
//	auto vRealPos = CKFMath::TransformInverse(mtxThis, Position);
//	
//	info.fPenetration = -vRealPos.Y;
//
//	info.bInTheField = true;
//	info.fHeight = vPSide.Y - ((Position.X - vPSide.X) * Normal.X + (Position.Z - vPSide.Z) * Normal.Z) / Normal.Y;
//	info.vFaceNormal = Normal;
//
//	return info;
//}

//--------------------------------------------------------------------------------
//	�֐����FGetProjectionInfo
//  �֐������F�|�C���g���t�B�[���h��ɓ��e���ď����v�Z����
//	�����F	Position�F�|�C���g�ʒu
//	�߂�l�FFINFO
//--------------------------------------------------------------------------------
CFieldColliderComponent::FINFO CFieldColliderComponent::GetProjectionInfo(const Vector3& Position)
{
	FINFO info;
	info.bInFieldRange = false;
	info.fPenetration = 0.0f;
	info.vFaceNormal = Vector3(0.0f);
	Vector3 vPLeftUp, vPRightDown, vPSide;
	if (!getPointInfo(Position, vPLeftUp, vPRightDown, vPSide, info.vFaceNormal)) { return info; }
	
	info.bInFieldRange = true;

	//�n�ʖ@���̊p�x��60�x�ȏ�Ȃ�n�ʖ@����Ԃ�
	//��������Ȃ��Ȃ�������Ԃ�
	auto fDot = CKFMath::Vec3Dot(CKFMath::sc_vUp, info.vFaceNormal);
	if (fDot > 0.5f) 
	{
		float fY = vPSide.Y - ((Position.X - vPSide.X) * info.vFaceNormal.X + (Position.Z - vPSide.Z) * info.vFaceNormal.Z) / info.vFaceNormal.Y;
		info.fPenetration = fY - Position.Y;
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
	Matrix44 mtxThis;
	mtxThis.Elements[0][0] = vRight.X;
	mtxThis.Elements[0][1] = vRight.Y;
	mtxThis.Elements[0][2] = vRight.Z;
	mtxThis.Elements[1][0] = info.vFaceNormal.X;
	mtxThis.Elements[1][1] = info.vFaceNormal.Y;
	mtxThis.Elements[1][2] = info.vFaceNormal.Z;
	mtxThis.Elements[2][0] = vForward.X;
	mtxThis.Elements[2][1] = vForward.Y;
	mtxThis.Elements[2][2] = vForward.Z;
	mtxThis.Elements[3][0] = vOffsetPos.X;
	mtxThis.Elements[3][1] = vOffsetPos.Y;
	mtxThis.Elements[3][2] = vOffsetPos.Z;
	auto vRealPos = CKFMath::TransformInverse(mtxThis, Position);

	if (vRealPos.Y < 0.0f)
	{
 		int n = 0;
	}

	//�o���Ȃ����ߖ@����������Ɛ��������ɂ���
	vRight = CKFMath::sc_vUp * info.vFaceNormal;
	info.vFaceNormal = vRight * CKFMath::sc_vUp;
	CKFMath::VecNormalize(info.vFaceNormal);
	info.fPenetration = -vRealPos.Y;
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
bool CFieldColliderComponent::GetVtxByRange(const Vector3& vBegin, const Vector3& vEnd, int& nNumVtxXOut, int& nNumVtxZOut, vector<Vector3>& vecOut)
{
	Vector3 vMin = Vector3(min(vBegin.X, vEnd.X), min(vBegin.Y, vEnd.Y), min(vBegin.Z, vEnd.Z));
	Vector3 vMax = Vector3(max(vBegin.X, vEnd.X), max(vBegin.Y, vEnd.Y), max(vBegin.Z, vEnd.Z));

	//�͈͂��Z�o
	Vector3 PositionCenter = Vector3(m_mtxOffset.Elements[3][0], m_mtxOffset.Elements[3][1], m_mtxOffset.Elements[3][2]);
	Vector3 vStartPos = PositionCenter + Vector3(-m_nNumBlockX * 0.5f * m_vBlockSize.X, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.Y);
	int nXMin = (int)(((vMin.X - vStartPos.X) / (m_vBlockSize.X * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZMin = -(int)(((vMin.Z - vStartPos.Z) / (m_vBlockSize.Y * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);
	int nXMax = 1 + (int)(((vMax.X - vStartPos.X) / (m_vBlockSize.X * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZMax = 1 - (int)(((vMax.Z - vStartPos.Z) / (m_vBlockSize.Y * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);
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
	fread(&m_vBlockSize, sizeof(Vector2), 1, pFile);

	//���_�f�[�^���̓Ǎ�
	int nNumVtx;
	fread(&nNumVtx, sizeof(int), 1, pFile);

	//���_�f�[�^�̓Ǎ�
	m_vectorVtx.resize(nNumVtx);
	fread(&m_vectorVtx[0], sizeof(Vector3), nNumVtx, pFile);

	fclose(pFile);
	return;
}

//--------------------------------------------------------------------------------
//	�֐����FgetPointInfo
//  �֐������F�t�@�C�����璸�_�f�[�^��ǂݍ���
//	�����F	strFieldName�F�t�B�[���h���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
bool CFieldColliderComponent::getPointInfo(const Vector3& Position, Vector3& vPLeftUp, Vector3& vPRightDown, Vector3& vPSide, Vector3& vFaceNormal)
{
	auto PositionCenter = Vector3(m_mtxOffset.Elements[3][0], m_mtxOffset.Elements[3][1], m_mtxOffset.Elements[3][2]);
	auto vStartPos = PositionCenter + Vector3(-m_nNumBlockX * 0.5f * m_vBlockSize.X, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.Y);
	int nXLeftUp = (int)(((Position.X - vStartPos.X) / (m_vBlockSize.X * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZLeftUp = -(int)(((Position.Z - vStartPos.Z) / (m_vBlockSize.Y * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);

	//�t�B�[���h�͈̔͊O�������珈���I��
	if (nXLeftUp < 0 || nXLeftUp >= m_nNumBlockX || nZLeftUp < 0 || nZLeftUp >= m_nNumBlockZ)
	{
		return false;
	}

	int nXRightDown = nXLeftUp + 1;
	int nZRightDown = nZLeftUp + 1;

	auto PositionTarget = Vector3(Position.X, 0.0f, Position.Z);
	vPLeftUp = m_vectorVtx[nZLeftUp * (m_nNumBlockX + 1) + nXLeftUp];
	vPRightDown = m_vectorVtx[nZRightDown * (m_nNumBlockX + 1) + nXRightDown];

	//Check Side
	auto vMid = vPRightDown - vPLeftUp;
	auto vTL = PositionTarget - vPLeftUp;
	auto vCross = vTL * vMid;
	int nXSide, nZSide;
	int nSign = 0;
	if (vCross.Y >= 0.0f)
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
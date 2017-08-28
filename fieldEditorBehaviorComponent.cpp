//--------------------------------------------------------------------------------
//	�v���C���[�r�w�C�r�A�R���|�l���g
//�@fieldEditorBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "meshManager.h"
#include "inputManager.h"
#include "mode.h"
#include "camera.h"
#include "fieldEditorBehaviorComponent.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
CFieldEditorBehaviorComponent::CFieldEditorBehaviorComponent(CGameObject* const pGameObj)
	: CBehaviorComponent(pGameObj)
	, m_nNumBlockX(100)
	, m_nNumBlockZ(100)
	, m_vBlockSize(CKFVec2(5.0f))
	, m_vPosMin(CKFVec3(0.0f))
	, m_vPosMax(CKFVec3(0.0f))
	, m_vEditorPos(CKFVec3(0.0f))
	, m_fEditorRadius(0.0f)
	, m_bActive(true)
{
	m_vectorVtx.clear();

	//Mesh����
	CMain::GetManager()->GetMeshManager()->CreateEditorField(m_nNumBlockX, m_nNumBlockZ, m_vBlockSize);
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
bool CFieldEditorBehaviorComponent::Init(void)
{
	CKFVec3 vStartPos = CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
	m_vectorVtx.resize((m_nNumBlockX + 1) * (m_nNumBlockZ + 1));
	int nCntVtx = 0;

	for (int nCntZ = 0; nCntZ < m_nNumBlockZ + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nNumBlockX + 1; nCntX++)
		{
			CKFVec3 vPos = vStartPos
				+ CKFVec3(nCntX * m_vBlockSize.m_fX, 0.0f, -nCntZ * m_vBlockSize.m_fY);
			m_vectorVtx[nCntVtx] = vPos;
			++nCntVtx;
		}
	}

	auto vHalfSize = CKFVec3(m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
	m_vPosMin = vHalfSize * -1.0f;
	m_vPosMax = vHalfSize;
	
	auto info = getInfo();
	CMain::GetManager()->GetMeshManager()->UpdateEditorField(m_vectorVtx, info.listChoosenIdx);

	return true;
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::Uninit(void)
{
	m_vectorVtx.clear();
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::Update(void)
{
	if (!m_bActive) 
	{
		return; 
	}

	auto pInput = CMain::GetManager()->GetInputManager();

	//�g�k
	auto fValue = (float)pInput->GetKeyPress(CInputManager::K_EXTEND)
		- (float)pInput->GetKeyPress(CInputManager::K_SHRINK);
	m_fEditorRadius += fValue;
	m_fEditorRadius = m_fEditorRadius < 0.0f ? 0.0f : m_fEditorRadius;
	
	auto info = getInfo();

	//���~
	fValue = (float)pInput->GetKeyPress(CInputManager::K_RAISE)
		- (float)pInput->GetKeyPress(CInputManager::K_REDUCE);
	for (auto nIdx : info.listChoosenIdx)
	{
		m_vectorVtx[nIdx].m_fY += fValue * 0.5f;
	}

	CMain::GetManager()->GetMeshManager()->UpdateEditorField(m_vectorVtx, info.listChoosenIdx);
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::AdjustPosInField(CKFVec3& vPos, const bool& bAdjustHeight)
{
	//�͈͓��ɂ���
	vPos.m_fX = vPos.m_fX < m_vPosMin.m_fX ? m_vPosMin.m_fX : vPos.m_fX > m_vPosMax.m_fX ? m_vPosMax.m_fX : vPos.m_fX;
	vPos.m_fZ = vPos.m_fZ < m_vPosMin.m_fZ ? m_vPosMin.m_fZ : vPos.m_fZ > m_vPosMax.m_fZ ? m_vPosMax.m_fZ : vPos.m_fZ;

	//�����̒���
	if (bAdjustHeight)
	{
		vPos.m_fY = getHeight(vPos);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FSaveAs
//  �֐������F�t�B�[���h����ۑ�����֐�
//	�����F	strFileName�F�t�@�C����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::SaveAs(const string& strFileName)
{
	//�t�B�[���h���b�V���̕ۑ�
	CMain::GetManager()->GetMeshManager()->SaveEditorFieldAs(strFileName);

	//�t�B�[���h�̕ۑ�
	string strName = "data/FIELD/" + strFileName + ".field";
	FILE *pFile;

	//file open
	fopen_s(&pFile, strName.c_str(), "wb");

	//�u���b�N���̕ۑ�
	fwrite(&m_nNumBlockX, sizeof(int), 1, pFile);
	fwrite(&m_nNumBlockZ, sizeof(int), 1, pFile);

	//�u���b�NSize�̕ۑ�
	fwrite(&m_vBlockSize, sizeof(CKFVec2), 1, pFile);

	//���_�f�[�^���̕ۑ�
	int nNumVtx = (int)m_vectorVtx.size();
	fwrite(&nNumVtx, sizeof(int), 1, pFile);

	//���_�f�[�^�̕ۑ�
	fwrite(&m_vectorVtx[0], sizeof(CKFVec3), nNumVtx, pFile);

	fclose(pFile);
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
float CFieldEditorBehaviorComponent::getHeight(const CKFVec3& vPos)
{
	CKFVec3 vStartPos = CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
	int nXLeftUp = (int)(((vPos.m_fX - vStartPos.m_fX) / (m_vBlockSize.m_fX * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZLeftUp = -(int)(((vPos.m_fZ - vStartPos.m_fZ) / (m_vBlockSize.m_fY * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);

	//�t�B�[���h�͈̔͊O�������珈���I��
	if (nXLeftUp < 0 || nXLeftUp >= m_nNumBlockX || nZLeftUp < 0 || nZLeftUp >= m_nNumBlockZ)
	{
		return 0.0f;
	}

	int nXRightDown = nXLeftUp + 1;
	int nZRightDown = nZLeftUp + 1;

	CKFVec3 vPosTarget = CKFVec3(vPos.m_fX, 0.0f, vPos.m_fZ);
	CKFVec3 vPLeftUp = m_vectorVtx[nZLeftUp * (m_nNumBlockX + 1) + nXLeftUp];
	CKFVec3 vPRightDown = m_vectorVtx[nZRightDown * (m_nNumBlockX + 1) + nXRightDown];

	//Check Side
	CKFVec3 vMid = vPRightDown - vPLeftUp;
	CKFVec3 vTL = vPosTarget - vPLeftUp;
	CKFVec3 vCross = vTL * vMid;
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
	CKFVec3 vPSide = m_vectorVtx[nZSide * (m_nNumBlockX + 1) + nXSide];
	CKFVec3 vLS = vPLeftUp - vPSide;
	CKFVec3 vRS = vPRightDown - vPSide;
	CKFVec3 vNormal = (vLS * vRS) * (float)nSign;
	CKFMath::VecNormalize(vNormal);

	auto fHeight = vPSide.m_fY - ((vPos.m_fX - vPSide.m_fX) * vNormal.m_fX + (vPos.m_fZ - vPSide.m_fZ) * vNormal.m_fZ) / vNormal.m_fY;

	return fHeight;
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
CFieldEditorBehaviorComponent::EINFO CFieldEditorBehaviorComponent::getInfo(void)
{
	EINFO info;
	auto vEditorPos = m_vEditorPos;
	vEditorPos.m_fY = 0.0f;
	auto vStartPos = CKFVec3(-m_nNumBlockX * 0.5f * m_vBlockSize.m_fX, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.m_fY);
	int nXLeftUp = (int)(((m_vEditorPos.m_fX - vStartPos.m_fX) / (m_vBlockSize.m_fX * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZLeftUp = -(int)(((m_vEditorPos.m_fZ - vStartPos.m_fZ) / (m_vBlockSize.m_fY * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);
	int nXRightDown = nXLeftUp + 1;
	int nZRightDown = nZLeftUp + 1;

	int nRange = (int)(m_fEditorRadius / m_vBlockSize.m_fX);
	int nXMin = nXLeftUp - nRange;
	int nXMax = nXRightDown + nRange;
	int nZMin = nZLeftUp - nRange;
	int nZMax = nZRightDown + nRange;
	nXMin = nXMin < 0 ? 0 : nXMin;
	nZMin = nZMin < 0 ? 0 : nZMin;
	nXMax = nXMax > m_nNumBlockX ? m_nNumBlockX : nXMax;
	nZMax = nZMax > m_nNumBlockZ ? m_nNumBlockZ : nZMax;

	for (int nCntZ = nZMin; nCntZ <= nZMax; ++nCntZ)
	{
		for (int nCntX = nXMin; nCntX <= nXMax; ++nCntX)
		{
			auto nIdx = nCntZ * (m_nNumBlockZ + 1) + nCntX;
			auto vPos = m_vectorVtx[nIdx];
			vPos.m_fY = 0.0f;
			if (CKFMath::VecMagnitudeSquare(vPos - vEditorPos) <= m_fEditorRadius * m_fEditorRadius)
			{
				info.listChoosenIdx.push_back(nIdx);
			}
		}
	}
	return info;
}
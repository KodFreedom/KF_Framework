//--------------------------------------------------------------------------------
//	�v���C���[�r�w�C�r�A�R���|�l���g
//�@fieldEditorBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#ifdef _DEBUG
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
#include "ImGui\imgui.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
CFieldEditorBehaviorComponent::CFieldEditorBehaviorComponent(CGameObject* const pGameObj)
	: CBehaviorComponent(pGameObj)
	, m_nNumBlockX(100)
	, m_nNumBlockZ(100)
	, m_vBlockSize(Vector2(3.0f))
	, m_PositionMin(Vector3(0.0f))
	, m_PositionMax(Vector3(0.0f))
	, m_vEditorPos(Vector3(0.0f))
	, m_fEditorRadius(0.0f)
	, RaiseSpeed(1.0f)
	, m_fExtendSpeed(1.0f)
	, m_bActive(true)
{
	m_vectorVtx.clear();

	//Mesh����
	Main::GetManager()->GetMeshManager()->CreateEditorField(m_nNumBlockX, m_nNumBlockZ, m_vBlockSize);
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
bool CFieldEditorBehaviorComponent::Init(void)
{
	auto& vStartPos = Vector3(-m_nNumBlockX * 0.5f * m_vBlockSize.X, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.Y);
	m_vectorVtx.resize((m_nNumBlockX + 1) * (m_nNumBlockZ + 1));
	int nCntVtx = 0;

	for (int nCntZ = 0; nCntZ < m_nNumBlockZ + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < m_nNumBlockX + 1; nCntX++)
		{
			auto& Position = vStartPos
				+ Vector3(nCntX * m_vBlockSize.X, 0.0f, -nCntZ * m_vBlockSize.Y);
			m_vectorVtx[nCntVtx] = Position;
			++nCntVtx;
		}
	}

	auto& vHalfSize = Vector3(m_nNumBlockX * 0.5f * m_vBlockSize.X, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.Y);
	m_PositionMin = vHalfSize * -1.0f;
	m_PositionMax = vHalfSize;
	
	auto info = getInfo();
	Main::GetManager()->GetMeshManager()->UpdateEditorField(m_vectorVtx, info.listChoosenIdx);

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
	if (!m_bActive) { return; }
	showMainWindow();

	auto pInput = Main::GetManager()->GetInputManager();

	//�g�k
	auto fValue = (float)pInput->GetKeyPress(CInputManager::K_EXTEND)
		- (float)pInput->GetKeyPress(CInputManager::K_SHRINK);
	m_fEditorRadius += fValue * m_fExtendSpeed;
	m_fEditorRadius = m_fEditorRadius < 0.0f ? 0.0f : m_fEditorRadius;
	
	auto info = getInfo();

	//���~
	fValue = (float)pInput->GetKeyPress(CInputManager::K_RAISE)
		- (float)pInput->GetKeyPress(CInputManager::K_REDUCE);
	for (auto nIdx : info.listChoosenIdx)
	{
		m_vectorVtx[nIdx].Y += fValue * RaiseSpeed;
	}

	Main::GetManager()->GetMeshManager()->UpdateEditorField(m_vectorVtx, info.listChoosenIdx);
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::AdjustPosInField(Vector3& Position, const bool& bAdjustHeight)
{
	//�͈͓��ɂ���
	Position.X = Position.X < m_PositionMin.X ? m_PositionMin.X : Position.X > m_PositionMax.X ? m_PositionMax.X : Position.X;
	Position.Z = Position.Z < m_PositionMin.Z ? m_PositionMin.Z : Position.Z > m_PositionMax.Z ? m_PositionMax.Z : Position.Z;

	//�����̒���
	if (bAdjustHeight)
	{
		Position.Y = getHeight(Position);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FSetActive
//  �֐������F�t�B�[���h����ۑ�����֐�
//	�����F	strFileName�F�t�@�C����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::SetActive(const bool& bActive)
{
	m_bActive = bActive;
	if (!m_bActive)
	{//Field Reset
		EINFO info;
		Main::GetManager()->GetMeshManager()->UpdateEditorField(m_vectorVtx, info.listChoosenIdx);
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
	Main::GetManager()->GetMeshManager()->SaveEditorFieldAs(strFileName);

	//�t�B�[���h�̕ۑ�
	string strName = "data/FIELD/" + strFileName + ".field";
	FILE *pFile;

	//file open
	fopen_s(&pFile, strName.c_str(), "wb");

	//�u���b�N���̕ۑ�
	fwrite(&m_nNumBlockX, sizeof(int), 1, pFile);
	fwrite(&m_nNumBlockZ, sizeof(int), 1, pFile);

	//�u���b�NSize�̕ۑ�
	fwrite(&m_vBlockSize, sizeof(Vector2), 1, pFile);

	//���_�f�[�^���̕ۑ�
	int nNumVtx = (int)m_vectorVtx.size();
	fwrite(&nNumVtx, sizeof(int), 1, pFile);

	//���_�f�[�^�̕ۑ�
	fwrite(&m_vectorVtx[0], sizeof(Vector3), nNumVtx, pFile);

	fclose(pFile);
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
float CFieldEditorBehaviorComponent::getHeight(const Vector3& Position)
{
	auto& vStartPos = Vector3(-m_nNumBlockX * 0.5f * m_vBlockSize.X, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.Y);
	int nXLeftUp = (int)(((Position.X - vStartPos.X) / (m_vBlockSize.X * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZLeftUp = -(int)(((Position.Z - vStartPos.Z) / (m_vBlockSize.Y * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);

	//�t�B�[���h�͈̔͊O�������珈���I��
	if (nXLeftUp < 0 || nXLeftUp >= m_nNumBlockX || nZLeftUp < 0 || nZLeftUp >= m_nNumBlockZ)
	{
		return 0.0f;
	}

	int nXRightDown = nXLeftUp + 1;
	int nZRightDown = nZLeftUp + 1;

	auto& PositionTarget = Vector3(Position.X, 0.0f, Position.Z);
	auto& vPLeftUp = m_vectorVtx[nZLeftUp * (m_nNumBlockX + 1) + nXLeftUp];
	auto& vPRightDown = m_vectorVtx[nZRightDown * (m_nNumBlockX + 1) + nXRightDown];

	//Check Side
	auto& vMid = vPRightDown - vPLeftUp;
	auto& vTL = PositionTarget - vPLeftUp;
	auto& vCross = vTL * vMid;
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
	auto& vPSide = m_vectorVtx[nZSide * (m_nNumBlockX + 1) + nXSide];
	auto& vLS = vPLeftUp - vPSide;
	auto& vRS = vPRightDown - vPSide;
	auto& Normal = (vLS * vRS) * (float)nSign;
	CKFMath::VecNormalize(Normal);

	auto fHeight = vPSide.Y - ((Position.X - vPSide.X) * Normal.X + (Position.Z - vPSide.Z) * Normal.Z) / Normal.Y;

	return fHeight;
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
CFieldEditorBehaviorComponent::EINFO CFieldEditorBehaviorComponent::getInfo(void)
{
	EINFO info;
	auto vEditorPos = m_vEditorPos;
	vEditorPos.Y = 0.0f;
	auto vStartPos = Vector3(-m_nNumBlockX * 0.5f * m_vBlockSize.X, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.Y);
	int nXLeftUp = (int)(((m_vEditorPos.X - vStartPos.X) / (m_vBlockSize.X * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZLeftUp = -(int)(((m_vEditorPos.Z - vStartPos.Z) / (m_vBlockSize.Y * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);
	int nXRightDown = nXLeftUp + 1;
	int nZRightDown = nZLeftUp + 1;

	int nRange = (int)(m_fEditorRadius / m_vBlockSize.X);
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
			auto Position = m_vectorVtx[nIdx];
			Position.Y = 0.0f;
			if (CKFMath::VecMagnitudeSquare(Position - vEditorPos) <= m_fEditorRadius * m_fEditorRadius)
			{
				info.listChoosenIdx.push_back(nIdx);
			}
		}
	}
	return info;
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::showMainWindow(void)
{
	// Begin
	if (!ImGui::Begin("Field Editor Window"))
	{
		ImGui::End();
		return;
	}

	// Controll
	ImGui::Text("Extend / Shrink : T / Y");
	ImGui::Text("Raise / Reduce : G / H");

	// Radius
	ImGui::Text("Radius : %f", m_fEditorRadius);

	// Raise Speed
	ImGui::InputFloat("Raise Speed", &RaiseSpeed);

	// Extend Speed
	ImGui::InputFloat("Extend Speed", &m_fExtendSpeed);

	// End
	ImGui::End();
}
#endif // _DEBUG
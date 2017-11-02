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
CFieldEditorBehaviorComponent::CFieldEditorBehaviorComponent(GameObject* const pGameObj)
	: CBehaviorComponent(pGameObj)
	, blockXNumber(100)
	, blockZNumber(100)
	, blockSize(Vector2(3.0f))
	, PositionMin(Vector3(0.0f))
	, PositionMax(Vector3(0.0f))
	, m_vEditorPos(Vector3(0.0f))
	, m_fEditorRadius(0.0f)
	, RaiseSpeed(1.0f)
	, m_fExtendSpeed(1.0f)
	, m_bActive(true)
{
	vertexes.clear();

	//Mesh����
	Main::GetManager()->GetMeshManager()->CreateEditorField(blockXNumber, blockZNumber, blockSize);
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
bool CFieldEditorBehaviorComponent::Init(void)
{
	auto& vStartPos = Vector3(-blockXNumber * 0.5f * blockSize.X, 0.0f, blockZNumber * 0.5f * blockSize.Y);
	vertexes.resize((blockXNumber + 1) * (blockZNumber + 1));
	int countVtx = 0;

	for (int countZ = 0; countZ < blockZNumber + 1; countZ++)
	{
		for (int countX = 0; countX < blockXNumber + 1; countX++)
		{
			auto& Position = vStartPos
				+ Vector3(countX * blockSize.X, 0.0f, -countZ * blockSize.Y);
			vertexes[countVtx] = Position;
			++countVtx;
		}
	}

	auto& vHalfSize = Vector3(blockXNumber * 0.5f * blockSize.X, 0.0f, blockZNumber * 0.5f * blockSize.Y);
	PositionMin = vHalfSize * -1.0f;
	PositionMax = vHalfSize;
	
	auto info = getInfo();
	Main::GetManager()->GetMeshManager()->UpdateEditorField(vertexes, info.listChoosenIdx);

	return true;
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::Uninit(void)
{
	vertexes.clear();
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
	auto fValue = (float)pInput->GetKeyPress(Input::Extend)
		- (float)pInput->GetKeyPress(Input::Shrink);
	m_fEditorRadius += fValue * m_fExtendSpeed;
	m_fEditorRadius = m_fEditorRadius < 0.0f ? 0.0f : m_fEditorRadius;
	
	auto info = getInfo();

	//���~
	fValue = (float)pInput->GetKeyPress(Input::Raise)
		- (float)pInput->GetKeyPress(Input::Reduce);
	for (auto nIdx : info.listChoosenIdx)
	{
		vertexes[nIdx].Y += fValue * RaiseSpeed;
	}

	Main::GetManager()->GetMeshManager()->UpdateEditorField(vertexes, info.listChoosenIdx);
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::AdjustPosInField(Vector3& Position, const bool& bAdjustHeight)
{
	//�͈͓��ɂ���
	Position.X = Position.X < PositionMin.X ? PositionMin.X : Position.X > PositionMax.X ? PositionMax.X : Position.X;
	Position.Z = Position.Z < PositionMin.Z ? PositionMin.Z : Position.Z > PositionMax.Z ? PositionMax.Z : Position.Z;

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
		Main::GetManager()->GetMeshManager()->UpdateEditorField(vertexes, info.listChoosenIdx);
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
	FILE *filePointer;

	//file open
	fopen_s(&filePointer, strName.c_str(), "wb");

	//�u���b�N���̕ۑ�
	fwrite(&blockXNumber, sizeof(int), 1, filePointer);
	fwrite(&blockZNumber, sizeof(int), 1, filePointer);

	//�u���b�NSize�̕ۑ�
	fwrite(&blockSize, sizeof(Vector2), 1, filePointer);

	//���_�f�[�^���̕ۑ�
	int nNumVtx = (int)vertexes.size();
	fwrite(&nNumVtx, sizeof(int), 1, filePointer);

	//���_�f�[�^�̕ۑ�
	fwrite(&vertexes[0], sizeof(Vector3), nNumVtx, filePointer);

	fclose(filePointer);
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
float CFieldEditorBehaviorComponent::getHeight(const Vector3& Position)
{
	auto& vStartPos = Vector3(-blockXNumber * 0.5f * blockSize.X, 0.0f, blockZNumber * 0.5f * blockSize.Y);
	int nXLeftUp = (int)(((Position.X - vStartPos.X) / (blockSize.X * (float)blockXNumber)) * (float)blockXNumber);
	int nZLeftUp = -(int)(((Position.Z - vStartPos.Z) / (blockSize.Y * (float)blockZNumber)) * (float)blockZNumber);

	//�t�B�[���h�͈̔͊O�������珈���I��
	if (nXLeftUp < 0 || nXLeftUp >= blockXNumber || nZLeftUp < 0 || nZLeftUp >= blockZNumber)
	{
		return 0.0f;
	}

	int nXRightDown = nXLeftUp + 1;
	int nZRightDown = nZLeftUp + 1;

	auto& PositionTarget = Vector3(Position.X, 0.0f, Position.Z);
	auto& vPLeftUp = vertexes[nZLeftUp * (blockXNumber + 1) + nXLeftUp];
	auto& vPRightDown = vertexes[nZRightDown * (blockXNumber + 1) + nXRightDown];

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
	auto& vPSide = vertexes[nZSide * (blockXNumber + 1) + nXSide];
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
	auto vStartPos = Vector3(-blockXNumber * 0.5f * blockSize.X, 0.0f, blockZNumber * 0.5f * blockSize.Y);
	int nXLeftUp = (int)(((m_vEditorPos.X - vStartPos.X) / (blockSize.X * (float)blockXNumber)) * (float)blockXNumber);
	int nZLeftUp = -(int)(((m_vEditorPos.Z - vStartPos.Z) / (blockSize.Y * (float)blockZNumber)) * (float)blockZNumber);
	int nXRightDown = nXLeftUp + 1;
	int nZRightDown = nZLeftUp + 1;

	int nRange = (int)(m_fEditorRadius / blockSize.X);
	int nXMin = nXLeftUp - nRange;
	int nXMax = nXRightDown + nRange;
	int nZMin = nZLeftUp - nRange;
	int nZMax = nZRightDown + nRange;
	nXMin = nXMin < 0 ? 0 : nXMin;
	nZMin = nZMin < 0 ? 0 : nZMin;
	nXMax = nXMax > blockXNumber ? blockXNumber : nXMax;
	nZMax = nZMax > blockZNumber ? blockZNumber : nZMax;

	for (int countZ = nZMin; countZ <= nZMax; countZ)
	{
		for (int countX = nXMin; countX <= nXMax; countX)
		{
			auto nIdx = countZ * (blockZNumber + 1) + countX;
			auto Position = vertexes[nIdx];
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
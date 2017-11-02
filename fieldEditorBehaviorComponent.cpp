//--------------------------------------------------------------------------------
//	プレイヤービヘイビアコンポネント
//　fieldEditorBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  インクルードファイル
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
//  クラス宣言
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

	//Mesh生成
	Main::GetManager()->GetMeshManager()->CreateEditorField(blockXNumber, blockZNumber, blockSize);
}

//--------------------------------------------------------------------------------
//  クラス宣言
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
//  クラス宣言
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::Uninit(void)
{
	vertexes.clear();
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::Update(void)
{
	if (!m_bActive) { return; }
	showMainWindow();

	auto pInput = Main::GetManager()->GetInputManager();

	//拡縮
	auto fValue = (float)pInput->GetKeyPress(Input::Extend)
		- (float)pInput->GetKeyPress(Input::Shrink);
	m_fEditorRadius += fValue * m_fExtendSpeed;
	m_fEditorRadius = m_fEditorRadius < 0.0f ? 0.0f : m_fEditorRadius;
	
	auto info = getInfo();

	//升降
	fValue = (float)pInput->GetKeyPress(Input::Raise)
		- (float)pInput->GetKeyPress(Input::Reduce);
	for (auto nIdx : info.listChoosenIdx)
	{
		vertexes[nIdx].Y += fValue * RaiseSpeed;
	}

	Main::GetManager()->GetMeshManager()->UpdateEditorField(vertexes, info.listChoosenIdx);
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::AdjustPosInField(Vector3& Position, const bool& bAdjustHeight)
{
	//範囲内にする
	Position.X = Position.X < PositionMin.X ? PositionMin.X : Position.X > PositionMax.X ? PositionMax.X : Position.X;
	Position.Z = Position.Z < PositionMin.Z ? PositionMin.Z : Position.Z > PositionMax.Z ? PositionMax.Z : Position.Z;

	//高さの調節
	if (bAdjustHeight)
	{
		Position.Y = getHeight(Position);
	}
}

//--------------------------------------------------------------------------------
//	関数名：SetActive
//  関数説明：フィールド情報を保存する関数
//	引数：	strFileName：ファイル名
//	戻り値：なし
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
//	関数名：SaveAs
//  関数説明：フィールド情報を保存する関数
//	引数：	strFileName：ファイル名
//	戻り値：なし
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::SaveAs(const string& strFileName)
{
	//フィールドメッシュの保存
	Main::GetManager()->GetMeshManager()->SaveEditorFieldAs(strFileName);

	//フィールドの保存
	string strName = "data/FIELD/" + strFileName + ".field";
	FILE *filePointer;

	//file open
	fopen_s(&filePointer, strName.c_str(), "wb");

	//ブロック数の保存
	fwrite(&blockXNumber, sizeof(int), 1, filePointer);
	fwrite(&blockZNumber, sizeof(int), 1, filePointer);

	//ブロックSizeの保存
	fwrite(&blockSize, sizeof(Vector2), 1, filePointer);

	//頂点データ数の保存
	int nNumVtx = (int)vertexes.size();
	fwrite(&nNumVtx, sizeof(int), 1, filePointer);

	//頂点データの保存
	fwrite(&vertexes[0], sizeof(Vector3), nNumVtx, filePointer);

	fclose(filePointer);
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
float CFieldEditorBehaviorComponent::getHeight(const Vector3& Position)
{
	auto& vStartPos = Vector3(-blockXNumber * 0.5f * blockSize.X, 0.0f, blockZNumber * 0.5f * blockSize.Y);
	int nXLeftUp = (int)(((Position.X - vStartPos.X) / (blockSize.X * (float)blockXNumber)) * (float)blockXNumber);
	int nZLeftUp = -(int)(((Position.Z - vStartPos.Z) / (blockSize.Y * (float)blockZNumber)) * (float)blockZNumber);

	//フィールドの範囲外だったら処理終了
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
//  クラス宣言
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
//  クラス宣言
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
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
CFieldEditorBehaviorComponent::CFieldEditorBehaviorComponent(CGameObject* const pGameObj)
	: CBehaviorComponent(pGameObj)
	, m_nNumBlockX(100)
	, m_nNumBlockZ(100)
	, m_vBlockSize(Vector2(3.0f))
	, m_vPosMin(Vector3(0.0f))
	, m_vPosMax(Vector3(0.0f))
	, m_vEditorPos(Vector3(0.0f))
	, m_fEditorRadius(0.0f)
	, RaiseSpeed(1.0f)
	, m_fExtendSpeed(1.0f)
	, m_bActive(true)
{
	m_vectorVtx.clear();

	//Mesh生成
	CMain::GetManager()->GetMeshManager()->CreateEditorField(m_nNumBlockX, m_nNumBlockZ, m_vBlockSize);
}

//--------------------------------------------------------------------------------
//  クラス宣言
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
			auto& vPos = vStartPos
				+ Vector3(nCntX * m_vBlockSize.X, 0.0f, -nCntZ * m_vBlockSize.Y);
			m_vectorVtx[nCntVtx] = vPos;
			++nCntVtx;
		}
	}

	auto& vHalfSize = Vector3(m_nNumBlockX * 0.5f * m_vBlockSize.X, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.Y);
	m_vPosMin = vHalfSize * -1.0f;
	m_vPosMax = vHalfSize;
	
	auto info = getInfo();
	CMain::GetManager()->GetMeshManager()->UpdateEditorField(m_vectorVtx, info.listChoosenIdx);

	return true;
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::Uninit(void)
{
	m_vectorVtx.clear();
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::Update(void)
{
	if (!m_bActive) { return; }
	showMainWindow();

	auto pInput = CMain::GetManager()->GetInputManager();

	//拡縮
	auto fValue = (float)pInput->GetKeyPress(CInputManager::K_EXTEND)
		- (float)pInput->GetKeyPress(CInputManager::K_SHRINK);
	m_fEditorRadius += fValue * m_fExtendSpeed;
	m_fEditorRadius = m_fEditorRadius < 0.0f ? 0.0f : m_fEditorRadius;
	
	auto info = getInfo();

	//升降
	fValue = (float)pInput->GetKeyPress(CInputManager::K_RAISE)
		- (float)pInput->GetKeyPress(CInputManager::K_REDUCE);
	for (auto nIdx : info.listChoosenIdx)
	{
		m_vectorVtx[nIdx].Y += fValue * RaiseSpeed;
	}

	CMain::GetManager()->GetMeshManager()->UpdateEditorField(m_vectorVtx, info.listChoosenIdx);
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
void CFieldEditorBehaviorComponent::AdjustPosInField(Vector3& vPos, const bool& bAdjustHeight)
{
	//範囲内にする
	vPos.X = vPos.X < m_vPosMin.X ? m_vPosMin.X : vPos.X > m_vPosMax.X ? m_vPosMax.X : vPos.X;
	vPos.Z = vPos.Z < m_vPosMin.Z ? m_vPosMin.Z : vPos.Z > m_vPosMax.Z ? m_vPosMax.Z : vPos.Z;

	//高さの調節
	if (bAdjustHeight)
	{
		vPos.Y = getHeight(vPos);
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
		CMain::GetManager()->GetMeshManager()->UpdateEditorField(m_vectorVtx, info.listChoosenIdx);
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
	CMain::GetManager()->GetMeshManager()->SaveEditorFieldAs(strFileName);

	//フィールドの保存
	string strName = "data/FIELD/" + strFileName + ".field";
	FILE *pFile;

	//file open
	fopen_s(&pFile, strName.c_str(), "wb");

	//ブロック数の保存
	fwrite(&m_nNumBlockX, sizeof(int), 1, pFile);
	fwrite(&m_nNumBlockZ, sizeof(int), 1, pFile);

	//ブロックSizeの保存
	fwrite(&m_vBlockSize, sizeof(Vector2), 1, pFile);

	//頂点データ数の保存
	int nNumVtx = (int)m_vectorVtx.size();
	fwrite(&nNumVtx, sizeof(int), 1, pFile);

	//頂点データの保存
	fwrite(&m_vectorVtx[0], sizeof(Vector3), nNumVtx, pFile);

	fclose(pFile);
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
float CFieldEditorBehaviorComponent::getHeight(const Vector3& vPos)
{
	auto& vStartPos = Vector3(-m_nNumBlockX * 0.5f * m_vBlockSize.X, 0.0f, m_nNumBlockZ * 0.5f * m_vBlockSize.Y);
	int nXLeftUp = (int)(((vPos.X - vStartPos.X) / (m_vBlockSize.X * (float)m_nNumBlockX)) * (float)m_nNumBlockX);
	int nZLeftUp = -(int)(((vPos.Z - vStartPos.Z) / (m_vBlockSize.Y * (float)m_nNumBlockZ)) * (float)m_nNumBlockZ);

	//フィールドの範囲外だったら処理終了
	if (nXLeftUp < 0 || nXLeftUp >= m_nNumBlockX || nZLeftUp < 0 || nZLeftUp >= m_nNumBlockZ)
	{
		return 0.0f;
	}

	int nXRightDown = nXLeftUp + 1;
	int nZRightDown = nZLeftUp + 1;

	auto& vPosTarget = Vector3(vPos.X, 0.0f, vPos.Z);
	auto& vPLeftUp = m_vectorVtx[nZLeftUp * (m_nNumBlockX + 1) + nXLeftUp];
	auto& vPRightDown = m_vectorVtx[nZRightDown * (m_nNumBlockX + 1) + nXRightDown];

	//Check Side
	auto& vMid = vPRightDown - vPLeftUp;
	auto& vTL = vPosTarget - vPLeftUp;
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
	auto& vNormal = (vLS * vRS) * (float)nSign;
	CKFMath::VecNormalize(vNormal);

	auto fHeight = vPSide.Y - ((vPos.X - vPSide.X) * vNormal.X + (vPos.Z - vPSide.Z) * vNormal.Z) / vNormal.Y;

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
			auto vPos = m_vectorVtx[nIdx];
			vPos.Y = 0.0f;
			if (CKFMath::VecMagnitudeSquare(vPos - vEditorPos) <= m_fEditorRadius * m_fEditorRadius)
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
//--------------------------------------------------------------------------------
//	プレイヤービヘイビアコンポネント
//　fieldEditorBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "versionSetting.h"
#if defined(_DEBUG) || defined(EDITOR)
#include "main.h"
#include "meshManager.h"
#include "input.h"
#include "mode.h"
#include "camera.h"
#include "fieldEditor.h"
#include "ImGui\imgui.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
FieldEditor::FieldEditor(GameObject* const owner)
	: Behavior(owner, "FieldEditor")
	, blockXNumber(100)
	, blockZNumber(100)
	, blockSize(Vector2(3.0f))
	, minPosition(Vector3::Zero)
	, maxPosition(Vector3::Zero)
	, editorPosition(Vector3::Zero)
	, editorRadius(0.0f)
	, raiseSpeed(1.0f)
	, extendSpeed(1.0f)
	, isActive(true)
{
	vertexes.clear();
	MeshManager::Instance()->CreateEditorField(blockXNumber, blockZNumber, blockSize);
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
bool FieldEditor::Init(void)
{
	auto& startPosition = Vector3(-blockXNumber * 0.5f * blockSize.X, 0.0f, blockZNumber * 0.5f * blockSize.Y);
	vertexes.resize((blockXNumber + 1) * (blockZNumber + 1));
	int countVtx = 0;

	for (int countZ = 0; countZ < blockZNumber + 1; countZ++)
	{
		for (int countX = 0; countX < blockXNumber + 1; countX++)
		{
			auto& position = startPosition
				+ Vector3(countX * blockSize.X, 0.0f, -countZ * blockSize.Y);
			vertexes[countVtx] = position;
			++countVtx;
		}
	}

	auto& halfSize = Vector3(blockXNumber * 0.5f * blockSize.X, 0.0f, blockZNumber * 0.5f * blockSize.Y);
	minPosition = halfSize * -1.0f;
	maxPosition = halfSize;
	
	auto& indexes = getChoosenIndexes();
	MeshManager::Instance()->UpdateEditorField(vertexes, indexes);

	return true;
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
void FieldEditor::Uninit(void)
{
	vertexes.clear();
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
void FieldEditor::Update(void)
{
	if (!isActive) { return; }
	showMainWindow();

	auto input = Input::Instance();

	//拡縮
	auto fValue = (float)input->GetKeyPress(Key::Extend)
		- (float)input->GetKeyPress(Key::Shrink);
	editorRadius += fValue * extendSpeed;
	editorRadius = editorRadius < 0.0f ? 0.0f : editorRadius;
	
	auto& indexes = getChoosenIndexes();

	//升降
	fValue = (float)input->GetKeyPress(Key::Raise)
		- (float)input->GetKeyPress(Key::Reduce);
	for (auto index : indexes)
	{
		vertexes[index].Y += fValue * raiseSpeed;
	}

	MeshManager::Instance()->UpdateEditorField(vertexes, indexes);
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
Vector3 FieldEditor::AdjustPosInField(const Vector3& position, const bool& isAdjustHeight)
{
	Vector3 result;

	//範囲内にする
	result.X = Math::Clamp(position.X, minPosition.X, maxPosition.X);
	result.Z = Math::Clamp(position.Z, minPosition.Z, maxPosition.Z);

	//高さの調節
	if (isAdjustHeight)
	{
		result.Y = getHeight(result);
	}

	return result;
}

//--------------------------------------------------------------------------------
//	関数名：SetActive
//  関数説明：フィールド情報を保存する関数
//	引数：	value：ファイル名
//	戻り値：なし
//--------------------------------------------------------------------------------
void FieldEditor::SetActive(const bool& value)
{
	isActive = value;
	if (!isActive)
	{//Field Reset
		list<int> indexes;
		MeshManager::Instance()->UpdateEditorField(vertexes, indexes);
	}
}

//--------------------------------------------------------------------------------
//	関数名：SaveAs
//  関数説明：フィールド情報を保存する関数
//	引数：	strFileName：ファイル名
//	戻り値：なし
//--------------------------------------------------------------------------------
void FieldEditor::SaveAs(const string& fileName)
{
	//フィールドメッシュの保存
	MeshManager::Instance()->SaveEditorFieldAs(fileName);

	//フィールドの保存
	string filePath = "data/FIELD/" + fileName + ".field";
	FILE *filePointer;

	//file open
	fopen_s(&filePointer, filePath.c_str(), "wb");

	//ブロック数の保存
	fwrite(&blockXNumber, sizeof(int), 1, filePointer);
	fwrite(&blockZNumber, sizeof(int), 1, filePointer);

	//ブロックSizeの保存
	fwrite(&blockSize, sizeof(Vector2), 1, filePointer);

	//頂点データ数の保存
	int vertexNumber = (int)vertexes.size();
	fwrite(&vertexNumber, sizeof(int), 1, filePointer);

	//頂点データの保存
	fwrite(&vertexes[0], sizeof(Vector3), vertexNumber, filePointer);

	fclose(filePointer);
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
float FieldEditor::getHeight(const Vector3& position)
{
	auto& startPosition = Vector3(-blockXNumber * 0.5f * blockSize.X, 0.0f, blockZNumber * 0.5f * blockSize.Y);
	int leftUpX = (int)(((position.X - startPosition.X) / (blockSize.X * (float)blockXNumber)) * (float)blockXNumber);
	int leftUpZ = -(int)(((position.Z - startPosition.Z) / (blockSize.Y * (float)blockZNumber)) * (float)blockZNumber);

	//フィールドの範囲外だったら処理終了
	if (leftUpX < 0 || leftUpX >= blockXNumber || leftUpZ < 0 || leftUpZ >= blockZNumber)
	{
		return 0.0f;
	}

	int rightDownX = leftUpX + 1;
	int rightDownZ = leftUpZ + 1;

	auto& targetPosition = Vector3(position.X, 0.0f, position.Z);
	auto& leftUpPosition = vertexes[leftUpZ * (blockXNumber + 1) + leftUpX];
	auto& rightDownPosition = vertexes[rightDownZ * (blockXNumber + 1) + rightDownX];

	//Check Side
	auto& midLine = rightDownPosition - leftUpPosition;
	auto& leftUpToTarget = targetPosition - leftUpPosition;
	auto& cross = leftUpToTarget * midLine;
	int sideX, sideZ;
	int sign = 0;
	if (cross.Y >= 0.0f)
	{//LeftSide
		sideX = leftUpX + 1;
		sideZ = leftUpZ;
		sign = -1;
	}
	else
	{//RightSide
		sideX = leftUpX;
		sideZ = leftUpZ + 1;
		sign = 1;
	}
	auto& sidePosition = vertexes[sideZ * (blockXNumber + 1) + sideX];
	auto& sideToLeftUp = leftUpPosition - sidePosition;
	auto& sideToRightDown = rightDownPosition - sidePosition;
	auto& normal = ((sideToLeftUp * sideToRightDown) * (float)sign).Normalized();
	return sidePosition.Y - ((position.X - sidePosition.X) * normal.X + (position.Z - sidePosition.Z) * normal.Z) / normal.Y;;
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
list<int> FieldEditor::getChoosenIndexes(void)
{
	list<int> indexes;
	auto editorPositionCopy = editorPosition;
	editorPositionCopy.Y = 0.0f;
	auto startPosition = Vector3(-blockXNumber * 0.5f * blockSize.X, 0.0f, blockZNumber * 0.5f * blockSize.Y);
	int leftUpX = (int)(((editorPosition.X - startPosition.X) / (blockSize.X * (float)blockXNumber)) * (float)blockXNumber);
	int leftUpZ = -(int)(((editorPosition.Z - startPosition.Z) / (blockSize.Y * (float)blockZNumber)) * (float)blockZNumber);
	int rightDownX = leftUpX + 1;
	int rightDownZ = leftUpZ + 1;

	int range = (int)(editorRadius / blockSize.X);
	int minX = leftUpX - range;
	int maxX = rightDownX + range;
	int minZ = leftUpZ - range;
	int maxZ = rightDownZ + range;
	minX = minX < 0 ? 0 : minX;
	minZ = minZ < 0 ? 0 : minZ;
	maxX = maxX > blockXNumber ? blockXNumber : maxX;
	maxZ = maxZ > blockZNumber ? blockZNumber : maxZ;

	for (int countZ = minZ; countZ <= maxZ; countZ)
	{
		for (int countX = minX; countX <= maxX; countX)
		{
			auto index = countZ * (blockZNumber + 1) + countX;
			auto position = vertexes[index];
			position.Y = 0.0f;
			if (Vector3::SquareDistanceBetween(position, editorPositionCopy) <= editorRadius * editorRadius)
			{
				indexes.push_back(index);
			}
		}
	}
	return indexes;
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
void FieldEditor::showMainWindow(void)
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
	ImGui::Text("Radius : %f", editorRadius);

	// Raise Speed
	ImGui::InputFloat("Raise Speed", &raiseSpeed);

	// Extend Speed
	ImGui::InputFloat("Extend Speed", &extendSpeed);

	// End
	ImGui::End();
}
#endif // _DEBUG
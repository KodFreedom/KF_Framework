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
#include "fieldCollider.h"

//--------------------------------------------------------------------------------
//  
//	Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FGetPolygonAt
//  �֐������F�|�C���g��������|���S���̏����擾
//	�����F	point�F�|�C���g�ʒu
//	�߂�l�FPolygonInfo*
//--------------------------------------------------------------------------------
PolygonInfo* FieldCollider::GetPolygonAt(const Vector3& point) const
{
	auto& center = Vector3(offset.Elements[3][0], offset.Elements[3][1], offset.Elements[3][2]);
	auto& startPosition = center + Vector3(-blockXNumber * 0.5f * blockSize.X, 0.0f, blockZNumber * 0.5f * blockSize.Y);
	int leftUpX = (int)(((point.X - startPosition.X) / (blockSize.X * (float)blockXNumber)) * (float)blockXNumber);
	int leftUpZ = -(int)(((point.Z - startPosition.Z) / (blockSize.Y * (float)blockZNumber)) * (float)blockZNumber);

	//�t�B�[���h�͈̔͊O�������珈���I��
	if (leftUpX < 0 || leftUpX >= blockXNumber || leftUpZ < 0 || leftUpZ >= blockZNumber)
	{
		return nullptr;
	}

	auto result = new PolygonInfo;
	int rightDownX = leftUpX + 1;
	int rightDownZ = leftUpZ + 1;

	auto& target = Vector3(point.X, 0.0f, point.Z);
	result->LeftUp = vertexes[leftUpZ * (blockXNumber + 1) + leftUpX];
	result->RightDown = vertexes[rightDownZ * (blockXNumber + 1) + rightDownX];

	//Check Side
	auto& leftUpToRightDown = result->RightDown - result->LeftUp;
	auto& leftUpToTarget = target - result->LeftUp;
	auto& cross = leftUpToTarget * leftUpToRightDown;
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
	result->Side = vertexes[sideZ * (blockXNumber + 1) + sideX];
	auto& sideToLeftUp = result->LeftUp - result->Side;
	auto& sideToRightDown = result->RightDown - result->Side;
	result->SurfaceNormal = ((sideToLeftUp * sideToRightDown) * (float)sign).Normalized();
	return result;
}

//--------------------------------------------------------------------------------
//	�֐����FGetVertexesInRange
//  �֐������F�͈͓��̒��_�f�[�^���擾����
//	�����F	begin�F�J�n�ʒu
//			end�F�I�_�ʒu
//	�߂�l�FVertexesInRange*
//--------------------------------------------------------------------------------
VertexesInRange* FieldCollider::GetVertexesInRange(const Vector3& begin, const Vector3& end)
{
	auto& minPosition = Vector3(min(begin.X, end.X), min(begin.Y, end.Y), min(begin.Z, end.Z));
	auto& maxPosition = Vector3(max(begin.X, end.X), max(begin.Y, end.Y), max(begin.Z, end.Z));

	//�͈͂��Z�o
	auto& center = Vector3(offset.Elements[3][0], offset.Elements[3][1], offset.Elements[3][2]);
	auto& startPosition = center + Vector3(-blockXNumber * 0.5f * blockSize.X, 0.0f, blockZNumber * 0.5f * blockSize.Y);
	int minX = (int)(((minPosition.X - startPosition.X) / (blockSize.X * (float)blockXNumber)) * (float)blockXNumber);
	int minZ = -(int)(((minPosition.Z - startPosition.Z) / (blockSize.Y * (float)blockZNumber)) * (float)blockZNumber);
	int maxX = 1 + (int)(((maxPosition.X - startPosition.X) / (blockSize.X * (float)blockXNumber)) * (float)blockXNumber);
	int maxZ = 1 - (int)(((maxPosition.Z - startPosition.Z) / (blockSize.Y * (float)blockZNumber)) * (float)blockZNumber);
	minX = minX < 0 ? 0 : minX;
	minZ = minZ < 0 ? 0 : minZ;
	maxX = maxX > blockXNumber + 1 ? blockXNumber + 1 : maxX;
	maxZ = maxZ > blockZNumber + 1 ? blockZNumber + 1 : maxZ;

	//�t�B�[���h�͈̔͊O�������珈���I��
	if (minX >= blockXNumber + 1 || minZ >= blockZNumber + 1
		|| maxX <= 0 || maxZ <= 0)
	{
		return nullptr;
	}

	//�͈͓��̒��_��ۑ�����
	auto result = new VertexesInRange;
	result->VertexesNumberX = maxX - minX;
	result->VertexesNumberZ = maxZ - minZ;
	result->vertexes.resize(result->VertexesNumberX * result->VertexesNumberZ);
	int count = 0;
	for (int countZ = minZ; countZ <= maxZ; ++countZ)
	{
		for (int countX = minX; countX <= maxX; ++countX)
		{
			result->vertexes[count] = vertexes[countZ * (blockXNumber + 1) + countX];
			++count;
		}
	}
	return result;
}

//--------------------------------------------------------------------------------
//  
//	Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����Fload
//  �֐������F�t�@�C�����璸�_�f�[�^��ǂݍ���
//	�����F	fieldName�F�t�B�[���h���O
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void FieldCollider::load(const string& fieldName)
{
	string strName = "data/FIELD/" + fieldName + ".field";
	FILE *filePointer;
	fopen_s(&filePointer, strName.c_str(), "rb");
	if (!filePointer) 
	{
		MessageBox(NULL, "FieldCollider : load ERROR!! �t�@�C����������Ȃ�!!", "�G���[", MB_OK | MB_ICONWARNING);
		return;
	}
	fread(&blockXNumber, sizeof(int), 1, filePointer);
	fread(&blockZNumber, sizeof(int), 1, filePointer);
	fread(&blockSize, sizeof(Vector2), 1, filePointer);
	int vertexNumber;
	fread(&vertexNumber, sizeof(int), 1, filePointer);
	vertexes.resize(vertexNumber);
	fread(&vertexes[0], sizeof(Vector3), vertexNumber, filePointer);
	fclose(filePointer);
	return;
}
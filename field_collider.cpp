//--------------------------------------------------------------------------------
//	fieldColliderコンポネント
//　fieldCollider.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "main.h"
#include "field_collider.h"

//--------------------------------------------------------------------------------
//  
//	Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	ポイント所属するポリゴンの情報を取得
//--------------------------------------------------------------------------------
PolygonInfo* FieldCollider::GetPolygonAt(const Vector3& point) const
{
	auto& center = Vector3(offset_.m30_, offset_.m31_, offset_.m32_);
	auto& start = center + Vector3(-block_number_x_ * 0.5f * block_size_.x_, 0.0f, block_number_z_ * 0.5f * block_size_.y_);
	int left_up_x = static_cast<int>(((point.x_ - start.x_) / (block_size_.x_ * static_cast<float>(block_number_x_))) * static_cast<float>(block_number_x_));
	int left_up_z = -static_cast<int>(((point.z_ - start.z_) / (block_size_.y_ * static_cast<float>(block_number_z_))) * static_cast<float>(block_number_z_));

	//フィールドの範囲外だったら処理終了
	if (left_up_x < 0 || left_up_x >= block_number_x_ || left_up_z < 0 || left_up_z >= block_number_z_)
	{
		return nullptr;
	}

	auto result = MY_NEW PolygonInfo;
	int right_down_x = left_up_x + 1;
	int right_down_z = left_up_z + 1;

	auto& target = Vector3(point.x_, 0.0f, point.z_);
	result->left_up = vertexes_[left_up_z * (block_number_x_ + 1) + left_up_x];
	result->right_down = vertexes_[right_down_z * (block_number_x_ + 1) + right_down_x];

	//Check Side
	auto& left_up_to_right_down = result->right_down - result->left_up;
	auto& left_up_to_target = target - result->left_up;
	auto& cross = left_up_to_target * left_up_to_right_down;
	int side_x, side_z;
	int sign = 0;
	if (cross.y_ >= 0.0f)
	{//LeftSide
		side_x = left_up_x + 1;
		side_z = left_up_z;
		sign = -1;
	}
	else
	{//RightSide
		side_x = left_up_x;
		side_z = left_up_z + 1;
		sign = 1;
	}
	result->side = vertexes_[side_z * (block_number_x_ + 1) + side_x];
	auto& side_to_left_up = result->left_up - result->side;
	auto& side_to_right_down = result->right_down - result->side;
	result->normal = ((side_to_left_up * side_to_right_down) * static_cast<float>(sign)).Normalized();
	return result;
}

//--------------------------------------------------------------------------------
//	範囲内の頂点データを取得する
//--------------------------------------------------------------------------------
VertexesInRange* FieldCollider::GetVertexesInRange(const Vector3& begin, const Vector3& end) const
{
	auto& min_position = Vector3(min(begin.x_, end.x_), min(begin.y_, end.y_), min(begin.z_, end.z_));
	auto& max_position = Vector3(max(begin.x_, end.x_), max(begin.y_, end.y_), max(begin.z_, end.z_));

	//範囲を算出
	auto& center = Vector3(offset_.m30_, offset_.m31_, offset_.m32_);
	auto& start = center + Vector3(-block_number_x_ * 0.5f * block_size_.x_, 0.0f, block_number_z_ * 0.5f * block_size_.y_);
	int min_x = static_cast<int>(((min_position.x_ - start.x_) / (block_size_.x_ * static_cast<float>(block_number_x_))) * static_cast<float>(block_number_x_));
	int min_z = -static_cast<int>(((min_position.z_ - start.z_) / (block_size_.y_ * static_cast<float>(block_number_z_))) * static_cast<float>(block_number_z_));
	int max_x = 1 + static_cast<int>(((max_position.x_ - start.x_) / (block_size_.x_ * static_cast<float>(block_number_x_))) * static_cast<float>(block_number_x_));
	int max_z = 1 - static_cast<int>(((max_position.z_ - start.z_) / (block_size_.y_ * static_cast<float>(block_number_z_))) * static_cast<float>(block_number_z_));
	min_x = min_x < 0 ? 0 : min_x;
	min_z = min_z < 0 ? 0 : min_z;
	max_x = max_x > block_number_x_ + 1 ? block_number_x_ + 1 : max_x;
	max_z = max_z > block_number_z_ + 1 ? block_number_z_ + 1 : max_z;

	//フィールドの範囲外だったら処理終了
	if (min_x >= block_number_x_ + 1 || min_z >= block_number_z_ + 1
		|| max_x <= 0 || max_z <= 0)
	{
		return nullptr;
	}

	//範囲内の頂点を保存する
	auto result = MY_NEW VertexesInRange;
	result->vertex_number_x = max_x - min_x;
	result->vertex_number_z = max_z - min_z;
	result->vertexes.reserve(result->vertex_number_x * result->vertex_number_z);
	for (int count_z = min_z; count_z <= max_z; ++count_z)
	{
		for (int count_x = min_x; count_x <= max_x; ++count_x)
		{
			result->vertexes.push_back(vertexes_[count_z * (block_number_x_ + 1) + count_x]);
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
//	ファイルから頂点データを読み込む
//--------------------------------------------------------------------------------
void FieldCollider::Load(const String& field_name)
{
	auto& path = L"data/field/" + field_name + L".field";
	ifstream file(path);
	if (!file.is_open())
	{
		assert("load error");
		return;
	}
	BinaryInputArchive archive(file);
	archive.loadBinary(&block_number_x_, sizeof(block_number_x_));
	archive.loadBinary(&block_number_z_, sizeof(block_number_z_));
	archive.loadBinary(&block_size_, sizeof(block_size_));
	int vertex_number;
	archive.loadBinary(&vertex_number, sizeof(vertex_number));
	vertexes_.resize(vertex_number);
	archive.loadBinary(&vertexes_[0], sizeof(Vector3) * vertex_number);
	file.close();
}
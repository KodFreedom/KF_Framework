//--------------------------------------------------------------------------------
//	プレイヤービヘイビアコンポネント
//　FieldEditor.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "field_editor.h"
#if defined(_DEBUG) || defined(EDITOR)
#include "main_system.h"
#include "mesh_manager.h"
#include "input.h"
#include "mode.h"
#include "camera.h"
#include "ImGui\imgui.h"

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
FieldEditor::FieldEditor(GameObject& owner)
	: Behavior(owner, L"FieldEditor")
	, block_number_x_(100)
	, block_number_z_(100)
	, block_size_(Vector2(3.0f))
	, min_position_(Vector3::kZero)
	, max_position_(Vector3::kZero)
	, editor_position_(Vector3::kZero)
	, editor_radius_(0.0f)
	, raise_speed_(1.0f)
	, extend_speed_(1.0f)
	, is_active_(true)
{}

//--------------------------------------------------------------------------------
//  初期化
//--------------------------------------------------------------------------------
bool FieldEditor::Init(void)
{
	// Vertex
	auto& start = Vector3(-block_number_x_ * 0.5f * block_size_.x_, 0.0f, block_number_z_ * 0.5f * block_size_.y_);
	vertexes_.resize((block_number_x_ + 1) * (block_number_z_ + 1));
	for (int count_z = 0; count_z < block_number_z_ + 1; ++count_z)
	{
		for (int count_x = 0; count_x < block_number_x_ + 1; ++count_x)
		{
			int index = count_z * (block_number_x_ + 1) + count_x;
			auto& position = start
				+ Vector3(count_x * block_size_.x_, 0.0f, -count_z * block_size_.y_);
			vertexes_[index].position = position;
			vertexes_[index].uv = Vector2(count_x * 1.0f / static_cast<float>(block_number_x_)
				, count_z * 1.0f / static_cast<float>(block_number_z_));
			vertexes_[index].color = Color::kWhite;
			vertexes_[index].normal = Vector3::kUp;
		}
	}

	auto& half_size = Vector3(block_number_x_ * 0.5f * block_size_.x_, 0.0f, block_number_z_ * 0.5f * block_size_.y_);
	min_position_ = half_size * -1.0f;
	max_position_ = half_size;
	
	// Index
	vector<int> indexes;
	int index_number = ((block_number_x_ + 1) * 2 + 2) * block_number_z_ - 1;
	indexes.resize(index_number);
	for (int count_z = 0; count_z < block_number_z_; ++count_z)
	{
		for (int count_x = 0; count_x < (block_number_x_ + 1) * 2 + 2; ++count_x)
		{
			if (count_x < (block_number_x_ + 1) * 2)
			{
				indexes[count_z * ((block_number_x_ + 1) * 2 + 2) + count_x] = count_x / 2 + (count_x + 1) % 2 * (block_number_x_ + 1) + count_z * (block_number_x_ + 1);
			}
			else if (count_z * ((block_number_x_ + 1) * 2 + 2) + count_x < (((block_number_x_ + 1) * 2 + 2) * block_number_z_ - 1))//縮退ポリゴン
			{
				indexes[count_z * ((block_number_x_ + 1) * 2 + 2) + count_x] = (count_x - 1) / 2 % (block_number_x_ + 1) + count_x % 2 * 2 * (block_number_x_ + 1) + count_z * (block_number_x_ + 1);
			}
		}
	}

	MainSystem::Instance()->GetMeshManager()->Use(L"field", DrawType::kTriangleStrip, vertexes_, indexes, (block_number_x_ + 2) * 2 * block_number_z_ - 4);
	return true;
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void FieldEditor::Uninit(void)
{
	vertexes_.clear();
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void FieldEditor::Update(void)
{
	if (!is_active_) return;
	ShowMainWindow();

	auto input = MainSystem::Instance()->GetInput();

	//拡縮
	auto input_value = static_cast<float>(input->GetKeyPress(Key::kExtend))
		- static_cast<float>(input->GetKeyPress(Key::kShrink));
	editor_radius_ += input_value * extend_speed_;
	editor_radius_ = editor_radius_ < 0.0f ? 0.0f : editor_radius_;
	
	auto& indexes = GetChoosenIndexes();

	//升降
	input_value = static_cast<float>(input->GetKeyPress(Key::kRaise))
		- static_cast<float>(input->GetKeyPress(Key::kReduce));
	for (auto index : indexes)
	{
		vertexes_[index].position.y_ += input_value * raise_speed_;
	}

	auto& choosen_indexes = GetChoosenIndexes();
	UpdateVertexesBy(choosen_indexes);
	MainSystem::Instance()->GetMeshManager()->Update(L"field", vertexes_, choosen_indexes);
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
Vector3 FieldEditor::AdjustPositionInField(const Vector3& position, const bool& is_adjust_height)
{
	Vector3 result;

	//範囲内にする
	result.x_ = Math::Clamp(position.x_, min_position_.x_, max_position_.x_);
	result.z_ = Math::Clamp(position.z_, min_position_.z_, max_position_.z_);

	//高さの調節
	if (is_adjust_height)
	{
		result.y_ = GetHeight(result);
	}

	return result;
}

//--------------------------------------------------------------------------------
//	アクティブフラグの設定
//--------------------------------------------------------------------------------
void FieldEditor::SetActive(const bool& value)
{
	is_active_ = value;
	if (!is_active_)
	{//Field Reset
		list<int> choosen_indexes;
		MainSystem::Instance()->GetMeshManager()->Update(L"field", vertexes_, choosen_indexes);
	}
}

//--------------------------------------------------------------------------------
//	フィールド情報を保存する関数
//--------------------------------------------------------------------------------
void FieldEditor::SaveAsBinary(const String& name)
{
	//フィールドメッシュの保存
	MainSystem::Instance()->GetMeshManager()->SaveMeshToFile(L"field", name);

	//フィールドの保存
	String path = L"data/field/" + name + L".field";
	ofstream file(path);
	if (!file.is_open())
	{
		assert("failed to open file");
		return;
	}
	BinaryOutputArchive archive(file);

	//ブロック数の保存
	archive.saveBinary(&block_number_x_, sizeof(block_number_x_));
	archive.saveBinary(&block_number_z_, sizeof(block_number_z_));

	//ブロックSizeの保存
	archive.saveBinary(&block_size_, sizeof(block_size_));

	//頂点データ数の保存
	size_t vertex_number = vertexes_.size();
	archive.saveBinary(&vertex_number, sizeof(vertex_number));

	//頂点データの保存
	archive.saveBinary(&vertexes_[0], sizeof(Vector3) * vertex_number);

	file.close();
}

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
float FieldEditor::GetHeight(const Vector3& position)
{
	auto& start = Vector3(-block_number_x_ * 0.5f * block_size_.x_, 0.0f, block_number_z_ * 0.5f * block_size_.y_);
	int left_up_x = static_cast<int>(((position.x_ - start.x_) / (block_size_.x_ * static_cast<float>(block_number_x_))) * static_cast<float>(block_number_x_));
	int left_up_z = -static_cast<int>(((position.z_ - start.z_) / (block_size_.y_ * static_cast<float>(block_number_z_))) * static_cast<float>(block_number_z_));

	//フィールドの範囲外だったら処理終了
	if (left_up_x < 0 || left_up_x >= block_number_x_ || left_up_z < 0 || left_up_z >= block_number_z_)
	{
		return 0.0f;
	}

	int right_down_x = left_up_x + 1;
	int right_down_z = left_up_z + 1;

	auto& target_position = Vector3(position.x_, 0.0f, position.z_);
	auto& left_up_position = vertexes_[left_up_z * (block_number_x_ + 1) + left_up_x].position;
	auto& right_down_position = vertexes_[right_down_z * (block_number_x_ + 1) + right_down_x].position;

	//Check Side
	auto& midline = right_down_position - left_up_position;
	auto& left_up_to_target = target_position - left_up_position;
	auto& cross = left_up_to_target * midline;
	int side_x, side_z;
	float sign = 0.0f;
	if (cross.y_ >= 0.0f)
	{//LeftSide
		side_x = left_up_x + 1;
		side_z = left_up_z;
		sign = -1.0f;
	}
	else
	{//RightSide
		side_x = left_up_x;
		side_z = left_up_z + 1;
		sign = 1.0f;
	}
	auto& side_position = vertexes_[side_z * (block_number_x_ + 1) + side_x].position;
	auto& side_to_left_up = left_up_position - side_position;
	auto& side_to_right_down = right_down_position - side_position;
	auto& normal = ((side_to_left_up * side_to_right_down) * sign).Normalized();
	return side_position.y_ - ((position.x_ - side_position.x_) * normal.x_ + (position.z_ - side_position.z_) * normal.z_) / normal.y_;
}

//--------------------------------------------------------------------------------
//  範囲内のインデックスを取得
//--------------------------------------------------------------------------------
list<int> FieldEditor::GetChoosenIndexes(void)
{
	list<int> indexes;
	auto editor_position_copy = editor_position_;
	editor_position_copy.y_ = 0.0f;
	auto start = Vector3(-block_number_x_ * 0.5f * block_size_.x_, 0.0f, block_number_z_ * 0.5f * block_size_.y_);
	int left_up_x = static_cast<int>(((editor_position_.x_ - start.x_) / (block_size_.x_ * static_cast<float>(block_number_x_))) * static_cast<float>(block_number_x_));
	int left_up_z = -static_cast<int>(((editor_position_.z_ - start.z_) / (block_size_.y_ * static_cast<float>(block_number_z_))) * static_cast<float>(block_number_z_));
	int right_down_x = left_up_x + 1;
	int right_down_z = left_up_z + 1;

	int range = static_cast<int>(editor_radius_ / block_size_.x_);
	int min_x = left_up_x - range;
	int max_x = right_down_x + range;
	int min_z = left_up_z - range;
	int max_z = right_down_z + range;
	min_x = min_x < 0 ? 0 : min_x;
	min_z = min_z < 0 ? 0 : min_z;
	max_x = max_x > block_number_x_ ? block_number_x_ : max_x;
	max_z = max_z > block_number_z_ ? block_number_z_ : max_z;

	for (int count_z = min_z; count_z <= max_z; ++count_z)
	{
		for (int count_x = min_x; count_x <= max_x; ++count_x)
		{
			auto index = count_z * (block_number_z_ + 1) + count_x;
			auto position = vertexes_[index].position;
			position.y_ = 0.0f;
			if (Vector3::SquareDistanceBetween(position, editor_position_copy) <= editor_radius_ * editor_radius_)
			{
				indexes.push_back(index);
			}
		}
	}
	return indexes;
}

//--------------------------------------------------------------------------------
//  ShowMainWindow
//--------------------------------------------------------------------------------
void FieldEditor::ShowMainWindow(void)
{
	// Begin
	if (!ImGui::Begin("Field Editor Window"))
	{
		ImGui::End();
		return;
	}

	// Controll
	ImGui::Text("Shrink / Extend : Left / Right");
	ImGui::Text("Raise / Reduce : Up / Down");

	// Radius
	ImGui::Text("Radius : %f", editor_radius_);

	// Raise Speed
	ImGui::InputFloat("Raise Speed", &raise_speed_);

	// Extend Speed
	ImGui::InputFloat("Extend Speed", &extend_speed_);

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  頂点の更新
//--------------------------------------------------------------------------------
void FieldEditor::UpdateVertexesBy(const list<int>& choosenIndexes)
{
	// 前フレーム選択された頂点の色を戻す
	for (int index : previous_choosen_indexes_)
	{
		vertexes_[index].color = Color::kWhite;
	}
	MainSystem::Instance()->GetMeshManager()->Update(L"field", vertexes_, previous_choosen_indexes_);
	
	for (int index : choosenIndexes)
	{
		//Choosen Color
		vertexes_[index].color = Color::kRed;
	
		//法線計算
		int count_z = index / (block_number_z_ + 1);
		int count_x = index - count_z * (block_number_z_ + 1);
		Vector3 normal;
		Vector3 self = vertexes_[index].position;
		Vector3 left;
		Vector3 right;
		Vector3 top;
		Vector3 bottom;
	
		if (count_x == 0)
		{
			left = self;
			right = vertexes_[count_z * (block_number_x_ + 1) + count_x + 1].position;
		}
		else if (count_x < block_number_x_)
		{
			left = vertexes_[count_z * (block_number_x_ + 1) + count_x - 1].position;
			right = vertexes_[count_z * (block_number_x_ + 1) + count_x + 1].position;
		}
		else
		{
			left = vertexes_[count_z * (block_number_x_ + 1) + count_x - 1].position;
			right = self;
		}
	
		if (count_z == 0)
		{
			top = self;
			bottom = vertexes_[(count_z + 1) * (block_number_x_ + 1) + count_x].position;
		}
		else if (count_z < block_number_z_)
		{
			top = vertexes_[(count_z - 1) * (block_number_x_ + 1) + count_x].position;
			bottom = vertexes_[(count_z + 1) * (block_number_x_ + 1) + count_x].position;
		}
		else
		{
			top = vertexes_[(count_z - 1) * (block_number_x_ + 1) + count_x].position;
			bottom = self;
		}
		normal = (right - left) * (bottom - top);
		vertexes_[index].normal = normal.Normalized();
	}
	previous_choosen_indexes_ = choosenIndexes;
}
#endif // _DEBUG
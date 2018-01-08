//--------------------------------------------------------------------------------
//	�v���C���[�r�w�C�r�A�R���|�l���g
//�@FieldEditor.h
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
#include "labels.h"
#include "debug_observer.h"
#include <cereal/archives/binary.hpp>
using namespace cereal;

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
FieldEditor::FieldEditor(GameObject& owner)
	: Behavior(owner, L"FieldEditor")
	, block_number_x_(100)
	, block_number_z_(100)
	, block_size_(Vector2(1.0f))
	, min_position_(Vector3::kZero)
	, max_position_(Vector3::kZero)
	, editor_position_(Vector3::kZero)
	, choose_range_(Vector2::kZero)
	, raise_speed_(1.0f)
	, extend_speed_(1.0f)
	, raise_mode_rate_(0.0f)
    , is_active_(true)
    , current_choose_mode_(kCircle)
{}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
bool FieldEditor::Init(void)
{
	// Vertex
    InitVertexes();
	
	// Index
    vector<int>& indexes = GetInitMeshIndexes();

	MainSystem::Instance()->GetMeshManager()->Use(L"field", DrawType::kTriangleStrip, vertexes_, indexes, (block_number_x_ + 2) * 2 * block_number_z_ - 4);
    return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void FieldEditor::Uninit(void)
{
	vertexes_.clear();
    MainSystem::Instance()->GetMeshManager()->Disuse(L"field");
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void FieldEditor::Update(void)
{
	if (!is_active_) return;

	ShowMainWindow();

	auto input = MainSystem::Instance()->GetInput();

    // �I��͈͓��̃C���f�b�N�X�̎擾
	auto& indexes = GetChoosenIndexes();

	// ���~
    float input_value = static_cast<float>(input->GetKeyPress(Key::kRaise))
		- static_cast<float>(input->GetKeyPress(Key::kReduce));
	float raise_amount = input_value * raise_speed_;

    // ���_�̍X�V
    UpdateVertexesBy(raise_amount, indexes);
	MainSystem::Instance()->GetMeshManager()->Update(L"field", vertexes_, indexes);
}

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
Vector3 FieldEditor::AdjustPositionInField(const Vector3& position, const bool& is_adjust_height)
{
	Vector3 result;

	//�͈͓��ɂ���
	result.x_ = Math::Clamp(position.x_, min_position_.x_, max_position_.x_);
	result.y_ = position.y_;
	result.z_ = Math::Clamp(position.z_, min_position_.z_, max_position_.z_);

	//�����̒���
	if (is_adjust_height)
	{
		result.y_ = GetHeight(result);
	}

	return result;
}

//--------------------------------------------------------------------------------
//	�A�N�e�B�u�t���O�̐ݒ�
//--------------------------------------------------------------------------------
void FieldEditor::SetActive(const bool& value)
{
	is_active_ = value;
	if (!is_active_)
	{// �O�t���[���I�����ꂽ���_�̐F��߂�
        UpdateVertexesBy(0.0f, list<int>());
	}
}

//--------------------------------------------------------------------------------
//	�t�B�[���h����ۑ�����֐�
//--------------------------------------------------------------------------------
void FieldEditor::SaveAsBinary(const String& name)
{
    // �O�t���[���I�����ꂽ���_�̐F��߂�
    UpdateVertexesBy(0.0f, list<int>());

	//�t�B�[���h���b�V���̕ۑ�
	MainSystem::Instance()->GetMeshManager()->SaveMeshToFile(L"field", name);

	//�t�B�[���h�̕ۑ�
	String path = L"data/field/" + name + L".field";
	ofstream file(path, ios::binary);
	if (!file.is_open())
	{
        MessageBox(NULL, L"�J���܂���ł���", path.c_str(), MB_OK | MB_ICONWARNING);
		return;
	}
	BinaryOutputArchive archive(file);

	//�u���b�N���̕ۑ�
	archive.saveBinary(&block_number_x_, sizeof(block_number_x_));
	archive.saveBinary(&block_number_z_, sizeof(block_number_z_));

	//�u���b�NSize�̕ۑ�
	archive.saveBinary(&block_size_, sizeof(block_size_));

	//���_�f�[�^���̕ۑ�
	size_t vertex_number = vertexes_.size();
	archive.saveBinary(&vertex_number, sizeof(vertex_number));

	//���_�f�[�^�̕ۑ�
	for (size_t count = 0; count < vertex_number; ++count)
	{
		archive.saveBinary(&vertexes_[count].position, sizeof(vertexes_[count].position));
	}

	file.close();

    MessageBox(NULL, L"�Z�[�u���܂���", path.c_str(), MB_OK | MB_ICONWARNING);
}

//--------------------------------------------------------------------------------
//  �t�B�[���h����Ǎ��֐�
//--------------------------------------------------------------------------------
void FieldEditor::LoadFrom(const String& name)
{
    //�t�B�[���h�̓Ǎ�
    String path = L"data/field/" + name + L".field";
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        MessageBox(NULL, L"�J���܂���ł���", path.c_str(), MB_OK | MB_ICONWARNING);
        return;
    }
    BinaryInputArchive archive(file);

    //���̒��_����j������
    MainSystem::Instance()->GetMeshManager()->Disuse(L"field");
    previous_choosen_indexes_.clear();
    vertexes_.clear();

    //�u���b�N���̓Ǎ�
    archive.loadBinary(&block_number_x_, sizeof(block_number_x_));
    archive.loadBinary(&block_number_z_, sizeof(block_number_z_));

    //�u���b�NSize�̓Ǎ�
    archive.loadBinary(&block_size_, sizeof(block_size_));

    //���_�f�[�^���̓Ǎ�
    size_t vertex_number;
    archive.loadBinary(&vertex_number, sizeof(vertex_number));

    //���_�ʒu�̓Ǎ�
    InitVertexes();
    for (size_t count = 0; count < vertex_number; ++count)
    {
        archive.loadBinary(&vertexes_[count].position, sizeof(vertexes_[count].position));
    }

    //�@���̍X�V
    RecalculateNormal(GetAllIndexes());

    //���b�V���C���f�b�N�X
    vector<int>& mesh_indexes = GetInitMeshIndexes();

    MainSystem::Instance()->GetMeshManager()->Use(L"field", DrawType::kTriangleStrip, vertexes_, mesh_indexes, (block_number_x_ + 2) * 2 * block_number_z_ - 4);
    file.close();

    MessageBox(NULL, L"���[�h���܂���", path.c_str(), MB_OK | MB_ICONWARNING);
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �t�B�[���h�̍������擾
//--------------------------------------------------------------------------------
float FieldEditor::GetHeight(const Vector3& position)
{
    Vector3 start(-block_number_x_ * 0.5f * block_size_.x_, 0.0f, block_number_z_ * 0.5f * block_size_.y_);
	int left_up_x = static_cast<int>(((position.x_ - start.x_) / (block_size_.x_ * static_cast<float>(block_number_x_))) * static_cast<float>(block_number_x_));
	int left_up_z = -static_cast<int>(((position.z_ - start.z_) / (block_size_.y_ * static_cast<float>(block_number_z_))) * static_cast<float>(block_number_z_));

	//�t�B�[���h�͈̔͊O�������珈���I��
	if (left_up_x < 0 || left_up_x >= block_number_x_ || left_up_z < 0 || left_up_z >= block_number_z_)
	{
		return 0.0f;
	}

	int right_down_x = left_up_x + 1;
	int right_down_z = left_up_z + 1;

    Vector3 target_position(position.x_, 0.0f, position.z_);
	Vector3& left_up_position = vertexes_[left_up_z * (block_number_x_ + 1) + left_up_x].position;
	Vector3& right_down_position = vertexes_[right_down_z * (block_number_x_ + 1) + right_down_x].position;

	//Check Side
	Vector3& midline = right_down_position - left_up_position;
	Vector3& left_up_to_target = target_position - left_up_position;
	Vector3& cross = left_up_to_target * midline;
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
	Vector3& side_position = vertexes_[side_z * (block_number_x_ + 1) + side_x].position;
	Vector3& side_to_left_up = left_up_position - side_position;
	Vector3& side_to_right_down = right_down_position - side_position;
	Vector3& normal = ((side_to_left_up * side_to_right_down) * sign).Normalized();
	return side_position.y_ - ((position.x_ - side_position.x_) * normal.x_ + (position.z_ - side_position.z_) * normal.z_) / normal.y_;
}

//--------------------------------------------------------------------------------
//  �͈͓��̃C���f�b�N�X���擾
//--------------------------------------------------------------------------------
list<int> FieldEditor::GetChoosenIndexes(void)
{
	list<int> indexes;
	Vector3 editor_position_copy = editor_position_;
	editor_position_copy.y_ = 0.0f;
    Vector3 start(-block_number_x_ * 0.5f * block_size_.x_, 0.0f, block_number_z_ * 0.5f * block_size_.y_);
	int left_up_x = static_cast<int>(((editor_position_.x_ - start.x_) / (block_size_.x_ * static_cast<float>(block_number_x_))) * static_cast<float>(block_number_x_));
	int left_up_z = -static_cast<int>(((editor_position_.z_ - start.z_) / (block_size_.y_ * static_cast<float>(block_number_z_))) * static_cast<float>(block_number_z_));
	int right_down_x = left_up_x + 1;
	int right_down_z = left_up_z + 1;

    int range_x = 1 + static_cast<int>(choose_range_.x_ / block_size_.x_);
    int range_z = 1 + static_cast<int>(choose_range_.y_ / block_size_.y_);
	int min_x = left_up_x - range_x;
	int max_x = right_down_x + range_x;
	int min_z = left_up_z - range_z;
	int max_z = right_down_z + range_z;
	min_x = min_x < 0 ? 0 : min_x;
	min_z = min_z < 0 ? 0 : min_z;
	max_x = max_x > block_number_x_ ? block_number_x_ : max_x;
	max_z = max_z > block_number_z_ ? block_number_z_ : max_z;

    if (current_choose_mode_ == kCircle)
    {// �~�`
        for (int count_z = min_z; count_z <= max_z; ++count_z)
        {
            for (int count_x = min_x; count_x <= max_x; ++count_x)
            {
                int index = count_z * (block_number_z_ + 1) + count_x;
                Vector3 position = vertexes_[index].position;
                position.y_ = 0.0f;
                if (Vector3::SquareDistanceBetween(position, editor_position_copy) <= choose_range_.x_ * choose_range_.x_)
                {
                    indexes.push_back(index);
                }
            }
        }
    }
    else
    {// ��`
        for (int count_z = min_z; count_z <= max_z; ++count_z)
        {
            for (int count_x = min_x; count_x <= max_x; ++count_x)
            {
                int index = count_z * (block_number_z_ + 1) + count_x;
                float delta_x = vertexes_[index].position.x_ - editor_position_copy.x_;
                float delta_z = vertexes_[index].position.z_ - editor_position_copy.z_;
                if (delta_x * delta_x <= choose_range_.x_ * choose_range_.x_
                    && delta_z * delta_z <= choose_range_.y_ * choose_range_.y_)
                {
                    indexes.push_back(index);
                }
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
	if (!ImGui::Begin("Field editor window"))
	{
		ImGui::End();
		return;
	}

    auto& current_language = MainSystem::Instance()->GetDebugObserver()->GetCurrentLanguage();

	// �������
	ImGui::Text(kExplainRaiseReduce[current_language]);

    // �N����ԃ��[�h���ςƐ��`�̔䗦
    ImGui::SliderFloat(kRaiseModeRate[current_language], &raise_mode_rate_, 0.0f, 1.0f);

	// �N�����x
	ImGui::InputFloat(kFieldRaiseSpeed[current_language], &raise_speed_);

    // �g�k�I�����[�h
    ImGui::ListBox(kFieldChooseMode[current_language], (int*)(&current_choose_mode_), kFieldChooseModeLabels[current_language], kChooseModeMax);

    // �g�k���x
    ImGui::InputFloat(kFieldExtendSpeed[current_language], &extend_speed_);

    // �g�k�͈�
    if (current_choose_mode_ == kCircle)
    {
        ImGui::DragFloat(kFieldRadius[current_language], &choose_range_.x_, extend_speed_, 0.0f, FLT_MAX);
        choose_range_.y_ = choose_range_.x_;
    }
    else
    {
        ImGui::DragFloat2(kFieldRange[current_language], &choose_range_.x_, extend_speed_, 0.0f, FLT_MAX);
    }

    // �u���b�N�T�C�Y
    if (ImGui::InputFloat2(kBlockSize[current_language], &block_size_.x_))
    {
        list<int>& indexes = GetAllIndexes();
        RecalculateVertexes();
        RecalculateNormal(indexes);
        MainSystem::Instance()->GetMeshManager()->Update(L"field", vertexes_, indexes);
    }

    // ����������������
    if (ImGui::Button(kResetHeight[current_language]))
    {
        list<int>& indexes = GetChoosenIndexes();
        SetHeightTo(0.0f, indexes);
        RecalculateNormal(indexes);
        MainSystem::Instance()->GetMeshManager()->Update(L"field", vertexes_, indexes);
    }

	// End
	ImGui::End();
}

//--------------------------------------------------------------------------------
//  ���_�̍X�V
//--------------------------------------------------------------------------------
void FieldEditor::UpdateVertexesBy(const float& raise_amount, const list<int>& choosen_indexes)
{
    // �O�t���[���I�����ꂽ���_�̐F��߂�
    for (int index : previous_choosen_indexes_)
    {
        vertexes_[index].color = Color::kWhite;
    }
    MainSystem::Instance()->GetMeshManager()->Update(L"field", vertexes_, previous_choosen_indexes_);

    // �N���ƐF�̍X�V
    if (current_choose_mode_ == kCircle)
    {
        for (int index : choosen_indexes)
        {
            float rate = choose_range_.x_ == 0.0f ? 1.0f :
                (choose_range_.x_ - Vector2(editor_position_.x_ - vertexes_[index].position.x_, editor_position_.z_ - vertexes_[index].position.z_).Magnitude()) / choose_range_.x_;

            if (rate >= raise_mode_rate_)
            {// ���ϓI�ɋN������
                vertexes_[index].position.y_ += raise_amount;
                vertexes_[index].color = Color::kRed;
            }
            else
            {// �����ɂ���Đ��`���
                vertexes_[index].position.y_ += raise_amount * (rate / raise_mode_rate_);
                vertexes_[index].color = Color(rate, 0.0f, 1.0f - rate, 1.0f);
            }
        }
    }
    else
    {
        for (int index : choosen_indexes)
        {
            float rate_x = choose_range_.x_ == 0.0f ? 1.0f :
                (choose_range_.x_ - fabsf(editor_position_.x_ - vertexes_[index].position.x_)) / choose_range_.x_;
            float rate_z = choose_range_.y_ == 0.0f ? 1.0f :
                (choose_range_.y_ - fabsf(editor_position_.z_ - vertexes_[index].position.z_)) / choose_range_.y_;
            float rate = min(rate_x, rate_z);

            if (rate >= raise_mode_rate_)
            {// ���ϓI�ɋN������
                vertexes_[index].position.y_ += raise_amount;
                vertexes_[index].color = Color::kRed;
            }
            else
            {// �����ɂ���Đ��`���
                vertexes_[index].position.y_ += raise_amount * (rate / raise_mode_rate_);
                vertexes_[index].color = Color(rate, 0.0f, 1.0f - rate, 1.0f);
            }
        }
    }
    
    // �@���v�Z
    if (raise_amount > 0.0f)
    {
        RecalculateNormal(choosen_indexes);
    }

    previous_choosen_indexes_ = choosen_indexes;
}

//--------------------------------------------------------------------------------
//  ���_�̏�����
//--------------------------------------------------------------------------------
void FieldEditor::InitVertexes(void)
{
    Vector3 start(-block_number_x_ * 0.5f * block_size_.x_, 0.0f, block_number_z_ * 0.5f * block_size_.y_);
    vertexes_.resize((block_number_x_ + 1) * (block_number_z_ + 1));
    for (int count_z = 0; count_z < block_number_z_ + 1; ++count_z)
    {
        for (int count_x = 0; count_x < block_number_x_ + 1; ++count_x)
        {
            int index = count_z * (block_number_x_ + 1) + count_x;
            Vector3& position = start
                + Vector3(count_x * block_size_.x_, 0.0f, -count_z * block_size_.y_);
            vertexes_[index].position = position;
            vertexes_[index].uv = Vector2(count_x * 1.0f / static_cast<float>(block_number_x_)
                , count_z * 1.0f / static_cast<float>(block_number_z_));
            vertexes_[index].color = Color::kWhite;
            vertexes_[index].normal = Vector3::kUp;
        }
    }

    Vector3 half_size(block_number_x_ * 0.5f * block_size_.x_, 0.0f, block_number_z_ * 0.5f * block_size_.y_);
    min_position_ = half_size * -1.0f;
    max_position_ = half_size;
}

//--------------------------------------------------------------------------------
//  �������C���f�b�N�X�̎擾
//--------------------------------------------------------------------------------
vector<int> FieldEditor::GetInitMeshIndexes(void)
{
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
            else if (count_z * ((block_number_x_ + 1) * 2 + 2) + count_x < (((block_number_x_ + 1) * 2 + 2) * block_number_z_ - 1))//�k�ރ|���S��
            {
                indexes[count_z * ((block_number_x_ + 1) * 2 + 2) + count_x] = (count_x - 1) / 2 % (block_number_x_ + 1) + count_x % 2 * 2 * (block_number_x_ + 1) + count_z * (block_number_x_ + 1);
            }
        }
    }
    return indexes;
}

//--------------------------------------------------------------------------------
//  ���_�̍Čv�Z
//--------------------------------------------------------------------------------
void FieldEditor::RecalculateVertexes(void)
{
    Vector3 start(-block_number_x_ * 0.5f * block_size_.x_, 0.0f, block_number_z_ * 0.5f * block_size_.y_);
    for (int count_z = 0; count_z < block_number_z_ + 1; ++count_z)
    {
        for (int count_x = 0; count_x < block_number_x_ + 1; ++count_x)
        {
            int index = count_z * (block_number_x_ + 1) + count_x;
            Vector3& position = start
                + Vector3(count_x * block_size_.x_, vertexes_[index].position.y_, -count_z * block_size_.y_);
            vertexes_[index].position = position;
        }
    }

    Vector3 half_size(block_number_x_ * 0.5f * block_size_.x_, 0.0f, block_number_z_ * 0.5f * block_size_.y_);
    min_position_ = half_size * -1.0f;
    max_position_ = half_size;
}

//--------------------------------------------------------------------------------
//  �@���̌v�Z
//--------------------------------------------------------------------------------
void FieldEditor::RecalculateNormal(const list<int>& indexes)
{
    for (int index : indexes)
    {
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
}

//--------------------------------------------------------------------------------
//  �S���̃C���f�b�N�X���擾
//--------------------------------------------------------------------------------
list<int> FieldEditor::GetAllIndexes(void)
{
    list<int> indexes;
    int index_max = (block_number_x_ + 1) * (block_number_z_ + 1);
    for (int count = 0; count < index_max; ++count)
    {
        indexes.push_back(count);
    }
    return indexes;
}

//--------------------------------------------------------------------------------
//  ���_����荂���ɃZ�b�g����
//--------------------------------------------------------------------------------
void FieldEditor::SetHeightTo(const float& height, const list<int>& indexes)
{
    for (int index : indexes)
    {
        vertexes_[index].position.y_ = height;
    }
}

#endif // _DEBUG
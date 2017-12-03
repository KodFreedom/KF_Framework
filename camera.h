//--------------------------------------------------------------------------------
//�@camera.h
//	�J�������N���X
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  �J�������N���X
//--------------------------------------------------------------------------------
class Camera
{
#ifdef _DEBUG
	friend class DebugObserver;
#endif // _DEBUG
	friend class CameraManager;
public:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static constexpr float kDefaultfar_ = 1000.0f;

	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	Camera();
	~Camera() {}

	//--------------------------------------------------------------------------------
	//  ����������
	//--------------------------------------------------------------------------------
	virtual void Init(void);

	//--------------------------------------------------------------------------------
	//  �X�V����
	//--------------------------------------------------------------------------------
	virtual void Update(void) {}

	//--------------------------------------------------------------------------------
	//  ��X�V����
	//--------------------------------------------------------------------------------
	virtual void LateUpdate(void)
	{
		UpdateParameter();
	}

	//--------------------------------------------------------------------------------
	//  �j���֐�
	//--------------------------------------------------------------------------------
	virtual void Release(void)
	{
		Uninit();
		delete this;
	}

	//--------------------------------------------------------------------------------
	//  �Z�b�g����(�`�撼�O)
	//--------------------------------------------------------------------------------
	void Set(void);

	//--------------------------------------------------------------------------------
	//  �J�����͈͓̔��ɓ����Ă邩�ǂ���
	//  position : �����̈ʒu
	//  radius : ���蔼�a
	//--------------------------------------------------------------------------------
	bool IsInRange(const Vector3& position, const float& radius = kDefaultfar_);
	
	//--------------------------------------------------------------------------------
	//  �ړ�����
	//  movement : �ړ���
	//--------------------------------------------------------------------------------
	void Move(const Vector3& movement)
	{
		rig_.position += movement;
	}

	const auto& GetWorldRight(void) const { return world_right_; }
	const auto& GetWorldUp(void) const { return world_up_; }
	const auto& GetWorldForward(void) const { return world_forward_; }
	const auto& GetWorldEyePosition(void) const { return world_eye_position_; }
	const auto& GetWorldAtPosition(void) const { return world_at_position_; }
	const auto& GetView(void) const { return view_; }
	const auto& GetViewTranspose(void) const { return view_transpose_; }
	const auto& GetProjection(void) const { return projection_; }
	const auto& GetFar(void) const { return far_; }
	virtual void SetDistance(const float& value)
	{
		distance_ = value;
		local_eye_position_.z_ = -distance_;
	}
	void SetWorldPosition(const Vector3& value) { rig_.position = value; }
	void SetOffsetY(const float& value) { pivot_.position.y_ = value; }
	void SetPitch(const float& radian) { pivot_.rotation.x_ = radian; }
	void SetYaw(const float& radian) { rig_.rotation.y_ = radian; }
	void SetFov(const float& radian) { fov_ = radian; }
	void SetNear(const float& value) { near_ = value; }
	void SetFar(const float& value) { far_ = value; }

protected:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct CameraTransform
	{
		Vector3	position;
		Vector3	rotation;
	};

	
	virtual void Uninit(void) {}

	//--------------------------------------------------------------------------------
	//  X����]
	//--------------------------------------------------------------------------------
	virtual void Pitch(const float& radian)
	{
		pivot_.rotation.x_ += radian;
	}

	//--------------------------------------------------------------------------------
	//  Y����]
	//--------------------------------------------------------------------------------
	virtual void Yaw(const float& radian)
	{
		rig_.rotation.y_ += radian;
	}

	//--------------------------------------------------------------------------------
	//  �p�����[�^�[�̎Z�o
	//--------------------------------------------------------------------------------
	void UpdateParameter(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	CameraTransform rig_;
	CameraTransform pivot_;
	Vector3         local_eye_position_;
	Vector3         world_eye_position_;
	Vector3         world_at_position_;
	Vector3         world_right_;
	Vector3         world_up_;
	Vector3         world_forward_;
	Matrix44        view_;
	Matrix44        view_transpose_;
	Matrix44        projection_;
	float           distance_;
	float           fov_;
	float           near_;
	float           far_;
};
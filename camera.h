//--------------------------------------------------------------------------------
//	�J�����N���X
//�@Camera.h
//	Author : Xu Wenjie
//	Date   : 2016-05-31
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class Camera
{
#ifdef _DEBUG
	friend class DebugObserver;
#endif // _DEBUG
	friend class CameraManager;
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	virtual void Init(void) {}
	virtual void Update(void) {}
	virtual void LateUpdate(void);
	virtual void Release(void)
	{
		uninit();
		delete this;
	}

	void		 Set(void);
	bool		 IsInRange(const Vector3& position);
	void		 Move(const Vector3& movement);
	auto		 GetRight(void) const { return worldRight; }
	auto		 GetUp(void) const { return worldUp; }
	auto		 GetForward(void) const { return worldForward; }
	auto		 GetPositionEye(void) const { return worldPositionEye; }
	auto		 GetPositionAt(void) const { return worldPositionAt; }
	auto		 GetViewTranspose(void) const { return viewTranspose; }
	auto		 GetFar(void) const { return farZ; }
	virtual void SetDistance(const float& value)
	{
		distance = value;
		localPositionEye.Z = -distance;
	}
	void		 SetPosition(const Vector3& value) { rig.Position = value; }
	void		 SetOffsetY(const float& value) { pivot.Position.Y = value; }
	void		 SetPitch(const float& radian) { pivot.Rotation.X = radian; }
	void		 SetYaw(const float& radian) { rig.Rotation.Y = radian; }
	void		 SetFovY(const float& radian) { fovY = radian; }
	void		 SetNearZ(const float& value) { nearZ = value; }
	void		 SetFarZ(const float& value) { farZ = value; }

protected:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct CameraTransform
	{
		Vector3	Position;
		Vector3	Rotation;
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	Camera();
	~Camera() {}
	virtual void uninit(void) {}
	virtual void pitch(const float& radian);	
	virtual void yaw(const float& radian);
	//virtual void normalize(void);
	void updateParamater(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	CameraTransform rig;
	CameraTransform pivot;
	Vector3			localPositionEye;
	Vector3			worldPositionEye;
	Vector3			worldPositionAt;
	Vector3			worldRight;
	Vector3			worldUp;
	Vector3			worldForward;
	Matrix44		view;
	Matrix44		viewTranspose;
	float			distance;
	float			fovY;
	float			nearZ;
	float			farZ;
};

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class NormalCamera : public Camera
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	NormalCamera();
	~NormalCamera() {}
};
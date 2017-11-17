//--------------------------------------------------------------------------------
//	�O�l�̃J�����N���X
//�@ThirdPersionCamera.h
//	Author : Xu Wenjie
//	Date   : 2017-06-20
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "camera.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class GameObject;

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class ThirdPersionCamera : public Camera
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	ThirdPersionCamera();
	~ThirdPersionCamera() {}
	
	void Init(void) override;
	void Update(void) override;
	void LateUpdate(void) override;
	void SetFollowTarget(GameObject* target) { followTarget = target; }

private:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static const float rotationSpeed;
	static const float startRotationMin;
	static const float rotationLerpTime;
	static const float distanceDefault;
	static const float distanceMin;
	static const float distanceMax;
	static const float moveLerpTime;
	static const float pitchDefault;
	static const float pitchMin;
	static const float pitchMax;
	static const float collisionRadius;
	static const float offsetY;

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void pitch(const float& radian) override;

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	GameObject*		followTarget;	
	float			pitchSpeed;
	float			yawSpeed;
};
//--------------------------------------------------------------------------------
//	三人称カメラクラス
//　ThirdPersionCamera.h
//	Author : Xu Wenjie
//	Date   : 2017-06-20
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "camera.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class GameObject;

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class ThirdPersionCamera : public Camera
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	ThirdPersionCamera();
	~ThirdPersionCamera() {}
	
	void Init(void) override;
	void Update(void) override;
	void LateUpdate(void) override;
	void SetFollowTarget(GameObject* target) { followTarget = target; }

private:
	//--------------------------------------------------------------------------------
	//  定数定義
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
	//  関数定義
	//--------------------------------------------------------------------------------
	void pitch(const float& radian) override;

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	GameObject*		followTarget;	
	float			pitchSpeed;
	float			yawSpeed;
};
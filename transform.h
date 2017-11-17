//--------------------------------------------------------------------------------
//	トランスフォームコンポネント
//　transform.h
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "component.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class Transform : public Component
{
#if defined(_DEBUG) || defined(EDITOR)
	friend class DebugObserver;
#endif // _DEBUG

public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	Transform(GameObject* const owner);
	~Transform() {}
	
	bool		Init(void) override { return true; }
	void		Uninit(void) override {}
	void		SwapParamater(void);
	void		UpdateMatrix(void);	
	void		UpdateMatrix(const Matrix44& parent);
	
	//親子関係
	void		RegisterChild(Transform* value) { children.push_back(value); }
	void		DeregisterChild(Transform* value) { children.remove(value); }
	void		RegisterParent(Transform* value, const Vector3& offsetPosition = Vector3::Zero, const Vector3& offsetRotation = Vector3::Zero);

	//Get関数
	auto		GetCurrentPosition(void) const { return currentPosition; }
	auto		GetCurrentRotation(void) const
	{
		return GetCurrentRotationMatrix().ToQuaternion();
	}
	auto		GetCurrentEulerRotation(void) const
	{
		return GetCurrentRotationMatrix().ToEular();
	}
	auto		GetCurrentScale(void) const { return currentScale; }
	auto		GetCurrentForward(void) const { return currentForward; }
	auto		GetCurrentUp(void) const { return currentUp; }
	auto		GetCurrentRight(void) const { return currentRight; }
	Matrix44	GetCurrentRotationMatrix(void) const
	{
		return Matrix44::Rotation(currentRight, currentUp, currentForward);
	}
	auto		GetCurrentWorldMatrix(void) const { return currentWorldMatrix; }
	auto		GetNextPosition(void) const { return nextPosition; }
	auto		GetNextRotation(void) const
	{
		return GetNextRotationMatrix().ToQuaternion();
	}
	auto		GetNextScale(void) const { return nextScale; }
	auto		GetNextForward(void) const { return nextForward; }
	auto		GetNextUp(void) const { return nextUp; }
	auto		GetNextRight(void) const { return nextRight; }
	Matrix44	GetNextRotationMatrix(void) const
	{
		return Matrix44::Rotation(nextRight, nextUp, nextForward);
	}
	Matrix44	GetNextWorldMatrix(void) const;
	auto		GetParent(void) const { return parent; }
	auto		GetChildren(void) const { return children; }

	//Set関数
	void		SetNextPosition(const Vector3& value) { nextPosition = value; }
	void		SetNextRotation(const Quaternion& value);
	void		SetNextRotation(const Vector3& euler);
	void		SetNextScale(const Vector3& value) { nextScale = value; }
	void		SetNextForward(const Vector3& value) { nextForward = value; }
	void		SetNextUp(const Vector3& value) { nextUp = value; }
	void		SetNextRight(const Vector3& value) { nextRight = value; }
	void		SetNextMatrix(const Matrix44& value);
	void		SetOffset(const Vector3& position, const Vector3& rotation);

	//回転関数
	void		RotateByEuler(const Vector3& euler);
	void		RotateByPitch(const float& radian);
	void		RotateByYaw(const float& radian);
	void		RotateByRoll(const float& radian);
	void		RotateByUp(const Vector3& up);
	void		RotateByForward(const Vector3& forward);
	void		RotateByRight(const Vector3& right);

	//他の関数
	Vector3		TransformDirectionToLocal(const Vector3& direction);

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void		calculateWorldMatrix(void);
	void		calculateWorldMatrix(const Matrix44& parent);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	//パラメーター
	Vector3		currentPosition;
	Vector3		currentScale;
	Vector3		currentForward;
	Vector3		currentUp;
	Vector3		currentRight;
	Matrix44	currentWorldMatrix;
	Vector3		nextPosition;
	Vector3		nextScale;
	Vector3		nextForward;
	Vector3		nextUp;
	Vector3		nextRight;
	
	
	//親子関係
	Transform*			parent;
	list<Transform*>	children;
	Matrix44			offset;
};
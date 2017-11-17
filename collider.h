//--------------------------------------------------------------------------------
//	colliderコンポネント
//　collider.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "component.h"
#include "collisionSystem.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class Collider : public Component
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	Collider(GameObject* const owner, const ColliderType& type, const ColliderMode& mode);
	~Collider() {}

	virtual bool	Init(void) override;
	virtual void	Uninit(void) override;
	virtual void	Update(void);
	void			Sleep(void) override;
	void			Awake(void) override;
	
	// Set関数
	void			SetTrigger(const bool& value) { isTrigger = value; }
	void			SetTag(const string& value) { tag = value; }
	void			SetOffset(const Vector3& position, const Vector3& rotation = Vector3::Zero);

	// Get関数
	Vector3			GetLocalPosition(void) const { return Vector3(offset.Elements[3][0], offset.Elements[3][1], offset.Elements[3][2]); }
	Vector3			GetNextWorldPosition(void) const { return Vector3(nextWorldMatrix.Elements[3][0], nextWorldMatrix.Elements[3][1], nextWorldMatrix.Elements[3][2]); }
	Matrix44		GetNextWorldMatrix(void) const { return nextWorldMatrix; }
	Matrix44		GetOffset(void) const { return offset; }
	const auto		GetType(void) const { return type; }
	const auto		GetMode(void) const { return mode; }
	const bool		IsTrigger(void) const { return isTrigger; }
	const string&	GetTag(void) const { return tag; }

protected:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	Collider() : Component() , type(CT_Max), mode(CM_Max) {}

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	ColliderType	type;
	ColliderMode	mode;
	bool			isTrigger;
	bool			isRegistered;
	Matrix44		offset;
	Matrix44		nextWorldMatrix;	// ワールド行列(処理速度向上のため)
	string			tag;
};
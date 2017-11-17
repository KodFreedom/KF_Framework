//--------------------------------------------------------------------------------
//	生き物ステートマシン
//　ActorState.h
//	Author : Xu Wenjie
//	Date   : 2017-11-07
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class ActorController;
class Animator;
class Transform;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class ActorState
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	ActorState(const string& name, const float& movementMultiplyer = 1.0f)
		: name(name)
		, movementMultiplyer(movementMultiplyer)
		, groundCheckDistance(0.1f) 
	{
		currentGroundState.Normal = Vector3::Up;
		currentGroundState.IsGrounded = false;
	}
	~ActorState() {}

	virtual void Init(ActorController& actor) = 0;
	virtual void Act(ActorController& actor) = 0;
	virtual void OnDamaged(ActorController& actor, const float& damage);

protected:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct GroundInfo
	{
		Vector3 Normal;
		bool	IsGrounded;
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	virtual void move(ActorController& actor);
	virtual void jump(ActorController& actor);
	void checkGrounded(const ActorController& actor);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	const string	name;
	float			movementMultiplyer;
	float			groundCheckDistance;
	GroundInfo		currentGroundState;
};
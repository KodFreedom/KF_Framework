//--------------------------------------------------------------------------------
//	�������X�e�[�g�}�V��
//�@ActorState.h
//	Author : Xu Wenjie
//	Date   : 2017-11-07
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class ActorController;
class Animator;
class Transform;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class ActorState
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
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
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct GroundInfo
	{
		Vector3 Normal;
		bool	IsGrounded;
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	virtual void move(ActorController& actor);
	virtual void jump(ActorController& actor);
	void checkGrounded(const ActorController& actor);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	const string	name;
	float			movementMultiplyer;
	float			groundCheckDistance;
	GroundInfo		currentGroundState;
};
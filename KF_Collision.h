//--------------------------------------------------------------------------------
//	�R���W��������N���X
//�@KF_Collision.h
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CSphereColliderComponent;
class CFieldColliderComponent;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CKFCollision
{
public:
	static void CheckCollisionSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR);
	static void CheckCollisionSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field);

private:
	CKFCollision() {}
	~CKFCollision() {}
};
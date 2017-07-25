//--------------------------------------------------------------------------------
//	�R���W��������N���X
//�@KF_CollisionSystem.h
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
class CColliderComponent;
class CSphereColliderComponent;
class CFieldColliderComponent;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���W�������
//--------------------------------------------------------------------------------
//class CCollisionInfo
//{
//public:
//	CCollisionInfo(const list<CKFVec3>& listPoints, CColliderComponent& collider)
//		: m_listPoints(listPoints), m_collider(collider) {}
//	~CCollisionInfo() {}
//
//	list<CKFVec3>		m_listPoints;	//���������̒��_
//	CColliderComponent&	m_collider;		//����̃R���C�_�[
//};

//--------------------------------------------------------------------------------
//  �R���W��������V�X�e��
//--------------------------------------------------------------------------------
class CKFCollisionSystem
{
public:
	static void CheckCollisionSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR);
	static void CheckCollisionSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field);

private:
	CKFCollisionSystem() {}
	~CKFCollisionSystem() {}
};
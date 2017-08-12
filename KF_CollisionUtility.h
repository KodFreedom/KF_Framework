//--------------------------------------------------------------------------------
//	�R���W�����v�Z�֐�
//�@KF_CollisionUtility.h
//	Author : Xu Wenjie
//	Date   : 2017-08-13
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class C3DRigidbodyComponent;
class CColliderComponent;
class CBoxColliderComponent;
class CSphereColliderComponent;
class CAABBColliderComponent;
class COBBColliderComponent;
class CFieldColliderComponent;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���W����
//--------------------------------------------------------------------------------
class CCollision
{
public:
	CCollision()
		: m_pRigidBodyOne(nullptr)
		, m_pRigidBodyTwo(nullptr)
		, m_vCollisionNormal(CKFVec3(0.0f))
		, m_vCollisionPos(CKFVec3(0.0f))
		, m_fPenetration(0.0f)
	{}
	~CCollision() {}

	C3DRigidbodyComponent*	m_pRigidBodyOne;
	C3DRigidbodyComponent*	m_pRigidBodyTwo;
	CKFVec3					m_vCollisionNormal;
	CKFVec3					m_vCollisionPos;
	float					m_fPenetration;
	CKFMtx44				m_mtxToWorld;
};

//--------------------------------------------------------------------------------
//  �R���W�������
//--------------------------------------------------------------------------------
class CCollisionInfo
{
public:
	CCollisionInfo()
		: m_pColliderThis(nullptr)
		, m_pCollider(nullptr)
	{
		m_listCollision.clear();
	}
	~CCollisionInfo()
	{
		m_listCollision.clear();
	}

	CColliderComponent*		m_pColliderThis;
	CColliderComponent*		m_pCollider;
	list<CCollision*>		m_listCollision;
};

//--------------------------------------------------------------------------------
//  �R���W�����e�X�g
//--------------------------------------------------------------------------------
class CCollisionTest
{
public:


private:
	CCollisionTest() {}
	~CCollisionTest() {}
};

//--------------------------------------------------------------------------------
//  �R���W�����v�Z�N���X
//--------------------------------------------------------------------------------
class CCollisionDetector
{
public:
	//�Փ˔���֐�
	static void	CheckSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR);
	static void	CheckSphereWithAABB(CSphereColliderComponent& sphere, CAABBColliderComponent& aabb);
	static void	CheckSphereWithOBB(CSphereColliderComponent& sphere, COBBColliderComponent& obb);
	static void	CheckAABBWithAABB(CAABBColliderComponent& aabbL, CAABBColliderComponent& aabbR);
	static void	CheckBoxWithBox(CBoxColliderComponent& boxL, CBoxColliderComponent& boxR);
	
	//Field
	static void	CheckBoxWithField(CBoxColliderComponent& box, CFieldColliderComponent& field);
	static void	CheckSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field);

private:
	CCollisionDetector() {}
	~CCollisionDetector() {}

	//�v�Z�p�֐�
	static bool		checkPointWithAABB(CCollision& collisionOut, const CKFVec3 vPoint, const CAABBColliderComponent& aabb);
	static bool		checkPointWithBox(CCollision& collisionOut, const CKFVec3 vPoint, const CBoxColliderComponent& box);

	static float	transformBoxToAxis(const CBoxColliderComponent& box, const CKFVec3& vAxis);
	static bool		checkOverlapOnAxis(const CBoxColliderComponent& boxL, const CBoxColliderComponent& boxR, const CKFVec3& vAxis);
	static bool		checkOverlapOnAxis(const CKFVec2& vMinL, const CKFVec2& vMaxL, const CKFVec2& vMinR, const CKFVec2& vMaxR);
	static bool		checkOverlapAABB(const CAABBColliderComponent& aabbL, const CAABBColliderComponent& aabbR);

};
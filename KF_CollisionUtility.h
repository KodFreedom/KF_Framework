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
		, m_vCollisionNormal(Vector3(0.0f))
		, m_vCollisionPos(Vector3(0.0f))
		, m_fPenetration(0.0f)
	{}
	~CCollision() {}

	C3DRigidbodyComponent*	m_pRigidBodyOne;
	C3DRigidbodyComponent*	m_pRigidBodyTwo;
	Vector3					m_vCollisionNormal;
	Vector3					m_vCollisionPos;
	float					m_fPenetration;
	Matrix44				m_mtxToWorld;
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
//  ���C�Փˏ��(�ŋߓ_)
//--------------------------------------------------------------------------------
class CRaycastHitInfo
{
public:
	CRaycastHitInfo()
		: m_pCollider(nullptr)
		, m_fDistance(0.0f)
		, m_Normal(Vector3(0.0f))
		, m_Position(Vector3(0.0f))
	{}
	~CRaycastHitInfo() {}

	CColliderComponent*		m_pCollider;	//�Փˑ���̃R���C�_�[
	float					m_fDistance;	//���C�̎n�_�ƏՓ˓_�Ƃ̋���
	Vector3					m_Normal;		//�Փ˓_�̖ʖ@��
	Vector3					m_Position;			//�Փ˓_�̈ʒu
};

//--------------------------------------------------------------------------------
//  �R���W�����v�Z�N���X
//--------------------------------------------------------------------------------
class CCollisionDetector
{
public:
	//�Փ˔���֐�
	static void		CheckSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR);
	static void		CheckSphereWithAABB(CSphereColliderComponent& sphere, CAABBColliderComponent& aabb);
	static void		CheckSphereWithOBB(CSphereColliderComponent& sphere, COBBColliderComponent& obb);
	static void		CheckAABBWithAABB(CAABBColliderComponent& aabbL, CAABBColliderComponent& aabbR);
	static void		CheckBoxWithBox(CBoxColliderComponent& boxL, CBoxColliderComponent& boxR);
	
	//Field
	static void		CheckBoxWithField(CBoxColliderComponent& box, CFieldColliderComponent& field);
	static void		CheckSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field);
	
	//Ray
	static bool		CheckRayWithBox(const Ray& ray, const float& fDistance, CBoxColliderComponent& box, CRaycastHitInfo& infoOut);
	static bool		CheckRayWithSphere(const Ray& ray, const float& fDistance, CSphereColliderComponent& sphere, CRaycastHitInfo& infoOut);
	static bool		CheckRayWithField(const Ray& ray, const float& fDistance, CFieldColliderComponent& field, CRaycastHitInfo& infoOut);

private:
	CCollisionDetector() {}
	~CCollisionDetector() {}

	//�v�Z�p�֐�
	static bool		checkPointWithAABB(CCollision& collisionOut, const Vector3 vPoint, const CAABBColliderComponent& aabb);
	static bool		checkPointWithBox(CCollision& collisionOut, const Vector3 vPoint, const CBoxColliderComponent& box);
	static bool		checkLineWithLine(const Vector2& vLA, const Vector2& vLB, const Vector2& vRA, const Vector2& vRB, Vector2& vOut);
	static bool		checkLineWithLine(const Vector3& vLA, const Vector3& vLB, const Vector3& vRA, const Vector3& vRB, Vector3& vOut);

	static float	transformBoxToAxis(const CBoxColliderComponent& box, const Vector3& vAxis);
	static bool		checkOverlapOnAxis(const CBoxColliderComponent& boxL, const CBoxColliderComponent& boxR, const Vector3& vAxis);
	static bool		checkOverlapOnAxis(const Vector2& vMinL, const Vector2& vMaxL, const Vector2& vMinR, const Vector2& vMaxR);
	static bool		checkOverlapAABB(const CAABBColliderComponent& aabbL, const CAABBColliderComponent& aabbR);

};
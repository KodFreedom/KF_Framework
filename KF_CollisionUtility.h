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
//  ���C�Փˏ��(�ŋߓ_)
//--------------------------------------------------------------------------------
class CRaycastHitInfo
{
public:
	CRaycastHitInfo()
		: m_pCollider(nullptr)
		, m_fDistance(0.0f)
		, m_vNormal(CKFVec3(0.0f))
		, m_vPos(CKFVec3(0.0f))
	{}
	~CRaycastHitInfo() {}

	CColliderComponent*		m_pCollider;	//�Փˑ���̃R���C�_�[
	float					m_fDistance;	//���C�̎n�_�ƏՓ˓_�Ƃ̋���
	CKFVec3					m_vNormal;		//�Փ˓_�̖ʖ@��
	CKFVec3					m_vPos;			//�Փ˓_�̈ʒu
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
	static void		CheckSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR);
	static void		CheckSphereWithAABB(CSphereColliderComponent& sphere, CAABBColliderComponent& aabb);
	static void		CheckSphereWithOBB(CSphereColliderComponent& sphere, COBBColliderComponent& obb);
	static void		CheckAABBWithAABB(CAABBColliderComponent& aabbL, CAABBColliderComponent& aabbR);
	static void		CheckBoxWithBox(CBoxColliderComponent& boxL, CBoxColliderComponent& boxR);
	
	//Field
	static void		CheckBoxWithField(CBoxColliderComponent& box, CFieldColliderComponent& field);
	static void		CheckSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field);
	
	//Ray
	static bool		CheckRayWithBox(const CKFRay& ray, const float& fDistance, CBoxColliderComponent& box, CRaycastHitInfo& infoOut);
	static bool		CheckRayWithSphere(const CKFRay& ray, const float& fDistance, CSphereColliderComponent& sphere, CRaycastHitInfo& infoOut);
	static bool		CheckRayWithField(const CKFRay& ray, const float& fDistance, CFieldColliderComponent& field, CRaycastHitInfo& infoOut);

private:
	CCollisionDetector() {}
	~CCollisionDetector() {}

	//�v�Z�p�֐�
	static bool		checkPointWithAABB(CCollision& collisionOut, const CKFVec3 vPoint, const CAABBColliderComponent& aabb);
	static bool		checkPointWithBox(CCollision& collisionOut, const CKFVec3 vPoint, const CBoxColliderComponent& box);
	static bool		checkLineWithLine(const CKFVec2& vLA, const CKFVec2& vLB, const CKFVec2& vRA, const CKFVec2& vRB, CKFVec2& vOut);
	static bool		checkLineWithLine(const CKFVec3& vLA, const CKFVec3& vLB, const CKFVec3& vRA, const CKFVec3& vRB, CKFVec3& vOut);

	static float	transformBoxToAxis(const CBoxColliderComponent& box, const CKFVec3& vAxis);
	static bool		checkOverlapOnAxis(const CBoxColliderComponent& boxL, const CBoxColliderComponent& boxR, const CKFVec3& vAxis);
	static bool		checkOverlapOnAxis(const CKFVec2& vMinL, const CKFVec2& vMaxL, const CKFVec2& vMinR, const CKFVec2& vMaxR);
	static bool		checkOverlapAABB(const CAABBColliderComponent& aabbL, const CAABBColliderComponent& aabbR);

};
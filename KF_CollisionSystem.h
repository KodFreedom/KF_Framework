//--------------------------------------------------------------------------------
//	�R���W�����V�X�e��
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
class CBoxColliderComponent;
class CSphereColliderComponent;
class CAABBColliderComponent;
class COBBColliderComponent;
class CFieldColliderComponent;
class CCollision;
class CRaycastHitInfo;
class CGameObject;

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define CS CKFCollisionSystem	//CKFCollisionSystem�̗���

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���W�����V�X�e��
//--------------------------------------------------------------------------------
class CKFCollisionSystem
{
public:
	//--------------------------------------------------------------------------------
	//  �񋓌^��`
	//--------------------------------------------------------------------------------
	enum COL_MODE
	{
		STATIC = 0,
		DYNAMIC,
		MODE_MAX
	};

	enum COL_TYPE
	{
		COL_SPHERE = 0,
		COL_AABB,
		COL_OBB,
		COL_PLANE,
		COL_CYLINDER,
		COL_MAX,		//�ȉ��͓��ʂ�Collider(Static����Dynamic�̂ݎg��)
		COL_FIELD
	};

	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	//typedef list<CColliderComponent*>::iterator COL_ITR;

	//struct COL_INFO
	//{//�܂��g��Ȃ��A�����蔻��^�C�v���w��ł���d�l
	//	CColliderComponent* pCollider;		//Collider�̃|�C���^
	//	list<COL_TYPE> listTargetType;	//Collider���ǂ̃^�C�v�Ɠ����蔻�肷�邩
	//};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CKFCollisionSystem();
	~CKFCollisionSystem() {}

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	LateUpdate(void);
	void	Release(void)
	{
		Uninit();
		delete this;
	}

#ifdef _DEBUG
	void	DrawCollider(void);
#endif

	void	RegisterCollider(const COL_MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider);
	void	DeregisterCollider(const COL_MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider);
	bool	RayCast(const CKFVec3& vOrigin, const CKFVec3& vDirection, const float& fDistance, CRaycastHitInfo& infoOut, const CGameObject* const pObjThis = nullptr);

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	//Dynamic Sphere
	void	checkWithDynamicSphere(const list<CColliderComponent*>::iterator& itrBegin, CSphereColliderComponent& sphere);
	
	//Dynamic AABB
	void	checkWithDynamicAABB(CSphereColliderComponent& sphere);
	void	checkWithDynamicAABB(const list<CColliderComponent*>::iterator& itrBegin, CAABBColliderComponent& aabb);
	bool	checkWithDynamicAABB(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut, const CGameObject* const pObjThis);

	//Dynamic OBB
	void	checkWithDynamicOBB(CSphereColliderComponent& sphere);
	void	checkWithDynamicOBB(CAABBColliderComponent& aabb);
	void	checkWithDynamicOBB(const list<CColliderComponent*>::iterator& itrBegin, COBBColliderComponent& obb);
	bool	checkWithDynamicOBB(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut, const CGameObject* const pObjThis);

	//Static Sphere
	void	checkWithStaticSphere(CSphereColliderComponent& sphere);
	void	checkWithStaticSphere(CAABBColliderComponent& aabb);
	void	checkWithStaticSphere(COBBColliderComponent& obb);
	
	//Static AABB
	void	checkWithStaticAABB(CSphereColliderComponent& sphere);
	void	checkWithStaticAABB(CAABBColliderComponent& aabb);
	void	checkWithStaticAABB(COBBColliderComponent& obb);
	bool	checkWithStaticAABB(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut);

	//Static OBB
	void	checkWithStaticOBB(CSphereColliderComponent& sphere);
	void	checkWithStaticOBB(CBoxColliderComponent& box);
	bool	checkWithStaticOBB(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut);

	//Field
	void	checkWithField(CSphereColliderComponent& sphere);
	void	checkWithField(CBoxColliderComponent& box);
	bool	checkWithField(const CKFRay& ray, const float& fDistance, CRaycastHitInfo& infoOut);

#ifdef _DEBUG
	string toString(const COL_MODE& mode);
	string toString(const COL_TYPE& type);
#endif

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	list<CColliderComponent*> m_alistCollider[MODE_MAX][COL_MAX];
	list<CColliderComponent*> m_listField;

#ifdef _DEBUG
	LPD3DXMESH m_pMeshSphere;
	LPD3DXMESH m_pMeshCube;
#endif
};

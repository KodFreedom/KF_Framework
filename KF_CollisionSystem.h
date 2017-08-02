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
class CSphereColliderComponent;
class COBBColliderComponent;
class CFieldColliderComponent;

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
	
private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void	CheckWithDynamicSphere(const list<CColliderComponent*>::iterator& itrBegin, CSphereColliderComponent& sphere);
	void	CheckWithStaticSphere(CSphereColliderComponent& sphere);
	void	CheckWithField(CSphereColliderComponent& sphere);

	//�Փ˔���֐�
	void	CheckCollisionSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR);
	void	CheckCollisionSphereWithOBB(CSphereColliderComponent& sphere, COBBColliderComponent& obb);
	void	CheckCollisionOBBWithOBB(COBBColliderComponent& obbL, COBBColliderComponent& obbR);

	//Field
	void	CheckCollisionOBBWithField(COBBColliderComponent& obb, CFieldColliderComponent& field);
	void	CheckCollisionSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field);

	//�v�Z�p�֐�
	float	transformBoxToAxis(const CKFVec3& vPos, const CKFVec3& vHalfSize, const CKFVec3& vAxis);

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

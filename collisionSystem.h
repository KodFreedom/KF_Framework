//--------------------------------------------------------------------------------
//	�R���W�����V�X�e��
//�@KF_CollisionSystem.h
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class Collider;
class BoxCollider;
class SphereCollider;
class AABBCollider;
class OBBCollider;
class FieldCollider;
class Collision;
class RayHitInfo;
class GameObject;

//--------------------------------------------------------------------------------
//  �񋓌^��`
//--------------------------------------------------------------------------------
enum ColliderMode
{
	CM_Static = 0,
	CM_Dynamic,
	CM_Max
};

enum ColliderType
{
	CT_Sphere = 0,
	CT_AABB,
	CT_OBB,
	CT_Plane,
	CT_Cylinder,
	CT_Max,		//�ȉ��͓��ʂ�Collider(Static����Dynamic�̂ݎg��)
	CT_Field
};

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CollisionSystem
{
#ifdef _DEBUG
	friend class DebugObserver;
#endif // _DEBUG

public:
	//--------------------------------------------------------------------------------
	//  �ÓI�����o�ϐ�
	//--------------------------------------------------------------------------------
	static const float maxCollisionRange;

	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	//struct COL_INFO
	//{//�܂��g��Ȃ��A�����蔻��^�C�v���w��ł���d�l
	//	Collider* pCollider;		//Collider�̃|�C���^
	//	list<ColliderType> listTargetType;	//Collider���ǂ̃^�C�v�Ɠ����蔻�肷�邩
	//};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	static auto	Create(void)
	{
		if (instance) return instance;
		instance = new CollisionSystem;
		instance->init();
		return instance;
	}
	static void Release(void) { SAFE_UNINIT(instance); }
	static auto Instance(void) { return instance; }

	void		Update(void);
	void		LateUpdate(void);
	void		Clear(void);

#ifdef _DEBUG
	void		DrawCollider(void);
#endif

	void		Register(Collider* collider);
	void		Deregister(Collider* collider);
	RayHitInfo*	RayCast(const Ray& ray, const float& distance, const GameObject* const rayOwner = nullptr);

private:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	typedef list<Collider*>::iterator Iterator;

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CollisionSystem();
	~CollisionSystem() {}
	void init(void);
	void uninit(void);
	void checkWithDynamicSphere(const Iterator& begin, SphereCollider& sphere);
	void checkWithDynamicAABB(SphereCollider& sphere);
	void checkWithDynamicAABB(const Iterator& begin, AABBCollider& aabb);
	void checkWithDynamicOBB(SphereCollider& sphere);
	void checkWithDynamicOBB(AABBCollider& aabb);
	void checkWithDynamicOBB(const Iterator& begin, OBBCollider& obb);
	void checkWithStaticSphere(SphereCollider& sphere);
	void checkWithStaticSphere(AABBCollider& aabb);
	void checkWithStaticSphere(OBBCollider& obb);
	void checkWithStaticAABB(SphereCollider& sphere);
	void checkWithStaticAABB(AABBCollider& aabb);
	void checkWithStaticAABB(OBBCollider& obb);
	void checkWithStaticOBB(SphereCollider& sphere);
	void checkWithStaticOBB(BoxCollider& box);
	void checkWithField(SphereCollider& sphere);
	void checkWithField(BoxCollider& box);
	
#ifdef _DEBUG
	string modeToString(const ColliderMode& mode);
	string typeToString(const ColliderType& type);
#endif

	// RayCast
	RayHitInfo*	raycastDynamicSphere(const Ray& ray, const float& distance, const GameObject* const rayOwner);
	RayHitInfo*	raycastDynamicAABB(const Ray& ray, const float& distance, const GameObject* const rayOwner);
	RayHitInfo*	raycastDynamicOBB(const Ray& ray, const float& distance, const GameObject* const rayOwner);
	RayHitInfo*	raycastStaticSphere(const Ray& ray, const float& distance);
	RayHitInfo*	raycastStaticAABB(const Ray& ray, const float& distance);
	RayHitInfo*	raycastStaticOBB(const Ray& ray, const float& distance);
	RayHitInfo*	raycastField(const Ray& ray, const float& distance);
	RayHitInfo* getRealResult(RayHitInfo* current, RayHitInfo* next);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	list<Collider*>			collidersArrays[CM_Max][CT_Max];
	list<Collider*>			fields;
	static CollisionSystem*	instance;

#ifdef _DEBUG
	//LPD3DXMESH	meshSphere;
	//LPD3DXMESH	meshCube;
	//bool		isDrawCollider;
#endif
};

//--------------------------------------------------------------------------------
//�@collision_system.h
//	�Փ˔���V�X�e��
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class Collider;
class BoxCollider;
class SphereCollider;
class AabbCollider;
class ObbCollider;
class FieldCollider;
class Collision;
class RayHitInfo;
class GameObject;

//--------------------------------------------------------------------------------
//  �񋓌^��`
//--------------------------------------------------------------------------------
enum ColliderMode
{
	kStatic = 0,
	kDynamic,
	kColliderModeMax
};

enum ColliderType
{
	kSphere = 0,
	kAabb,
	kObb,
	kPlane,
	kCylinder,
	kColliderTypeMax, //�ȉ��͓��ʂ�Collider(Static����Dynamic�̂ݎg��)
	kField
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
	//  �萔��`
	//--------------------------------------------------------------------------------
	static constexpr float kMaxCollisionCheckRange = 250.0f;

	//--------------------------------------------------------------------------------
	//  ��������
	//  return : CollisionSystem*
	//--------------------------------------------------------------------------------
	static CollisionSystem* Create(void)
	{
		auto instance = MY_NEW CollisionSystem;
		instance->Init();
		return instance;
	}

	//--------------------------------------------------------------------------------
	//  �j������
	//--------------------------------------------------------------------------------
	void Release(void) { Uninit(); }

	//--------------------------------------------------------------------------------
	//  �X�V����
	//--------------------------------------------------------------------------------
	void Update(void);

	//--------------------------------------------------------------------------------
	//  ��X�V����
	//--------------------------------------------------------------------------------
	void LateUpdate(void);

	//--------------------------------------------------------------------------------
	//  �N���A����
	//--------------------------------------------------------------------------------
	void Clear(void);

#ifdef _DEBUG
	//--------------------------------------------------------------------------------
	//  �R���C�_�[�\������
	//--------------------------------------------------------------------------------
	void Render(void);
#endif

	//--------------------------------------------------------------------------------
	//  �o�^����
	//--------------------------------------------------------------------------------
	void Register(Collider* collider);

	//--------------------------------------------------------------------------------
	//  �폜����
	//--------------------------------------------------------------------------------
	void Deregister(Collider* collider);

	//--------------------------------------------------------------------------------
	//  ���C�L���X�g����
	//--------------------------------------------------------------------------------
	RayHitInfo*	RayCast(const Ray& ray, const float& distance, const GameObject* const rayOwner = nullptr);

private:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	typedef list<Collider*>::iterator Iterator;

	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	CollisionSystem() {}
	CollisionSystem(const CollisionSystem& value) {}
	CollisionSystem& operator=(const CollisionSystem& value) {}
	~CollisionSystem() {}

	//--------------------------------------------------------------------------------
	//  ����������
	//--------------------------------------------------------------------------------
	void Init(void);

	//--------------------------------------------------------------------------------
	//  �I������
	//--------------------------------------------------------------------------------
	void Uninit(void);

#ifdef _DEBUG
	// �f�B�o�Q�\��
	String ModeToString(const ColliderMode& mode);
	String TypeToString(const ColliderType& type);
#endif

	// �Փ˔���
	void CheckWithDynamicSphere(const Iterator& begin, SphereCollider& sphere);
	void CheckWithDynamicAABB(SphereCollider& sphere);
	void CheckWithDynamicAABB(const Iterator& begin, AabbCollider& aabb);
	void CheckWithDynamicOBB(SphereCollider& sphere);
	void CheckWithDynamicOBB(AabbCollider& aabb);
	void CheckWithDynamicOBB(const Iterator& begin, ObbCollider& obb);
	void CheckWithStaticSphere(SphereCollider& sphere);
	void CheckWithStaticSphere(AabbCollider& aabb);
	void CheckWithStaticSphere(ObbCollider& obb);
	void CheckWithStaticAABB(SphereCollider& sphere);
	void CheckWithStaticAABB(AabbCollider& aabb);
	void CheckWithStaticAABB(ObbCollider& obb);
	void CheckWithStaticOBB(SphereCollider& sphere);
	void CheckWithStaticOBB(BoxCollider& box);
	void CheckWithField(SphereCollider& sphere);
	void CheckWithField(BoxCollider& box);

	// RayCast
	RayHitInfo*	RaycastDynamicSphere(const Ray& ray, const float& distance, const GameObject* const rayOwner);
	RayHitInfo*	RaycastDynamicAABB(const Ray& ray, const float& distance, const GameObject* const rayOwner);
	RayHitInfo*	RaycastDynamicOBB(const Ray& ray, const float& distance, const GameObject* const rayOwner);
	RayHitInfo*	RaycastStaticSphere(const Ray& ray, const float& distance);
	RayHitInfo*	RaycastStaticAABB(const Ray& ray, const float& distance);
	RayHitInfo*	RaycastStaticOBB(const Ray& ray, const float& distance);
	RayHitInfo*	RaycastField(const Ray& ray, const float& distance);
	RayHitInfo* GetRealResult(RayHitInfo* current, RayHitInfo* next);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	list<Collider*> colliders_arrays_[kColliderModeMax][kColliderTypeMax];
	list<Collider*> fields_;
};

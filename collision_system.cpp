//--------------------------------------------------------------------------------
//�@collision_system.cpp
//	�Փ˔���V�X�e��
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "collision_system.h"
#include "rigidbody3D.h"
#include "behavior.h"
#include "transform.h"
#include "sphere_collider.h"
#include "aabb_collider.h"
#include "obb_collider.h"
#include "field_collider.h"
#include "game_object.h"
#include "collision_detector.h"

#if defined(_DEBUG) || defined(EDITOR)
//#include "rendererDX.h"
//#include "textureManager.h"
#endif

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CollisionSystem::Update(void)
{
	//kDynamic�w�̃R���W�������Ƃɂ܂�FStatic�w�Ɠ����蔻�肵��
	//kDynamic�w�̑��̃R���W�����Ɠ����蔻�肵��
	//�Ō��Field�Ɠ����蔻�肷��
	//Sphere
	for (auto iterator = colliders_arrays_[kDynamic][kSphere].begin(); iterator != colliders_arrays_[kDynamic][kSphere].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject().IsActive()) continue;
		auto& sphere = *static_cast<SphereCollider*>(*iterator);
		CheckWithDynamicSphere(iterator, sphere);
		CheckWithDynamicAabb(sphere);
		CheckWithDynamicObb(sphere);
		CheckWithStaticSphere(sphere);
		CheckWithStaticAabb(sphere);
		CheckWithStaticObb(sphere);
		CheckWithField(sphere);
	}

	//aabb
	for (auto iterator = colliders_arrays_[kDynamic][kAabb].begin(); iterator != colliders_arrays_[kDynamic][kAabb].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject().IsActive()) continue;
		auto& aabb = *static_cast<AabbCollider*>(*iterator);
		CheckWithDynamicAabb(iterator, aabb);
		CheckWithDynamicObb(aabb);
		CheckWithStaticSphere(aabb);
		CheckWithStaticAabb(aabb);
		CheckWithStaticObb(aabb);
		CheckWithField(aabb);
	}

	//obb
	for (auto iterator = colliders_arrays_[kDynamic][kObb].begin(); iterator != colliders_arrays_[kDynamic][kObb].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject().IsActive()) continue;
		auto& obb = *static_cast<ObbCollider*>(*iterator);
		CheckWithDynamicObb(iterator, obb);
		CheckWithStaticSphere(obb);
		CheckWithStaticAabb(obb);
		CheckWithStaticObb(obb);
		CheckWithField(obb);
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CollisionSystem::LateUpdate(void)
{
}

//--------------------------------------------------------------------------------
//  �N���A����
//--------------------------------------------------------------------------------
void CollisionSystem::Clear(void)
{
	for (auto& colliders_array : colliders_arrays_)
	{
		for (auto& colliders : colliders_array)
		{
			colliders.clear();
		}
	}
	fields_.clear();
}

//--------------------------------------------------------------------------------
//  �o�^����
//--------------------------------------------------------------------------------
void CollisionSystem::Register(Collider* collider)
{
	if (collider->GetType() < kColliderTypeMax)
	{
		colliders_arrays_[collider->GetMode()][collider->GetType()].push_back(collider);
		return;
	}
	switch (collider->GetType())
	{
	case kField:
		fields_.push_back(collider);
		break;
	default:
		assert("error type!!!");
		break;
	}
}

//--------------------------------------------------------------------------------
//  �폜����
//--------------------------------------------------------------------------------
void CollisionSystem::Deregister(Collider* collider)
{
	if (collider->GetType() < kColliderTypeMax)
	{
		colliders_arrays_[collider->GetMode()][collider->GetType()].remove(collider);
		return;
	}
	switch (collider->GetType())
	{
	case kField:
	{
		fields_.remove(collider);
		break;
	}
	default:
		assert("error type!!!");
		break;
	}
}

//--------------------------------------------------------------------------------
//  ���C�L���X�g�֐��A���C�ƏՓ˂����̍ŋߓ_���Z�o����
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::RayCast(const Ray& ray, const float& distance, const GameObject* const ray_owner)
{
	RayHitInfo* real_result = nullptr;
	auto result = RaycastDynamicSphere(ray, distance, *ray_owner);
	real_result = GetRealResult(real_result, result);
	result = RaycastDynamicAabb(ray, distance, *ray_owner);
	real_result = GetRealResult(real_result, result);
	result = RaycastDynamicObb(ray, distance, *ray_owner);
	real_result = GetRealResult(real_result, result);
	result = RaycastStaticSphere(ray, distance);
	real_result = GetRealResult(real_result, result);
	result = RaycastStaticAabb(ray, distance);
	real_result = GetRealResult(real_result, result);
	result = RaycastStaticObb(ray, distance);
	real_result = GetRealResult(real_result, result);
	result = RaycastField(ray, distance);
	real_result = GetRealResult(real_result, result);
	return real_result;
}

#if defined(_DEBUG) || defined(EDITOR)
//--------------------------------------------------------------------------------
//  DrawCollider
//--------------------------------------------------------------------------------
void CollisionSystem::Render(void)
{
	//if (!m_bDrawCollider) { return; }

	//auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	//LPDIRECT3DTEXTURE9 texturePointer = Main::GetManager()->GetTextureManager()->GetTexture("polygon.png");
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//pDevice->SetTexture(0, texturePointer);

	////sphere
	//for (auto pCol : colliders[kDynamic][Sphere])
	//{
	//	if (!pCol->GetGameObject().IsActive()) continue;
	//	D3DXVECTOR3 Position = pCol->GetNextWorldPosition();
	//	float fRadius = ((SphereCollider*)pCol)->GetRadius();
	//	D3DXMATRIX mtx,mtxPos,mtxScale;
	//	D3DXMatrixIdentity(&mtx);
	//	D3DXMatrixScaling(&mtxScale, fRadius, fRadius, fRadius);
	//	mtx *= mtxScale;
	//	D3DXMatrixTranslation(&mtxPos, Position.x, Position.y, Position.z);
	//	mtx *= mtxPos;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	m_meshSphere->DrawSubset(0);
	//}

	//for (auto pCol : colliders[Static][Sphere])
	//{
	//	if (!pCol->GetGameObject().IsActive()) continue;
	//	auto Position = pCol->GetNextWorldPosition();
	//	float fRadius = ((SphereCollider*)pCol)->GetRadius();
	//	D3DXMATRIX mtx, mtxPos, mtxScale;
	//	D3DXMatrixIdentity(&mtx);
	//	D3DXMatrixScaling(&mtxScale, fRadius, fRadius, fRadius);
	//	mtx *= mtxScale;
	//	D3DXMatrixTranslation(&mtxPos, Position.X, Position.Y, Position.Z);
	//	mtx *= mtxPos;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	m_meshSphere->DrawSubset(0);
	//}

	////AABB
	//for (auto pCol : colliders[kDynamic][AABB])
	//{
	//	if (!pCol->GetGameObject().IsActive()) continue;
	//	auto Position = pCol->GetNextWorldPosition();
	//	auto vHalfSize = ((AabbCollider*)pCol)->GetHalfSize();
	//	D3DXMATRIX mtx, mtxPos, mtxScale;
	//	D3DXMatrixIdentity(&mtx);
	//	D3DXMatrixScaling(&mtxScale, vHalfSize.X * 2.0f, vHalfSize.Y * 2.0f, vHalfSize.Z * 2.0f);
	//	mtx *= mtxScale;
	//	D3DXMatrixTranslation(&mtxPos, Position.X, Position.Y, Position.Z);
	//	mtx *= mtxPos;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	m_meshCube->DrawSubset(0);
	//}

	//for (auto pCol : colliders[Static][AABB])
	//{
	//	if (!pCol->GetGameObject().IsActive()) continue;
	//	auto Position = pCol->GetNextWorldPosition();
	//	auto vHalfSize = ((AabbCollider*)pCol)->GetHalfSize();
	//	D3DXMATRIX mtx, mtxPos, mtxScale;
	//	D3DXMatrixIdentity(&mtx);
	//	D3DXMatrixScaling(&mtxScale, vHalfSize.X * 2.0f, vHalfSize.Y * 2.0f, vHalfSize.Z * 2.0f);
	//	mtx *= mtxScale;
	//	D3DXMatrixTranslation(&mtxPos, Position.X, Position.Y, Position.Z);
	//	mtx *= mtxPos;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	m_meshCube->DrawSubset(0);
	//}

	////OBB
	//for (auto pCol : colliders[kDynamic][OBB])
	//{
	//	if (!pCol->GetGameObject().IsActive()) continue;
	//	auto& vHalfSize = ((ObbCollider*)pCol)->GetHalfSize();
	//	D3DXMATRIX mtx;
	//	D3DXMatrixScaling(&mtx, vHalfSize.X * 2.0f, vHalfSize.Y * 2.0f, vHalfSize.Z * 2.0f);
	//	D3DXMATRIX mtxOff = pCol->GetOffset();
	//	mtx *= mtxOff;
	//	D3DXMATRIX mtxObj = pCol->GetGameObject()->GetTransform()->GetMatrix();
	//	mtx *= mtxObj;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	m_meshCube->DrawSubset(0);
	//}

	//for (auto pCol : colliders[Static][OBB])
	//{
	//	if (!pCol->GetGameObject().IsActive()) continue;
	//	auto& vHalfSize = ((ObbCollider*)pCol)->GetHalfSize();
	//	D3DXMATRIX mtx;
	//	D3DXMatrixScaling(&mtx, vHalfSize.X * 2.0f, vHalfSize.Y * 2.0f, vHalfSize.Z * 2.0f);
	//	D3DXMATRIX mtxOff = pCol->GetOffset();
	//	mtx *= mtxOff;
	//	D3DXMATRIX mtxObj = pCol->GetGameObject()->GetTransform()->GetMatrix();
	//	mtx *= mtxObj;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	m_meshCube->DrawSubset(0);
	//}


	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
#endif

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CollisionSystem::CollisionSystem()
#if defined(_DEBUG) || defined(EDITOR)
//: m_meshSphere(nullptr)
//, m_meshCube(nullptr)
//, m_bDrawCollider(false)
#endif
{}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CollisionSystem::Init(void)
{
#if defined(_DEBUG) || defined(EDITOR)
	//auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	//Main::GetManager()->GetTextureManager()->UseTexture("polygon.png");
	//D3DXCreateSphere(pDevice, 1.0f, 10, 10, &m_meshSphere, nullptr);
	//D3DXCreateBox(pDevice, 1.0f, 1.0f, 1.0f, &m_meshCube, nullptr);
#endif
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CollisionSystem::Uninit(void)
{
	Clear();

#if defined(_DEBUG) || defined(EDITOR)
	//SAFE_RELEASE(m_meshSphere);
	//SAFE_RELEASE(m_meshCube);
	//Main::GetManager()->GetTextureManager()->DisuseTexture("polygon.png");
#endif
}

//--------------------------------------------------------------------------------
//
//  �Փ˔��菈��
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃX�t�B�A�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithDynamicSphere(const Iterator& begin, SphereCollider& sphere)
{
	for (auto iterator = begin; iterator != colliders_arrays_[kDynamic][kSphere].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject().IsActive()) continue;
		if ((*iterator)->GetGameObject() == sphere.GetGameObject()) continue;
		CollisionDetector::Detect(sphere, *static_cast<SphereCollider*>(*iterator));
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��AABB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithDynamicAabb(SphereCollider& sphere)
{
	for (auto collider : colliders_arrays_[kDynamic][kAabb])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		if (collider->GetGameObject() == sphere.GetGameObject()) continue;
		CollisionDetector::Detect(sphere, *static_cast<AabbCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  AABB��AABB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithDynamicAabb(const Iterator& begin, AabbCollider& aabb)
{
	for (auto iterator = begin; iterator != colliders_arrays_[kDynamic][kAabb].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject().IsActive()) continue;
		if ((*iterator)->GetGameObject() == aabb.GetGameObject()) continue;
		CollisionDetector::Detect(aabb, *static_cast<AabbCollider*>(*iterator));
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithDynamicObb(SphereCollider& sphere)
{
	for (auto collider : colliders_arrays_[kDynamic][kObb])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		if (collider->GetGameObject() == sphere.GetGameObject()) continue;
		CollisionDetector::Detect(sphere, *static_cast<ObbCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  AABB��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithDynamicObb(AabbCollider& aabb)
{
	for (auto collider : colliders_arrays_[kDynamic][kObb])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		if (collider->GetGameObject() == aabb.GetGameObject()) continue;
		CollisionDetector::Detect(aabb, *static_cast<ObbCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  OBB��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithDynamicObb(const Iterator& begin, ObbCollider& obb)
{
	for (auto iterator = begin; iterator != colliders_arrays_[kDynamic][kObb].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject().IsActive()) continue;
		if ((*iterator)->GetGameObject() == obb.GetGameObject()) continue;
		CollisionDetector::Detect(obb, *static_cast<ObbCollider*>(*iterator));
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃX�t�B�A�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithStaticSphere(SphereCollider& sphere)
{
	for (auto collider : colliders_arrays_[kStatic][kSphere])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		CollisionDetector::Detect(sphere, *static_cast<SphereCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��AAB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithStaticSphere(AabbCollider& aabb)
{
	for (auto collider : colliders_arrays_[kStatic][kSphere])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		CollisionDetector::Detect(*static_cast<SphereCollider*>(collider), aabb);
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithStaticSphere(ObbCollider& obb)
{
	for (auto collider : colliders_arrays_[kStatic][kSphere])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		CollisionDetector::Detect(*static_cast<SphereCollider*>(collider), obb);
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithStaticAabb(SphereCollider& sphere)
{
	for (auto collider : colliders_arrays_[kStatic][kAabb])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		CollisionDetector::Detect(sphere, *static_cast<AabbCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  AABB��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithStaticAabb(AabbCollider& aabb)
{
	for (auto collider : colliders_arrays_[kStatic][kAabb])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		CollisionDetector::Detect(aabb, *static_cast<AabbCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  OBB��AABB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithStaticAabb(ObbCollider& obb)
{
	for (auto collider : colliders_arrays_[kStatic][kAabb])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		CollisionDetector::Detect(obb, *static_cast<AabbCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithStaticObb(SphereCollider& sphere)
{
	for (auto collider : colliders_arrays_[kStatic][kObb])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		CollisionDetector::Detect(sphere, *static_cast<ObbCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  box��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithStaticObb(BoxCollider& box)
{
	for (auto collider : colliders_arrays_[kStatic][kObb])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		CollisionDetector::Detect(box, *static_cast<ObbCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃt�B�[���h�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithField(SphereCollider& sphere)
{
	for (auto collider : fields_)
	{
		if (!collider->GetGameObject().IsActive()) continue;
		CollisionDetector::Detect(sphere, *static_cast<FieldCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  OBB�ƃt�B�[���h�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::CheckWithField(BoxCollider& box)
{
	for (auto collider : fields_)
	{
		if (!collider->GetGameObject().IsActive()) continue;
		CollisionDetector::Detect(box, *static_cast<FieldCollider*>(collider));
	}
}

#if defined(_DEBUG) || defined(EDITOR)
//--------------------------------------------------------------------------------
//	���[�h��enum��string�ɕϊ�
//--------------------------------------------------------------------------------
String CollisionSystem::ModeToString(const ColliderMode& mode)
{
	switch (mode)
	{
	case kStatic:
		return L"Static";
	case kDynamic:
		return L"kDynamic";
	default:
		return L"ErrorMode";
	}
}

//--------------------------------------------------------------------------------
//	�^�C�v��enum��string�ɕϊ�
//--------------------------------------------------------------------------------
String CollisionSystem::TypeToString(const ColliderType& type)
{
	switch (type)
	{
	case kSphere:
		return L"Sphere";
	case kAabb:
		return L"AABB";
	case kObb:
		return L"OBB";
	case kPlane:
		return L"Plane";
	case kCylinder:
		return L"Cylinder";
	default:
		return L"ErrorType";
	}
}
#endif

//--------------------------------------------------------------------------------
//
//  Raycast����
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	DynamicSphere�Ƃ�raycast
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::RaycastDynamicSphere(const Ray& ray, const float& distance, const GameObject& ray_owner)
{
	RayHitInfo* real_result = nullptr;
	for (auto collider : colliders_arrays_[kDynamic][kSphere])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		if (collider->GetGameObject() == ray_owner) continue;
		auto result = CollisionDetector::Detect(ray, distance, *static_cast<SphereCollider*>(collider));
		real_result = GetRealResult(real_result, result);
	}
	return real_result;
}

//--------------------------------------------------------------------------------
//	DynamicAABB�Ƃ�raycast
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::RaycastDynamicAabb(const Ray& ray, const float& distance, const GameObject& ray_owner)
{
	RayHitInfo* real_result = nullptr;
	for (auto collider : colliders_arrays_[kDynamic][kAabb])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		if (collider->GetGameObject() == ray_owner) continue;
		auto result = CollisionDetector::Detect(ray, distance, *static_cast<AabbCollider*>(collider));
		real_result = GetRealResult(real_result, result);
	}
	return real_result;
}

//--------------------------------------------------------------------------------
//	DynamicOBB�Ƃ�raycast
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::RaycastDynamicObb(const Ray& ray, const float& distance, const GameObject& ray_owner)
{
	RayHitInfo* real_result = nullptr;
	for (auto collider : colliders_arrays_[kDynamic][kObb])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		if (collider->GetGameObject() == ray_owner) continue;
		auto result = CollisionDetector::Detect(ray, distance, *static_cast<ObbCollider*>(collider));
		real_result = GetRealResult(real_result, result);
	}
	return real_result;
}

//--------------------------------------------------------------------------------
//	StaticSphere�Ƃ�raycast
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::RaycastStaticSphere(const Ray& ray, const float& distance)
{
	RayHitInfo* real_result = nullptr;
	for (auto collider : colliders_arrays_[kStatic][kSphere])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		auto result = CollisionDetector::Detect(ray, distance, *static_cast<SphereCollider*>(collider));
		real_result = GetRealResult(real_result, result);
	}
	return real_result;
}

//--------------------------------------------------------------------------------
//	StaticAABB�Ƃ�raycast
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::RaycastStaticAabb(const Ray& ray, const float& distance)
{
	RayHitInfo* real_result = nullptr;
	for (auto collider : colliders_arrays_[kStatic][kAabb])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		auto result = CollisionDetector::Detect(ray, distance, *static_cast<AabbCollider*>(collider));
		real_result = GetRealResult(real_result, result);
	}
	return real_result;
}

//--------------------------------------------------------------------------------
//	StaticOBB�Ƃ�raycast
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::RaycastStaticObb(const Ray& ray, const float& distance)
{
	RayHitInfo* real_result = nullptr;
	for (auto collider : colliders_arrays_[kStatic][kObb])
	{
		if (!collider->GetGameObject().IsActive()) continue;
		auto result = CollisionDetector::Detect(ray, distance, *static_cast<ObbCollider*>(collider));
		real_result = GetRealResult(real_result, result);
	}
	return real_result;
}

//--------------------------------------------------------------------------------
//	StaticOBB�Ƃ�raycast
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::RaycastField(const Ray& ray, const float& distance)
{
	RayHitInfo* real_result = nullptr;
	for (auto collider : fields_)
	{
		if (!collider->GetGameObject().IsActive()) continue;
		auto result = CollisionDetector::Detect(ray, distance, *static_cast<FieldCollider*>(collider));
		real_result = GetRealResult(real_result, result);
	}
	return real_result;
}

//--------------------------------------------------------------------------------
//	���C�Ƃ̋������߂��̏���Ԃ��A�c��͔̂j������
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::GetRealResult(RayHitInfo* current, RayHitInfo* next)
{
	if (!next) return current;
	if (!current) return next;
	if (next->distance < current->distance)
	{
		delete current;
		return next;
	}
	delete next;
	return current;
}
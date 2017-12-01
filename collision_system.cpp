//--------------------------------------------------------------------------------
//　collision_system.cpp
//	衝突判定システム
//	Author : 徐文杰(KodFreedom)
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
//  更新処理
//--------------------------------------------------------------------------------
void CollisionSystem::Update(void)
{
	//kDynamic層のコリジョンごとにまずFStatic層と当たり判定して
	//kDynamic層の他のコリジョンと当たり判定して
	//最後にFieldと当たり判定する
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
//  更新処理
//--------------------------------------------------------------------------------
void CollisionSystem::LateUpdate(void)
{
}

//--------------------------------------------------------------------------------
//  クリア処理
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
//  登録処理
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
//  削除処理
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
//  レイキャスト関数、レイと衝突したの最近点を算出する
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
//  コンストラクタ
//--------------------------------------------------------------------------------
CollisionSystem::CollisionSystem()
#if defined(_DEBUG) || defined(EDITOR)
//: m_meshSphere(nullptr)
//, m_meshCube(nullptr)
//, m_bDrawCollider(false)
#endif
{}

//--------------------------------------------------------------------------------
//  初期化処理
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
//  終了処理
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
//  衝突判定処理
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  スフィアとスフィアの当たり判定
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
//  スフィアとAABBの当たり判定
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
//  AABBとAABBの当たり判定
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
//  スフィアとOBBの当たり判定
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
//  AABBとOBBの当たり判定
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
//  OBBとOBBの当たり判定
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
//  スフィアとスフィアの当たり判定
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
//  スフィアとAABの当たり判定
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
//  スフィアとOBBの当たり判定
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
//  スフィアとOBBの当たり判定
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
//  AABBとOBBの当たり判定
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
//  OBBとAABBの当たり判定
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
//  スフィアとOBBの当たり判定
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
//  boxとOBBの当たり判定
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
//  スフィアとフィールドの当たり判定
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
//  OBBとフィールドの当たり判定
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
//	モードのenumをstringに変換
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
//	タイプのenumをstringに変換
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
//  Raycast処理
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	DynamicSphereとのraycast
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
//	DynamicAABBとのraycast
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
//	DynamicOBBとのraycast
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
//	StaticSphereとのraycast
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
//	StaticAABBとのraycast
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
//	StaticOBBとのraycast
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
//	StaticOBBとのraycast
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
//	レイとの距離が近いの情報を返す、残りのは破棄する
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
//--------------------------------------------------------------------------------
//	コリジョン判定クラス
//　KF_Collision.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "collisionSystem.h"
#include "collisionDetector.h"
#include "physicsSystem.h"
#include "gameObject.h"
#include "sphereCollider.h"
#include "fieldCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"
#include "3DRigidbodyComponent.h"
#include "behaviorComponent.h"
#include "transformComponent.h"

#ifdef _DEBUG
//#include "rendererDX.h"
//#include "textureManager.h"
#include "debugObserver.h"
#endif

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
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
	//Dynamic層のコリジョンごとにまずFStatic層と当たり判定して
	//Dynamic層の他のコリジョンと当たり判定して
	//最後にFieldと当たり判定する
	//Sphere
	for (auto iterator = colliders[Dynamic][Sphere].begin(); iterator != colliders[Dynamic][Sphere].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject()->IsActive()) continue;
		auto& sphere = *dynamic_cast<SphereCollider*>(*iterator);

#pragma omp parallel sections
		{
#pragma omp section 
			checkWithDynamicSphere(iterator, sphere);
#pragma omp section 
			checkWithDynamicAABB(sphere);
#pragma omp section 
			checkWithDynamicOBB(sphere);
#pragma omp section 
			checkWithStaticSphere(sphere);
#pragma omp section 
			checkWithStaticAABB(sphere);
#pragma omp section
			checkWithStaticOBB(sphere);
#pragma omp section
			checkWithField(sphere);
		}
	}

	//aabb
	for (auto iterator = colliders[Dynamic][AABB].begin(); iterator != colliders[Dynamic][AABB].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject()->IsActive()) continue;
		auto& AABB = *dynamic_cast<AABBCollider*>(*iterator);

#pragma omp parallel sections
		{
#pragma omp section 
			checkWithDynamicAABB(iterator, AABB);
#pragma omp section 
			checkWithDynamicOBB(AABB);
#pragma omp section 
			checkWithStaticSphere(AABB);
#pragma omp section 
			checkWithStaticAABB(AABB);
#pragma omp section 
			checkWithStaticOBB(AABB);
#pragma omp section
			checkWithField(AABB);
		}
	}

	//obb
	for (auto iterator = colliders[Dynamic][OBB].begin(); iterator != colliders[Dynamic][OBB].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject()->IsActive()) continue;
		auto& OBB = *dynamic_cast<OBBCollider*>(*iterator);

#pragma omp parallel sections
		{
#pragma omp section 
			checkWithDynamicOBB(iterator, OBB);
#pragma omp section 
			checkWithStaticSphere(OBB);
#pragma omp section 
			checkWithStaticAABB(OBB);
#pragma omp section 
			checkWithStaticOBB(OBB);
#pragma omp section 
			checkWithField(OBB);
		}
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CollisionSystem::LateUpdate(void)
{
#ifdef _DEBUG
	auto debugObserver = DebugObserver::Instance();
	debugObserver->DisplayAlways("Collider情報 : \n");
	for (int countMode = 0; countMode < ColliderMode::Max; ++countMode)
	{
		debugObserver->DisplayAlways(modeToString((ColliderMode)countMode) + " : ");
		for (int countType = 0; countType < ColliderType::Max; ++countType)
		{
			int nNumCol = (int)colliders[countMode][countType].size();
			debugObserver->DisplayAlways(
				typeToString((ColliderType)countType) +
				"-" +
				to_string(nNumCol) + "    ");
		}
		debugObserver->DisplayAlways('\n');
	}
#endif
}

//--------------------------------------------------------------------------------
//	関数名：Register
//  関数説明：コライダーを登録する
//	引数：	collider
//	戻り値：なし
//--------------------------------------------------------------------------------
void CollisionSystem::Register(Collider* collider)
{
	if (collider->GetType() < ColliderType::Max)
	{
		colliders[collider->GetMode()][collider->GetType()].push_back(collider);
		return;
	}
	switch (collider->GetType())
	{
	case ColliderType::Field:
		fields.push_back(collider);
		break;
	default:
		assert("error type!!!");
		break;
	}
}

//--------------------------------------------------------------------------------
//	関数名：Deregister
//  関数説明：コライダーを削除する
//	引数：	collider
//	戻り値：なし
//--------------------------------------------------------------------------------
void CollisionSystem::Deregister(Collider* collider)
{
	if (collider->GetType() < ColliderType::Max)
	{
		colliders[collider->GetMode()][collider->GetType()].remove(collider);
		return;
	}
	switch (collider->GetType())
	{
	case ColliderType::Field:
	{
		fields.remove(collider);
		break;
	}
	default:
		assert("error type!!!");
		break;
	}
}

//--------------------------------------------------------------------------------
//	関数名：RayCast
//  関数説明：レイキャスト関数、レイと衝突したの最近点を算出する
//	引数：	ray：レイ
//			distance：レイの長さ
//			rayOwner：自分のゲームオブジェクト
//	戻り値：RayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::RayCast(const Ray& ray, const float& distance, const GameObject* const rayOwner)
{
	RayHitInfo* realResult = nullptr;
#pragma omp parallel sections
	{
#pragma omp section
		{
			auto result = raycastDynamicSphere(ray, distance, rayOwner);
			realResult = getRealResult(realResult, result);
		}

#pragma omp section
		{
			auto result = raycastDynamicAABB(ray, distance, rayOwner);
			realResult = getRealResult(realResult, result);
		}

#pragma omp section
		{
			auto result = raycastDynamicOBB(ray, distance, rayOwner);
			realResult = getRealResult(realResult, result);
		}

#pragma omp section
		{
			auto result = raycastStaticSphere(ray, distance);
			realResult = getRealResult(realResult, result);
		}

#pragma omp section
		{
			auto result = raycastStaticAABB(ray, distance);
			realResult = getRealResult(realResult, result);
		}

#pragma omp section
		{
			auto result = raycastStaticOBB(ray, distance);
			realResult = getRealResult(realResult, result);
		}

#pragma omp section
		{
			auto result = raycastField(ray, distance);
			realResult = getRealResult(realResult, result);
		}
	}
	return realResult;
}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  DrawCollider
//--------------------------------------------------------------------------------
void CollisionSystem::DrawCollider(void)
{
	//if (!m_bDrawCollider) { return; }

	//auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	//LPDIRECT3DTEXTURE9 texturePointer = Main::GetManager()->GetTextureManager()->GetTexture("polygon.png");
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//pDevice->SetTexture(0, texturePointer);

	////sphere
	//for (auto pCol : colliders[Dynamic][Sphere])
	//{
	//	if (!pCol->GetGameObject()->IsActive()) continue;
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
	//	if (!pCol->GetGameObject()->IsActive()) continue;
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
	//for (auto pCol : colliders[Dynamic][AABB])
	//{
	//	if (!pCol->GetGameObject()->IsActive()) continue;
	//	auto Position = pCol->GetNextWorldPosition();
	//	auto vHalfSize = ((AABBCollider*)pCol)->GetHalfSize();
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
	//	if (!pCol->GetGameObject()->IsActive()) continue;
	//	auto Position = pCol->GetNextWorldPosition();
	//	auto vHalfSize = ((AABBCollider*)pCol)->GetHalfSize();
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
	//for (auto pCol : colliders[Dynamic][OBB])
	//{
	//	if (!pCol->GetGameObject()->IsActive()) continue;
	//	auto& vHalfSize = ((OBBCollider*)pCol)->GetHalfSize();
	//	D3DXMATRIX mtx;
	//	D3DXMatrixScaling(&mtx, vHalfSize.X * 2.0f, vHalfSize.Y * 2.0f, vHalfSize.Z * 2.0f);
	//	D3DXMATRIX mtxOff = pCol->GetOffset();
	//	mtx *= mtxOff;
	//	D3DXMATRIX mtxObj = pCol->GetGameObject()->GetTransformComponent()->GetMatrix();
	//	mtx *= mtxObj;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	m_meshCube->DrawSubset(0);
	//}

	//for (auto pCol : colliders[Static][OBB])
	//{
	//	if (!pCol->GetGameObject()->IsActive()) continue;
	//	auto& vHalfSize = ((OBBCollider*)pCol)->GetHalfSize();
	//	D3DXMATRIX mtx;
	//	D3DXMatrixScaling(&mtx, vHalfSize.X * 2.0f, vHalfSize.Y * 2.0f, vHalfSize.Z * 2.0f);
	//	D3DXMATRIX mtxOff = pCol->GetOffset();
	//	mtx *= mtxOff;
	//	D3DXMATRIX mtxObj = pCol->GetGameObject()->GetTransformComponent()->GetMatrix();
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
#ifdef _DEBUG
//: m_meshSphere(nullptr)
//, m_meshCube(nullptr)
//, m_bDrawCollider(false)
#endif
{
	for (auto& lists : colliders)
	{
		for (auto& list : lists)
		{
			list.clear();
		}
	}

	fields.clear();
}

//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
void CollisionSystem::init(void)
{
#ifdef _DEBUG
	//auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	//Main::GetManager()->GetTextureManager()->UseTexture("polygon.png");
	//D3DXCreateSphere(pDevice, 1.0f, 10, 10, &m_meshSphere, nullptr);
	//D3DXCreateBox(pDevice, 1.0f, 1.0f, 1.0f, &m_meshCube, nullptr);
#endif
}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void CollisionSystem::uninit(void)
{
	for (auto& lists : colliders)
	{
		for (auto& list : lists)
		{
			assert(!list.empty());
			list.clear();
		}
	}

	fields.clear();

#ifdef _DEBUG
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
void CollisionSystem::checkWithDynamicSphere(const Iterator& begin, SphereCollider& sphere)
{
	for (auto iterator = begin; iterator != colliders[Dynamic][Sphere].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject()->IsActive()) continue;
		if ((*iterator)->GetGameObject() == sphere.GetGameObject()) continue;
		CollisionDetector::Detect(sphere, *dynamic_cast<SphereCollider*>(*iterator));
	}
}

//--------------------------------------------------------------------------------
//  スフィアとAABBの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithDynamicAABB(SphereCollider& sphere)
{
	for (auto collider : colliders[Dynamic][AABB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		if (collider->GetGameObject() == sphere.GetGameObject()) continue;
		CollisionDetector::Detect(sphere, *dynamic_cast<AABBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  AABBとAABBの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithDynamicAABB(const Iterator& begin, AABBCollider& aabb)
{
	for (auto iterator = begin; iterator != colliders[Dynamic][AABB].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject()->IsActive()) continue;
		if ((*iterator)->GetGameObject() == aabb.GetGameObject()) continue;
		CollisionDetector::Detect(aabb, *dynamic_cast<AABBCollider*>(*iterator));
	}
}

//--------------------------------------------------------------------------------
//  スフィアとOBBの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithDynamicOBB(SphereCollider& sphere)
{
	for (auto collider : colliders[Dynamic][OBB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		if (collider->GetGameObject() == sphere.GetGameObject()) continue;
		CollisionDetector::Detect(sphere, *dynamic_cast<OBBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  AABBとOBBの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithDynamicOBB(AABBCollider& aabb)
{
	for (auto collider : colliders[Dynamic][OBB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		if (collider->GetGameObject() == aabb.GetGameObject()) continue;
		CollisionDetector::Detect(aabb, *dynamic_cast<OBBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  OBBとOBBの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithDynamicOBB(const Iterator& begin, OBBCollider& obb)
{
	for (auto iterator = begin; iterator != colliders[Dynamic][OBB].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject()->IsActive()) continue;
		if ((*iterator)->GetGameObject() == obb.GetGameObject()) continue;
		CollisionDetector::Detect(obb, *dynamic_cast<OBBCollider*>(*iterator));
	}
}

//--------------------------------------------------------------------------------
//  スフィアとスフィアの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticSphere(SphereCollider& sphere)
{
	for (auto collider : colliders[Static][Sphere])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(sphere, *dynamic_cast<SphereCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  スフィアとAABの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticSphere(AABBCollider& aabb)
{
	for (auto collider : colliders[Static][Sphere])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(*dynamic_cast<SphereCollider*>(collider), aabb);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとOBBの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticSphere(OBBCollider& obb)
{
	for (auto collider : colliders[Static][Sphere])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(*dynamic_cast<SphereCollider*>(collider), obb);
	}
}

//--------------------------------------------------------------------------------
//  スフィアとOBBの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticAABB(SphereCollider& sphere)
{
	for (auto collider : colliders[Static][AABB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(sphere, *dynamic_cast<AABBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  AABBとOBBの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticAABB(AABBCollider& aabb)
{
	for (auto collider : colliders[Static][AABB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(aabb, *dynamic_cast<AABBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  OBBとAABBの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticAABB(OBBCollider& obb)
{
	for (auto collider : colliders[Static][AABB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(obb, *dynamic_cast<AABBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  スフィアとOBBの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticOBB(SphereCollider& sphere)
{
	for (auto collider : colliders[Static][OBB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(sphere, *dynamic_cast<OBBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  boxとOBBの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticOBB(BoxCollider& box)
{
	for (auto collider : colliders[Static][OBB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(box, *dynamic_cast<OBBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  スフィアとフィールドの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithField(SphereCollider& sphere)
{
	for (auto collider : fields)
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(sphere, *dynamic_cast<FieldCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  OBBとフィールドの当たり判定
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithField(BoxCollider& box)
{
	for (auto collider : fields)
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(box, *dynamic_cast<FieldCollider*>(collider));
	}
}

#ifdef _DEBUG
//--------------------------------------------------------------------------------
//	関数名：modeToString
//  関数説明：モードのenumをstringに変換
//	引数：	mode
//	戻り値：string
//--------------------------------------------------------------------------------
string CollisionSystem::modeToString(const ColliderMode& mode)
{
	switch (mode)
	{
	case Static:
		return "Static";
	case Dynamic:
		return "Dynamic";
	default:
		return "ErrorMode";
	}
}

//--------------------------------------------------------------------------------
//	関数名：toString
//  関数説明：タイプのenumをstringに変換
//	引数：	type
//	戻り値：string
//--------------------------------------------------------------------------------
string CollisionSystem::typeToString(const ColliderType& type)
{
	switch (type)
	{
	case Sphere:
		return "Sphere";
	case AABB:
		return "AABB";
	case OBB:
		return "OBB";
	case Plane:
		return "Plane";
	case Cylinder:
		return "Cylinder";
	default:
		return "ErrorType";
	}
}
#endif

//--------------------------------------------------------------------------------
//
//  Raycast処理
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	関数名：raycastDynamicSphere
//  関数説明：DynamicSphereとのraycast
//	引数：	ray：レイ
//			distance：レイの距離
//			rayOwner：レイの所有者
//	戻り値：RayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::raycastDynamicSphere(const Ray& ray, const float& distance, const GameObject* const rayOwner)
{
	RayHitInfo* realResult = nullptr;
	for (auto collider : colliders[Dynamic][Sphere])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		if (collider->GetGameObject() == rayOwner) continue;
		auto result = CollisionDetector::Detect(ray, distance, *dynamic_cast<SphereCollider*>(collider));
		realResult = getRealResult(realResult, result);
	}
	return realResult;
}

//--------------------------------------------------------------------------------
//	関数名：raycastDynamicAABB
//  関数説明：DynamicAABBとのraycast
//	引数：	ray：レイ
//			distance：レイの距離
//			rayOwner：レイの所有者
//	戻り値：RayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::raycastDynamicAABB(const Ray& ray, const float& distance, const GameObject* const rayOwner)
{
	RayHitInfo* realResult = nullptr;
	for (auto collider : colliders[Dynamic][AABB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		if (collider->GetGameObject() == rayOwner) continue;
		auto result = CollisionDetector::Detect(ray, distance, *dynamic_cast<AABBCollider*>(collider));
		realResult = getRealResult(realResult, result);
	}
	return realResult;
}

//--------------------------------------------------------------------------------
//	関数名：raycastDynamicOBB
//  関数説明：DynamicOBBとのraycast
//	引数：	ray：レイ
//			distance：レイの距離
//			rayOwner：レイの所有者
//	戻り値：RayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::raycastDynamicOBB(const Ray& ray, const float& distance, const GameObject* const rayOwner)
{
	RayHitInfo* realResult = nullptr;
	for (auto collider : colliders[Dynamic][OBB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		if (collider->GetGameObject() == rayOwner) continue;
		auto result = CollisionDetector::Detect(ray, distance, *dynamic_cast<OBBCollider*>(collider));
		realResult = getRealResult(realResult, result);
	}
	return realResult;
}

//--------------------------------------------------------------------------------
//	関数名：raycastStaticSphere
//  関数説明：StaticSphereとのraycast
//	引数：	ray：レイ
//			distance：レイの距離
//	戻り値：RayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::raycastStaticSphere(const Ray& ray, const float& distance)
{
	RayHitInfo* realResult = nullptr;
	for (auto collider : colliders[Static][Sphere])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		auto result = CollisionDetector::Detect(ray, distance, *dynamic_cast<SphereCollider*>(collider));
		realResult = getRealResult(realResult, result);
	}
	return realResult;
}

//--------------------------------------------------------------------------------
//	関数名：raycastStaticAABB
//  関数説明：StaticAABBとのraycast
//	引数：	ray：レイ
//			distance：レイの距離
//	戻り値：RayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::raycastStaticAABB(const Ray& ray, const float& distance)
{
	RayHitInfo* realResult = nullptr;
	for (auto collider : colliders[Static][AABB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		auto result = CollisionDetector::Detect(ray, distance, *dynamic_cast<AABBCollider*>(collider));
		realResult = getRealResult(realResult, result);
	}
	return realResult;
}

//--------------------------------------------------------------------------------
//	関数名：raycastStaticOBB
//  関数説明：StaticOBBとのraycast
//	引数：	ray：レイ
//			distance：レイの距離
//	戻り値：RayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::raycastStaticOBB(const Ray& ray, const float& distance)
{
	RayHitInfo* realResult = nullptr;
	for (auto collider : colliders[Static][OBB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		auto result = CollisionDetector::Detect(ray, distance, *dynamic_cast<OBBCollider*>(collider));
		realResult = getRealResult(realResult, result);
	}
	return realResult;
}

//--------------------------------------------------------------------------------
//	関数名：raycastField
//  関数説明：StaticOBBとのraycast
//	引数：	ray：レイ
//			distance：レイの距離
//	戻り値：RayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::raycastField(const Ray& ray, const float& distance)
{
	RayHitInfo* realResult = nullptr;
	for (auto collider : fields)
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		auto result = CollisionDetector::Detect(ray, distance, *dynamic_cast<FieldCollider*>(collider));
		realResult = getRealResult(realResult, result);
	}
	return realResult;
}

//--------------------------------------------------------------------------------
//	関数名：getRealResult
//  関数説明：レイとの距離が近いの情報を返す、残りのは破棄する
//	引数：	current：今の一番近い情報
//			next：新しい情報
//	戻り値：RayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::getRealResult(RayHitInfo* current, RayHitInfo* next)
{
	if (!next) return current;
	if (!current) return next;
	if (next->Distance < current->Distance)
	{
		delete current;
		return next;
	}
	delete next;
	return current;
}